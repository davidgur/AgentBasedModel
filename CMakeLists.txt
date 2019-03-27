cmake_minimum_required(VERSION 3.12)
project(C_AgentBasedModel)

set(CMAKE_CXX_STANDARD 17)

add_executable(C_AgentBasedModel src/main.cpp src/agent.cpp include/agent.h src/graph_building.cpp include/graph_building.h include/json.hpp)