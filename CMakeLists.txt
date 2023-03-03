cmake_minimum_required(VERSION 3.24)
project(GSM CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(GSM main.cpp util.cpp util.h QuadTree.cpp QuadTree.h task.cpp)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")