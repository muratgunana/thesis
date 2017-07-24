# Thesis
MSc project code
# About
This is an ongoing project for my master thesis that is to build a framework in YARP in order to implement the scenario below to an iCub robot.

 The robot performs a task with assistance of a human agent. The human points an object on the scene by describing it verbally. The robot then uses a 3D sensor to differentiate human arm as a vector and calculates with collision detection (which is popularly implemented in gaming) where it points to by verifying with speech inputs. After that stage, it performs object recognition and is required to grab the object and put in a box within its reach. The task is modelled as the collaboration that a robot being assistant to a human in a kitchen.

# Requirements for setup

1. Install YARP from source from http://www.yarp.it/install_yarp_linux.html. You need to make sure that OpenNIDeviceServer modules are enabled.

2. Install iCub simulation from http://wiki.icub.org/wiki/ICub_Software_Installation.

3. You also need to download OpenNI opensource libraries and reference them in your yarp configuration. See the link for details http://wiki.icub.org/wiki/OpenNI2.

# Run

1. Launch yarpserver

2. Launch iCub simulation with `iCub_SIM` within `/thesis` directory.

3. Launch OpenNIDeviceServer by command `yarpdev --device OpenNI2DeviceServer` within `/build` directory.

4. Initialize iKinGazeCtrl with `iKinGazeCtrl --from configSim.ini`.

5. Compile the project within /build directory and run `./main` executable file.

# Further Info

Please note that there are some experimental code within the project. The main files are `CollisionDetectionThread.*`, `ObjectDetectionThread.*`,`MultiModalModule.*` and `main.cpp`.

# Update

This file will be updated regularly in accordance with the project progression.  
