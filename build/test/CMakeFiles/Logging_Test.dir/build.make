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
CMAKE_SOURCE_DIR = /home/dell/ZhY/liaotianshi/simple_server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dell/ZhY/liaotianshi/simple_server/build

# Include any dependencies generated for this target.
include test/CMakeFiles/Logging_Test.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/Logging_Test.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/Logging_Test.dir/flags.make

test/CMakeFiles/Logging_Test.dir/Logging_test.cpp.o: test/CMakeFiles/Logging_Test.dir/flags.make
test/CMakeFiles/Logging_Test.dir/Logging_test.cpp.o: ../test/Logging_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dell/ZhY/liaotianshi/simple_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/Logging_Test.dir/Logging_test.cpp.o"
	cd /home/dell/ZhY/liaotianshi/simple_server/build/test && /bin/clang++-10  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Logging_Test.dir/Logging_test.cpp.o -c /home/dell/ZhY/liaotianshi/simple_server/test/Logging_test.cpp

test/CMakeFiles/Logging_Test.dir/Logging_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Logging_Test.dir/Logging_test.cpp.i"
	cd /home/dell/ZhY/liaotianshi/simple_server/build/test && /bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dell/ZhY/liaotianshi/simple_server/test/Logging_test.cpp > CMakeFiles/Logging_Test.dir/Logging_test.cpp.i

test/CMakeFiles/Logging_Test.dir/Logging_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Logging_Test.dir/Logging_test.cpp.s"
	cd /home/dell/ZhY/liaotianshi/simple_server/build/test && /bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dell/ZhY/liaotianshi/simple_server/test/Logging_test.cpp -o CMakeFiles/Logging_Test.dir/Logging_test.cpp.s

# Object files for target Logging_Test
Logging_Test_OBJECTS = \
"CMakeFiles/Logging_Test.dir/Logging_test.cpp.o"

# External object files for target Logging_Test
Logging_Test_EXTERNAL_OBJECTS =

bin/Logging_Test: test/CMakeFiles/Logging_Test.dir/Logging_test.cpp.o
bin/Logging_Test: test/CMakeFiles/Logging_Test.dir/build.make
bin/Logging_Test: lib/libzbase_shared.so
bin/Logging_Test: test/CMakeFiles/Logging_Test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dell/ZhY/liaotianshi/simple_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/Logging_Test"
	cd /home/dell/ZhY/liaotianshi/simple_server/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Logging_Test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/Logging_Test.dir/build: bin/Logging_Test

.PHONY : test/CMakeFiles/Logging_Test.dir/build

test/CMakeFiles/Logging_Test.dir/clean:
	cd /home/dell/ZhY/liaotianshi/simple_server/build/test && $(CMAKE_COMMAND) -P CMakeFiles/Logging_Test.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/Logging_Test.dir/clean

test/CMakeFiles/Logging_Test.dir/depend:
	cd /home/dell/ZhY/liaotianshi/simple_server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dell/ZhY/liaotianshi/simple_server /home/dell/ZhY/liaotianshi/simple_server/test /home/dell/ZhY/liaotianshi/simple_server/build /home/dell/ZhY/liaotianshi/simple_server/build/test /home/dell/ZhY/liaotianshi/simple_server/build/test/CMakeFiles/Logging_Test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/Logging_Test.dir/depend

