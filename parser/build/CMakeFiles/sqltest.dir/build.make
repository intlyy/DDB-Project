# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.25

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = D:\cmake\bin\cmake.exe

# The command to remove a file.
RM = D:\cmake\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = E:\360MoveData\Users\87539\Desktop\myDDB\parser

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = E:\360MoveData\Users\87539\Desktop\myDDB\parser\build

# Include any dependencies generated for this target.
include CMakeFiles/sqltest.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/sqltest.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/sqltest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sqltest.dir/flags.make

CMakeFiles/sqltest.dir/SELECT_sta.cpp.obj: CMakeFiles/sqltest.dir/flags.make
CMakeFiles/sqltest.dir/SELECT_sta.cpp.obj: E:/360MoveData/Users/87539/Desktop/myDDB/parser/SELECT_sta.cpp
CMakeFiles/sqltest.dir/SELECT_sta.cpp.obj: CMakeFiles/sqltest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\360MoveData\Users\87539\Desktop\myDDB\parser\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/sqltest.dir/SELECT_sta.cpp.obj"
	D:\Mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/sqltest.dir/SELECT_sta.cpp.obj -MF CMakeFiles\sqltest.dir\SELECT_sta.cpp.obj.d -o CMakeFiles\sqltest.dir\SELECT_sta.cpp.obj -c E:\360MoveData\Users\87539\Desktop\myDDB\parser\SELECT_sta.cpp

CMakeFiles/sqltest.dir/SELECT_sta.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sqltest.dir/SELECT_sta.cpp.i"
	D:\Mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\360MoveData\Users\87539\Desktop\myDDB\parser\SELECT_sta.cpp > CMakeFiles\sqltest.dir\SELECT_sta.cpp.i

CMakeFiles/sqltest.dir/SELECT_sta.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sqltest.dir/SELECT_sta.cpp.s"
	D:\Mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\360MoveData\Users\87539\Desktop\myDDB\parser\SELECT_sta.cpp -o CMakeFiles\sqltest.dir\SELECT_sta.cpp.s

CMakeFiles/sqltest.dir/utils.cpp.obj: CMakeFiles/sqltest.dir/flags.make
CMakeFiles/sqltest.dir/utils.cpp.obj: E:/360MoveData/Users/87539/Desktop/myDDB/parser/utils.cpp
CMakeFiles/sqltest.dir/utils.cpp.obj: CMakeFiles/sqltest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\360MoveData\Users\87539\Desktop\myDDB\parser\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/sqltest.dir/utils.cpp.obj"
	D:\Mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/sqltest.dir/utils.cpp.obj -MF CMakeFiles\sqltest.dir\utils.cpp.obj.d -o CMakeFiles\sqltest.dir\utils.cpp.obj -c E:\360MoveData\Users\87539\Desktop\myDDB\parser\utils.cpp

CMakeFiles/sqltest.dir/utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sqltest.dir/utils.cpp.i"
	D:\Mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\360MoveData\Users\87539\Desktop\myDDB\parser\utils.cpp > CMakeFiles\sqltest.dir\utils.cpp.i

CMakeFiles/sqltest.dir/utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sqltest.dir/utils.cpp.s"
	D:\Mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\360MoveData\Users\87539\Desktop\myDDB\parser\utils.cpp -o CMakeFiles\sqltest.dir\utils.cpp.s

# Object files for target sqltest
sqltest_OBJECTS = \
"CMakeFiles/sqltest.dir/SELECT_sta.cpp.obj" \
"CMakeFiles/sqltest.dir/utils.cpp.obj"

# External object files for target sqltest
sqltest_EXTERNAL_OBJECTS =

sqltest.exe: CMakeFiles/sqltest.dir/SELECT_sta.cpp.obj
sqltest.exe: CMakeFiles/sqltest.dir/utils.cpp.obj
sqltest.exe: CMakeFiles/sqltest.dir/build.make
sqltest.exe: CMakeFiles/sqltest.dir/linkLibs.rsp
sqltest.exe: CMakeFiles/sqltest.dir/objects1
sqltest.exe: CMakeFiles/sqltest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=E:\360MoveData\Users\87539\Desktop\myDDB\parser\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable sqltest.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\sqltest.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sqltest.dir/build: sqltest.exe
.PHONY : CMakeFiles/sqltest.dir/build

CMakeFiles/sqltest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\sqltest.dir\cmake_clean.cmake
.PHONY : CMakeFiles/sqltest.dir/clean

CMakeFiles/sqltest.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" E:\360MoveData\Users\87539\Desktop\myDDB\parser E:\360MoveData\Users\87539\Desktop\myDDB\parser E:\360MoveData\Users\87539\Desktop\myDDB\parser\build E:\360MoveData\Users\87539\Desktop\myDDB\parser\build E:\360MoveData\Users\87539\Desktop\myDDB\parser\build\CMakeFiles\sqltest.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sqltest.dir/depend

