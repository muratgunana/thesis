#include <yarp/os/Network.h>
#include <yarp/os/LogStream.h>
#include <yarp/os/ResourceFinder.h>

#include "MultiModalModule.h"

using namespace yarp::os;


int main(int argc, char * argv[])
{
    Network yarp;

    MultiModalModule module;
    ResourceFinder rf;
    rf.configure(argc, argv);
    
    module.runModule(rf);
    return 0;
}
