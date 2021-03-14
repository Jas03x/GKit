static const char* SPRITE_PIXEL_SHADER =
"	#version 330						\n\
										\n\
	in vec2 uv;							\n\
										\n\
	uniform sampler2D sprite;			\n\
										\n\
	out vec4 color;						\n\
										\n\
	void main()							\n\
	{									\n\
		color = texture(sprite, uv);	\n\
	}									\n\
";
