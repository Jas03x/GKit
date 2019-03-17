#ifndef GK_NODE_H
#define GK_NODE_H

#include <string>

#include <gk/transform_3d.hpp>

class Node
{
private:
	std::string m_Name;
	Node* m_Parent;

public:
	Transform3D Transform;

	Node(const std::string& name, const Vector3F& t, const Quaternion& r, const Vector3F& s);

	void SetParent(Node* node);

	const std::string& GetName() const;
	const Node* GetParent() const;

	Matrix4F GetLocalMatrix() const;
	Matrix4F GetGlobalMatrix() const;
};

#endif // GK_NODE_H