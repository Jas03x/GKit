static const char* SKYBOX_PIXEL_SHADER =
"	#version 330						\n\
										\n\
	in vec3 uv;							\n\
										\n\
	uniform samplerCube cubeMap;		\n\
										\n\
	out vec4 color;						\n\
										\n\
	void main()							\n\
	{									\n\
		color = texture(cubeMap, uv);	\n\
	}									\n\
";