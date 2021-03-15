#include <gk/3d/node.hpp>

Node::Node() : Node("", Matrix4F(1.0f))
{
}

Node::Node(const std::string& name, const Matrix4F& offset)
{
	m_Name = name;
	m_OffsetMatrix = offset;
	m_ParentIndex = -1;

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

void Node::SetParentIndex(int index)
{
	m_ParentIndex = index;
}

Matrix4F Node::GetLocalTransform() const
{
	return this->Transform.ToMatrix();
}

const Matrix4F& Node::GetOffsetMatrix() const
{
	return this->m_OffsetMatrix;
}

void Node::SetOffsetMatrix(const Matrix4F& matrix)
{
	m_OffsetMatrix = matrix;
}
