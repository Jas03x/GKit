static const char* SPRITE_VERTEX_SHADER =
"	#version 330									\n\
													\n\
	in vec2 vertex;									\n\
													\n\
	uniform mat3 matrix;							\n\
													\n\
	out vec2 uv;									\n\
													\n\
	void main()										\n\
	{												\n\
		vec2 v = vec2(matrix * vec3(vertex, 1));	\n\
		gl_Position = vec4(v.x, v.y, 0.0, 1.0);		\n\
													\n\
		uv = (vertex + vec2(1.0)) * 0.5;			\n\
	}												\n\
";