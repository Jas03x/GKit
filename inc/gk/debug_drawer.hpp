#ifndef GK_DEBUG_DRAWER_HPP
#define GK_DEBUG_DRAWER_HPP

#include <vector>

#include <gk/vertex_array.hpp>
#include <gk/vertex_buffer.hpp>
#include <gk/shader.hpp>

class DebugDrawer : public Shader
{
public:
    enum Color
    {
        BLACK = 0,
        WHITE = 1,
        BLUE  = 2,
        RED   = 3,
        GREEN = 4
    };

private:
    typedef struct Vertex
	{
		float vertex[3];
        unsigned char colour;
	} Vertex;

    typedef enum
	{
		VERTEX = 0,
        COLOUR = 1
	} VertexAttributes;

private:
	static DebugDrawer* Instance;

private:
    bool m_Enabled;
    unsigned int m_LineLimit;

	unsigned int m_Matrix;

    VertexArray  *m_VAO;
	VertexBuffer *m_VBO;

    std::vector<Vertex> m_Lines;

private:
	DebugDrawer(unsigned int line_limit);
	~DebugDrawer();
    
public:
	static void CreateInstance(unsigned int line_limit);
	static void DeleteInstance();

    static void Enable();
    static void Disable();

    static bool Enabled();
    
	static void Render();
};

#endif // GK_DEBUG_DRAWER_HPP