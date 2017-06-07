#include <yarp/os/Network.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Bottle.h>
#include <stdio.h>
#include <yarp/os/Vocab.h>
using namespace yarp::os;

void showBottle(Bottle& anUnknownBottle, int indentation = 0) {
    for (int i=0; i<anUnknownBottle.size(); i++) {
        for (int j=0; j<indentation; j++) { printf(" "); }
        printf("[%d]: ", i);
        Value& element = anUnknownBottle.get(i);
        switch (element.getCode()) {
        case BOTTLE_TAG_INT:
            printf("int %d\n", element.asInt());
            break;
        case BOTTLE_TAG_DOUBLE:
            printf("float %g\n", element.asDouble());
            break;
        case BOTTLE_TAG_STRING:
            printf("string \"%s\"\n", element.asString().c_str());
            break;
        case BOTTLE_TAG_BLOB:
            printf("binary blob of length %d\n", element.asBlobLength());
            break;
        case BOTTLE_TAG_VOCAB:
            printf("vocab [%s]\n", Vocab::decode(element.asVocab()).c_str());
            break;
        default:
            if (element.isList()) {
                Bottle *lst = element.asList();
                printf("list of %d elements\n", lst->size());
                showBottle(*lst,indentation+2);
            } else {
                printf("unrecognized type\n");
            }
            break;
        }
    }
}
 
int main(int argc, char* argv[]) {
  Network yarp;
  BufferedPort<Bottle> input;
  input.open("/receiver");
  Network::connect("/OpenNI2/userSkeleton:o","/receiver");
  while(true) {
    Bottle *bot = input.read();
    if (!bot->find("USER").isNull()) {
      printf("User: %d\n", bot->find("USER").asInt());
    }
    Bottle& pos = bot->findGroup("POS");
    printf("Pos-whole : %s\n", pos.toString().c_str());
    printf("Pos-size: %d\n", bot->size());
    for (int i=1; i < bot->size(); i++) {
      //printf("Pos[%d]: %d\n", i, bot->get(i).asInt());
    }
    showBottle(*bot);
    printf("Position: %sa\n", bot->toString().c_str());
    //input.read(bot);
    //printf("Got message: %s\n", bot.toString().c_str());
  }
  input.close();
  return 0;
}
