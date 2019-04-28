#ifndef GK_NODE_H
#define GK_NODE_H

#include <string>

#include <gk/transform_3d.hpp>

class Node
{
private:
	std::string m_Name;
	Matrix4F    m_OffsetMatrix;
	
	Node* m_Parent;

public:
	Transform3D Transform;

	Node();
	Node(const std::string& name);
	Node(const std::string& name, const Matrix4F& offset);

	void SetParent(Node* node);

	const std::string& GetName() const;
	const Node* GetParent() const;

	Matrix4F GetLocalMatrix() const;
	Matrix4F GetGlobalMatrix() const;
	
	const Matrix4F& GetOffsetMatrix() const;
};

#endif // GK_NODE_H