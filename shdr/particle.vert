#version 330

in vec3  position;
in uvec2 uv_coord;
in uint  size;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;

flat out uvec2 _uv_coord;

void main()
{
	float radius = size * 0.5;

	vec3 pos_world_space = (view_matrix * vec4(position, 1)).xyz;

	vec4 r = projection_matrix * vec4(pos_world_space + vec3(-radius, radius, 0), 1);
	vec4 l = projection_matrix * vec4(pos_world_space + vec3( radius, radius, 0), 1);
	r.xyz /= r.w;
	l.xyz /= l.w;

    gl_PointSize = length(r.xyz - l.xyz);
	gl_Position = projection_matrix * vec4(pos_world_space, 1);

    _uv_coord = uv_coord;
}
