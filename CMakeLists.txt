# Copyright: (C) 2017 Robotics Lab - MAC Heriot-Watt University
# Author: Murat Gunana
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

add_executable(main CollisionDetectionThread.cpp ObjectDetectionThread.cpp MultiModalModule.cpp main.cpp)

# link with YARP libraries
target_link_libraries(main ${YARP_LIBRARIES} ${OpenCV_LIBS})
