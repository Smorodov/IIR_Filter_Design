cmake_minimum_required(VERSION 3.27)
cmake_policy(SET CMP0063 NEW) # Honor visibility properties.

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

project(plot2d)

# Some basics, set accordingly to your needs.
set(CMAKE_CXX_STANDARD 17)
set(BLEND2D_DIR "${CMAKE_CURRENT_SOURCE_DIR}/blend2d" CACHE PATH "Location of 'blend2d'")

# Blend2D
# -------

set(BLEND2D_STATIC TRUE)
include("${BLEND2D_DIR}/CMakeLists.txt")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libharu/zlib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libharu/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/libharu/libpng)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/libharu/zlib)
add_definitions(-D_USE_MATH_DEFINES)

set(SOURCES 
#main.cpp
dash.cpp
dash.h
BLAlignedText.cpp
BLAlignedText.h
XYPoint.cpp
XYPoint.h
XYRect.cpp
XYRect.h
XYTrack.cpp
XYTrack.h
XYData.cpp
XYData.h
XYAxis.cpp
XYAxis.h
XYPlot.cpp
XYPlot.h
XYDrawingBackend.cpp
XYDrawingBackend.h
XYblend2dBackend.cpp
XYblend2dBackend.h
)
  add_library(${PROJECT_NAME} STATIC ${SOURCES})
  target_link_libraries(${PROJECT_NAME} Blend2D::Blend2D)

