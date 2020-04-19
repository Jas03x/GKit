static const char* PARTICLE_PIXEL_SHADER =
"   #version 330                                                        \n\
                                                                        \n\
    flat in uvec2 _uv_coord;                                            \n\
                                                                        \n\
    uniform sampler2D sprite;                                           \n\
    uniform vec2 sprite_size;                                           \n\
                                                                        \n\
    out vec4 color;                                                     \n\
                                                                        \n\
    void main()                                                         \n\
    {                                                                   \n\
        float x = sprite_size.x * (_uv_coord.x + gl_PointCoord.x);      \n\
        float y = sprite_size.y * (_uv_coord.y + gl_PointCoord.y);      \n\
	    color = texture(sprite, vec2(x, y));                            \n\
    }                                                                   \n\
";