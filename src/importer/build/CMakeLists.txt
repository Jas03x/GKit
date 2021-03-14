cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_STATIC_LIBRARY_SUFFIX "")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../../../bin)

include_directories("../../../inc/" "/usr/local/include/")

set(sources
    ../src/bitmap.cpp
    ../src/bmp_reader.cpp
    ../src/collada_format.cpp
    ../src/collada_importer.cpp
    ../src/collada_parser.cpp
    ../src/mdl_importer.cpp
    ../src/mesh_data.cpp
    ../src/tga_reader.cpp
)

add_library(importer.lib STATIC ${sources})
