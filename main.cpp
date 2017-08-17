/**
 * Copyright: (C) 2017 Robotics Lab - MAC Heriot-Watt University
 * Author: Murat Gunana
 */

#include <yarp/os/Network.h>
#include <yarp/os/LogStream.h>
#include <yarp/os/ResourceFinder.h>
#include "MultiModalModule.h"

using namespace yarp::os;

int main(int argc, char * argv[])
{
    Network yarp;
    
    // Create an instance.
    MultiModalModule module;

    // Create an instance in order to handle multimodal instance gracefully.
    ResourceFinder rf;
    rf.configure(argc, argv);
    module.runModule(rf);

    return 0;
}
