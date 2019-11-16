#ifndef GK_BONE_HPP
#define GK_BONE_HPP

#include <string>

#include <gk/node.hpp>
#include <gk/matrix.hpp>

class Bone
{
private:
	const Node* m_Node;

    std::string m_Name;
    Matrix4F    m_OffsetMatrix;

public:
    Bone();
    Bone(const std::string& name, const Node* node, const Matrix4F& matrix);

    const std::string& GetName() const;
    const Node*        GetNode() const;
    const Matrix4F&    GetOffsetMatrix() const;
};

#endif // GK_BONE_HPP