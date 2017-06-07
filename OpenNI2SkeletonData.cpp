#include <yarp/os/Network.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Bottle.h>
#include <stdio.h>

using namespace yarp::os;
 
int main(int argc, char* argv[]) {
  Network yarp;
  BufferedPort<Bottle> input;
  input.open("/receiver");
  Network::connect("/OpenNI2/userSkeleton:o","/receiver");
  while(true) {
    Bottle *bot = input.read();
    if (!bot->find("ORI").isNull()) {
      printf("Pos: %s\n", bot->find("ORI").asString());
    }
    Bottle& pos = bot->findGroup("ORI");
    printf("ORI : %s\n", pos.toString().c_str());
    printf("Size: %d\n", bot->size());
    for (int i=1; i < bot->size(); i++) {
      printf("Pos[%d]:\n", *bot->get(i).asInt());
    }
    printf("Position: %sa\n", bot->toString().c_str());
    //input.read(bot);
    //printf("Got message: %s\n", bot.toString().c_str());
  }
  input.close();
  return 0;
}
