static const char* DEBUG_VERTEX_SHADER =
"	#version 330																			\n\
																							\n\
	in vec3 vertex;																			\n\
	in uint colour;																			\n\
																							\n\
	uniform mat4 matrix;																	\n\
																							\n\
	flat out uint _colour;																	\n\
																							\n\
	void main()																				\n\
	{																						\n\
		_colour = colour;																	\n\
		gl_Position = matrix * vec4(vertex, 1);												\n\
    }																						\n\
";
