static const char* FRAME_VERTEX_SHADER =
"   #version 330                                        \n\
                                                        \n\
    in  vec2 vertex;                                    \n\
    out vec2 uv;                                        \n\
                                                        \n\
    void main()                                         \n\
    {                                                   \n\
        gl_Position = vec4(vertex.x, vertex.y, 0, 1);   \n\
                                                        \n\
        uv = (vertex + vec2(1.0)) * 0.5;                \n\
    }                                                   \n\
";
