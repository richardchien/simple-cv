cmake_minimum_required(VERSION 3.10)

add_library(SimpleCV core.c io.c matrix.c)
target_include_directories(SimpleCV PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
