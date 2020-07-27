static const char* STATIC_MESH_VERTEX_SHADER =
"	#version 330																			\n\
																							\n\
	in vec3 vertex;																			\n\
	in vec3 normal;																			\n\
	in vec2 uv;																				\n\
	in int node;																			\n\
																							\n\
	uniform vec3 sun_position;																\n\
																							\n\
	uniform mat4 projection_matrix;															\n\
	uniform mat4 vertex_matrices[32];														\n\
	uniform mat4 normal_matrices[32];														\n\
																							\n\
	out vec2 _uv;																			\n\
	out float af;																			\n\
	out float df;																			\n\
	out float sf;																			\n\
																							\n\
	void main()																				\n\
	{																						\n\
		vec3 vertex_world = vec3(vertex_matrices[node] * vec4(vertex, 1.0));				\n\
		gl_Position = projection_matrix * vec4(vertex_world, 1.0);							\n\
																							\n\
		_uv = uv;																			\n\
		vec3 _normal = normalize(vec3(normal_matrices[node] * vec4(normal, 0.0)));			\n\
		vec3 _camera_ray = normalize(vec3(0.0) - vertex_world);								\n\
		vec3 _light_ray = normalize(sun_position - vertex_world);							\n\
																							\n\
		af = 0.5;																			\n\
		df = clamp(dot(_normal, _light_ray), 0.0, 1.0);										\n\
		sf = clamp(dot(_camera_ray, reflect(-_light_ray, _normal)), 0.0, 1.0);				\n\
	}																						\n\
";
