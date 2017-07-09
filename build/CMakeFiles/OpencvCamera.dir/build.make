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
include CMakeFiles/OpencvCamera.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/OpencvCamera.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/OpencvCamera.dir/flags.make

CMakeFiles/OpencvCamera.dir/OpencvCamera.cpp.o: CMakeFiles/OpencvCamera.dir/flags.make
CMakeFiles/OpencvCamera.dir/OpencvCamera.cpp.o: ../OpencvCamera.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/muri/thesis/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/OpencvCamera.dir/OpencvCamera.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/OpencvCamera.dir/OpencvCamera.cpp.o -c /home/muri/thesis/OpencvCamera.cpp

CMakeFiles/OpencvCamera.dir/OpencvCamera.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/OpencvCamera.dir/OpencvCamera.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/muri/thesis/OpencvCamera.cpp > CMakeFiles/OpencvCamera.dir/OpencvCamera.cpp.i

CMakeFiles/OpencvCamera.dir/OpencvCamera.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/OpencvCamera.dir/OpencvCamera.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/muri/thesis/OpencvCamera.cpp -o CMakeFiles/OpencvCamera.dir/OpencvCamera.cpp.s

CMakeFiles/OpencvCamera.dir/OpencvCamera.cpp.o.requires:

.PHONY : CMakeFiles/OpencvCamera.dir/OpencvCamera.cpp.o.requires

CMakeFiles/OpencvCamera.dir/OpencvCamera.cpp.o.provides: CMakeFiles/OpencvCamera.dir/OpencvCamera.cpp.o.requires
	$(MAKE) -f CMakeFiles/OpencvCamera.dir/build.make CMakeFiles/OpencvCamera.dir/OpencvCamera.cpp.o.provides.build
.PHONY : CMakeFiles/OpencvCamera.dir/OpencvCamera.cpp.o.provides

CMakeFiles/OpencvCamera.dir/OpencvCamera.cpp.o.provides.build: CMakeFiles/OpencvCamera.dir/OpencvCamera.cpp.o


# Object files for target OpencvCamera
OpencvCamera_OBJECTS = \
"CMakeFiles/OpencvCamera.dir/OpencvCamera.cpp.o"

# External object files for target OpencvCamera
OpencvCamera_EXTERNAL_OBJECTS =

OpencvCamera: CMakeFiles/OpencvCamera.dir/OpencvCamera.cpp.o
OpencvCamera: CMakeFiles/OpencvCamera.dir/build.make
OpencvCamera: /usr/local/lib/libYARP_dev.so.2.3.68
OpencvCamera: /usr/local/lib/libYARP_name.so.2.3.68
OpencvCamera: /usr/local/lib/libYARP_init.so.2.3.68
OpencvCamera: /usr/lib/x86_64-linux-gnu/libopencv_videostab.so.2.4.9
OpencvCamera: /usr/lib/x86_64-linux-gnu/libopencv_ts.so.2.4.9
OpencvCamera: /usr/lib/x86_64-linux-gnu/libopencv_superres.so.2.4.9
OpencvCamera: /usr/lib/x86_64-linux-gnu/libopencv_stitching.so.2.4.9
OpencvCamera: /usr/lib/x86_64-linux-gnu/libopencv_ocl.so.2.4.9
OpencvCamera: /usr/lib/x86_64-linux-gnu/libopencv_gpu.so.2.4.9
OpencvCamera: /usr/lib/x86_64-linux-gnu/libopencv_contrib.so.2.4.9
OpencvCamera: /usr/local/lib/libYARP_math.so.2.3.68
OpencvCamera: /usr/local/lib/libYARP_sig.so.2.3.68
OpencvCamera: /usr/local/lib/libYARP_OS.so.2.3.68
OpencvCamera: /usr/lib/x86_64-linux-gnu/libopencv_photo.so.2.4.9
OpencvCamera: /usr/lib/x86_64-linux-gnu/libopencv_legacy.so.2.4.9
OpencvCamera: /usr/lib/x86_64-linux-gnu/libopencv_video.so.2.4.9
OpencvCamera: /usr/lib/x86_64-linux-gnu/libopencv_objdetect.so.2.4.9
OpencvCamera: /usr/lib/x86_64-linux-gnu/libopencv_ml.so.2.4.9
OpencvCamera: /usr/lib/x86_64-linux-gnu/libopencv_calib3d.so.2.4.9
OpencvCamera: /usr/lib/x86_64-linux-gnu/libopencv_features2d.so.2.4.9
OpencvCamera: /usr/lib/x86_64-linux-gnu/libopencv_highgui.so.2.4.9
OpencvCamera: /usr/lib/x86_64-linux-gnu/libopencv_imgproc.so.2.4.9
OpencvCamera: /usr/lib/x86_64-linux-gnu/libopencv_flann.so.2.4.9
OpencvCamera: /usr/lib/x86_64-linux-gnu/libopencv_core.so.2.4.9
OpencvCamera: CMakeFiles/OpencvCamera.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/muri/thesis/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable OpencvCamera"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/OpencvCamera.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/OpencvCamera.dir/build: OpencvCamera

.PHONY : CMakeFiles/OpencvCamera.dir/build

CMakeFiles/OpencvCamera.dir/requires: CMakeFiles/OpencvCamera.dir/OpencvCamera.cpp.o.requires

.PHONY : CMakeFiles/OpencvCamera.dir/requires

CMakeFiles/OpencvCamera.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/OpencvCamera.dir/cmake_clean.cmake
.PHONY : CMakeFiles/OpencvCamera.dir/clean

CMakeFiles/OpencvCamera.dir/depend:
	cd /home/muri/thesis/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/muri/thesis /home/muri/thesis /home/muri/thesis/build /home/muri/thesis/build /home/muri/thesis/build/CMakeFiles/OpencvCamera.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/OpencvCamera.dir/depend

