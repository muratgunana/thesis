#include <string>
#include <yarp/os/Thread.h>
#include <yarp/os/Semaphore.h>
#include <yarp/sig/Image.h>
#include <yarp/os/BufferedPort.h>

using namespace yarp::sig;
using namespace yarp::os;

class ObjectDetectionThread : public yarp::os::Thread {

public:
  ObjectDetectionThread();
  virtual ~ObjectDetectionThread();
  
  /**
   * @brief run thraed main function.
   */
  virtual void run();

  /**
   * @brief detects the circles and particular colors from the image frame,
   */
  void objectDetection();

  /**
   * @brief suspends the calling thread until the current thread execution finishes.
   */
  void wait();

  /**
   * @brief interrupt any ongoing process or waiting thread.
   */ 
  void interrupt();

//private:
  BufferedPort<ImageOf<PixelRgb> > imagePort;
  bool interrupted;
  yarp::os::Semaphore semStart;
  yarp::os::Semaphore semDone;    
};
