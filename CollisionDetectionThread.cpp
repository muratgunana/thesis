#include "CollisionDetectionThread.h"
#include <yarp/os/LogStream.h>
#include <yarp/sig/Image.h>
#include <yarp/os/Time.h>

#define WRIST_JOINT 44
#define ELBOW_JOINT 32

#define GAZE_EVENT_PORT "/iKinGazeCtrl/events:o"
#define GAZE_EVENT_LOCAL_PORT "/gazeEventLocal"
#define IKIN_GAZE_LOCAL_PORT "/iKinGazeLocal"
#define IKIN_GAZE_PORT "/iKinGazeCtrl/xd:i"
#define OPENNI_DATA_PORT "/OpenNI2/userSkeleton:o"
#define SKELETON_DATA_PORT "/skeletonPort"
#define MSPEAK_PORT "/MSpeak/text:i"
#define SPEECH_PORT "/speechPort"

#define RED_COLOR    "red"
#define GREEN_COLOR  "green"
#define PURPLE_COLOR "purple"
#define LEFT_EYE     "left_eye"

using namespace std;

CollisionDetectionThread::CollisionDetectionThread()
  : semStart(0), semDone(0), interrupted(false) { }

CollisionDetectionThread::~CollisionDetectionThread() { }

void CollisionDetectionThread::run() {
  
  //Loop until the thread is running.
  while (!isStopping()) {
    CollisionDetectionThread::collisionDetector();
    Time::delay(0);
  }
}

void CollisionDetectionThread::collisionDetector() {
  
  yarp::sig::Vector elbow_joint(3), wrist_joint(3), hand_vector(3);
  Property prop;
  Bottle* bot = skeletonPort.read();
  Bottle bottle;
  
  // Extract skeleton parts.
  bottle = CollisionDetectionThread::showBottle(*bot, 0);
  if (!bottle.isNull()) {
    for (int i = 0; i < bottle.size(); i++) {
      Value& element_list = bottle.get(i);
      Bottle *lst = element_list.asList();
      for (int j = 0; j < lst->size(); j++) {
        Value& element = lst->get(j);
        if (i == 0) {
          // Scale up the value to cm from mm.
          elbow_joint[j] = element.asDouble()/10.0f;
        }
        if (i == 1) {
          wrist_joint[j] = element.asDouble()/10.0f;
        }
      }
    }
   
    // Create the hand vector from elbow and wrist joints.
    hand_vector = wrist_joint - elbow_joint;
      
    if (norm(hand_vector) > 0) {
      hand_vector /= norm(hand_vector);
    } 
    
    // Now we need to create ball centre point as vector.
    yarp::sig::Vector ball_center(3);
       
    prop.fromConfigFile("object.ini");
    Bottle& envBottle = prop.findGroup("robotics");
    
    for (int i = 1; i < envBottle.size(); i++ ) {
      // We are only interested in the color elements.
      Bottle *lst = envBottle.get(i).asList();
      if (lst->size() > 3 && lst->get(0).asString() != LEFT_EYE) {
        ball_center[0] = lst->get(1).asDouble(); 
        ball_center[1] = lst->get(2).asDouble(); 
        ball_center[2] = lst->get(3).asDouble();
       
        yarp::sig::Vector elbow_ball_vector(3);
        elbow_ball_vector = ball_center - elbow_joint;
        //printf("Elbow joint: %s\n", elbow_joint.toString().c_str());
        
        // We need to take dot product of hand_vector with elbow_ball_vector 
        // and this will give us the distance. If the result is less than zero then the hand vector
        // is pointing away from the ball.
        double distance, radius = envBottle.find("radius").asDouble();
        distance = dot(hand_vector, elbow_ball_vector);
      
        // Scale hand vector in order to calculate the closest point to the ball.
        yarp::sig::Vector hand_vector_scaled(3), closest_point(3);
        hand_vector_scaled = hand_vector * distance;
        closest_point = elbow_joint + hand_vector_scaled;
      
        // Check the distance between the closest point and object center. If it is smaller than 
        // the radius then the points is inside the object. That means that the hand points the object with 
        // correct coordinates. 
        if (distance > 0 && dot(ball_center - closest_point, ball_center - closest_point) < radius * radius) {
          printf("Pointing correctly: %f\n", distance);
          
          Bottle& eyeBottle = envBottle.findGroup(LEFT_EYE);
          yarp::sig::Vector eyeVector(3);
          eyeVector[0] = eyeBottle.get(1).asDouble();
          eyeVector[1] = eyeBottle.get(2).asDouble();
          eyeVector[2] = eyeBottle.get(3).asDouble();
          
          if (lst->get(0).asString() == RED_COLOR) {
            this->setColorCode(RED);
          }
          else if (lst->get(0).asString() == GREEN_COLOR) {
            this->setColorCode(GREEN);
          }
          else if (lst->get(0).asString() == PURPLE_COLOR) {
            this->setColorCode(PURPLE);
          }
          
          //this->igaze->lookAtFixationPoint((ball_center - eyeVector)/100.0f);// request to gaze at the desired fixation point and wait for reply (sync method)
          //this->igaze->waitMotionDone();
          printf("Color: %s\n", lst->get(0).asString().c_str());
          yarp::sig::Vector tempVector(3), gazeVector(3);
          tempVector = (ball_center - eyeVector) / 100.0f;
          //printf("Temp vector: %s\n", tempVector.toString().c_str());
          //printf("Eye vector: %s\n", eyeVector.toString().c_str());
          gazeVector[0] = -tempVector[2];
          gazeVector[1] = -tempVector[0];
          gazeVector[2] = tempVector[1];
          
          printf("Gaze Bottle: %s\n", gazeVector.toString().c_str());
          Bottle& iKinGazeBottle = this->iKinGazePort.prepare();
          iKinGazeBottle.read(gazeVector);
          this->iKinGazePort.write();
          printf("iKinGaze Bottle: %s\n", iKinGazeBottle.toString().c_str());
          
          while (true) {
            Bottle* eventBottle = this->gazeEventPort.read();
            printf("Event bottle: %s\n", eventBottle->toString().c_str());
            if (eventBottle->get(0).asString() == "motion-done") {
              Bottle& speechBottle = this->speechPort.prepare();
              std::string speechText = "You have selected " + lst->get(0).asString() + " object.";
              speechBottle.addString(speechText);
              this->speechPort.write();
              break;
            }
          }
          printf("Ball: %s\n", lst->get(0).asString().c_str());
        }
      } 
    }  
  }
}

