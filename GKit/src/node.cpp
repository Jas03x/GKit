#include <gk/node.hpp>

Node::Node(const std::string& name, const Vector3F& t, const Quaternion& r, const Vector3F& s)
{
	m_Name = name;
	m_Parent = nullptr;
	this->Transform = Transform3D(t, r, s);
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
	Matrix4F ret;
	const Node* node = this;

	while (node != nullptr)
	{
		//printf("%s *", node->m_Name.c_str());
		//printf("Parent is %s\n", m_Parent == nullptr ? "None" : m_Parent->m_Name.c_str());
		ret = node->GetLocalMatrix() * ret;
		//ret *= node->GetLocalMatrix();
		node = node->m_Parent;
	}
	//printf("\n");

	return ret;
}
