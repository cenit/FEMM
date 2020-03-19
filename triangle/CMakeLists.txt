cmake_minimum_required(VERSION 3.10)
project(triangle C CXX)

if(MSVC)
  set(ADDITIONAL_CXX_FLAGS "/wd4311 /wd4312 /wd4477")
  set(ADDITIONAL_C_FLAGS "/wd4311 /wd4312 /wd4477")
  set(CMAKE_CXX_FLAGS "${ADDITIONAL_CXX_FLAGS} ${CMAKE_CXX_FLAGS}")
  set(CMAKE_C_FLAGS "${ADDITIONAL_C_FLAGS} ${CMAKE_C_FLAGS}")
endif()

set(triangle_SOURCES
  resource.h
  triangle.h
  triangle.c
  triangle.rc
)

add_library(triangle ${triangle_SOURCES})
target_include_directories(triangle PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
