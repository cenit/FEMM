cmake_minimum_required(VERSION 3.10)
project(csolv C CXX)

set(INSTALL_BIN_DIR  "${CMAKE_SOURCE_DIR}" CACHE PATH "Path where exe will be installed")

find_package(MFC REQUIRED)
set(CMAKE_MFC_FLAG 2)
add_definitions(-D_WIN32_WINNT=0x0501)

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

add_executable(csolv WIN32 ${csolv_SOURCES})
target_include_directories(csolv PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
install(TARGETS csolv DESTINATION "${INSTALL_BIN_DIR}")
