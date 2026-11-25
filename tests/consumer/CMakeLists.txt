# Application using the installed library, checks the exported CMake package
cmake_minimum_required(VERSION 3.16)
project(consumer CXX)

find_package(OpenCVGUI 2.0 REQUIRED)

add_executable(consumer main.cpp)
target_link_libraries(consumer PRIVATE OpenCVGUI::OpenCVGUI)
