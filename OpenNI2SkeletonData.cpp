#include <yarp/os/Network.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Bottle.h>

#include <yarp/sig/Vector.h>
#include <yarp/sig/Matrix.h>
#include <yarp/math/api.h>
#include <yarp/math/Quaternion.h>
#include <yarp/math/Math.h>
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
          if (i == 38 || i == 50) {
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
  printf("Newlist: %s\n", bot.toString().c_str());
  return bot;
}
 
int main(int argc, char* argv[]) {
  Network yarp;
  BufferedPort<Bottle> input;
  input.open("/receiver");
  Network::connect("/OpenNI2/userSkeleton:o","/receiver");
  Vector right_elbow(3), right_hand(3);
  while(true) {
    Bottle *bot = input.read();
    if (!bot->find("USER").isNull()) {
      printf("User: %d\n", bot->find("USER").asInt());
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
    
    for (int i = 0; i < bottle.size(); i++) {
      Value& element_list = bottle.get(i);
      Bottle *lst = element_list.asList();
      for (int j = 0; j < lst->size(); j++) {
        Value& element = lst->get(j);
        if (i == 0) {
          right_elbow[j] = element.asDouble();
        }
        if (i == 1) {
          right_hand[j] = element.asDouble();
        }
      }
    }
    double distance;
    distance = dot(right_elbow, right_hand);
    printf("Distance: %f\n", distance);
    //printf("Position: %sa\n", bot->toString().c_str());
    //input.read(bot);
    //printf("Got message: %s\n", bot.toString().c_str());
  }
  input.close();
  return 0;
}
