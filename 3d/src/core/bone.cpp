#include <gk/3d/core/bone.hpp>

Bone::Bone()
{
}

Bone::Bone(const std::string& name, unsigned int node_index, const Matrix4F& bind_pose)
{
    m_Name = name;
    m_BindPoseMatrix = bind_pose;
    m_NodeIndex = node_index;
}

const std::string& Bone::GetName() const
{
    return m_Name;
}

const Matrix4F& Bone::GetBindPoseMatrix() const
{
    return m_BindPoseMatrix;
}

unsigned int Bone::GetNodeID() const
{
    return m_NodeIndex;
}
