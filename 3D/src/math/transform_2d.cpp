#include <gk/3d/math/transform_2d.hpp>

Transform2D::Transform2D() : Transform2D(Vector2F(0.0f), 0.0f, Vector2F(1.0f)) {}

Transform2D::Transform2D(const Vector2F& t, float r, const Vector2F& s)
{
    this->Position = t;
    this->Rotation = r;
    this->Size = s;
}

Matrix3F Transform2D::ToMatrix() const
{
    Matrix3F t = Matrix3F::Translate(this->Position);
    Matrix3F r = Matrix3F::Rotate(this->Rotation);
    Matrix3F s = Matrix3F::Scale(this->Size);
    return t * r * s;
}
