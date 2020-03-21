cmake_minimum_required(VERSION 3.10)
project(femmplot C CXX)

find_package(MFC REQUIRED)
set(CMAKE_MFC_FLAG 2)
add_definitions(-D_WIN32_WINNT=0x0501)

set(femmplot_SOURCES
  femmplot.h
  femmplotDoc.h
  femmplotView.h
  MainFrm.h
  resource.h
  StdAfx.h
  femmplot.cpp
  femmplotDoc.cpp
  femmplotView.cpp
  MainFrm.cpp
  StdAfx.cpp
  femmplot.rc
)

add_executable(femmplot WIN32 ${femmplot_SOURCES})
target_include_directories(femmplot PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
