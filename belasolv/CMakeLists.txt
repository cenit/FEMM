cmake_minimum_required(VERSION 3.10)
project(belasolv C CXX)

set(belasolv_SOURCES
  belasolv.h
  belasolvDlg.h
  femmedoccore.h
  mesh.h
  resource.h
  spars.h
  StdAfx.h
  belasolv.cpp
  belasolvDlg.cpp
  cuthill.cpp
  femmedoccore.cpp
  main.cpp
  prob1big.cpp
  spars.cpp
  StdAfx.cpp
  belasolv.rc
)

add_library(belasolv ${belasolv_SOURCES})
target_include_directories(belasolv PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
