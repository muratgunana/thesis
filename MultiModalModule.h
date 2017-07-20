#include <yarp/os/RFModule.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/sig/Image.h>
#include <MultiModalThread.h>

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
  MultiModalThread objectDetector, objectPointer;
  PolyDriver clientGaze;
  IGazeControl *igaze;
  BufferedPort<Bottle> skeletonPort;
  BufferedPort<ImageOf<PixelRgb> > imagePort;
  int count;
  int color_code = COLOR_CODE_OFFSET;
};
