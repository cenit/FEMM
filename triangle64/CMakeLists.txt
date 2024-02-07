cmake_minimum_required(VERSION 3.10)
project(triangle C CXX)

set(INSTALL_BIN_DIR  "${CMAKE_SOURCE_DIR}" CACHE PATH "Path where exe will be installed")

if(MSVC)
  set(ADDITIONAL_CXX_FLAGS "/wd4311 /wd4312 /wd4477")
  set(ADDITIONAL_C_FLAGS "/wd4311 /wd4312 /wd4477")
  set(CMAKE_CXX_FLAGS "${ADDITIONAL_CXX_FLAGS} ${CMAKE_CXX_FLAGS}")
  set(CMAKE_C_FLAGS "${ADDITIONAL_C_FLAGS} ${CMAKE_C_FLAGS}")
endif()

set(triangle_SOURCES
  resource.h
  triangle.h
  triangle.c
  triangle.rc
)

#set(CMAKE_EXE_LINKER_FLAGS "/ENTRY:mainCRTStartup")
#add_executable(triangle WIN32 ${triangle_SOURCES})

add_executable(triangle ${triangle_SOURCES})
target_include_directories(triangle PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(triangle PRIVATE _CRT_SECURE_NO_WARNINGS)
#target_link_options(triangle PRIVATE /OPT:ICF)

install(TARGETS triangle DESTINATION "${INSTALL_BIN_DIR}")
