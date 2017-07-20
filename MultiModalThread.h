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
  void detect(yarp::sig::ImageOf<yarp::sig::PixelRgb>& image);

  /**
   * @brief suspends the calling thread until the current thread execution finishes.
   */
  void wait();

  /**
   * @brief interrupt any ongoing process or waiting thread.
   */ 
  void interrupt();

private:
  yarp::sig::ImageOf<PixelRgb>* image;
  bool interrupted;
  yarp::os::Semaphore semStart;
  yarp::os::Semaphore semDone;    
};
