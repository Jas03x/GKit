#include <gk/node.hpp>

Node::Node() : Node("", Matrix4F(1.0f))
{
}

Node::Node(const std::string& name, const Matrix4F& offset)
{
	m_Name = name;
	m_OffsetMatrix = offset;

	m_BoneIndex = -1;

	m_NumChildren = 0;
	m_Children = nullptr;

	this->Transform = Transform3D();
}

Node::~Node()
{
	if (m_Children != nullptr)
	{
		delete[] m_Children;
	}
}

const std::string& Node::GetName() const
{
	return m_Name;
}

void Node::SetChildren(unsigned int count, unsigned int* children)
{
	m_NumChildren = count;
	m_Children = new unsigned int[count];

	for (unsigned int i = 0; i < count; i++)
	{
		m_Children[i] = children[i];
	}
}

unsigned int Node::GetNumChildren() const
{
	return m_NumChildren;
}

const unsigned int* Node::GetChildren() const
{
	return m_Children;
}

void Node::SetBoneID(int id)
{
	m_BoneIndex = id;
}

const int Node::GetBoneID() const
{
	return m_BoneIndex;
}

Matrix4F Node::GetLocalTransform() const
{
	return this->Transform.ToMatrix();
}

const Matrix4F& Node::GetOffsetMatrix() const
{
	return this->m_OffsetMatrix;
}