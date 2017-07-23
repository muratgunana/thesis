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
#include <yarp/sig/Image.h>
#include <yarp/os/Time.h>

using namespace yarp::sig;

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

void ObjectDetectionThread::objectDetection() {
  //ObjectDetectionThread::image = &image;
  
  printf("Copying YARP image to an OpenCV/IPL image\n");
  ImageOf<PixelRgb> *image;
  while (cv::waitKey(27) != 'Esc') {
    
  std::cout << "Image shows ..." << std::endl;
    cv::Mat orig_image;
    image = imagePort.read();
    IplImage *cvImage = cvCreateImage(cvSize(image->width(),  
                                             image->height()), 
                                      IPL_DEPTH_8U, 3 );
    cvCvtColor((IplImage*)image->getIplImage(), cvImage, CV_RGB2BGR);
    cv::Mat bgr_image(cvImage);
    
    orig_image = bgr_image.clone();

    cv::medianBlur(bgr_image, bgr_image, 3);
      
    // Convert input image to HSV
    cv::Mat hsv_image;
    cv::cvtColor(bgr_image, hsv_image, cv::COLOR_BGR2HSV);
 
    // Threshold the HSV image, keep only the red pixels
    cv::Mat lower_red_hue_range;
    cv::Mat upper_red_hue_range;
    cv::inRange(hsv_image, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), lower_red_hue_range);
    cv::inRange(hsv_image, cv::Scalar(160, 100, 100), cv::Scalar(179, 255, 255), upper_red_hue_range);

    // Combine the above two images
    cv::Mat red_hue_image;
    cv::addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_image);
    cv::GaussianBlur(red_hue_image, red_hue_image, cv::Size(9, 9), 2, 2);

    // Use the Hough transform to detect circles in the combined threshold image
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(red_hue_image, circles, CV_HOUGH_GRADIENT, 1, red_hue_image.rows/8, 200, 40, 0, 0);

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
    cv::imshow("Threshold lower image", lower_red_hue_range);
    cv::namedWindow("Threshold upper image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Threshold upper image", upper_red_hue_range);
    cv::namedWindow("Combined threshold images", cv::WINDOW_AUTOSIZE);
    cv::imshow("Combined threshold images", red_hue_image);
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
