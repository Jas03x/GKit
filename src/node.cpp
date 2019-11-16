#include <gk/node.hpp>

Node::Node()
{
	m_Name   = "";
	m_Parent = nullptr;

	m_OffsetMatrix  = Matrix4F(1.0f);
	this->Transform = Transform3D();
}

Node::Node(const std::string& name) : Node(name, Matrix4F(1.0f))
{
}

Node::Node(const std::string& name, const Matrix4F& offset)
{
	m_Name   = name;
	m_Parent = nullptr;

	m_OffsetMatrix  = offset;
	this->Transform = Transform3D();
}

void Node::SetParent(Node* node)
{
	m_Parent = node;
}

const std::string& Node::GetName() const
{
	return m_Name;
}

const Node* Node::GetParent() const
{
	return m_Parent;
}

Matrix4F Node::GetLocalMatrix() const
{
	return this->Transform.ToMatrix();
}

Matrix4F Node::GetGlobalMatrix() const
{
	Matrix4F ret = Matrix4F(1.0f);
	const Node* node = this;

	while (node != nullptr)
	{
		//printf("%s *", node->m_Name.c_str());
		//printf("Parent is %s\n", m_Parent == nullptr ? "None" : m_Parent->m_Name.c_str());
		ret = node->GetOffsetMatrix() * node->GetLocalMatrix() * ret;
		//ret *= node->GetLocalMatrix();
		node = node->m_Parent;
	}
	//printf("\n");

	return ret;
}

const Matrix4F& Node::GetOffsetMatrix() const
{
	return this->m_OffsetMatrix;
}
