cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_STATIC_LIBRARY_SUFFIX "")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../../../bin)

include_directories("../../../inc/")

set(root_sources
    ../src/bone.cpp
    ../src/camera_2d.cpp
    ../src/camera_3d.cpp
    ../src/debug_draw_interface.cpp
    ../src/dynamic_mesh.cpp
    ../src/node.cpp
    ../src/skybox.cpp
    ../src/sprite.cpp
    ../src/static_mesh.cpp
    ../src/sun.cpp
    ../src/texture_manager.cpp
    ../src/view.cpp
    ../src/mesh_util.cpp
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

add_library(GKit3D.lib STATIC
    ${root_sources}
    ${renderer_sources}
)
