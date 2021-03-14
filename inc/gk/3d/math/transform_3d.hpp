#ifndef GK_TRANSFORM_3D_H
#define GK_TRANSFORM_3D_H

#include <gk/3d/math/linear.hpp>

class Transform3D
{
public:
	Vector3F   Translation;
	Quaternion Rotation;
	Vector3F   Scale;

	Transform3D();
	Transform3D(const Matrix4F& transform);
	Transform3D(const Vector3F& t, const Quaternion& r, const Vector3F& s);

	Matrix4F ToMatrix() const;
};

#endif // GK_TRANSFORM_3D_H