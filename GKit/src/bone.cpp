#include <gk/bone.hpp>

Bone::Bone()
{
    m_Node = nullptr;
}

Bone::Bone(const std::string& name, const Node* node, const Matrix4F& matrix)
{
    m_Name   = name;
    m_Node   = node;
    m_OffsetMatrix = matrix;
}

const Node* Bone::GetNode() const
{
    return m_Node;
}

const std::string& Bone::GetName() const
{
    return m_Name;
}

const Matrix4F& Bone::GetOffsetMatrix() const
{
    return m_OffsetMatrix;
}
