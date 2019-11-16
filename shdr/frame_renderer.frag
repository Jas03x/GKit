#version 330

in vec2 uv;

uniform float opacity;
uniform sampler2D textureID;

out vec4 frag_color;

void main()
{
    frag_color = texture(textureID, uv);
    frag_color.a *= opacity;
}
