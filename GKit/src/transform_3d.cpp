#include <gk/transform_3d.hpp>

Transform3D::Transform3D() : Transform3D(Vector3F(0.0f), Quaternion(), Vector3F(0.0f)) {}

Transform3D::Transform3D(const Vector3F& t, const Quaternion& r, const Vector3F& s)
{
    this->Translation = t;
    this->Rotation = r;
    this->Scale = s;
}

Matrix4F Transform3D::ToMatrix() const
{
    Matrix4F t = Matrix4F::Translate(this->Translation);
    Matrix4F r = this->Rotation.matrix();
    Matrix4F s = Matrix4F::Scale(this->Scale);
    return t * r * s;
}
