cmake_minimum_required(VERSION 3.16.0)

find_package(OpenCV REQUIRED)

add_subdirectory(video-decoder)

add_executable(prom main.cpp)
target_include_directories(prom PRIVATE ${PROJECT_BINARY_DIR} ./video-decoder ${OpenCV_INCLUDE_DIRS})
target_link_libraries(prom decoder ${OpenCV_LIBS})