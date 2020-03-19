cmake_minimum_required(VERSION 3.10)
project(scifemm C CXX)

set(scifemm_SOURCES
  femm.h
  femm.cpp
  scilink.cpp
)

add_library(scifemm ${scifemm_SOURCES})
target_include_directories(scifemm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
