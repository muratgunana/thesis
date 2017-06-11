#include <stdio.h>
#include <yarp/os/all.h>
#include <yarp/sig/all.h>

using namespace yarp::os;
using namespace yarp::sig;

int main(int argc, char* argv[]) {
  Network yarp;
  // Make a port for reading image.
  BufferedPort<ImageOf<PixelRgb> > imagePort;
  BufferedPort<Vector> targetPort;
  // Give the port a name.
  //imagePort.open("/image/in");
  targetPort.open("/target/out");
  //Network::connect("/icubSim/cam/left", "/image/in");
  while (true) {
    ImageOf<PixelRgb> *image = imagePort.read();
    if (image != NULL) {
      printf("The image size: %dx%d\n", image->width(), image->height());
      double xMean = 0;
      double yMean = 0;
      int ct = 0;
      for (int x = 0; x < image->width(); x++) {
        for (int y = 0; y < image->height(); y++) {
          PixelRgb& pixel = image->pixel(x,y);
          // Simple test for bluishness. 
          // Make sure blue level exceeds red and green by a factor of 2.
          if (pixel.b > pixel.r * 1.2 + 10 && pixel.b > pixel.g * 1.2 + 10) {
            // Let's find the average location of these pixels.
            xMean += x;
            yMean += y;
            ct++;
          }  
        }
      }
      if (ct > 0) {
        xMean /= ct;
        yMean /= ct;
      }
      if (ct > (image->width() / 20) * (image->height()/20)) {
        printf("Best guess at blue target: %g %g \n", xMean, yMean);
        Vector& v = targetPort.prepare();
   	v.resize(3);
	v[0] = xMean;
	v[1] = yMean;
	v[2] = 1; // A confidence value, always good to practice to add. We are sure of it.
	targetPort.write(); // Send data.
      }
    }
  }
  return 0;
}
