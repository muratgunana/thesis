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
#add_executable(OpenNI2SkeletonData OpenNI2SkeletonData.cpp)
#add_executable(MultiModalThread MultiModalThread.cpp)
#add_executable(MultiModalModule MultiModalModule.cpp)
#add_executable(main main.cpp)

add_executable(main MultiModalThread.cpp MultiModalModule.cpp main.cpp)
add_executable(thread Thread.cpp)
#add_executable(ObjectDetection ObjectDetection.cpp)
#add_executable(FindLocation FindLocation.cpp)
#add_executable(LookAtLocation LookAtLocation.cpp)
#add_executable(OpencvGrabber OpencvGrabber.cpp)
#add_executable(OpencvCamera OpencvCamera.cpp)

# link with YARP librariesi
target_link_libraries(main ${YARP_LIBRARIES} ${OpenCV_LIBS})
target_link_libraries(thread ${YARP_LIBRARIES} ${OpenCV_LIBS})
#target_link_libraries(ObjectDetection ${YARP_LIBRARIES} ${OpenCV_LIBS})
#target_link_libraries(OpenNI2SkeletonData ${YARP_LIBRARIES} ${OpenCV_LIBS})
#target_link_libraries(MultiModalThread ${YARP_LIBRARIES} ${OpenCV_LIBS})
#target_link_libraries(MultiModalModule ${YARP_LIBRARIES} ${OpenCV_LIBS})
#target_link_libraries(main ${YARP_LIBRARIES} ${OpenCV_LIBS})
#target_link_libraries(FindLocation ${YARP_LIBRARIES} ${OpenCV_LIBS})
#target_link_libraries(LookAtLocation ${YARP_LIBRARIES} ${OpenCV_LIBS})
#target_link_libraries(OpencvGrabber ${YARP_LIBRARIES} ${OpenCV_LIBS})
#target_link_libraries(OpencvCamera ${YARP_LIBRARIES} ${OpenCV_LIBS})
