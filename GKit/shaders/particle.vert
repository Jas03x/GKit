#version 330

in vec3  position;
in uvec2 uv_coord;
in uint  size;

uniform mat4 matrix;

flat out uvec2 _uv_coord;

void main()
{
    gl_PointSize = size;
    gl_Position = matrix * vec4(position, 1);

    _uv_coord = uv_coord;
}
