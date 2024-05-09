cmake_minimum_required(VERSION 3.25)
project(PEA3_release)

set(CMAKE_CXX_STANDARD 17)

add_executable(PEA3_release main.cpp src/Graph.h src/Graph.cpp src/AlgorytmGenetyczny.h src/AlgorytmGenetyczny.cpp src/Menu.h src/Menu.cpp)
