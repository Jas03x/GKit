#version 330

in vec2 vertex;

uniform mat3 matrix;

out vec2 uv;

void main()
{
	vec2 v = vec2(matrix * vec3(vertex, 1));
	gl_Position = vec4(v.x, v.y, 0.0, 1.0);
	
	uv = (vertex + vec2(1.0)) * 0.5;
}
