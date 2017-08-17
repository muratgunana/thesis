/**
 * Copyright: (C) 2017 Robotics Lab - MAC Heriot-Watt University
 * Author: Murat Gunana
 */

#include <yarp/dev/GazeControl.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/os/LogStream.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Bottle.h>
#include <yarp/sig/Image.h>
#include "ObjectDetectionThread.h"
#include "CollisionDetectionThread.h"

using namespace yarp::os;
using namespace yarp::sig;

class MultiModalModule : public yarp::os::RFModule {
public:

  MultiModalModule();
  virtual ~MultiModalModule();

  virtual bool configure(yarp::os::ResourceFinder &rf);

  virtual double getPeriod();

  virtual bool updateModule();

  virtual bool interruptModule();

  virtual bool close();

private:

  double period;
  int threadCount;
  ObjectDetectionThread *objectThread;
  CollisionDetectionThread *collisionThread;
  int count;
};
