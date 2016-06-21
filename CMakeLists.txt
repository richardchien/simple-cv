cmake_minimum_required(VERSION 3.5)
project(SimpleCV)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(simplecv)
add_subdirectory(demo)
