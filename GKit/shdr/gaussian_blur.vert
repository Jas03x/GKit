#version 330

in  vec2 vertex;

uniform vec2 direction;
uniform float length;

out vec2 uv_coords[11];

void main()
{
    gl_Position = vec4(vertex.x, vertex.y, 0, 1);

    vec2 center = (vertex + vec2(1.0)) * 0.5;
    float size = 1.0 / length;

    uv_coords[ 0] = center + size * vec2(-5 * direction.x, -5 * direction.y);
    uv_coords[ 1] = center + size * vec2(-4 * direction.x, -4 * direction.y);
    uv_coords[ 2] = center + size * vec2(-3 * direction.x, -3 * direction.y);
    uv_coords[ 3] = center + size * vec2(-2 * direction.x, -2 * direction.y);
    uv_coords[ 4] = center + size * vec2(-1 * direction.x, -1 * direction.y);
    uv_coords[ 5] = center + size * vec2( 0 * direction.x,  0 * direction.y);
    uv_coords[ 6] = center + size * vec2( 1 * direction.x,  1 * direction.y);
    uv_coords[ 7] = center + size * vec2( 2 * direction.x,  2 * direction.y);
    uv_coords[ 8] = center + size * vec2( 3 * direction.x,  3 * direction.y);
    uv_coords[ 9] = center + size * vec2( 4 * direction.x,  4 * direction.y);
    uv_coords[10] = center + size * vec2( 5 * direction.x,  5 * direction.y);
}
