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
CMAKE_SOURCE_DIR = /home/lyk/myDDB/executor

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lyk/myDDB/executor/build

# Include any dependencies generated for this target.
include CMakeFiles/main.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/main.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/main.dir/flags.make

CMakeFiles/main.dir/src/SELECT_sta.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/src/SELECT_sta.cpp.o: ../src/SELECT_sta.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lyk/myDDB/executor/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/main.dir/src/SELECT_sta.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/src/SELECT_sta.cpp.o -c /home/lyk/myDDB/executor/src/SELECT_sta.cpp

CMakeFiles/main.dir/src/SELECT_sta.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/src/SELECT_sta.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lyk/myDDB/executor/src/SELECT_sta.cpp > CMakeFiles/main.dir/src/SELECT_sta.cpp.i

CMakeFiles/main.dir/src/SELECT_sta.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/src/SELECT_sta.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lyk/myDDB/executor/src/SELECT_sta.cpp -o CMakeFiles/main.dir/src/SELECT_sta.cpp.s

CMakeFiles/main.dir/src/Tree.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/src/Tree.cpp.o: ../src/Tree.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lyk/myDDB/executor/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/main.dir/src/Tree.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/src/Tree.cpp.o -c /home/lyk/myDDB/executor/src/Tree.cpp

CMakeFiles/main.dir/src/Tree.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/src/Tree.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lyk/myDDB/executor/src/Tree.cpp > CMakeFiles/main.dir/src/Tree.cpp.i

CMakeFiles/main.dir/src/Tree.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/src/Tree.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lyk/myDDB/executor/src/Tree.cpp -o CMakeFiles/main.dir/src/Tree.cpp.s

CMakeFiles/main.dir/src/connect2sql.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/src/connect2sql.cpp.o: ../src/connect2sql.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lyk/myDDB/executor/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/main.dir/src/connect2sql.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/src/connect2sql.cpp.o -c /home/lyk/myDDB/executor/src/connect2sql.cpp

CMakeFiles/main.dir/src/connect2sql.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/src/connect2sql.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lyk/myDDB/executor/src/connect2sql.cpp > CMakeFiles/main.dir/src/connect2sql.cpp.i

CMakeFiles/main.dir/src/connect2sql.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/src/connect2sql.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lyk/myDDB/executor/src/connect2sql.cpp -o CMakeFiles/main.dir/src/connect2sql.cpp.s

CMakeFiles/main.dir/src/ddb_server.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/src/ddb_server.cpp.o: ../src/ddb_server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lyk/myDDB/executor/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/main.dir/src/ddb_server.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/src/ddb_server.cpp.o -c /home/lyk/myDDB/executor/src/ddb_server.cpp

CMakeFiles/main.dir/src/ddb_server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/src/ddb_server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lyk/myDDB/executor/src/ddb_server.cpp > CMakeFiles/main.dir/src/ddb_server.cpp.i

CMakeFiles/main.dir/src/ddb_server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/src/ddb_server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lyk/myDDB/executor/src/ddb_server.cpp -o CMakeFiles/main.dir/src/ddb_server.cpp.s

CMakeFiles/main.dir/src/executor.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/src/executor.cpp.o: ../src/executor.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lyk/myDDB/executor/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/main.dir/src/executor.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/src/executor.cpp.o -c /home/lyk/myDDB/executor/src/executor.cpp

CMakeFiles/main.dir/src/executor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/src/executor.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lyk/myDDB/executor/src/executor.cpp > CMakeFiles/main.dir/src/executor.cpp.i

CMakeFiles/main.dir/src/executor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/src/executor.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lyk/myDDB/executor/src/executor.cpp -o CMakeFiles/main.dir/src/executor.cpp.s

CMakeFiles/main.dir/src/main.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lyk/myDDB/executor/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/main.dir/src/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/src/main.cpp.o -c /home/lyk/myDDB/executor/src/main.cpp

CMakeFiles/main.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lyk/myDDB/executor/src/main.cpp > CMakeFiles/main.dir/src/main.cpp.i

CMakeFiles/main.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lyk/myDDB/executor/src/main.cpp -o CMakeFiles/main.dir/src/main.cpp.s

CMakeFiles/main.dir/src/myetcd.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/src/myetcd.cpp.o: ../src/myetcd.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lyk/myDDB/executor/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/main.dir/src/myetcd.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/src/myetcd.cpp.o -c /home/lyk/myDDB/executor/src/myetcd.cpp

CMakeFiles/main.dir/src/myetcd.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/src/myetcd.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lyk/myDDB/executor/src/myetcd.cpp > CMakeFiles/main.dir/src/myetcd.cpp.i

CMakeFiles/main.dir/src/myetcd.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/src/myetcd.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lyk/myDDB/executor/src/myetcd.cpp -o CMakeFiles/main.dir/src/myetcd.cpp.s

CMakeFiles/main.dir/src/utils.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/src/utils.cpp.o: ../src/utils.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lyk/myDDB/executor/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/main.dir/src/utils.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/src/utils.cpp.o -c /home/lyk/myDDB/executor/src/utils.cpp

CMakeFiles/main.dir/src/utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/src/utils.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lyk/myDDB/executor/src/utils.cpp > CMakeFiles/main.dir/src/utils.cpp.i

CMakeFiles/main.dir/src/utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/src/utils.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lyk/myDDB/executor/src/utils.cpp -o CMakeFiles/main.dir/src/utils.cpp.s

