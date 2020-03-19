cmake_minimum_required(VERSION 3.10)
project(ResizableLib C CXX)

set(ResizableLib_SOURCES
  ResizableDialog.h
  ResizableFormView.h
  ResizableFrame.h
  ResizableGrip.h
  ResizableLayout.h
  ResizableMDIChild.h
  ResizableMDIFrame.h
  ResizableMinMax.h
  ResizableMsgSupport.h
  ResizablePage.h
  ResizablePageEx.h
  ResizableSheet.h
  ResizableSheetEx.h
  ResizableState.h
  StdAfx.h
  ResizableDialog.cpp
  ResizableFormView.cpp
  ResizableFrame.cpp
  ResizableGrip.cpp
  ResizableLayout.cpp
  ResizableMDIChild.cpp
  ResizableMDIFrame.cpp
  ResizableMinMax.cpp
  ResizablePage.cpp
  ResizablePageEx.cpp
  ResizableSheet.cpp
  ResizableSheetEx.cpp
  ResizableState.cpp
  StdAfx.cpp
)

add_library(ResizableLib ${ResizableLib_SOURCES})
target_include_directories(ResizableLib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
