 #include <yarp/os/Network.h>
 #include <yarp/os/Vocab.h>
 #include <yarp/math/Math.h>
 #include <yarp/sig/Image.h>
 #include <string>
 #include <iostream>
 #ifdef OPENNI2_DRIVER_USES_NITE2
     #include "NiTE.h"
 #endif
 
 //included so that the enumeration can be accessed
 #include <yarp/dev/IOpenNI2DeviceDriver.h>
 
 #define USER_DETECTED_MSG "[USER_DETECTED]"
 #define USER_CALIBRATING_MSG "[USER_CALIBRATING]"
 #define USER_LOST_MSG "[USER_LOST]"
 #define USER_VOCAB VOCAB4('U','S','E','R')
 #define POSITION_VOCAB VOCAB3('P','O','S')
 #define ORIENTATION_VOCAB VOCAB3('O','R','I')
 #define DMAP_VOCAB VOCAB4('D','M','A','P')
 
 using namespace std;
 using namespace yarp::os;
 using namespace yarp::sig;
 
 #define MAX_USERS 10
 #define TOTAL_JOINTS 15
 
 class OpenNI2SkeletonData
 {
 public:
     OpenNI2SkeletonData();
     ~OpenNI2SkeletonData();
     void storeData(Bottle& b);
     void storeData(ImageOf<PixelRgb>& img);
     void storeData(ImageOf<PixelMono16>& img);
     Vector* getOrientation(int userID);
     Vector* getPosition(int userID);
     float* getOrientationConf(int userID);
     float* getPositionConf(int userID);
 
     nite::SkeletonState getSkeletonState(int userID);
     ImageOf<PixelMono16> getDepthFrame();
     ImageOf<PixelRgb> getImageFrame();
 private:
     typedef struct USER_SKELETON
     {
         nite::SkeletonState skeletonState;
         Vector skeletonPointsPos[TOTAL_JOINTS];
         Vector skeletonPointsOri[TOTAL_JOINTS];
         float skeletonPosConf[TOTAL_JOINTS];
         float skeletonOriConf[TOTAL_JOINTS];
         bool visible;
         int uID;
     }UserSkeleton;
     UserSkeleton *userSkeleton;
     ImageOf<PixelMono16> depthFrame;
     ImageOf<PixelRgb> imageFrame;
     void initUserSkeletons();
 };
// #endif