# Object files for target main
main_OBJECTS = \
"CMakeFiles/main.dir/src/SELECT_sta.cpp.o" \
"CMakeFiles/main.dir/src/Tree.cpp.o" \
"CMakeFiles/main.dir/src/connect2sql.cpp.o" \
"CMakeFiles/main.dir/src/ddb_server.cpp.o" \
"CMakeFiles/main.dir/src/executor.cpp.o" \
"CMakeFiles/main.dir/src/main.cpp.o" \
"CMakeFiles/main.dir/src/myetcd.cpp.o" \
"CMakeFiles/main.dir/src/utils.cpp.o"

# External object files for target main
main_EXTERNAL_OBJECTS =

main: CMakeFiles/main.dir/src/SELECT_sta.cpp.o
main: CMakeFiles/main.dir/src/Tree.cpp.o
main: CMakeFiles/main.dir/src/connect2sql.cpp.o
main: CMakeFiles/main.dir/src/ddb_server.cpp.o
main: CMakeFiles/main.dir/src/executor.cpp.o
main: CMakeFiles/main.dir/src/main.cpp.o
main: CMakeFiles/main.dir/src/myetcd.cpp.o
main: CMakeFiles/main.dir/src/utils.cpp.o
main: CMakeFiles/main.dir/build.make
main: libhw_grpc_proto.a
main: /root/.local/lib/libgrpc++_reflection.a
main: /root/.local/lib/libgrpc++.a
main: /root/.local/lib/libprotobuf.a
main: /root/.local/lib/libgrpc.a
main: /usr/lib/x86_64-linux-gnu/libz.so
main: /root/.local/lib/libcares.a
main: /root/.local/lib/libaddress_sorting.a
main: /root/.local/lib/libre2.a
main: /root/.local/lib/libupb.a
main: /root/.local/lib/libabsl_raw_hash_set.a
main: /root/.local/lib/libabsl_hashtablez_sampler.a
main: /root/.local/lib/libabsl_hash.a
main: /root/.local/lib/libabsl_city.a
main: /root/.local/lib/libabsl_low_level_hash.a
main: /root/.local/lib/libabsl_statusor.a
main: /root/.local/lib/libgpr.a
main: /root/.local/lib/libabsl_bad_variant_access.a
main: /root/.local/lib/libabsl_status.a
main: /root/.local/lib/libabsl_strerror.a
main: /root/.local/lib/libabsl_random_distributions.a
main: /root/.local/lib/libabsl_random_seed_sequences.a
main: /root/.local/lib/libabsl_random_internal_pool_urbg.a
main: /root/.local/lib/libabsl_random_internal_randen.a
main: /root/.local/lib/libabsl_random_internal_randen_hwaes.a
main: /root/.local/lib/libabsl_random_internal_randen_hwaes_impl.a
main: /root/.local/lib/libabsl_random_internal_randen_slow.a
main: /root/.local/lib/libabsl_random_internal_platform.a
main: /root/.local/lib/libabsl_random_internal_seed_material.a
main: /root/.local/lib/libabsl_random_seed_gen_exception.a
main: /root/.local/lib/libabsl_cord.a
main: /root/.local/lib/libabsl_bad_optional_access.a
main: /root/.local/lib/libabsl_cordz_info.a
main: /root/.local/lib/libabsl_cord_internal.a
main: /root/.local/lib/libabsl_cordz_functions.a
main: /root/.local/lib/libabsl_exponential_biased.a
main: /root/.local/lib/libabsl_cordz_handle.a
main: /root/.local/lib/libabsl_str_format_internal.a
main: /root/.local/lib/libabsl_synchronization.a
main: /root/.local/lib/libabsl_stacktrace.a
main: /root/.local/lib/libabsl_symbolize.a
main: /root/.local/lib/libabsl_debugging_internal.a
main: /root/.local/lib/libabsl_demangle_internal.a
main: /root/.local/lib/libabsl_graphcycles_internal.a
main: /root/.local/lib/libabsl_malloc_internal.a
main: /root/.local/lib/libabsl_time.a
main: /root/.local/lib/libabsl_strings.a
main: /root/.local/lib/libabsl_throw_delegate.a
main: /root/.local/lib/libabsl_int128.a
main: /root/.local/lib/libabsl_strings_internal.a
main: /root/.local/lib/libabsl_base.a
main: /root/.local/lib/libabsl_spinlock_wait.a
main: /root/.local/lib/libabsl_raw_logging_internal.a
main: /root/.local/lib/libabsl_log_severity.a
main: /root/.local/lib/libabsl_civil_time.a
main: /root/.local/lib/libabsl_time_zone.a
main: /usr/lib/x86_64-linux-gnu/libssl.so
main: /usr/lib/x86_64-linux-gnu/libcrypto.so
main: CMakeFiles/main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lyk/myDDB/executor/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX executable main"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/main.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/main.dir/build: main

.PHONY : CMakeFiles/main.dir/build

CMakeFiles/main.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/main.dir/cmake_clean.cmake
.PHONY : CMakeFiles/main.dir/clean

CMakeFiles/main.dir/depend:
	cd /home/lyk/myDDB/executor/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lyk/myDDB/executor /home/lyk/myDDB/executor /home/lyk/myDDB/executor/build /home/lyk/myDDB/executor/build /home/lyk/myDDB/executor/build/CMakeFiles/main.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/main.dir/depend
