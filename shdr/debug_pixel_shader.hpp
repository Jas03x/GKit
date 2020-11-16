static const char* DEBUG_PIXEL_SHADER =
"	#version 330																			\n\
																							\n\
	flat in uint _colour;																	\n\
																							\n\
	out vec4 pixel;																			\n\
																							\n\
	const vec3 colour_array[5] = vec3[5]													\n\
	(																						\n\
		vec3(0, 0, 0),																		\n\
		vec3(1, 1, 1),																		\n\
		vec3(0, 0, 1),																		\n\
		vec3(1, 0, 0),																		\n\
		vec3(0, 1, 0)																		\n\
	);																						\n\
																							\n\
	void main()																				\n\
	{																						\n\
		pixel = vec4(colour_array[_colour], 1);												\n\
    }																						\n\
";
