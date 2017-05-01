cmake_minimum_required(VERSION 3.3)
project(core)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#set (OpenCV_DIR C:/Users/fbiedrzy/Documents/ubik/opencv/build/x64/vc14)
file(GLOB_RECURSE SOURCES src/*)
file(GLOB_RECURSE INCLUDES include/*)
file(GLOB_RECURSE LIBS lib/*)

set(SOURCE_FILES ${SOURCES} ${INCLUDES} ${LIBS})

add_executable( core ${SOURCE_FILES})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(OpenCV_FOUND 1)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(core ${OpenCV_LIBS})
