# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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
CMAKE_COMMAND = /opt/cmake-3.17.5-Linux-x86_64/bin/cmake

# The command to remove a file.
RM = /opt/cmake-3.17.5-Linux-x86_64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/Users/Lukas/CLionProjects/vs

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/Lukas/CLionProjects/vs/cmake-build-wsl_profile

# Include any dependencies generated for this target.
include CMakeFiles/serverSynchroEncrypt.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/serverSynchroEncrypt.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/serverSynchroEncrypt.dir/flags.make

CMakeFiles/serverSynchroEncrypt.dir/serverSynchroEncrypt.cpp.o: CMakeFiles/serverSynchroEncrypt.dir/flags.make
CMakeFiles/serverSynchroEncrypt.dir/serverSynchroEncrypt.cpp.o: ../serverSynchroEncrypt.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/Lukas/CLionProjects/vs/cmake-build-wsl_profile/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/serverSynchroEncrypt.dir/serverSynchroEncrypt.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serverSynchroEncrypt.dir/serverSynchroEncrypt.cpp.o -c /mnt/c/Users/Lukas/CLionProjects/vs/serverSynchroEncrypt.cpp

CMakeFiles/serverSynchroEncrypt.dir/serverSynchroEncrypt.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serverSynchroEncrypt.dir/serverSynchroEncrypt.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/Lukas/CLionProjects/vs/serverSynchroEncrypt.cpp > CMakeFiles/serverSynchroEncrypt.dir/serverSynchroEncrypt.cpp.i

CMakeFiles/serverSynchroEncrypt.dir/serverSynchroEncrypt.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serverSynchroEncrypt.dir/serverSynchroEncrypt.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/Lukas/CLionProjects/vs/serverSynchroEncrypt.cpp -o CMakeFiles/serverSynchroEncrypt.dir/serverSynchroEncrypt.cpp.s

# Object files for target serverSynchroEncrypt
serverSynchroEncrypt_OBJECTS = \
"CMakeFiles/serverSynchroEncrypt.dir/serverSynchroEncrypt.cpp.o"

# External object files for target serverSynchroEncrypt
serverSynchroEncrypt_EXTERNAL_OBJECTS =

serverSynchroEncrypt: CMakeFiles/serverSynchroEncrypt.dir/serverSynchroEncrypt.cpp.o
serverSynchroEncrypt: CMakeFiles/serverSynchroEncrypt.dir/build.make
serverSynchroEncrypt: CMakeFiles/serverSynchroEncrypt.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/Lukas/CLionProjects/vs/cmake-build-wsl_profile/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable serverSynchroEncrypt"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/serverSynchroEncrypt.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/serverSynchroEncrypt.dir/build: serverSynchroEncrypt

.PHONY : CMakeFiles/serverSynchroEncrypt.dir/build

CMakeFiles/serverSynchroEncrypt.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/serverSynchroEncrypt.dir/cmake_clean.cmake
.PHONY : CMakeFiles/serverSynchroEncrypt.dir/clean

CMakeFiles/serverSynchroEncrypt.dir/depend:
	cd /mnt/c/Users/Lukas/CLionProjects/vs/cmake-build-wsl_profile && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/Lukas/CLionProjects/vs /mnt/c/Users/Lukas/CLionProjects/vs /mnt/c/Users/Lukas/CLionProjects/vs/cmake-build-wsl_profile /mnt/c/Users/Lukas/CLionProjects/vs/cmake-build-wsl_profile /mnt/c/Users/Lukas/CLionProjects/vs/cmake-build-wsl_profile/CMakeFiles/serverSynchroEncrypt.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/serverSynchroEncrypt.dir/depend

