#include <stdio.h>
#include <yarp/os/all.h>
#include <yarp/sig/all.h>

using namespace yarp::os;
using namespace yarp::sig;
int main (int argc, char *argv[]) {
  Network yarp;
  BufferedPort<Vector> targetPort;
  targetPort.open("/target/in");
  while (true) {
    Vector *target = targetPort.read(); // read a target.
    if (target !=NULL) {
      printf("We got a vector of size %d\n", target->size());
    }
  }
  return 0;
}
