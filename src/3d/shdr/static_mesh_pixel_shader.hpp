static const char* STATIC_MESH_PIXEL_SHADER =
"	#version 330																			\n\
																							\n\
	in vec2 _uv;																			\n\
	in float _df;																			\n\
	in float _sf;																			\n\
																							\n\
	uniform vec3 sun_color;																	\n\
	uniform sampler2D ambient_texture;														\n\
	uniform sampler2D diffuse_texture;														\n\
	uniform sampler2D specular_texture;														\n\
																							\n\
	out vec4 fragment_color;																\n\
																							\n\
	void main()																				\n\
	{																						\n\
		vec3 color = texture(diffuse_texture, _uv).rgb;										\n\
		float af = texture(ambient_texture, _uv).r;											\n\
		float df = _df;																		\n\
		float sf = texture(specular_texture, _uv).r * _sf;									\n\
																							\n\
		vec3 ambient = color * af;															\n\
		vec3 diffuse = color * df;															\n\
		vec3 specular = sun_color * sf;														\n\
		vec3 pixel = ambient + diffuse + specular;											\n\
																							\n\
		fragment_color = vec4(pixel, 1.0);													\n\
		//fragment_color = vec4(df,df,df,1);												\n\
fragment_color = vec4(vec3(af),1);												\n\
	}																						\n\
";
