cmake_minimum_required(VERSION 3.10)
project(scifemm C CXX)

find_package(MFC REQUIRED)
set(CMAKE_MFC_FLAG 2)
add_definitions(-D_WIN32_WINNT=0x0501)

set(scifemm_SOURCES
  femm.h
  femm.cpp
  scilink.cpp
)

add_library(scifemm ${scifemm_SOURCES})
target_include_directories(scifemm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
