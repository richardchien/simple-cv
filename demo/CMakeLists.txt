cmake_minimum_required(VERSION 3.5)

add_executable(SimpleCVDemo demo.cpp)
target_link_libraries (SimpleCVDemo LINK_PUBLIC SimpleCV)
