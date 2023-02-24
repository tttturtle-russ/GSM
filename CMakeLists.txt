cmake_minimum_required(VERSION 3.24)
project(GSM CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(GSM main.cpp QuadTree.cpp QuadTree.h util.cpp)
