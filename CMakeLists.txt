cmake_minimum_required(VERSION 3.16.0)
project(prom VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

option(WITH_GUI "Build with OpenCV highgui" ON)

configure_file(config.h.in config.h)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)
