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
include CMakeFiles/MultiModalModule.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/MultiModalModule.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MultiModalModule.dir/flags.make

CMakeFiles/MultiModalModule.dir/MultiModalModule.cpp.o: CMakeFiles/MultiModalModule.dir/flags.make
CMakeFiles/MultiModalModule.dir/MultiModalModule.cpp.o: ../MultiModalModule.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/muri/thesis/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/MultiModalModule.dir/MultiModalModule.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MultiModalModule.dir/MultiModalModule.cpp.o -c /home/muri/thesis/MultiModalModule.cpp

CMakeFiles/MultiModalModule.dir/MultiModalModule.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MultiModalModule.dir/MultiModalModule.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/muri/thesis/MultiModalModule.cpp > CMakeFiles/MultiModalModule.dir/MultiModalModule.cpp.i

CMakeFiles/MultiModalModule.dir/MultiModalModule.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MultiModalModule.dir/MultiModalModule.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/muri/thesis/MultiModalModule.cpp -o CMakeFiles/MultiModalModule.dir/MultiModalModule.cpp.s

CMakeFiles/MultiModalModule.dir/MultiModalModule.cpp.o.requires:

.PHONY : CMakeFiles/MultiModalModule.dir/MultiModalModule.cpp.o.requires

CMakeFiles/MultiModalModule.dir/MultiModalModule.cpp.o.provides: CMakeFiles/MultiModalModule.dir/MultiModalModule.cpp.o.requires
	$(MAKE) -f CMakeFiles/MultiModalModule.dir/build.make CMakeFiles/MultiModalModule.dir/MultiModalModule.cpp.o.provides.build
.PHONY : CMakeFiles/MultiModalModule.dir/MultiModalModule.cpp.o.provides

CMakeFiles/MultiModalModule.dir/MultiModalModule.cpp.o.provides.build: CMakeFiles/MultiModalModule.dir/MultiModalModule.cpp.o


# Object files for target MultiModalModule
MultiModalModule_OBJECTS = \
"CMakeFiles/MultiModalModule.dir/MultiModalModule.cpp.o"

# External object files for target MultiModalModule
MultiModalModule_EXTERNAL_OBJECTS =

MultiModalModule: CMakeFiles/MultiModalModule.dir/MultiModalModule.cpp.o
MultiModalModule: CMakeFiles/MultiModalModule.dir/build.make
MultiModalModule: /usr/local/lib/libYARP_dev.so.2.3.68
MultiModalModule: /usr/local/lib/libYARP_name.so.2.3.68
MultiModalModule: /usr/local/lib/libYARP_init.so.2.3.68
MultiModalModule: /usr/lib/x86_64-linux-gnu/libopencv_videostab.so.2.4.9
MultiModalModule: /usr/lib/x86_64-linux-gnu/libopencv_ts.so.2.4.9
MultiModalModule: /usr/lib/x86_64-linux-gnu/libopencv_superres.so.2.4.9
MultiModalModule: /usr/lib/x86_64-linux-gnu/libopencv_stitching.so.2.4.9
MultiModalModule: /usr/lib/x86_64-linux-gnu/libopencv_ocl.so.2.4.9
MultiModalModule: /usr/lib/x86_64-linux-gnu/libopencv_gpu.so.2.4.9
MultiModalModule: /usr/lib/x86_64-linux-gnu/libopencv_contrib.so.2.4.9
MultiModalModule: /usr/local/lib/libYARP_math.so.2.3.68
MultiModalModule: /usr/local/lib/libYARP_sig.so.2.3.68
MultiModalModule: /usr/local/lib/libYARP_OS.so.2.3.68
MultiModalModule: /usr/lib/x86_64-linux-gnu/libopencv_photo.so.2.4.9
MultiModalModule: /usr/lib/x86_64-linux-gnu/libopencv_legacy.so.2.4.9
MultiModalModule: /usr/lib/x86_64-linux-gnu/libopencv_video.so.2.4.9
MultiModalModule: /usr/lib/x86_64-linux-gnu/libopencv_objdetect.so.2.4.9
MultiModalModule: /usr/lib/x86_64-linux-gnu/libopencv_ml.so.2.4.9
MultiModalModule: /usr/lib/x86_64-linux-gnu/libopencv_calib3d.so.2.4.9
MultiModalModule: /usr/lib/x86_64-linux-gnu/libopencv_features2d.so.2.4.9
MultiModalModule: /usr/lib/x86_64-linux-gnu/libopencv_highgui.so.2.4.9
MultiModalModule: /usr/lib/x86_64-linux-gnu/libopencv_imgproc.so.2.4.9
MultiModalModule: /usr/lib/x86_64-linux-gnu/libopencv_flann.so.2.4.9
MultiModalModule: /usr/lib/x86_64-linux-gnu/libopencv_core.so.2.4.9
MultiModalModule: CMakeFiles/MultiModalModule.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/muri/thesis/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable MultiModalModule"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MultiModalModule.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MultiModalModule.dir/build: MultiModalModule

.PHONY : CMakeFiles/MultiModalModule.dir/build

CMakeFiles/MultiModalModule.dir/requires: CMakeFiles/MultiModalModule.dir/MultiModalModule.cpp.o.requires

.PHONY : CMakeFiles/MultiModalModule.dir/requires

CMakeFiles/MultiModalModule.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MultiModalModule.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MultiModalModule.dir/clean

CMakeFiles/MultiModalModule.dir/depend:
	cd /home/muri/thesis/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/muri/thesis /home/muri/thesis /home/muri/thesis/build /home/muri/thesis/build /home/muri/thesis/build/CMakeFiles/MultiModalModule.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/MultiModalModule.dir/depend

