#include <yarp/os/Network.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Bottle.h>

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
 
int main(int argc, char* argv[]) {
  Network yarp;
  BufferedPort<Bottle> input;
  input.open("/receiver");
  Network::connect("/OpenNI2/userSkeleton:o","/receiver");
  Vector elbow_joint(3), wrist_joint(3), hand_vector(3);
  while(true) {
    Bottle *bot = input.read();
    if (!bot->find("USER").isNull()) {
      //printf("User: %d\n", bot->find("USER").asInt());
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
      printf("hand vector after: %s\n", hand_vector.toString().c_str());
      // Now we need to create ball centre point as vector.
      Vector ball_center(3);
      
      // Home ball position coordinates.
       //ball_center[0] = 0.0;
       //ball_center[1] = -27.5; 
       //ball_center[2] = 40.0;
    
      // Robotics lab ball position coordinates.
      ball_center[0] = 26.0;
      ball_center[1] = -16.0; 
      ball_center[2] = 43.5;
      
      VectorOf<Vector> vector_storage(3);
      //vector_storage.push_back(ball_center);
      
      std::map<int,Vector> mymap;
      //mymap.insert(2, ball_center);
      printf("Storage size: %d\n", vector_storage.size());
      // Create another vector from elbow joint to ball center.
      // This is needed to get dot product of hand_vector.
      Vector elbow_ball_vector(3);
      elbow_ball_vector = ball_center - elbow_joint;

  
      // We need to take dot product of hand_vector with elbow_ball_vector 
      // and this will give us the distance. If the result is less than zero then the hand vector
      // is pointing away from the ball.
      double distance, radius = 10.0;
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
    //printf("Position: %sa\n", bot->toString().c_str());
    //input.read(bot);
    //printf("Got message: %s\n", bot.toString().c_str());
  }
  input.close();
  return 0;
}
