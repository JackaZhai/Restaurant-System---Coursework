# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/aarch64/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/aarch64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/Zhai/CLionProjects/点餐系统

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/Zhai/CLionProjects/点餐系统/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/cashier.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/cashier.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/cashier.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cashier.dir/flags.make

CMakeFiles/cashier.dir/cashier.cpp.o: CMakeFiles/cashier.dir/flags.make
CMakeFiles/cashier.dir/cashier.cpp.o: /Users/Zhai/CLionProjects/点餐系统/cashier.cpp
CMakeFiles/cashier.dir/cashier.cpp.o: CMakeFiles/cashier.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/Zhai/CLionProjects/点餐系统/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/cashier.dir/cashier.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/cashier.dir/cashier.cpp.o -MF CMakeFiles/cashier.dir/cashier.cpp.o.d -o CMakeFiles/cashier.dir/cashier.cpp.o -c /Users/Zhai/CLionProjects/点餐系统/cashier.cpp

CMakeFiles/cashier.dir/cashier.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/cashier.dir/cashier.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Zhai/CLionProjects/点餐系统/cashier.cpp > CMakeFiles/cashier.dir/cashier.cpp.i

CMakeFiles/cashier.dir/cashier.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/cashier.dir/cashier.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Zhai/CLionProjects/点餐系统/cashier.cpp -o CMakeFiles/cashier.dir/cashier.cpp.s

# Object files for target cashier
cashier_OBJECTS = \
"CMakeFiles/cashier.dir/cashier.cpp.o"

# External object files for target cashier
cashier_EXTERNAL_OBJECTS =

cashier: CMakeFiles/cashier.dir/cashier.cpp.o
cashier: CMakeFiles/cashier.dir/build.make
cashier: CMakeFiles/cashier.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/Zhai/CLionProjects/点餐系统/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable cashier"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cashier.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cashier.dir/build: cashier
.PHONY : CMakeFiles/cashier.dir/build

CMakeFiles/cashier.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cashier.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cashier.dir/clean

CMakeFiles/cashier.dir/depend:
	cd /Users/Zhai/CLionProjects/点餐系统/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/Zhai/CLionProjects/点餐系统 /Users/Zhai/CLionProjects/点餐系统 /Users/Zhai/CLionProjects/点餐系统/cmake-build-debug /Users/Zhai/CLionProjects/点餐系统/cmake-build-debug /Users/Zhai/CLionProjects/点餐系统/cmake-build-debug/CMakeFiles/cashier.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/cashier.dir/depend
