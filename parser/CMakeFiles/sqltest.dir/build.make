# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/lyk/myDDB/parser

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lyk/myDDB/parser

# Include any dependencies generated for this target.
include CMakeFiles/sqltest.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/sqltest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sqltest.dir/flags.make

CMakeFiles/sqltest.dir/SELECT_sta.cpp.o: CMakeFiles/sqltest.dir/flags.make
CMakeFiles/sqltest.dir/SELECT_sta.cpp.o: SELECT_sta.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lyk/myDDB/parser/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/sqltest.dir/SELECT_sta.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sqltest.dir/SELECT_sta.cpp.o -c /home/lyk/myDDB/parser/SELECT_sta.cpp

CMakeFiles/sqltest.dir/SELECT_sta.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sqltest.dir/SELECT_sta.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lyk/myDDB/parser/SELECT_sta.cpp > CMakeFiles/sqltest.dir/SELECT_sta.cpp.i

CMakeFiles/sqltest.dir/SELECT_sta.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sqltest.dir/SELECT_sta.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lyk/myDDB/parser/SELECT_sta.cpp -o CMakeFiles/sqltest.dir/SELECT_sta.cpp.s

CMakeFiles/sqltest.dir/utils.cpp.o: CMakeFiles/sqltest.dir/flags.make
CMakeFiles/sqltest.dir/utils.cpp.o: utils.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lyk/myDDB/parser/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/sqltest.dir/utils.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sqltest.dir/utils.cpp.o -c /home/lyk/myDDB/parser/utils.cpp

CMakeFiles/sqltest.dir/utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sqltest.dir/utils.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lyk/myDDB/parser/utils.cpp > CMakeFiles/sqltest.dir/utils.cpp.i

CMakeFiles/sqltest.dir/utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sqltest.dir/utils.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lyk/myDDB/parser/utils.cpp -o CMakeFiles/sqltest.dir/utils.cpp.s

# Object files for target sqltest
sqltest_OBJECTS = \
"CMakeFiles/sqltest.dir/SELECT_sta.cpp.o" \
"CMakeFiles/sqltest.dir/utils.cpp.o"

# External object files for target sqltest
sqltest_EXTERNAL_OBJECTS =

sqltest: CMakeFiles/sqltest.dir/SELECT_sta.cpp.o
sqltest: CMakeFiles/sqltest.dir/utils.cpp.o
sqltest: CMakeFiles/sqltest.dir/build.make
sqltest: CMakeFiles/sqltest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lyk/myDDB/parser/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable sqltest"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sqltest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sqltest.dir/build: sqltest

.PHONY : CMakeFiles/sqltest.dir/build

CMakeFiles/sqltest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sqltest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sqltest.dir/clean

CMakeFiles/sqltest.dir/depend:
	cd /home/lyk/myDDB/parser && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lyk/myDDB/parser /home/lyk/myDDB/parser /home/lyk/myDDB/parser /home/lyk/myDDB/parser /home/lyk/myDDB/parser/CMakeFiles/sqltest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sqltest.dir/depend

