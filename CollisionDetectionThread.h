#include <string>
#include <yarp/os/Thread.h>
#include <yarp/os/Semaphore.h>
#include <yarp/sig/Image.h>

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

#define COLOR_CODE_OFFSET 0
#define RED               1
#define GREEN             2
#define PURPLE            3

using namespace yarp::os;
using namespace yarp::dev;
using namespace yarp::sig;
using namespace yarp::math;

class CollisionDetectionThread : public yarp::os::Thread {

public:
  CollisionDetectionThread();
  virtual ~CollisionDetectionThread();
  
  /**
   * @brief run thraed main function.
   */
  virtual void run();
  
  /**
   * @brief iniitialize thread.
   */
  virtual bool threadInit();

  /**
   * @brief extracts skeleton data from 3D and handles collision detection in regard to objects.
   */
  void collisionDetector();
  
  /**
   * @brief show the bottle data.
   */
  Bottle showBottle(Bottle& bottle, int indentation);
  
  /**
   * @brief setter for color_code property.
   */
  void setColorCode(int colorCode);

  /**
   * @brief getter for color_code property.
   */
  int getColorCode();


  /**
   * @brief suspends the calling thread until the current thread execution finishes.
   */
  void wait();

  /**
   * @brief interrupt any ongoing process or waiting thread.
   */ 
  void interrupt();

private:
  PolyDriver clientGaze;
  IGazeControl *igaze;
  BufferedPort<Bottle> skeletonPort;
  int colorCode;
  bool interrupted;
  yarp::os::Semaphore semStart;
  yarp::os::Semaphore semDone;    
};
