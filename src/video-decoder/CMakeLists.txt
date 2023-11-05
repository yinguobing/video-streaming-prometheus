cmake_minimum_required(VERSION 3.16.0)

find_package(PkgConfig REQUIRED)
pkg_check_modules(
  LIBAV
  REQUIRED
  IMPORTED_TARGET
  libavdevice
  libavfilter
  libavformat
  libavcodec
  libswresample
  libswscale
  libavutil)

add_library(decoder STATIC video_decoder.cpp)
target_link_libraries(decoder PUBLIC PkgConfig::LIBAV)
