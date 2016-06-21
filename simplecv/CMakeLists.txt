cmake_minimum_required(VERSION 3.5)

add_library(SimpleCV types.h core.h core.c io.c io.h scv.h ../LogUtils/log_utils.h ../LogUtils/log_utils.c matrix.c matrix.h)
target_include_directories(SimpleCV PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
