#version 330

in vec2 uv_coords[11];

uniform sampler2D textureID;

out vec4 frag_color;

void main()
{
    frag_color = vec4(0.0);
    frag_color += texture(textureID, uv_coords[ 0])  * 0.0093;
    frag_color += texture(textureID, uv_coords[ 1])  * 0.028002;
    frag_color += texture(textureID, uv_coords[ 2])  * 0.065984;
    frag_color += texture(textureID, uv_coords[ 3])  * 0.121703;
    frag_color += texture(textureID, uv_coords[ 4])  * 0.175713;
    frag_color += texture(textureID, uv_coords[ 5])  * 0.198596;
    frag_color += texture(textureID, uv_coords[ 6])  * 0.175713;
    frag_color += texture(textureID, uv_coords[ 7])  * 0.121703;
    frag_color += texture(textureID, uv_coords[ 8])  * 0.065984;
    frag_color += texture(textureID, uv_coords[ 9])  * 0.028002;
    frag_color += texture(textureID, uv_coords[10])  * 0.0093;
}
