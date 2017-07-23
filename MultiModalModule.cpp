#include "MultiModalModule.h"
#include <yarp/os/LogStream.h>
#include <yarp/sig/ImageDraw.h>
#include <yarp/os/Time.h>

using namespace yarp::os;
using namespace yarp::sig;


MultiModalModule::MultiModalModule() { }

MultiModalModule::~MultiModalModule() { }


bool MultiModalModule::configure(yarp::os::ResourceFinder &rf) {
  collisionThread = new CollisionDetectionThread();
  objectThread = new ObjectDetectionThread();

  collisionThread->skeletonPort.open("/receiver");
  Network::connect("/OpenNI2/userSkeleton:o","/receiver");
    
  // open a client interface to connect to the gaze server
  // we suppose that:
  // 1 - the iCub simulator is running;
  // 2 - the gaze server iKinGazeCtrl is running and
  //     launched with the following options: "--from configSim.ini"
  Property optGaze("(device gazecontrollerclient)");
  optGaze.put("remote","/iKinGazeCtrl");
  optGaze.put("local","/gaze_client");

  if (!collisionThread->clientGaze.open(optGaze))
    return false;
  // open the view
  collisionThread->clientGaze.view(collisionThread->igaze);
  objectThread->imagePort.open("/imagergb");
  Network::connect("/OpenNI2/imageFrame:o","/imagergb");
  printf("Starting threads...\n");
  collisionThread->start();
  objectThread->start();
  
  // everything is fine
  return true;
}


double MultiModalModule::getPeriod() {
  return 0.0;
}


bool MultiModalModule::updateModule() {    
  return true;
}

bool MultiModalModule::interruptModule() {
  collisionThread->interrupt();
  objectThread->interrupt();
  return true;
}

bool MultiModalModule::close() {
  objectThread->stop();
  delete objectThread;
  collisionThread->stop();
  delete collisionThread;
  return true;
}
