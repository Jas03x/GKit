static const char* GAUSSIAN_BLUR_VERTEX_SHADER =
"   #version 330                                                                        \n\
                                                                                        \n\
    in  vec2 vertex;                                                                    \n\
                                                                                        \n\
    uniform vec2 direction;                                                             \n\
    uniform float length;                                                               \n\
                                                                                        \n\
    out vec2 uv_coords[11];                                                             \n\
                                                                                        \n\
    void main()                                                                         \n\
    {                                                                                   \n\
        gl_Position = vec4(vertex.x, vertex.y, 0, 1);                                   \n\
                                                                                        \n\
        vec2 center = (vertex + vec2(1.0)) * 0.5;                                       \n\
        float size = 1.0 / length;                                                      \n\
                                                                                        \n\
        uv_coords[ 0] = center + size * vec2(-5 * direction.x, -5 * direction.y);       \n\
        uv_coords[ 1] = center + size * vec2(-4 * direction.x, -4 * direction.y);       \n\
        uv_coords[ 2] = center + size * vec2(-3 * direction.x, -3 * direction.y);       \n\
        uv_coords[ 3] = center + size * vec2(-2 * direction.x, -2 * direction.y);       \n\
        uv_coords[ 4] = center + size * vec2(-1 * direction.x, -1 * direction.y);       \n\
        uv_coords[ 5] = center + size * vec2( 0 * direction.x,  0 * direction.y);       \n\
        uv_coords[ 6] = center + size * vec2( 1 * direction.x,  1 * direction.y);       \n\
        uv_coords[ 7] = center + size * vec2( 2 * direction.x,  2 * direction.y);       \n\
        uv_coords[ 8] = center + size * vec2( 3 * direction.x,  3 * direction.y);       \n\
        uv_coords[ 9] = center + size * vec2( 4 * direction.x,  4 * direction.y);       \n\
        uv_coords[10] = center + size * vec2( 5 * direction.x,  5 * direction.y);       \n\
    }                                                                                   \n\
";
