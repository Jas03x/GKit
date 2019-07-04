#version 330

in vec3 uv;

uniform samplerCube cubeMap;

out vec4 color;

void main()
{
	color = texture(cubeMap, uv);
}
