cmake_minimum_required(VERSION 3.10)
project(hsolv C CXX)

find_package(MFC REQUIRED)
set(CMAKE_MFC_FLAG 2)
add_definitions(-D_WIN32_WINNT=0x0501)

set(hsolv_SOURCES
  complex.h
  HSOLV.H
  hsolvDlg.h
  hsolvdoc.h
  MESH.H
  RESOURCE.H
  SPARS.H
  StdAfx.h
  complex.cpp
  CUTHILL.CPP
  hsolv.cpp
  hsolvDlg.cpp
  hsolvdoc.cpp
  MAIN.CPP
  prob1big.cpp
  SPARS.CPP
  STDAFX.CPP
  hsolv.rc
)

add_executable(hsolv WIN32 ${hsolv_SOURCES})
target_include_directories(hsolv PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
