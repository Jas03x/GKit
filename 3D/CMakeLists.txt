cmake_minimum_required(VERSION 3.10)

project(3D)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_STATIC_LIBRARY_SUFFIX "")

include_directories("../inc/" "/usr/local/include/")

set(common_sources
    src/common/mesh_util.cpp
)

set(core_sources
    src/core/bone.cpp
    src/core/camera_2d.cpp
    src/core/camera_3d.cpp
    src/core/debug_draw_interface.cpp
    src/core/dynamic_mesh.cpp
    src/core/node.cpp
    src/core/skybox.cpp
    src/core/sprite.cpp
    src/core/static_mesh.cpp
    src/core/sun.cpp
    src/core/texture_manager.cpp
    src/core/view.cpp
)

set(graphics_sources
    src/graphics/cubemap.cpp
    src/graphics/framebuffer.cpp
    src/graphics/graphics.cpp
    src/graphics/mesh.cpp
    src/graphics/particle_array.cpp
    src/graphics/quad.cpp
    src/graphics/render_buffer.cpp
    src/graphics/shader.cpp
    src/graphics/texture.cpp
    src/graphics/vertex_array.cpp
    src/graphics/vertex_buffer.cpp
)

set(importer_sources
    src/importer/bitmap.cpp
    src/importer/bmp_reader.cpp
    src/importer/collada_format.cpp
    src/importer/collada_importer.cpp
    src/importer/collada_parser.cpp
    src/importer/mdl_importer.cpp
    src/importer/mesh_data.cpp
    src/importer/tga_reader.cpp
)

set(io_sources
    src/io/file.cpp
    src/io/filesystem.cpp
    src/io/xml.cpp
)

set(math_sources
    src/math/frustum.cpp
    src/math/linear.cpp
    src/math/transform_2d.cpp
    src/math/transform_3d.cpp
)

set(physics_sources
)

set(renderer_sources
    src/renderer/blur_shader.cpp
    src/renderer/debug_drawer.cpp
    src/renderer/dynamic_mesh_renderer.cpp
    src/renderer/frame_renderer.cpp
    src/renderer/particle_renderer_3d.cpp
    src/renderer/skybox_renderer.cpp
    src/renderer/sprite_renderer.cpp
    src/renderer/static_mesh_renderer.cpp
)

set(system_sources
    src/system/clock.cpp
    src/system/context.cpp
)

set(utility_sources
    src/utility/random.cpp
    src/utility/string_buffer.cpp
)

add_library(3D.lib STATIC
    ${common_sources}
    ${core_sources}
    ${graphics_sources}
    ${importer_sources}
    ${io_sources}
    ${math_sources}
    ${physics_sources}
    ${renderer_sources}
    ${system_sources}
    ${utility_sources}
)
