# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.25.2/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.25.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/isaac/Documents/obd2-reader/lib

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/isaac/Documents/obd2-reader/lib/build

# Include any dependencies generated for this target.
include CMakeFiles/OBD2-Reader.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/OBD2-Reader.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/OBD2-Reader.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/OBD2-Reader.dir/flags.make

CMakeFiles/OBD2-Reader.dir/main.cpp.o: CMakeFiles/OBD2-Reader.dir/flags.make
CMakeFiles/OBD2-Reader.dir/main.cpp.o: /Users/isaac/Documents/obd2-reader/lib/main.cpp
CMakeFiles/OBD2-Reader.dir/main.cpp.o: CMakeFiles/OBD2-Reader.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/isaac/Documents/obd2-reader/lib/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/OBD2-Reader.dir/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/OBD2-Reader.dir/main.cpp.o -MF CMakeFiles/OBD2-Reader.dir/main.cpp.o.d -o CMakeFiles/OBD2-Reader.dir/main.cpp.o -c /Users/isaac/Documents/obd2-reader/lib/main.cpp

CMakeFiles/OBD2-Reader.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/OBD2-Reader.dir/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/isaac/Documents/obd2-reader/lib/main.cpp > CMakeFiles/OBD2-Reader.dir/main.cpp.i

CMakeFiles/OBD2-Reader.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/OBD2-Reader.dir/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/isaac/Documents/obd2-reader/lib/main.cpp -o CMakeFiles/OBD2-Reader.dir/main.cpp.s

# Object files for target OBD2-Reader
OBD2__Reader_OBJECTS = \
"CMakeFiles/OBD2-Reader.dir/main.cpp.o"

# External object files for target OBD2-Reader
OBD2__Reader_EXTERNAL_OBJECTS =

OBD2-Reader: CMakeFiles/OBD2-Reader.dir/main.cpp.o
OBD2-Reader: CMakeFiles/OBD2-Reader.dir/build.make
OBD2-Reader: data/libData.a
OBD2-Reader: server/libServer.a
OBD2-Reader: /opt/homebrew/lib/libboost_system-mt.dylib
OBD2-Reader: /opt/homebrew/lib/libboost_program_options-mt.dylib
OBD2-Reader: CMakeFiles/OBD2-Reader.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/isaac/Documents/obd2-reader/lib/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable OBD2-Reader"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/OBD2-Reader.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/OBD2-Reader.dir/build: OBD2-Reader
.PHONY : CMakeFiles/OBD2-Reader.dir/build

CMakeFiles/OBD2-Reader.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/OBD2-Reader.dir/cmake_clean.cmake
.PHONY : CMakeFiles/OBD2-Reader.dir/clean

CMakeFiles/OBD2-Reader.dir/depend:
	cd /Users/isaac/Documents/obd2-reader/lib/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/isaac/Documents/obd2-reader/lib /Users/isaac/Documents/obd2-reader/lib /Users/isaac/Documents/obd2-reader/lib/build /Users/isaac/Documents/obd2-reader/lib/build /Users/isaac/Documents/obd2-reader/lib/build/CMakeFiles/OBD2-Reader.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/OBD2-Reader.dir/depend
