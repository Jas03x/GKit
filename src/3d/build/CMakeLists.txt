cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_STATIC_LIBRARY_SUFFIX "")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../../../bin)

include_directories("../../../inc/")

set(common_sources
    ../src/common/mesh_util.cpp
)

set(core_sources
    ../src/core/bone.cpp
    ../src/core/camera_2d.cpp
    ../src/core/camera_3d.cpp
    ../src/core/debug_draw_interface.cpp
    ../src/core/dynamic_mesh.cpp
    ../src/core/node.cpp
    ../src/core/skybox.cpp
    ../src/core/sprite.cpp
    ../src/core/static_mesh.cpp
    ../src/core/sun.cpp
    ../src/core/texture_manager.cpp
    ../src/core/view.cpp
)

set(importer_sources
    ../src/importer/bitmap.cpp
    ../src/importer/bmp_reader.cpp
    ../src/importer/collada_format.cpp
    ../src/importer/collada_importer.cpp
    ../src/importer/collada_parser.cpp
    ../src/importer/mdl_importer.cpp
    ../src/importer/mesh_data.cpp
    ../src/importer/tga_reader.cpp
)

set(io_sources
    ../src/io/file.cpp
    ../src/io/filesystem.cpp
    ../src/io/xml.cpp
)

set(physics_sources
)

set(renderer_sources
    ../src/renderer/blur_shader.cpp
    ../src/renderer/debug_drawer.cpp
    ../src/renderer/dynamic_mesh_renderer.cpp
    ../src/renderer/frame_renderer.cpp
    ../src/renderer/particle_renderer_3d.cpp
    ../src/renderer/skybox_renderer.cpp
    ../src/renderer/sprite_renderer.cpp
    ../src/renderer/static_mesh_renderer.cpp
)

set(utility_sources
    ../src/utility/random.cpp
    ../src/utility/string_buffer.cpp
)

add_library(graphics.lib STATIC
    ${common_sources}
    ${core_sources}
    ${importer_sources}
    ${io_sources}
    ${physics_sources}
    ${renderer_sources}
    ${utility_sources}
)
