# Copyright: (C) 2010 RobotCub Consortium
# Author: Paul Fitzpatrick
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
# YARP needs CMake 2.8.9 or greater
cmake_minimum_required(VERSION 2.8.9)
# find YARP
find_package(YARP REQUIRED)
find_package(OpenCV REQUIRED)
# add YARP include directories
include_directories(${OPENCV_INCLUDE_DIRS})
include_directories(${YARP_INCLUDE_DIRS})
# set up our program
add_executable(device_detector device_detector.cpp)
# link with YARP libraries
target_link_libraries(device_detector ${YARP_LIBRARIES} ${OpenCV_LIBS})
