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

#include <yarp/os/LogStream.h>
#include <yarp/sig/ImageDraw.h>
#include <yarp/os/Time.h>

#include <yarp/os/RFModule.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/sig/Image.h>
#include "MultiModalThread.h"

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
  yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> > imagePort;
  MultiModalThread *objectDetector, *objectPointer;
  yarp::dev::PolyDriver clientGaze;
  yarp::dev::IGazeControl *igaze;
  yarp::os::BufferedPort<yarp::os::Bottle> skeletonPort;
  int count;
};
