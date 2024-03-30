cmake_minimum_required (VERSION 3.27)
project (main C CXX)

# Uncomment below to build and run in Unicode
add_definitions(-DUNICODE -D_UNICODE)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_DEBUG_POSTFIX _d)

add_subdirectory(plot2d)

# add the binary tree to the search path for include files
include_directories("${PROJECT_BINARY_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/plot2d")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/fftw)
add_definitions(-D_USE_MATH_DEFINES -D_CRT_SECURE_NO_WARNINGS -DWIN32 -D__STDC__ )
add_definitions(-DDEBUG=1 -DLOG_OUTPUT=1)
set( headers 
main.h
)

set( sources 
main.cpp
 Iir/iir/RBJ.cpp
Iir/iir/Biquad.cpp
Iir/iir/Cascade.cpp
Iir/iir/Custom.cpp
Iir/iir/PoleFilter.cpp
Iir/iir/Butterworth.cpp
Iir/iir/ChebyshevI.cpp
Iir/iir/ChebyshevII.cpp
)

add_executable(${PROJECT_NAME} ${sources} ${headers})
target_link_libraries(${PROJECT_NAME} plot2d ${CMAKE_CURRENT_SOURCE_DIR}/fftw/libfftw3-3.lib)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

