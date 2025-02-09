cmake_minimum_required(VERSION 3.10)

add_executable(SimpleCVDemo demo.c)
target_link_libraries (SimpleCVDemo LINK_PUBLIC SimpleCV)
