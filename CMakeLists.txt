cmake_minimum_required(VERSION 3.0)
project(AgentBasedModel)


# Search directory for all source code files and assets
file(GLOB_RECURSE sources src/*.cpp include/*.h include/*.hh)
file(GLOB_RECURSE util tools/*.*)
# Add the data to the target
add_executable(AgentBasedModel ${sources} ${data})

# Threads
find_package(Threads)
target_link_libraries(AgentBasedModel ${CMAKE_THREAD_LIBS_INIT})
# Compiler flags
target_compile_options(AgentBasedModel PUBLIC -std=c++11 -O3 -march=native -pthread)

# Add files that are needed for execution to the execution folder
file(COPY ${util} DESTINATION export)

# Packaging stuff
set(CPACK_PACKAGE_NAME "Agent Based Model")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_MONOLITHIC_INSTALL 1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(CPack)
