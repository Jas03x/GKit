static const char* DYNAMIC_MESH_VERTEX_SHADER =
"	#version 330																					\n\
																									\n\
	in vec3  vertex;																				\n\
	in vec3  normal;																				\n\
	in vec2  uv;																					\n\
	in int   node;																					\n\
	in uvec4 bone_indices;																			\n\
	in vec4  bone_weights;																			\n\
																									\n\
	uniform vec3 sun_position;																		\n\
																									\n\
	uniform mat4 projection_matrix;																	\n\
	uniform mat4 vertex_matrices[32]; // includes the bone matrices									\n\
	uniform mat4 normal_matrices[32];																\n\
																									\n\
	out vec2 _uv;																					\n\
	out float af;																					\n\
	out float df;																					\n\
	out float sf;																					\n\
																									\n\
	void main()																						\n\
	{																								\n\
		mat4 vertex_matrix = vertex_matrices[bone_indices[0]] * bone_weights[0];					\n\
		vertex_matrix     += vertex_matrices[bone_indices[1]] * bone_weights[1];					\n\
		vertex_matrix     += vertex_matrices[bone_indices[2]] * bone_weights[2];					\n\
		vertex_matrix     += vertex_matrices[bone_indices[3]] * bone_weights[3];					\n\
																									\n\
		mat4 normal_matrix = normal_matrices[bone_indices[0]] * bone_weights[0];					\n\
		normal_matrix     += normal_matrices[bone_indices[1]] * bone_weights[1];					\n\
		normal_matrix     += normal_matrices[bone_indices[2]] * bone_weights[2];					\n\
		normal_matrix     += normal_matrices[bone_indices[3]] * bone_weights[3];					\n\
																									\n\
		vec3 vertex_position = vec3(vertex_matrix * vec4(vertex.x, vertex.y, vertex.z, 1.0f));		\n\
		//vec3 vertex_position = vec3(vertex_matrix * vec4(vertex.x, -vertex.z, vertex.y, 1.0f));	\n\
		gl_Position = projection_matrix * vec4(vertex_position, 1.0);								\n\
																									\n\
		_uv = uv;																					\n\
		vec3 _normal = normalize(vec3(normal_matrix * vec4(normal, 0.0)));							\n\
		vec3 _camera_ray = normalize(vec3(0.0) - vertex_position);									\n\
		vec3 _light_ray = normalize(sun_position + _camera_ray);									\n\
																									\n\
		af = 0.5;																					\n\
		df = clamp(dot(_normal, _light_ray), 0.0, 1.0);												\n\
		sf = clamp(dot(_camera_ray, reflect(-_light_ray, _normal)), 0.0, 1.0) * 0.1;				\n\
	}																								\n\
";
