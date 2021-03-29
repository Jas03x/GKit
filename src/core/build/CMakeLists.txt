cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_STATIC_LIBRARY_SUFFIX "")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../../../bin)

include_directories("../../../inc/" "/usr/local/include/")

set(context_sources
    ../src/context/context.cpp
    ../src/context/clock.cpp
)

set(importer_sources
    ../src/importer/bitmap.cpp
    ../src/importer/mesh_data.cpp
    ../src/importer/tga_reader.cpp
    ../src/importer/collada_parser.cpp
    ../src/importer/collada_importer.cpp
    ../src/importer/mdl_importer.cpp
    ../src/importer/collada_format.cpp
    ../src/importer/bmp_reader.cpp
)

set(io_sources
    ../src/io/filesystem.cpp
    ../src/io/file.cpp
    ../src/io/xml.cpp
)

set(graphics_sources
    ../src/graphics/vertex_array.cpp
    ../src/graphics/mesh.cpp
    ../src/graphics/cubemap.cpp
    ../src/graphics/render_buffer.cpp
    ../src/graphics/framebuffer.cpp
    ../src/graphics/texture.cpp
    ../src/graphics/quad.cpp
    ../src/graphics/vertex_buffer.cpp
    ../src/graphics/shader.cpp
    ../src/graphics/graphics.cpp
    ../src/graphics/particle_array.cpp
)

set(math_sources
    ../src/math/linear.cpp
    ../src/math/frustum.cpp
    ../src/math/transform_3d.cpp
    ../src/math/transform_2d.cpp
)

set(utility_sources
    ../src/utility/random.cpp
    ../src/utility/string_buffer.cpp
)

add_library(GKitCore.lib STATIC
    ${context_sources}
    ${importer_sources}
    ${io_sources}
    ${graphics_sources}
    ${math_sources}
    ${utility_sources}
)
