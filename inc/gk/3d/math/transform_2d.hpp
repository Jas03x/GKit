#ifndef GK_TRANSFORM_2D_H
#define GK_TRANSFORM_2D_H

#include <gk/3d/math/linear.hpp>

class Transform2D
{
public:
	Vector2F  Size;
	Vector2F  Position;
	float	  Rotation;

	Transform2D();
	Transform2D(const Vector2F& t, float r, const Vector2F& s);

	Matrix3F ToMatrix() const;
};

#endif // GK_TRANSFORM_2D_H