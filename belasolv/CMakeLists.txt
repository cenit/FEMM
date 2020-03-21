cmake_minimum_required(VERSION 3.10)
project(belasolv C CXX)

find_package(MFC REQUIRED)
set(CMAKE_MFC_FLAG 2)
add_definitions(-D_WIN32_WINNT=0x0501)

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

add_executable(belasolv WIN32 ${belasolv_SOURCES})
target_include_directories(belasolv PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
