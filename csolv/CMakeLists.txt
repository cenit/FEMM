cmake_minimum_required(VERSION 3.10)
project(csolv C CXX)

set(csolv_SOURCES
  complex.h
  CSOLV.H
  CSOLVDLG.H
  femmedoccore.h
  MESH.H
  RESOURCE.H
  SPARS.H
  StdAfx.h
  complex.cpp
  csolv.cpp
  CSOLVDLG.CPP
  cspars.cpp
  CUTHILL.CPP
  femmedoccore.cpp
  MAIN.CPP
  PROB1BIG.CPP
  STDAFX.CPP
  CSOLV.RC
)

add_library(csolv ${csolv_SOURCES})
target_include_directories(csolv PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
