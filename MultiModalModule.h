#include <yarp/dev/GazeControl.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/os/LogStream.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Bottle.h>
#include <yarp/sig/Image.h>
#include "MultiModalThread.h"

class MultiModalModule : public yarp::os::RFModule {
public:

  MultiModalModule();
  virtual ~MultiModalModule();

  virtual bool configure(yarp::os::ResourceFinder &rf);

  virtual double getPeriod();

  virtual bool updateModule();

  yarp::os::Bottle showBottle(yarp::os::Bottle& anUnknownBottle, int indentation = 0);

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
  int color_code = 0;
};
