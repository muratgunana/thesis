#include <stdio.h>
#include <cv.h>
#include <cvaux.h>
#include <highgui.h>
#include <yarp/sig/all.h>
#include <yarp/os/all.h>
#include <yarp/dev/all.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

using namespace yarp::os;
using namespace yarp::dev;
using namespace yarp::sig;
using namespace yarp::sig::draw;
using namespace yarp::sig::file;
using namespace cv;
int main(int argc, char *argv[]) {
  IGazeControl *igaze;

  printf("Copying YARP image to an OpenCV/IPL image\n");
  //cvImage = image->getImage(cvImage);
  ImageOf<PixelRgb> *image;
  //OpenCVGrabber grabber;
  //if (dev.isValid()) {
  Network yarp;
  BufferedPort<ImageOf<PixelRgb> > input;
  input.open("/receiver");
  Network::connect("/icubSim/cam/left","/receiver");
  printf("It is valid.\n");
  Mat imageFrame, gframe, baseframe;
  
  yarp::sig::Vector ang;
  //igaze->getAngles(ang);
  while (cv::waitKey(27) != 'Esc') { 
    image = input.read();
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
    HoughCircles( gframe, circles, CV_HOUGH_GRADIENT, 1, gframe.rows/8, 80, 15, 10, 60);

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
    yarp::sig::Vector px(2);
    px[0] = circles[0][0];
    px[1] = circles[0][1];
    igaze->lookAtMonoPixel(1, px, 1);
    imshow("Hough circles", baseframe);

    //printf("Showing OpenCV/IPL image\n");
    //cvNamedWindow("test",1);
    //cvShowImage("test",cvImage);
    //printf("Taking image back into YARP...\n");
    //ImageOf<PixelBgr> yarpReturnImage;
    //yarpReturnImage.wrapIplImage(cvImage);
    //yarp::sig::file::write(yarpReturnImage,"test.ppm");
    //printf("Saving YARP image to test.ppm\n");
    //cvWaitKey(3000);
    //cvDestroyWindow("test");
    //cvReleaseImage(&cvImage);
    //printf("...done\n");
  }   

   Mat frame;
   Mat grayframe;
   VideoCapture cap;
   cap.open(0);

   while(cv::waitKey(27) != 'Esc') {
     cap.read(frame);
     cvtColor(frame,grayframe, CV_BGR2GRAY);
     GaussianBlur(grayframe,grayframe,Size(9,9),2,2);
     vector<Vec3f> circles;

     // Apply the Hough Transform to find the circles
     HoughCircles( grayframe, circles, CV_HOUGH_GRADIENT, 1, grayframe.rows/8, 200, 100, 0, 0 );

        /// Draw the circles detected
        for( size_t i = 0; i < circles.size(); i++ )
        {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);
            // circle center
            circle( frame, center, 3, Scalar(0,255,0), -1, 8, 0 );
            // circle outline
            circle( frame, center, radius, Scalar(0,0,255), 3, 8, 0 );
            printf("Circle sizes: %d\n",radius);
        }           

        imshow("Hough circles",frame);
    }  
 return 0;
}
