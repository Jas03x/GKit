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

	int m_ParentIndex;

public:
	Transform3D Transform;

	Node();
	Node(const std::string& name, int parent_index, const Matrix4F& offset);
	~Node();

	int GetParentIndex() const;
	const std::string& GetName() const;

	Matrix4F GetLocalTransform() const;

	const Matrix4F& GetOffsetMatrix() const;
	void SetOffsetMatrix(const Matrix4F& matrix);
};

#endif // GK_NODE_H