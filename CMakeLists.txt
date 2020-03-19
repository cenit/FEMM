cmake_minimum_required(VERSION 3.15)
project(FEMM C CXX)

option(BUILD_SHARED_LIBS OFF)

#necessary only for femm, liblua and triangle moved to their own CMakeLists.txt
#if(MSVC)
#  set(ADDITIONAL_CXX_FLAGS "/wd4311 /wd4312 /wd4477")
#  set(ADDITIONAL_C_FLAGS "/wd4311 /wd4312 /wd4477")
#  set(CMAKE_CXX_FLAGS "${ADDITIONAL_CXX_FLAGS} ${CMAKE_CXX_FLAGS}")
#  set(CMAKE_C_FLAGS "${ADDITIONAL_C_FLAGS} ${CMAKE_C_FLAGS}")
#endif()

find_package(MFC REQUIRED)
set(CMAKE_MFC_FLAG 2)
add_definitions(-D_WIN32_WINNT=0x0501)

add_subdirectory(belasolv)
add_subdirectory(csolv)
add_subdirectory(femm)
add_subdirectory(femmplot)
add_subdirectory(fkn)
add_subdirectory(hsolv)
add_subdirectory(liblua)
add_subdirectory(ResizableLib)
add_subdirectory(scifemm)
add_subdirectory(triangle)
