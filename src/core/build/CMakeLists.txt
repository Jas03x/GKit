cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_STATIC_LIBRARY_SUFFIX "")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../../../bin)

include_directories("../../../inc/" "/usr/local/include/")

set(context_sources
    ../context/context.cpp
    ../context/clock.cpp
)

set(importer_sources
    ../importer/bitmap.cpp
    ../importer/mesh_data.cpp
    ../importer/tga_reader.cpp
    ../importer/collada_parser.cpp
    ../importer/collada_importer.cpp
    ../importer/mdl_importer.cpp
    ../importer/collada_format.cpp
    ../importer/bmp_reader.cpp
)

set(io_sources
    ../io/filesystem.cpp
    ../io/file.cpp
    ../io/xml.cpp
)

set(graphics_sources
    ../graphics/vertex_array.cpp
    ../graphics/mesh.cpp
    ../graphics/cubemap.cpp
    ../graphics/render_buffer.cpp
    ../graphics/framebuffer.cpp
    ../graphics/texture.cpp
    ../graphics/quad.cpp
    ../graphics/vertex_buffer.cpp
    ../graphics/shader.cpp
    ../graphics/graphics.cpp
    ../graphics/particle_array.cpp
)

set(math_sources
    ../math/linear.cpp
    ../math/frustum.cpp
    ../math/transform_3d.cpp
    ../math/transform_2d.cpp
)

set(utility_sources
    ../utility/random.cpp
    ../utility/string_buffer.cpp
)

add_library(gk_core.lib STATIC
    ${context_sources}
    ${importer_sources}
    ${io_sources}
    ${graphics_sources}
    ${math_sources}
    ${utility_sources}
)
