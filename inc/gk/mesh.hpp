#ifndef GK_MESH_H
#define GK_MESH_H

#include <stddef.h>

#include <gk/graphics.hpp>
#include <gk/vertex_array.hpp>
#include <gk/vertex_buffer.hpp>

class Mesh
{
protected:
	VertexArray  *m_VAO;
	VertexBuffer *m_IBO;
	VertexBuffer *m_VBO;
	
	unsigned int m_ElementCount;

protected:
    void Destroy();

public:
	Mesh();
	~Mesh();

	void Bind() const;
	void Unbind() const;

	unsigned int GetElementCount() const;
};

#endif // GK_MESH_H