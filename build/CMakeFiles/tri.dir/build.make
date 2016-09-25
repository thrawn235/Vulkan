# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

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
CMAKE_SOURCE_DIR = /home/sebastian/Programming/C++/VulkanSDK/1.0.26.0/examples

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sebastian/Programming/C++/VulkanSDK/1.0.26.0/examples/build

# Include any dependencies generated for this target.
include CMakeFiles/tri.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/tri.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tri.dir/flags.make

tri-vert.spv: ../tri.vert
tri-vert.spv: /home/sebastian/Programming/C++/VulkanSDK/1.0.26.0/x86_64/bin/glslangValidator
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/sebastian/Programming/C++/VulkanSDK/1.0.26.0/examples/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating tri-vert.spv"
	/home/sebastian/Programming/C++/VulkanSDK/1.0.26.0/x86_64/bin/glslangValidator -s -V -o /home/sebastian/Programming/C++/VulkanSDK/1.0.26.0/examples/build/tri-vert.spv /home/sebastian/Programming/C++/VulkanSDK/1.0.26.0/examples/tri.vert

tri-frag.spv: ../tri.frag
tri-frag.spv: /home/sebastian/Programming/C++/VulkanSDK/1.0.26.0/x86_64/bin/glslangValidator
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/sebastian/Programming/C++/VulkanSDK/1.0.26.0/examples/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating tri-frag.spv"
	/home/sebastian/Programming/C++/VulkanSDK/1.0.26.0/x86_64/bin/glslangValidator -s -V -o /home/sebastian/Programming/C++/VulkanSDK/1.0.26.0/examples/build/tri-frag.spv /home/sebastian/Programming/C++/VulkanSDK/1.0.26.0/examples/tri.frag

CMakeFiles/tri.dir/tri.c.o: CMakeFiles/tri.dir/flags.make
CMakeFiles/tri.dir/tri.c.o: ../tri.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sebastian/Programming/C++/VulkanSDK/1.0.26.0/examples/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/tri.dir/tri.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/tri.dir/tri.c.o   -c /home/sebastian/Programming/C++/VulkanSDK/1.0.26.0/examples/tri.c

CMakeFiles/tri.dir/tri.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tri.dir/tri.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sebastian/Programming/C++/VulkanSDK/1.0.26.0/examples/tri.c > CMakeFiles/tri.dir/tri.c.i

CMakeFiles/tri.dir/tri.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tri.dir/tri.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sebastian/Programming/C++/VulkanSDK/1.0.26.0/examples/tri.c -o CMakeFiles/tri.dir/tri.c.s

CMakeFiles/tri.dir/tri.c.o.requires:

.PHONY : CMakeFiles/tri.dir/tri.c.o.requires

CMakeFiles/tri.dir/tri.c.o.provides: CMakeFiles/tri.dir/tri.c.o.requires
	$(MAKE) -f CMakeFiles/tri.dir/build.make CMakeFiles/tri.dir/tri.c.o.provides.build
.PHONY : CMakeFiles/tri.dir/tri.c.o.provides

CMakeFiles/tri.dir/tri.c.o.provides.build: CMakeFiles/tri.dir/tri.c.o


# Object files for target tri
tri_OBJECTS = \
"CMakeFiles/tri.dir/tri.c.o"

# External object files for target tri
tri_EXTERNAL_OBJECTS =

tri: CMakeFiles/tri.dir/tri.c.o
tri: CMakeFiles/tri.dir/build.make
tri: /usr/lib64/libxcb.so
tri: /usr/lib64/libSM.so
tri: /usr/lib64/libICE.so
tri: /usr/lib64/libX11.so
tri: /usr/lib64/libXext.so
tri: CMakeFiles/tri.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/sebastian/Programming/C++/VulkanSDK/1.0.26.0/examples/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable tri"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tri.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tri.dir/build: tri

.PHONY : CMakeFiles/tri.dir/build

CMakeFiles/tri.dir/requires: CMakeFiles/tri.dir/tri.c.o.requires

.PHONY : CMakeFiles/tri.dir/requires

CMakeFiles/tri.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tri.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tri.dir/clean

CMakeFiles/tri.dir/depend: tri-vert.spv
CMakeFiles/tri.dir/depend: tri-frag.spv
	cd /home/sebastian/Programming/C++/VulkanSDK/1.0.26.0/examples/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sebastian/Programming/C++/VulkanSDK/1.0.26.0/examples /home/sebastian/Programming/C++/VulkanSDK/1.0.26.0/examples /home/sebastian/Programming/C++/VulkanSDK/1.0.26.0/examples/build /home/sebastian/Programming/C++/VulkanSDK/1.0.26.0/examples/build /home/sebastian/Programming/C++/VulkanSDK/1.0.26.0/examples/build/CMakeFiles/tri.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/tri.dir/depend
