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
	float SUN_POWER = 0.8; // TODO: MAKE THIS A CONFIGURABLE UNIFORM IN SHADER LATER ON		\n\
	// WE CAN MAKE THIS SHADER LEVEL OR GLOBAL LEVEL IN SUN NAMESPACE						\n\
																							\n\
	out vec4 fragment_color;																\n\
																							\n\
	void main()																				\n\
	{																						\n\
		vec3 color = texture(diffuse_texture, _uv).rgb;										\n\
																							\n\
		vec3 ambient = color * af;															\n\
		vec3 diffuse = color * df;															\n\
		vec3 specular = sun_color * sf;														\n\
		vec3 pixel = ambient + diffuse + specular;											\n\
																							\n\
		fragment_color = vec4(pixel * SUN_POWER, 1.0);									    \n\
		//fragment_color = vec4(df,df,df,1);												\n\
	}																						\n\
";
