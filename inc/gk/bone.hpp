#ifndef GK_BONE_HPP
#define GK_BONE_HPP

#include <string>

#include <gk/matrix.hpp>

class Bone
{
private:
    std::string m_Name;
    Matrix4F    m_BindPoseMatrix;

    unsigned int m_NodeIndex;

public:
    Bone();
    Bone(const std::string& name, unsigned int node_index, const Matrix4F& bind_pose);

    unsigned int GetNodeID() const;

    const std::string& GetName() const;
    const Matrix4F& GetBindPoseMatrix() const;
};

#endif // GK_BONE_HPP