#version 330

in vec3  vertex;
in vec3  normal;
in vec2  uv;
in int   node;
in uvec4 bone_indices;
in vec4  bone_weights;

uniform vec3 sun_position;

uniform mat4 projection_matrix;
uniform mat4 vertex_matrices[32]; // includes the bone matrices
uniform mat4 normal_matrices[32];

out vec2 _uv;
out float af;
out float df;
out float sf;

void main()
{
	mat4 vertex_matrix = vertex_matrices[bone_indices[0]] * bone_weights[0];
    vertex_matrix     += vertex_matrices[bone_indices[1]] * bone_weights[1];
    vertex_matrix     += vertex_matrices[bone_indices[2]] * bone_weights[2];
    vertex_matrix     += vertex_matrices[bone_indices[3]] * bone_weights[3];

	mat4 normal_matrix = normal_matrices[bone_indices[0]] * bone_weights[0];
    normal_matrix     += normal_matrices[bone_indices[1]] * bone_weights[1];
    normal_matrix     += normal_matrices[bone_indices[2]] * bone_weights[2];
    normal_matrix     += normal_matrices[bone_indices[3]] * bone_weights[3];
	
	vec3 vertex_position = vec3(vertex_matrix * vec4(vertex.x, vertex.y, vertex.z, 1.0f));
	//vec3 vertex_position = vec3(vertex_matrix * vec4(vertex.x, -vertex.z, vertex.y, 1.0f));
	gl_Position = projection_matrix * vec4(vertex_position, 1.0);

	_uv = uv;
	vec3 _normal = normalize(vec3(normal_matrix * vec4(normal, 0.0)));
	vec3 _camera_ray = normalize(vec3(0.0) - vertex_position);
	vec3 _light_ray = normalize(sun_position + _camera_ray);

	af = 0.5;
	df = clamp(dot(_normal, _light_ray), 0.0, 1.0);
	sf = clamp(dot(_camera_ray, reflect(-_light_ray, _normal)), 0.0, 1.0) * 0.1;
}
