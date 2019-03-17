#version 330

flat in uvec2 _uv_coord;

uniform sampler2D sprite;
uniform vec2 sprite_size;

out vec4 color;

void main()
{
    float x = sprite_size.x * (_uv_coord.x + gl_PointCoord.x);
    float y = sprite_size.y * (_uv_coord.y + gl_PointCoord.y);
	color = texture(sprite, vec2(x, y));
}
