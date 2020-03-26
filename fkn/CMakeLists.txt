cmake_minimum_required(VERSION 3.10)
project(fkn C CXX)

set(INSTALL_BIN_DIR  "${CMAKE_SOURCE_DIR}" CACHE PATH "Path where exe will be installed")

find_package(MFC REQUIRED)
set(CMAKE_MFC_FLAG 2)
add_definitions(-D_WIN32_WINNT=0x0501)

set(fkn_SOURCES
  complex.h
  femmedoccore.h
  fkn.h
  fknDlg.h
  fullmatrix.h
  #lua.h
  mesh.h
  resource.h
  spars.h
  StdAfx.h
  #complex.cpp
  cspars.cpp
  cuthill.cpp
  femmedoccore.cpp
  fkn.cpp
  fknDlg.cpp
  fullmatrix.cpp
  main.cpp
  matprop.cpp
  prob1big.cpp
  prob2big.cpp
  prob3big.cpp
  prob4big.cpp
  spars.cpp
  StdAfx.cpp
  #tmp_prob1big.cpp
  fkn.rc
)

add_executable(fkn WIN32 ${fkn_SOURCES})
target_include_directories(fkn PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(fkn liblua)
install(TARGETS fkn DESTINATION "${INSTALL_BIN_DIR}")
