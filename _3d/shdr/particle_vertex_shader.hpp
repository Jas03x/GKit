static const char* PARTICLE_VERTEX_SHADER =
"	#version 330																				\n\
																								\n\
	in vec3  position;																			\n\
	in uvec2 uv_coord;																			\n\
	in uint  size;																				\n\
																								\n\
	uniform mat4 view_matrix;																	\n\
	uniform mat4 projection_matrix;																\n\
																								\n\
	flat out uvec2 _uv_coord;																	\n\
																								\n\
	void main()																					\n\
	{																							\n\
		float radius = size * 0.5;																\n\
																								\n\
		vec3 pos_world_space = (view_matrix * vec4(position, 1)).xyz;							\n\
																								\n\
		vec4 r = projection_matrix * vec4(pos_world_space + vec3(-radius, radius, 0), 1);		\n\
		vec4 l = projection_matrix * vec4(pos_world_space + vec3( radius, radius, 0), 1);		\n\
		r.xyz /= r.w;																			\n\
		l.xyz /= l.w;																			\n\
																								\n\
		gl_PointSize = length(r.xyz - l.xyz);													\n\
		gl_Position = projection_matrix * vec4(pos_world_space, 1);								\n\
																								\n\
		_uv_coord = uv_coord;																	\n\
	}																							\n\
";
