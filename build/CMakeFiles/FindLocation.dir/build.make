# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/muri/thesis

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/muri/thesis/build

# Include any dependencies generated for this target.
include CMakeFiles/FindLocation.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/FindLocation.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/FindLocation.dir/flags.make

CMakeFiles/FindLocation.dir/FindLocation.cpp.o: CMakeFiles/FindLocation.dir/flags.make
CMakeFiles/FindLocation.dir/FindLocation.cpp.o: ../FindLocation.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/muri/thesis/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/FindLocation.dir/FindLocation.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FindLocation.dir/FindLocation.cpp.o -c /home/muri/thesis/FindLocation.cpp

CMakeFiles/FindLocation.dir/FindLocation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FindLocation.dir/FindLocation.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/muri/thesis/FindLocation.cpp > CMakeFiles/FindLocation.dir/FindLocation.cpp.i

CMakeFiles/FindLocation.dir/FindLocation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FindLocation.dir/FindLocation.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/muri/thesis/FindLocation.cpp -o CMakeFiles/FindLocation.dir/FindLocation.cpp.s

CMakeFiles/FindLocation.dir/FindLocation.cpp.o.requires:

.PHONY : CMakeFiles/FindLocation.dir/FindLocation.cpp.o.requires

CMakeFiles/FindLocation.dir/FindLocation.cpp.o.provides: CMakeFiles/FindLocation.dir/FindLocation.cpp.o.requires
	$(MAKE) -f CMakeFiles/FindLocation.dir/build.make CMakeFiles/FindLocation.dir/FindLocation.cpp.o.provides.build
.PHONY : CMakeFiles/FindLocation.dir/FindLocation.cpp.o.provides

CMakeFiles/FindLocation.dir/FindLocation.cpp.o.provides.build: CMakeFiles/FindLocation.dir/FindLocation.cpp.o


# Object files for target FindLocation
FindLocation_OBJECTS = \
"CMakeFiles/FindLocation.dir/FindLocation.cpp.o"

# External object files for target FindLocation
FindLocation_EXTERNAL_OBJECTS =

FindLocation: CMakeFiles/FindLocation.dir/FindLocation.cpp.o
FindLocation: CMakeFiles/FindLocation.dir/build.make
FindLocation: /usr/local/lib/libYARP_dev.so.2.3.68
FindLocation: /usr/local/lib/libYARP_name.so.2.3.68
FindLocation: /usr/local/lib/libYARP_init.so.2.3.68
FindLocation: /usr/lib/x86_64-linux-gnu/libopencv_videostab.so.2.4.9
FindLocation: /usr/lib/x86_64-linux-gnu/libopencv_ts.so.2.4.9
FindLocation: /usr/lib/x86_64-linux-gnu/libopencv_superres.so.2.4.9
FindLocation: /usr/lib/x86_64-linux-gnu/libopencv_stitching.so.2.4.9
FindLocation: /usr/lib/x86_64-linux-gnu/libopencv_ocl.so.2.4.9
FindLocation: /usr/lib/x86_64-linux-gnu/libopencv_gpu.so.2.4.9
FindLocation: /usr/lib/x86_64-linux-gnu/libopencv_contrib.so.2.4.9
FindLocation: /usr/local/lib/libYARP_math.so.2.3.68
FindLocation: /usr/local/lib/libYARP_sig.so.2.3.68
FindLocation: /usr/local/lib/libYARP_OS.so.2.3.68
FindLocation: /usr/lib/x86_64-linux-gnu/libopencv_photo.so.2.4.9
FindLocation: /usr/lib/x86_64-linux-gnu/libopencv_legacy.so.2.4.9
FindLocation: /usr/lib/x86_64-linux-gnu/libopencv_video.so.2.4.9
FindLocation: /usr/lib/x86_64-linux-gnu/libopencv_objdetect.so.2.4.9
FindLocation: /usr/lib/x86_64-linux-gnu/libopencv_ml.so.2.4.9
FindLocation: /usr/lib/x86_64-linux-gnu/libopencv_calib3d.so.2.4.9
FindLocation: /usr/lib/x86_64-linux-gnu/libopencv_features2d.so.2.4.9
FindLocation: /usr/lib/x86_64-linux-gnu/libopencv_highgui.so.2.4.9
FindLocation: /usr/lib/x86_64-linux-gnu/libopencv_imgproc.so.2.4.9
FindLocation: /usr/lib/x86_64-linux-gnu/libopencv_flann.so.2.4.9
FindLocation: /usr/lib/x86_64-linux-gnu/libopencv_core.so.2.4.9
FindLocation: CMakeFiles/FindLocation.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/muri/thesis/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable FindLocation"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/FindLocation.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/FindLocation.dir/build: FindLocation

.PHONY : CMakeFiles/FindLocation.dir/build

CMakeFiles/FindLocation.dir/requires: CMakeFiles/FindLocation.dir/FindLocation.cpp.o.requires

.PHONY : CMakeFiles/FindLocation.dir/requires

CMakeFiles/FindLocation.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/FindLocation.dir/cmake_clean.cmake
.PHONY : CMakeFiles/FindLocation.dir/clean

CMakeFiles/FindLocation.dir/depend:
	cd /home/muri/thesis/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/muri/thesis /home/muri/thesis /home/muri/thesis/build /home/muri/thesis/build /home/muri/thesis/build/CMakeFiles/FindLocation.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/FindLocation.dir/depend
