#include <gk/core/graphics/mesh.hpp>

Mesh::Mesh()
{
	m_VAO = nullptr;
	m_IBO = nullptr;
	m_VBO = nullptr;
	
	m_ElementCount = 0;
}

Mesh::~Mesh()
{

}

void Mesh::Destroy()
{
    if(m_VAO != nullptr) { delete m_VAO; m_VAO = nullptr; }
    if(m_IBO != nullptr) { delete m_IBO; m_IBO = nullptr; }
	if(m_VBO != nullptr) { delete m_VBO; m_VBO = nullptr; }
}

void Mesh::Bind() const
{
	m_VAO->Bind();
}

void Mesh::Unbind() const
{
	RenderingContext::GetInstance()->BindVertexArray(0);
}

unsigned int Mesh::GetElementCount() const
{
	return m_ElementCount;
}
