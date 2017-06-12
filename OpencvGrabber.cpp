#include <stdio.h>
#include <cv.h>
#include <cvaux.h>
#include <highgui.h>
#include <yarp/sig/all.h>
#include <yarp/os/all.h>
#include <yarp/dev/all.h>

using namespace yarp::os;
using namespace yarp::dev;
using namespace yarp::sig;
using namespace yarp::sig::draw;
using namespace yarp::sig::file;
int main(int argc, char *argv[]) {
  printf("Copying YARP image to an OpenCV/IPL image\n");
  //cvImage = image->getImage(cvImage);
  ImageOf<PixelRgb> *image;
  //OpenCVGrabber grabber;
  //if (dev.isValid()) {
  Network yarp;
  BufferedPort<ImageOf<PixelRgb> > input;
  input.open("/receiver");
  Network::connect("/grabber","/receiver");
  printf("It is valid.\n");
      
  image = input.read();
  //}
    IplImage *cvImage = cvCreateImage(cvSize(image->width(),  
                                             image->height()), 
                                      IPL_DEPTH_8U, 3 );
    cvCvtColor((IplImage*)image->getIplImage(), cvImage, CV_RGB2BGR);
    printf("Showing OpenCV/IPL image\n");
    cvNamedWindow("test",1);
    cvShowImage("test",cvImage);
    printf("Taking image back into YARP...\n");
    ImageOf<PixelBgr> yarpReturnImage;
    yarpReturnImage.wrapIplImage(cvImage);
    yarp::sig::file::write(yarpReturnImage,"test.ppm");
    printf("Saving YARP image to test.ppm\n");
    cvWaitKey(3000);
    cvDestroyWindow("test");
    cvReleaseImage(&cvImage);
    printf("...done\n");
    return 0;
}
