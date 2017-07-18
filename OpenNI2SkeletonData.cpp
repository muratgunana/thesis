#include <cv.h>
#include <cvaux.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include <yarp/os/Network.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Bottle.h>
#include <yarp/sig/all.h>
#include <yarp/os/Property.h>
#include <yarp/os/RFModule.h>
#include <yarp/sig/Vector.h>
#include <yarp/sig/Matrix.h>
#include <yarp/math/api.h>
#include <yarp/math/Quaternion.h>
#include <yarp/math/Math.h>
#include <yarp/dev/GazeControl.h>
#include <yarp/dev/PolyDriver.h>

#include <iomanip>
#include <iostream>
#include <map>
#include <stdio.h>
#include <yarp/os/Vocab.h>

using namespace std;
using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;
using namespace yarp::math;
using namespace yarp::sig::draw;
using namespace yarp::sig::file;
using namespace cv;

class MultiModal : public RFModule {
  
protected:
  PolyDriver clientGaze;
  IGazeControl *igaze;
  BufferedPort<Bottle> skeletonPort;
  BufferedPort<ImageOf<PixelRgb> > imagePort; 
  int count;
  
public:
  double getPeriod() {
    // module periodicity (seconds).
    return 0.0;
  }

  // This is the main function that will be called periodically.
  bool updateModule() {
    count++;
    cout << "[" << count << "]" << " updateModule..." << endl;
    
    yarp::sig::Vector elbow_joint(3), wrist_joint(3), hand_vector(3);
    Property prop;
  
    Bottle *bot = skeletonPort.read();
    Bottle& pos = bot->findGroup("POS");
    //printf("Pos-whole : %s\n", pos.toString().c_str());
    //printf("Pos-size: %d\n", bot->size());
    for (int i=1; i < bot->size(); i++) {
      //printf("Pos[%d]: %d\n", i, bot->get(i).asInt());
    }

    Bottle bottle;
    // Extract skeleton parts.
    bottle = showBottle(*bot);
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
      yarp::sig::Vector ball_center(3), red_ball(3), purple_ball(3);
       
      prop.fromConfigFile("object.ini");
      Bottle envBottle;
      envBottle = prop.findGroup("robotics");
      
      red_ball[0] = envBottle.findGroup("red").get(1).asDouble();
      red_ball[1] = envBottle.findGroup("red").get(2).asDouble(); 
      red_ball[2] = envBottle.findGroup("red").get(3).asDouble();

      purple_ball[0] = envBottle.findGroup("purple").get(1).asDouble();
      purple_ball[1] = envBottle.findGroup("purple").get(2).asDouble();
      purple_ball[2] = envBottle.findGroup("purple").get(3).asDouble();
     
      VectorOf<yarp::sig::Vector> vector_storage(2);
      //vector_storage[1] = red_ball;
      //vector_storage[2] = purple_ball;
      
      for (int i = 0; i < vector_storage.size(); ++i) {
        // Create another vector from elbow joint to ball center.
        // This is needed to get dot product of hand_vector.
        if (i == 0) ball_center = red_ball;
        else
          ball_center = purple_ball;
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
        printf("Distance: %f\n", distance);      
        printf("D2: %f\n", dot(ball_center - closest_point, ball_center - closest_point));
        if (distance > 0 && dot(ball_center - closest_point, ball_center - closest_point) < radius * radius) {
          printf("Pointing correctly: %f\n", distance);
          printf("Ball center before: %s\n", ball_center.toString().c_str());
          igaze->lookAtFixationPoint(ball_center/100.0);// request to gaze at the desired fixation point and wait for reply (sync method)
          printf("Ball center after: %s\n", (ball_center/100.0).toString().c_str());
          igaze->waitMotionDone();
          if (objectRecognition()) {
            printf("Streaming works.\n"); 
          } 
        }
      }
    }
    return true;
  }
  
  // Performs obbject recognition.
  bool objectRecognition() {
    printf("Copying YARP image to an OpenCV/IPL image\n");
    
    ImageOf<PixelRgb> *image = imagePort.read();
    if (image != NULL) {
      printf("The image size: %dx%d\n", image->width(), image->height());
    }
    Mat imageFrame, gframe, baseframe;
  
    yarp::sig::Vector ang;
    //igaze->getAngles(ang);
    while (cv::waitKey(27) != 'Esc') { 
      image = imagePort.read();
      IplImage *cvImage = cvCreateImage(cvSize(image->width(),  
                                             image->height()), 
                                      IPL_DEPTH_8U, 3 );
      cvCvtColor((IplImage*)image->getIplImage(), cvImage, CV_RGB2BGR);
      cv::Mat image_mat(cvImage);
      baseframe = image_mat.clone();

      cvtColor(baseframe,gframe, CV_BGR2GRAY);
      GaussianBlur(gframe,gframe,Size(9,9),2,2);
      cv::vector<cv::Vec3f> circles;

      // Apply the Hough Transform to find the circles
      HoughCircles( gframe, circles, CV_HOUGH_GRADIENT, 2, gframe.rows/4, 179, 20, 0, 0);

      // Draw the circles detected
      for( size_t i = 0; i < circles.size(); i++ ) {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        // circle center
        circle(baseframe, center, 3, Scalar(0,255,0), -1, 8, 0 );
        // circle outline
        circle(baseframe, center, radius, Scalar(0,0,255), 3, 8, 0 );
        printf("Circle sizes: %d\n",radius);
      }

      imshow("Hough circles", baseframe); 
    }   
    return true;
  }
  
  // Message handler.
  bool respond(const Bottle& command, Bottle& reply) {
    if (command.get(0).asString() == "quit")
      return false;
    else
      reply = command;
    return true;
  }

  // Configure function. Receive a previously initialized
  // resource finder object. Use it to configure your module.
  // If you are migrating from the old module, this is the function
  // equivalent to the "open" method.
  bool configure(yarp::os::ResourceFinder &rf) {
    count=0;
    // optional, attach a port to the module
    // so that messages received from the port are redirected
    // to the respond method
    skeletonPort.open("/receiver");
    imagePort.open("/imagergb");
    Network::connect("/OpenNI2/imageFrame:o","/imagergb");
    Network::connect("/OpenNI2/userSkeleton:o","/receiver");
    
    // open a client interface to connect to the gaze server
    // we suppose that:
    // 1 - the iCub simulator is running;
    // 2 - the gaze server iKinGazeCtrl is running and
    //     launched with the following options: "--from configSim.ini"
    Property optGaze("(device gazecontrollerclient)");
    optGaze.put("remote","/iKinGazeCtrl");
    optGaze.put("local","/gaze_client");

    if (!clientGaze.open(optGaze))
      return false;
    // open the view
    clientGaze.view(igaze);
    
    return true;
  }
  // Interrupt function.
  bool interruptModule() {
    cout << "Interrupting your module, for port cleanup" << endl;
    return true;
  }
  // Close function, to perform cleanup.
  bool close() {
    // optional, close port explicitly
    cout << "Calling close function\n";
    skeletonPort.close();
    imagePort.close();
    return true;
  }
 
  Bottle showBottle(Bottle& anUnknownBottle, int indentation = 0) { 
    Bottle bot;
    for (int i=0; i<anUnknownBottle.size(); i++) {
      for (int j=0; j<indentation; j++) { printf(" "); }
      //printf("[%d]: ", i);
        
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
            if (i == 32 || i == 44) {
              bot.add(element);
              //container->asList()->fromString(lst->toString());
            } 
            showBottle(*lst,indentation+2);
          } else {
            printf("unrecognized type\n");
          }
          break;
      }
    }
    //printf("Newlist: %s\n", bot.toString().c_str());
    return bot;
  }
};

int main(int argc, char* argv[]) {
  Network yarp;
  
  MultiModal multiModal;

  ResourceFinder rf;
  rf.configure(argc, argv);
  rf.setVerbose(true);
  
  cout << "Configuring and starting module. \n";
  multiModal.runModule(rf);

  return 0;
}
