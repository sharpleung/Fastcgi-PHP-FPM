# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/gqleung/CLionProjects/fpm

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/gqleung/CLionProjects/fpm/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/fpm.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/fpm.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/fpm.dir/flags.make

CMakeFiles/fpm.dir/main.c.o: CMakeFiles/fpm.dir/flags.make
CMakeFiles/fpm.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/gqleung/CLionProjects/fpm/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/fpm.dir/main.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/fpm.dir/main.c.o -c /Users/gqleung/CLionProjects/fpm/main.c

CMakeFiles/fpm.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/fpm.dir/main.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/gqleung/CLionProjects/fpm/main.c > CMakeFiles/fpm.dir/main.c.i

CMakeFiles/fpm.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/fpm.dir/main.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/gqleung/CLionProjects/fpm/main.c -o CMakeFiles/fpm.dir/main.c.s

# Object files for target fpm
fpm_OBJECTS = \
"CMakeFiles/fpm.dir/main.c.o"

# External object files for target fpm
fpm_EXTERNAL_OBJECTS =

fpm: CMakeFiles/fpm.dir/main.c.o
fpm: CMakeFiles/fpm.dir/build.make
fpm: CMakeFiles/fpm.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/gqleung/CLionProjects/fpm/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable fpm"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fpm.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/fpm.dir/build: fpm

.PHONY : CMakeFiles/fpm.dir/build

CMakeFiles/fpm.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/fpm.dir/cmake_clean.cmake
.PHONY : CMakeFiles/fpm.dir/clean

CMakeFiles/fpm.dir/depend:
	cd /Users/gqleung/CLionProjects/fpm/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/gqleung/CLionProjects/fpm /Users/gqleung/CLionProjects/fpm /Users/gqleung/CLionProjects/fpm/cmake-build-debug /Users/gqleung/CLionProjects/fpm/cmake-build-debug /Users/gqleung/CLionProjects/fpm/cmake-build-debug/CMakeFiles/fpm.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/fpm.dir/depend
