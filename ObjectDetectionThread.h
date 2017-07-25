#include <string>
#include <yarp/os/Thread.h>
#include <yarp/os/Semaphore.h>
#include <yarp/sig/Image.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Bottle.h>

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
   * @brief iniitialize thread.
   */
  virtual bool threadInit();
  
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
  
  /**
   * @brief setter for color_code property.
   */
  void setColorCode(int colorCode);
  
  /**
   * @brief getter for color_code property.
   */
  int getColorCode();

private:
  BufferedPort<ImageOf<PixelRgb> > imagePort;
  bool interrupted;
  int colorCode;
  yarp::os::Semaphore semStart;
  yarp::os::Semaphore semDone;    
};
