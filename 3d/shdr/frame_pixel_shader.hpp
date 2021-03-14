static const char* FRAME_PIXEL_SHADER =
"   #version 330                                \n\
                                                \n\
    in vec2 uv;                                 \n\
                                                \n\
    uniform float opacity;                      \n\
    uniform sampler2D textureID;                \n\
                                                \n\
    out vec4 frag_color;                        \n\
                                                \n\
    void main()                                 \n\
    {                                           \n\
        frag_color = texture(textureID, uv);    \n\
        frag_color.a *= opacity;                \n\
    }                                           \n\
";
