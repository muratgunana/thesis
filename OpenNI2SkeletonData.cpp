#include <yarp/os/Network.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Bottle.h>
#include <yarp/sig/all.h>
#include <yarp/os/Property.h>

#include <yarp/sig/Vector.h>
#include <yarp/sig/Matrix.h>
#include <yarp/math/api.h>
#include <yarp/math/Quaternion.h>
#include <yarp/math/Math.h>

#include <iostream>
#include <map>
#include <stdio.h>
#include <yarp/os/Vocab.h>
using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::math;

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

void showObjectDetection(Vector& hand_vector, Vector& ball_center) { 
  
} 

int main(int argc, char* argv[]) {
  Network yarp;
  BufferedPort<Bottle> input;
  BufferedPort<ImageOf<PixelRgb> > imagePort;
  input.open("/receiver");
  imagePort.open("/imagergb");
  Network::connect("/OpenNI2/imageFrame:o","/imagergb");
  Network::connect("/OpenNI2/userSkeleton:o","/receiver");
  Vector elbow_joint(3), wrist_joint(3), hand_vector(3);
  yarp::os::Property prop;
  
  while(true) {
    Bottle *bot = input.read();
    ImageOf<PixelRgb> *image = imagePort.read();
    if (!bot->find("USER").isNull()) {
      //printf("User: %d\n", bot->find("USER").asInt());
    }
    if (image != NULL) {
      printf("The image size: %dx%d\n", image->width(), image->height());
    }
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
      Vector ball_center(3), red_ball(3), purple_ball(3);
       
      prop.fromConfigFile("object.ini");
      Bottle envBottle;
      envBottle = prop.findGroup("home");
      
      red_ball[0] = envBottle.findGroup("red").get(1).asDouble();
      red_ball[1] = envBottle.findGroup("red").get(2).asDouble(); 
      red_ball[2] = envBottle.findGroup("red").get(3).asDouble();

      purple_ball[0] = envBottle.findGroup("purple").get(1).asDouble();
      purple_ball[1] = envBottle.findGroup("purple").get(2).asDouble();
      purple_ball[2] = envBottle.findGroup("purple").get(3).asDouble();
     
      VectorOf<Vector> vector_storage(2);
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
        Vector elbow_ball_vector(3);
        elbow_ball_vector = ball_center - elbow_joint;

  
        // We need to take dot product of hand_vector with elbow_ball_vector 
        // and this will give us the distance. If the result is less than zero then the hand vector
        // is pointing away from the ball.
        double distance, radius = envBottle.find("radius").asDouble();
        distance = dot(hand_vector, elbow_ball_vector);
      
        // Scale hand vector in order to calculate the closest point to the ball.
        Vector hand_vector_scaled(3), closest_point(3);
        hand_vector_scaled = hand_vector * distance;
        closest_point = elbow_joint + hand_vector_scaled;
      
        // Check the distance between the closest point and object center. If it is smaller than 
        // the radius then the points is inside the object. That means that the hand points the object with 
        // correct coordinates. 
        printf("Distance: %f\n", distance);      
        printf("D2: %f\n", dot(ball_center - closest_point, ball_center - closest_point));
        if (distance > 0 && dot(ball_center - closest_point, ball_center - closest_point) < radius * radius) {
          printf("Pointing correctly: %f\n", distance);
        }
      }
    }
    //printf("Position: %sa\n", bot->toString().c_str());
    //input.read(bot);
    //printf("Got message: %s\n", bot.toString().c_str());
  }
  input.close();
  return 0;
}
