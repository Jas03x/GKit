#ifndef GK_DEBUG_DRAWER_HPP
#define GK_DEBUG_DRAWER_HPP

#include <vector>

#include <gk/3d/math/linear.hpp>
#include <gk/3d/graphics/shader.hpp>
#include <gk/3d/graphics/vertex_array.hpp>
#include <gk/3d/graphics/vertex_buffer.hpp>

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
    unsigned int m_VertexLimit;

	unsigned int m_Matrix;

    VertexArray  *m_VAO;
	VertexBuffer *m_VBO;

    std::vector<Vertex> m_LineBuffer;

private:
	DebugDrawer(unsigned int line_limit);
	~DebugDrawer();
    
public:
	static DebugDrawer* CreateInstance(unsigned int vertex_limit);
	static void DeleteInstance();
    
    static DebugDrawer* GetInstance();
    
public:
    void Enable();
    void Disable();
    bool Enabled();
    
    void Clear();
    void DrawLine(const Vertex& v0, const Vertex& v1);

	void Render();
};

#endif // GK_DEBUG_DRAWER_HPP
