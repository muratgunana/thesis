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
    HoughCircles( grayframe, circles, CV_HOUGH_GRADIENT, 2, grayframe.rows/4, 200, 100, 0, 0);

    // Draw the circles detected
    for( size_t i = 0; i < circles.size(); i++ ) {
      Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
      int radius = cvRound(circles[i][2]);
      // circle center
      circle( frame, center, 3, Scalar(0,255,0), -1, 8, 0 );
      // circle outline
      circle( frame, center, radius, Scalar(0,0,255), 3, 8, 0 );
      printf("Circle sizes: %d\n",radius);
      printf("Estimated distance: %f\n", 331.58*5.7/radius);
    }           
    
    imshow("Hough circles",frame);
  }  
 
  return 0;
}
