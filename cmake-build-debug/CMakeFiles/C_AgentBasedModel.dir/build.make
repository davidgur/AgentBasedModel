# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.12

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2018.2.5\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2018.2.5\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\Users\david\Desktop\Agent Based Model\C_AgentBasedModel"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Users\david\Desktop\Agent Based Model\C_AgentBasedModel\cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/C_AgentBasedModel.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/C_AgentBasedModel.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/C_AgentBasedModel.dir/flags.make

CMakeFiles/C_AgentBasedModel.dir/main.cpp.obj: CMakeFiles/C_AgentBasedModel.dir/flags.make
CMakeFiles/C_AgentBasedModel.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\david\Desktop\Agent Based Model\C_AgentBasedModel\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/C_AgentBasedModel.dir/main.cpp.obj"
	C:\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\C_AgentBasedModel.dir\main.cpp.obj -c "C:\Users\david\Desktop\Agent Based Model\C_AgentBasedModel\main.cpp"

CMakeFiles/C_AgentBasedModel.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/C_AgentBasedModel.dir/main.cpp.i"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\Users\david\Desktop\Agent Based Model\C_AgentBasedModel\main.cpp" > CMakeFiles\C_AgentBasedModel.dir\main.cpp.i

CMakeFiles/C_AgentBasedModel.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/C_AgentBasedModel.dir/main.cpp.s"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\Users\david\Desktop\Agent Based Model\C_AgentBasedModel\main.cpp" -o CMakeFiles\C_AgentBasedModel.dir\main.cpp.s

CMakeFiles/C_AgentBasedModel.dir/agent.cpp.obj: CMakeFiles/C_AgentBasedModel.dir/flags.make
CMakeFiles/C_AgentBasedModel.dir/agent.cpp.obj: ../agent.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\david\Desktop\Agent Based Model\C_AgentBasedModel\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/C_AgentBasedModel.dir/agent.cpp.obj"
	C:\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\C_AgentBasedModel.dir\agent.cpp.obj -c "C:\Users\david\Desktop\Agent Based Model\C_AgentBasedModel\agent.cpp"

CMakeFiles/C_AgentBasedModel.dir/agent.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/C_AgentBasedModel.dir/agent.cpp.i"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\Users\david\Desktop\Agent Based Model\C_AgentBasedModel\agent.cpp" > CMakeFiles\C_AgentBasedModel.dir\agent.cpp.i

CMakeFiles/C_AgentBasedModel.dir/agent.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/C_AgentBasedModel.dir/agent.cpp.s"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\Users\david\Desktop\Agent Based Model\C_AgentBasedModel\agent.cpp" -o CMakeFiles\C_AgentBasedModel.dir\agent.cpp.s

# Object files for target C_AgentBasedModel
C_AgentBasedModel_OBJECTS = \
"CMakeFiles/C_AgentBasedModel.dir/main.cpp.obj" \
"CMakeFiles/C_AgentBasedModel.dir/agent.cpp.obj"

# External object files for target C_AgentBasedModel
C_AgentBasedModel_EXTERNAL_OBJECTS =

C_AgentBasedModel.exe: CMakeFiles/C_AgentBasedModel.dir/main.cpp.obj
C_AgentBasedModel.exe: CMakeFiles/C_AgentBasedModel.dir/agent.cpp.obj
C_AgentBasedModel.exe: CMakeFiles/C_AgentBasedModel.dir/build.make
C_AgentBasedModel.exe: CMakeFiles/C_AgentBasedModel.dir/linklibs.rsp
C_AgentBasedModel.exe: CMakeFiles/C_AgentBasedModel.dir/objects1.rsp
C_AgentBasedModel.exe: CMakeFiles/C_AgentBasedModel.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="C:\Users\david\Desktop\Agent Based Model\C_AgentBasedModel\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable C_AgentBasedModel.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\C_AgentBasedModel.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/C_AgentBasedModel.dir/build: C_AgentBasedModel.exe

.PHONY : CMakeFiles/C_AgentBasedModel.dir/build

CMakeFiles/C_AgentBasedModel.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\C_AgentBasedModel.dir\cmake_clean.cmake
.PHONY : CMakeFiles/C_AgentBasedModel.dir/clean

CMakeFiles/C_AgentBasedModel.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "C:\Users\david\Desktop\Agent Based Model\C_AgentBasedModel" "C:\Users\david\Desktop\Agent Based Model\C_AgentBasedModel" "C:\Users\david\Desktop\Agent Based Model\C_AgentBasedModel\cmake-build-debug" "C:\Users\david\Desktop\Agent Based Model\C_AgentBasedModel\cmake-build-debug" "C:\Users\david\Desktop\Agent Based Model\C_AgentBasedModel\cmake-build-debug\CMakeFiles\C_AgentBasedModel.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/C_AgentBasedModel.dir/depend

