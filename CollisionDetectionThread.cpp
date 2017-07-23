#include "CollisionDetectionThread.h"
#include <yarp/os/LogStream.h>
#include <yarp/sig/Image.h>
#include <yarp/os/Time.h>

#define WRIST_JOINT 44
#define ELBOW_JOINT 32

using namespace std;

CollisionDetectionThread::CollisionDetectionThread()
  : semStart(0), semDone(0), interrupted(false) { }

CollisionDetectionThread::~CollisionDetectionThread() { }

void CollisionDetectionThread::run() {
  //Loop until the thread is running.
  while (!isStopping()) {
    printf("Hello, from thread1\n");
    CollisionDetectionThread::collisionDetector();
    Time::delay(0);
  }
}

void CollisionDetectionThread::collisionDetector() {
  std::cout << "Hello from the pointer" << std::endl;
  yarp::sig::Vector elbow_joint(3), wrist_joint(3), hand_vector(3);
  Property prop;
  Bottle* bot = skeletonPort.read();
  Bottle& pos = bot->findGroup("POS");
  //printf("Pos-whole : %s\n", pos.toString().c_str());
  //printf("Pos-size: %d\n", bot->size());
  for (int i=1; i < bot->size(); i++) {
    //printf("Pos[%d]: %d\n", i, bot->get(i).asInt());
  }

  Bottle bottle;
  // Extract skeleton parts.
  bottle = CollisionDetectionThread::showBottle(*bot, 0);
  if (!bottle.isNull()) {
    printf("Elbow-joint and wrist-joint vectors: %s\n", bottle.toString().c_str()); 
    for (int i = 0; i < bottle.size(); i++) {
      Value& element_list = bottle.get(i);
      Bottle *lst = element_list.asList();
      for (int j = 0; j < lst->size(); j++) {
        Value& element = lst->get(j);
        if (i == 0) {
          elbow_joint[j] = element.asDouble()/10.0;
        }
        if (i == 1) {
          wrist_joint[j] = element.asDouble()/10.0;
        }
      }
    }
   
    // Create the hand vector from elbow and wrist joints.
    hand_vector = wrist_joint - elbow_joint;
    printf("hand vector before: %s\n", hand_vector.toString().c_str());
      
    if (norm(hand_vector) > 0) {
      hand_vector /= norm(hand_vector);
    } 
    printf("hand vector normalized: %s\n", hand_vector.toString().c_str());
    // Now we need to create ball centre point as vector.
    yarp::sig::Vector ball_center(3), red_ball(3), purple_ball(3), green_ball(3);
       
    prop.fromConfigFile("object.ini");
    Bottle envBottle;
    envBottle = prop.findGroup("home");
      
    red_ball[0] = envBottle.findGroup("red").get(1).asDouble();
    red_ball[1] = envBottle.findGroup("red").get(2).asDouble(); 
    red_ball[2] = envBottle.findGroup("red").get(3).asDouble();

    purple_ball[0] = envBottle.findGroup("purple").get(1).asDouble();
    purple_ball[1] = envBottle.findGroup("purple").get(2).asDouble();
    purple_ball[2] = envBottle.findGroup("purple").get(3).asDouble();
    
    green_ball[0] = envBottle.findGroup("green").get(1).asDouble();
    green_ball[1] = envBottle.findGroup("green").get(2).asDouble();
    green_ball[2] = envBottle.findGroup("green").get(3).asDouble();
     
    VectorOf<yarp::sig::Vector> vector_storage(2);
    //vector_storage[1] = red_ball;
    //vector_storage[2] = purple_ball;
      
    for (int i = 0; i < vector_storage.size(); ++i) {
      // Create another vector from elbow joint to ball center.
      // This is needed to get dot product of hand_vector.
      if (i == 0) 
        ball_center = red_ball;
      else if (i == 1)
        ball_center = purple_ball;
      else if (i == 2)
        ball_center = green_ball;
      //printf("Vector storage: %s\n", vector_storage[i].toString().c_str());
      //ball_center = vector_storage[i];
      yarp::sig::Vector elbow_ball_vector(3);
      elbow_ball_vector = ball_center - elbow_joint;
  
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
        this->igaze->lookAtFixationPoint(ball_center/100.0);// request to gaze at the desired fixation point and wait for reply (sync method)
        this->igaze->waitMotionDone();
        this->setColorCode(i+1);
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
  this->skeletonPort.open("/receiver");
  Network::connect("/OpenNI2/userSkeleton:o","/receiver");

  // open a client interface to connect to the gaze server
  // we suppose that:
  // 1 - the iCub simulator is running;
  // 2 - the gaze server iKinGazeCtrl is running and
  //     launched with the following options: "--from configSim.ini"
  Property optGaze("(device gazecontrollerclient)");
  optGaze.put("remote","/iKinGazeCtrl");
  optGaze.put("local","/gaze_client");

  if (!this->clientGaze.open(optGaze))
    return false;
  // open the view
  this->clientGaze.view(this->igaze);
 
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
