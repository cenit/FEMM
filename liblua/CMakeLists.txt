cmake_minimum_required(VERSION 3.10)
project(liblua C CXX)

if(MSVC)
  set(ADDITIONAL_CXX_FLAGS "/wd4311 /wd4312 /wd4477")
  set(ADDITIONAL_C_FLAGS "/wd4311 /wd4312 /wd4477")
  set(CMAKE_CXX_FLAGS "${ADDITIONAL_CXX_FLAGS} ${CMAKE_CXX_FLAGS}")
  set(CMAKE_C_FLAGS "${ADDITIONAL_C_FLAGS} ${CMAKE_C_FLAGS}")
  find_package(MFC REQUIRED)
  set(CMAKE_MFC_FLAG 2)
endif()

set(liblua_SOURCES
  COMPLEX.H
  lapi.h
  lauxlib.h
  lcode.h
  ldebug.h
  ldo.h
  lfunc.h
  lgc.h
  llex.h
  llimits.h
  lmem.h
  lobject.h
  lopcodes.h
  lparser.h
  lstate.h
  lstring.h
  ltable.h
  ltm.h
  lua.h
  luadebug.h
  lualib.h
  lundump.h
  lvm.h
  lzio.h
  COMPLEX.CPP
  lapi.cpp
  lauxlib.cpp
  lbaselib.cpp
  lcode.cpp
  ldblib.cpp
  ldebug.cpp
  ldo.cpp
  lfunc.cpp
  lgc.cpp
  liolib.cpp
  llex.cpp
  lmathlib.cpp
  lmem.cpp
  lobject.cpp
  lparser.cpp
  lstate.cpp
  lstring.cpp
  lstrlib.cpp
  ltable.cpp
  ltests.cpp
  ltm.cpp
  lundump.cpp
  lvm.cpp
  lzio.cpp
)

add_library(liblua ${liblua_SOURCES})
target_include_directories(liblua PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(liblua PRIVATE _CRT_SECURE_NO_WARNINGS)
