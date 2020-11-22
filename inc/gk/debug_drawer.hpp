#ifndef GK_DEBUG_DRAWER_HPP
#define GK_DEBUG_DRAWER_HPP

#include <vector>

#include <gk/matrix.hpp>
#include <gk/shader.hpp>
#include <gk/vertex_array.hpp>
#include <gk/vertex_buffer.hpp>

class DebugDrawer : public Shader
{
public:
    typedef struct Vertex
	{
		Vector3F vertex;
        uint8_t  colour;
	} Vertex;

private:
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
    
    static void Clear();
    static void DrawLine(const Vertex& v0, const Vertex& v1);

	static void Render();
};

#endif // GK_DEBUG_DRAWER_HPP