Bottle CollisionDetectionThread::showBottle(Bottle& anUnknownBottle, int indentation = 0) { 
  Bottle bot;
  for (int i=0; i<anUnknownBottle.size(); i++) {
    for (int j=0; j<indentation; j++) { printf(" "); }
        
    Value& element = anUnknownBottle.get(i);
    switch (element.getCode()) {
      case BOTTLE_TAG_INT:
        //printf("int %d\n", element.asInt());
        break;
      case BOTTLE_TAG_DOUBLE:
        //printf("float %g\n", element.asDouble());
        break;
      case BOTTLE_TAG_STRING:
        //printf("string \"%s\"\n", element.asString().c_str());
        break;
      case BOTTLE_TAG_BLOB:
        //printf("binary blob of length %d\n", element.asBlobLength());
        break;
      case BOTTLE_TAG_VOCAB:
        //printf("vocab [%s]\n", Vocab::decode(element.asVocab()).c_str());
        break;
      default:
        if (element.isList()) {
          Bottle *lst = element.asList();
          //printf("list of %d elements\n", lst->size());
          if (i == WRIST_JOINT || i == ELBOW_JOINT) {
            bot.add(element);
            //container->asList()->fromString(lst->toString());
          } 
          showBottle(*lst,indentation+2);
        }
        else {
          printf("unrecognized type\n");
        }
        break;
    }
  }
    
  return bot;
}

bool CollisionDetectionThread::threadInit() {
  // Open port for streaminf 3D sensor skeleton data.
  this->skeletonPort.open(SKELETON_DATA_PORT);
  Network::connect(OPENNI_DATA_PORT, SKELETON_DATA_PORT);
  
  // Open the port for iKinGazeCtrl server to write gaze coordinates.
  this->iKinGazePort.open(IKIN_GAZE_LOCAL_PORT);
  Network::connect(IKIN_GAZE_LOCAL_PORT, IKIN_GAZE_PORT);
  
  // Open the port for MSpeak speech server to text output.
  this->speechPort.open(SPEECH_PORT);
  Network::connect(SPEECH_PORT, MSPEAK_PORT);
  
  // Connect the iKinGazeCtrl events port to feed in the status of motion. 
  this->gazeEventPort.open(GAZE_EVENT_LOCAL_PORT);
  Network::connect(GAZE_EVENT_PORT, GAZE_EVENT_LOCAL_PORT);
  
  this->setColorCode(COLOR_CODE_OFFSET);

  // open a client interface to connect to the gaze server
  // we suppose that:
  // 1 - the iCub simulator is running;
  // 2 - the gaze server iKinGazeCtrl is running and
  //     launched with the following options: "--from configSim.ini"
  //Property optGaze("(device gazecontrollerclient)");
  //optGaze.put("remote","/iKinGazeCtrl/xd:i");
  //optGaze.put("local","/gaze_client");

  //if (!this->clientGaze.open(optGaze))
    //return false;
  // open the view
  //this->clientGaze.view(this->igaze);
 
  return true;
}
 
void CollisionDetectionThread::setColorCode(int colorCode) {
  this->colorCode = colorCode;
}

int CollisionDetectionThread::getColorCode() {
  return this->colorCode;
}

void CollisionDetectionThread::wait() {
  semDone.wait();
}

void CollisionDetectionThread::interrupt() {
  interrupted = true;
  semStart.post();
}
