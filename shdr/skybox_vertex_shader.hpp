const char* SKYBOX_VERTEX_SHADER =
"	#version 330																		\n\
																						\n\
	in vec2 vertex;																		\n\
																						\n\
	uniform mat4 inv_vp_matrix;															\n\
																						\n\
	out vec3 uv;																		\n\
																						\n\
	void main()																			\n\
	{																					\n\
		gl_Position = vec4(vertex.x, vertex.y, 0, 1);									\n\
																						\n\
		uv = normalize(vec3(inv_vp_matrix * -vec4(vertex.x, vertex.y, -1, 1)).xyz);		\n\
	}																					\n\
";