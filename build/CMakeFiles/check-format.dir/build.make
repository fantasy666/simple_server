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

# Utility rule file for check-format.

# Include the progress variables for this target.
include CMakeFiles/check-format.dir/progress.make

CMakeFiles/check-format:
	../build_support/run_clang_format.py /usr/bin/clang-format /home/dell/ZhY/liaotianshi/simple_server/build_support/clang_format_exclusions.txt --source_dirs /home/dell/ZhY/liaotianshi/simple_server/src,/home/dell/ZhY/liaotianshi/simple_server/test, --quiet

check-format: CMakeFiles/check-format
check-format: CMakeFiles/check-format.dir/build.make

.PHONY : check-format

# Rule to build all files generated by this target.
CMakeFiles/check-format.dir/build: check-format

.PHONY : CMakeFiles/check-format.dir/build

CMakeFiles/check-format.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/check-format.dir/cmake_clean.cmake
.PHONY : CMakeFiles/check-format.dir/clean

CMakeFiles/check-format.dir/depend:
	cd /home/dell/ZhY/liaotianshi/simple_server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dell/ZhY/liaotianshi/simple_server /home/dell/ZhY/liaotianshi/simple_server /home/dell/ZhY/liaotianshi/simple_server/build /home/dell/ZhY/liaotianshi/simple_server/build /home/dell/ZhY/liaotianshi/simple_server/build/CMakeFiles/check-format.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/check-format.dir/depend

