#version 330

in vec2 vertex;

uniform mat4 inv_vp_matrix;

out vec3 uv;

void main()
{
	gl_Position = vec4(vertex.x, vertex.y, 0, 1);
	
	uv = normalize(vec3(inv_vp_matrix * -vec4(vertex.x, vertex.y, -1, 1)).xyz);
}
