cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_STATIC_LIBRARY_SUFFIX "")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../../../bin)

include_directories("../../../inc/" "/usr/local/include/")

set(sources
    ../src/random.cpp
    ../src/string_buffer.cpp
)

add_library(utility.lib STATIC ${sources})
