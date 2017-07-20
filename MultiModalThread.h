#include <string>
#include <yarp/os/Thread.h>
#include <yarp/os/Semaphore.h>
#include <yarp/sig/Image.h>

class MultiModalThread : public yarp::os::Thread {

public:
  MultiModalThread();
  virtual ~MultiModalThread();
  
  /**
   * @brief run thraed main function.
   */
  virtual void run();

  /**
   * @brief detects the circles and particular colors from the image frame,
   */
  void objectDetection(yarp::sig::ImageOf<yarp::sig::PixelRgb>& image);

  /**
   * @brief extracts skeleton data from 3D and handles collision detection in regard to objects.
   */
  void objectPointer();

  /**
   * @brief suspends the calling thread until the current thread execution finishes.
   */
  void wait();

  /**
   * @brief interrupt any ongoing process or waiting thread.
   */ 
  void interrupt();

private:
  yarp::sig::ImageOf<yarp::sig::PixelRgb>* image;
  bool interrupted;
  yarp::os::Semaphore semStart;
  yarp::os::Semaphore semDone;    
};
