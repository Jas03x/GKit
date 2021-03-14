cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_STATIC_LIBRARY_SUFFIX "")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../../../bin)

include_directories("../../../inc/")

set(sources
    ../src/frustum.cpp
    ../src/linear.cpp
    ../src/transform_2d.cpp
    ../src/transform_3d.cpp
)

add_library(math.lib STATIC ${sources})
