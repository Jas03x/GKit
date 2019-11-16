#version 330

in vec3 vertex;
in vec3 normal;
in vec2 uv;
in int node;

uniform vec3 sun_position;

uniform mat4 projection_matrix;
uniform mat4 vertex_matrices[32];
uniform mat4 normal_matrices[32];

out vec2 _uv;
out float af;
out float df;
out float sf;

void main()
{
	vec3 vertex_position = vec3(vertex_matrices[node] * vec4(vertex, 1.0f));
	gl_Position = projection_matrix * vec4(vertex_position, 1.0);

	_uv = uv;
	vec3 _normal = normalize(vec3(normal_matrices[node] * vec4(normal, 0.0)));
	vec3 _camera_ray = normalize(vec3(0.0) - vertex_position);
	vec3 _light_ray = normalize(sun_position + _camera_ray);

	af = 0.5;
	df = clamp(dot(_normal, _light_ray), 0.0, 1.0);
	sf = clamp(dot(_camera_ray, reflect(-_light_ray, _normal)), 0.0, 1.0) * 0.1;
}
