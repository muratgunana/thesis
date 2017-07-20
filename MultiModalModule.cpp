#include <cv.h>
#include <cvaux.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include <yarp/os/Network.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Bottle.h>
#include <yarp/sig/all.h>
#include <yarp/os/Property.h>
#include <yarp/os/RFModule.h>
#include <yarp/sig/Vector.h>
#include <yarp/sig/Matrix.h>
#include <yarp/math/api.h>
#include <yarp/math/Quaternion.h>
#include <yarp/math/Math.h>
#include <yarp/dev/GazeControl.h>
#include <yarp/dev/PolyDriver.h>

#include <iomanip>
#include <iostream>
#include <map>
#include <stdio.h>
#include <yarp/os/Vocab.h>

#include <MultiModalModule.h>
#include <yarp/os/LogStream.h>
#include <yarp/sig/ImageDraw.h>
#include <yarp/os/Time.h>

#define COLOR_CODE_OFFSET 0;
#define RED               1;
#define PURPLE            2;
#define GREEN             3;

using namespace std;
using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;
using namespace yarp::math;
using namespace yarp::sig::draw;
using namespace yarp::sig::file;
using namespace cv;


MultiModalModule::MultiModalModule() { }

MultiModalModule::~MultiModalModule() { }


bool MultiModalModule::configure(yarp::os::ResourceFinder &rf) {

    yInfo()<<"Configuring the codedc module...";

    threadsCount = rf.check("threads", Value(1)).asInt();
    if(threadsCount <1) {
        yError()<< "Number of threads should be >= 1)";
        return false;
    }


    // create and start the object detector and pointer threads
    objectDetector = new MultiModalThread();
    objectDetector.start();

    objectPointer = new MultiModalThread();
    objectPointer.start();

    count=0;
    // optional, attach a port to the module
    // so that messages received from the port are redirected
    // to the respond method
    skeletonPort.open("/receiver");
    imagePort.open("/imagergb");
    Network::connect("/OpenNI2/imageFrame:o","/imagergb");
    Network::connect("/OpenNI2/userSkeleton:o","/receiver");

    // open a client interface to connect to the gaze server
    // we suppose that:
    // 1 - the iCub simulator is running;
    // 2 - the gaze server iKinGazeCtrl is running and
    //     launched with the following options: "--from configSim.ini"
    Property optGaze("(device gazecontrollerclient)");
    optGaze.put("remote","/iKinGazeCtrl");
    optGaze.put("local","/gaze_client");

    if (!clientGaze.open(optGaze))
      return false;
    // open the view
    clientGaze.view(igaze);

    // everything is fine
    return true;
}


double MultiModalModule::getPeriod() {
    return 0.0;
}


bool MultiModalModule::updateModule() {    

    // create an empty image
    ImageOf<PixelRgb>& img = imagePort.prepare();
    img.resize(imgWidth, imgHeight);

    double t1 = Time::now();

    // calling paint and running the threads
    // devide image regions for each thread
    for(int i=0; i<threadsCount; i++) {
        int xi = i * imgWidth/threadsCount;
        painters[i]->paint(img, xi, 0, imgWidth/threadsCount, imgHeight);
    }

    // waiting for the painter to do their jobs
    for(int i=0; i<threadsCount; i++)
        painters[i]->wait();

    // print the total proccessing time
    yInfo()<<"Processing time "<<(Time::now()-t1)*1000.0<<"ms";

    // write the image to the port
    imagePort.write();
    return true;
}


bool MultiModalModule::interruptModule() {
    for(int i=0; i<painters.size(); i++) {
        painters[i]->interrupt();
    }
    return true;
}


bool MultiModalModule::close() {
    yInfo()<<"Closing MultiModalModule";
    for(int i=0; i<painters.size(); i++) {
        painters[i]->stop();
        delete painters[i];
    }
    imagePort.close();
    return true;
}
