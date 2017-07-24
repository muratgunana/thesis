#include <cv.h>
#include <cvaux.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include "ObjectDetectionThread.h"
#include <yarp/os/LogStream.h>
#include <yarp/os/Network.h>
#include <yarp/sig/Image.h>
#include <yarp/os/Time.h>
#include <yarp/os/Property.h>

using namespace yarp::sig;
using namespace std;

ObjectDetectionThread::ObjectDetectionThread()
  : semStart(0), semDone(0), interrupted(false) { }

ObjectDetectionThread::~ObjectDetectionThread() { }

void ObjectDetectionThread::run() {
  while (!isStopping()) {
    printf("Hello from thread2\n");
    ObjectDetectionThread::objectDetection();
    Time::delay(0);
  }
}

bool ObjectDetectionThread::threadInit() {

  // Open the port and connect OPenNI RGB port to stream.
  this->imagePort.open("/imagergb");
  Network::connect("/OpenNI2/imageFrame:o","/imagergb");
 
 return true;
}

void ObjectDetectionThread::objectDetection() {
  
  ImageOf<PixelRgb> *image;
  while (cv::waitKey(27) != 'Esc') {
    
    printf("Object color code: %d\n", this->colorCode);
    cv::Mat orig_image;
    image = imagePort.read();
    IplImage *cvImage = cvCreateImage(cvSize(image->width(),  
                                             image->height()), 
                                      IPL_DEPTH_8U, 3 );
    cvCvtColor((IplImage*)image->getIplImage(), cvImage, CV_RGB2BGR);
    cv::Mat bgr_image(cvImage);

    // Read hue colors from the file.
    Property hueBottle; 
    hueBottle.fromConfigFile("colorcode.ini");
    
    string color = "red";
    int objectColor = this->getColorCode();
    switch (objectColor) {
      case 1:
        color = "red";
        break;
      case 2:
        color = "green";
        break;

      case 3:
        color = "purple";
        break;

      default:
        color = "red";
        break;
    }
    orig_image = bgr_image.clone();

    cv::medianBlur(bgr_image, bgr_image, 3);
      
    // Convert input image to HSV
    cv::Mat hsv_image;
    cv::cvtColor(bgr_image, hsv_image, cv::COLOR_BGR2HSV);
 
    // Threshold the HSV image, keep only the red pixels
    cv::Mat lower_hue_range;
    cv::Mat upper_hue_range;
    cv::inRange(hsv_image, cv::Scalar(hueBottle.findGroup(color + "_lower").get(1).asInt(), 100, 100), cv::Scalar(hueBottle.findGroup(color + "_lower").get(2).asInt(), 255, 255), lower_hue_range);
    cv::inRange(hsv_image, cv::Scalar(hueBottle.findGroup(color + "_upper").get(1).asInt(), 100, 100), cv::Scalar(hueBottle.findGroup(color + "_upper").get(2).asInt(), 255, 255), upper_hue_range);

    // Combine the above two images
    cv::Mat hue_image;
    cv::addWeighted(lower_hue_range, 1.0, upper_hue_range, 1.0, 0.0, hue_image);
    cv::GaussianBlur(hue_image, hue_image, cv::Size(9, 9), 2, 2);

    // Use the Hough transform to detect circles in the combined threshold image
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(hue_image, circles, CV_HOUGH_GRADIENT, 1, hue_image.rows/8, 200, 40, 0, 0);

    // Loop over all detected circles and outline them on the original image
    //if (color_offset != 0) {
    for(size_t current_circle = 0; current_circle < circles.size(); ++current_circle) {
      cv::Point center(cvRound(circles[current_circle][0]), cvRound(circles[current_circle][1]));
      int radius = cvRound(circles[current_circle][2]);

      cv::circle(orig_image, center, radius, cv::Scalar(0, 255, 0), 5);
    }
    // }

    // Show images
    cv::namedWindow("Threshold lower image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Threshold lower image", lower_hue_range);
    cv::namedWindow("Threshold upper image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Threshold upper image", upper_hue_range);
    cv::namedWindow("Combined threshold images", cv::WINDOW_AUTOSIZE);
    cv::imshow("Combined threshold images", hue_image);
    cv::namedWindow("Detected red circles on the input image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Detected red circles on the input image", orig_image);
    //return true; 
  // Signal the thread to start processing.
  //semStart.post();
  }
}

void ObjectDetectionThread::wait() {
  semDone.wait();
}

void ObjectDetectionThread::interrupt() {
  interrupted = true;
  semStart.post();
}

void ObjectDetectionThread::setColorCode(int colorCode) {
  this->colorCode = colorCode;
}

int ObjectDetectionThread::getColorCode() {
  return this->colorCode;
}
