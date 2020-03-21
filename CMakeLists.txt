cmake_minimum_required(VERSION 3.15)
project(FEMM C CXX)

option(BUILD_MANUAL "Build the manual" OFF)

if(BUILD_MANUAL)
  add_subdirectory(manual)
endif()

add_subdirectory(belasolv)
add_subdirectory(csolv)
add_subdirectory(liblua)
add_subdirectory(ResizableLib)
add_subdirectory(femm)
add_subdirectory(femmplot)
add_subdirectory(fkn)
add_subdirectory(hsolv)
add_subdirectory(scifemm)
add_subdirectory(triangle)
