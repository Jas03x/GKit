cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_STATIC_LIBRARY_SUFFIX "")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../../../bin)

include_directories("../../../inc/")

set(graphics_sources
    ../src/cubemap.cpp
    ../src/framebuffer.cpp
    ../src/graphics.cpp
    ../src/mesh.cpp
    ../src/particle_array.cpp
    ../src/quad.cpp
    ../src/render_buffer.cpp
    ../src/shader.cpp
    ../src/texture.cpp
    ../src/vertex_array.cpp
    ../src/vertex_buffer.cpp
)

add_library(3D.lib STATIC ${graphics_sources})
