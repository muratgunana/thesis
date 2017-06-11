#include <stdio.h>
#include <yarp/os/all.h>
#include <yarp/sig/all.h>

using namespace yarp::os;
using namespace yarp::sig;

int main(int argc, char* argv[]) {
  Network yarp;
  // Make a port for reading image.
  BufferedPort<ImageOf<PixelRgb> > imagePort;
  // Give the port a name.
  imagePort.open("/image/in");
  //Network::connect("/icubSim/cam/left", "/image/in");
  while (true) {
    ImageOf<PixelRgb> *image = imagePort.read();
    if (image != NULL) {
      printf("The image size: %dx%d\n", image->width(), image->height());
    }
  }
  return 0;
}
