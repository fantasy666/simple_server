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
include src/CMakeFiles/zbase_shared.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/zbase_shared.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/zbase_shared.dir/flags.make

src/CMakeFiles/zbase_shared.dir/base/ThreadPool.cpp.o: src/CMakeFiles/zbase_shared.dir/flags.make
src/CMakeFiles/zbase_shared.dir/base/ThreadPool.cpp.o: ../src/base/ThreadPool.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dell/ZhY/liaotianshi/simple_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/zbase_shared.dir/base/ThreadPool.cpp.o"
	cd /home/dell/ZhY/liaotianshi/simple_server/build/src && /bin/clang++-10  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/zbase_shared.dir/base/ThreadPool.cpp.o -c /home/dell/ZhY/liaotianshi/simple_server/src/base/ThreadPool.cpp

src/CMakeFiles/zbase_shared.dir/base/ThreadPool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/zbase_shared.dir/base/ThreadPool.cpp.i"
	cd /home/dell/ZhY/liaotianshi/simple_server/build/src && /bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dell/ZhY/liaotianshi/simple_server/src/base/ThreadPool.cpp > CMakeFiles/zbase_shared.dir/base/ThreadPool.cpp.i

src/CMakeFiles/zbase_shared.dir/base/ThreadPool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/zbase_shared.dir/base/ThreadPool.cpp.s"
	cd /home/dell/ZhY/liaotianshi/simple_server/build/src && /bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dell/ZhY/liaotianshi/simple_server/src/base/ThreadPool.cpp -o CMakeFiles/zbase_shared.dir/base/ThreadPool.cpp.s

src/CMakeFiles/zbase_shared.dir/base/util.cpp.o: src/CMakeFiles/zbase_shared.dir/flags.make
src/CMakeFiles/zbase_shared.dir/base/util.cpp.o: ../src/base/util.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dell/ZhY/liaotianshi/simple_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/zbase_shared.dir/base/util.cpp.o"
	cd /home/dell/ZhY/liaotianshi/simple_server/build/src && /bin/clang++-10  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/zbase_shared.dir/base/util.cpp.o -c /home/dell/ZhY/liaotianshi/simple_server/src/base/util.cpp

src/CMakeFiles/zbase_shared.dir/base/util.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/zbase_shared.dir/base/util.cpp.i"
	cd /home/dell/ZhY/liaotianshi/simple_server/build/src && /bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dell/ZhY/liaotianshi/simple_server/src/base/util.cpp > CMakeFiles/zbase_shared.dir/base/util.cpp.i

src/CMakeFiles/zbase_shared.dir/base/util.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/zbase_shared.dir/base/util.cpp.s"
	cd /home/dell/ZhY/liaotianshi/simple_server/build/src && /bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dell/ZhY/liaotianshi/simple_server/src/base/util.cpp -o CMakeFiles/zbase_shared.dir/base/util.cpp.s

# Object files for target zbase_shared
zbase_shared_OBJECTS = \
"CMakeFiles/zbase_shared.dir/base/ThreadPool.cpp.o" \
"CMakeFiles/zbase_shared.dir/base/util.cpp.o"

# External object files for target zbase_shared
zbase_shared_EXTERNAL_OBJECTS =

lib/libzbase_shared.so: src/CMakeFiles/zbase_shared.dir/base/ThreadPool.cpp.o
lib/libzbase_shared.so: src/CMakeFiles/zbase_shared.dir/base/util.cpp.o
lib/libzbase_shared.so: src/CMakeFiles/zbase_shared.dir/build.make
lib/libzbase_shared.so: src/CMakeFiles/zbase_shared.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dell/ZhY/liaotianshi/simple_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX shared library ../lib/libzbase_shared.so"
	cd /home/dell/ZhY/liaotianshi/simple_server/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/zbase_shared.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/zbase_shared.dir/build: lib/libzbase_shared.so

.PHONY : src/CMakeFiles/zbase_shared.dir/build

src/CMakeFiles/zbase_shared.dir/clean:
	cd /home/dell/ZhY/liaotianshi/simple_server/build/src && $(CMAKE_COMMAND) -P CMakeFiles/zbase_shared.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/zbase_shared.dir/clean

src/CMakeFiles/zbase_shared.dir/depend:
	cd /home/dell/ZhY/liaotianshi/simple_server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dell/ZhY/liaotianshi/simple_server /home/dell/ZhY/liaotianshi/simple_server/src /home/dell/ZhY/liaotianshi/simple_server/build /home/dell/ZhY/liaotianshi/simple_server/build/src /home/dell/ZhY/liaotianshi/simple_server/build/src/CMakeFiles/zbase_shared.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/zbase_shared.dir/depend

