static const char* STATIC_MESH_PIXEL_SHADER =
"	#version 330																			\n\
																							\n\
	in vec2 _uv;																			\n\
	in float af;																			\n\
	in float df;																			\n\
	in float sf;																			\n\
																							\n\
	uniform vec3 sun_color;																	\n\
	uniform sampler2D diffuse_texture;														\n\
																							\n\
	// TODO: Make these paramaters configurable in the shader								\n\
	float AMBIENT_FACTOR = 0.5;																\n\
	float DIFFUSE_FACTOR = 0.5;																\n\
	float SPECULAR_FACTOR = 0.1;															\n\
																							\n\
	out vec4 fragment_color;																\n\
																							\n\
	void main()																				\n\
	{																						\n\
		vec3 color = texture(diffuse_texture, _uv).rgb;										\n\
																							\n\
		vec3 ambient = color * af * AMBIENT_FACTOR;											\n\
		vec3 diffuse = color * df * DIFFUSE_FACTOR;											\n\
		vec3 specular = sun_color * sf * SPECULAR_FACTOR;									\n\
		vec3 pixel = ambient + diffuse + specular;											\n\
																							\n\
		fragment_color = vec4(pixel, 1.0);													\n\
		//fragment_color = vec4(df,df,df,1);												\n\
	}																						\n\
";
