cmake_minimum_required(VERSION 3.10)
project(femmplot C CXX)

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

add_library(femmplot ${femmplot_SOURCES})
target_include_directories(femmplot PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
