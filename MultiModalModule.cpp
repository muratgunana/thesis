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
 
  // Start the threads.
  collisionThread->start();
  objectThread->start();
  
  // everything is fine
  return true;
}


double MultiModalModule::getPeriod() {
  return 0.0;
}


bool MultiModalModule::updateModule() {    
  // After pointing an object assign it's color so we can spot its color.
  objectThread->setColorCode(collisionThread->getColorCode());
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
