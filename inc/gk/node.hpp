#ifndef GK_NODE_H
#define GK_NODE_H

#include <string>

#include <gk/bone.hpp>
#include <gk/transform_3d.hpp>

class Node
{
private:
	std::string m_Name;
	Matrix4F    m_OffsetMatrix;

	int m_BoneIndex;

	unsigned int m_NumChildren;
	unsigned int* m_Children;

public:
	Transform3D Transform;

	Node();
	Node(const std::string& name, const Matrix4F& offset);
	~Node();

	const std::string& GetName() const;

	unsigned int GetNumChildren() const;
	const unsigned int* GetChildren() const;
	void SetChildren(unsigned int count, unsigned int* children);

	void SetBoneID(int id);
	const int GetBoneID() const;

	Matrix4F GetLocalTransform() const;
	const Matrix4F& GetOffsetMatrix() const;
};

#endif // GK_NODE_H