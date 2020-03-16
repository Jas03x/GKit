#include <gk/node.hpp>

Node::Node() : Node("", -1, Matrix4F(1.0f))
{
}

Node::Node(const std::string& name, int parent_index, const Matrix4F& offset)
{
	m_Name = name;
	m_OffsetMatrix = offset;
	m_ParentIndex = parent_index;

	this->Transform = Transform3D();
}

Node::~Node()
{
}

const std::string& Node::GetName() const
{
	return m_Name;
}

int Node::GetParentIndex() const
{
	return m_ParentIndex;
}

Matrix4F Node::GetLocalTransform() const
{
	return this->Transform.ToMatrix() * m_OffsetMatrix;
}

const Matrix4F& Node::GetOffsetMatrix() const
{
	return this->m_OffsetMatrix;
}

void Node::SetOffsetMatrix(const Matrix4F& matrix)
{
	m_OffsetMatrix = matrix;
}
