static const char* GAUSSIAN_BLUR_PIXEL_SHADER =
"   #version 330                                                        \n\
                                                                        \n\
    in vec2 uv_coords[11];                                              \n\
                                                                        \n\
    uniform sampler2D textureID;                                        \n\
                                                                        \n\
    out vec4 frag_color;                                                \n\
                                                                        \n\
    void main()                                                         \n\
    {                                                                   \n\
        frag_color = vec4(0.0);                                         \n\
        frag_color += texture(textureID, uv_coords[ 0])  * 0.0093;      \n\
        frag_color += texture(textureID, uv_coords[ 1])  * 0.028002;    \n\
        frag_color += texture(textureID, uv_coords[ 2])  * 0.065984;    \n\
        frag_color += texture(textureID, uv_coords[ 3])  * 0.121703;    \n\
        frag_color += texture(textureID, uv_coords[ 4])  * 0.175713;    \n\
        frag_color += texture(textureID, uv_coords[ 5])  * 0.198596;    \n\
        frag_color += texture(textureID, uv_coords[ 6])  * 0.175713;    \n\
        frag_color += texture(textureID, uv_coords[ 7])  * 0.121703;    \n\
        frag_color += texture(textureID, uv_coords[ 8])  * 0.065984;    \n\
        frag_color += texture(textureID, uv_coords[ 9])  * 0.028002;    \n\
        frag_color += texture(textureID, uv_coords[10])  * 0.0093;      \n\
    }                                                                   \n\
";
