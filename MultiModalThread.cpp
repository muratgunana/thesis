#include "MultiModalThread.h"
#include <yarp/os/LogStream.h>
#include <yarp/sig/Image.h>
using namespace yarp::os;
using namespace yarp::sig;

MultiModalThread::MultiModalThread()
  : semStart(0), semDone(0), interrupted(false) { }

MultiModalThread::~MultiModalThread() { }

void MultiModalThread::run() {
  // Loop until the thread is running.
  while (!isStopping()) {
    // Wait to start the processing.
    semStart.wait();
    if (interrupted) {
      semDone.post();
      return;
    }
    // @TODO: Think of any possible image processing.
    
    // Let other know the thread already processed.
    semDone.post();
  }
}

void MultiModalThread::objectDetection(yarp::sig::ImageOf<yarp::sig::PixelRgb>& image) {
  MultiModalThread::image = &image;
 
  // Signal the thread to start processing.
  semStart.post();
}

void MultiModalThread::objectPointer() {

}

void MultiModalThread::wait() {
  semDone.wait();
}

void MultiModalThread::interrupt() {
  interrupted = true;
  semStart.post();
}
