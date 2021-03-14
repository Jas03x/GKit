#include <gk/3d/math/transform_3d.hpp>

Transform3D::Transform3D() : Transform3D(Vector3F(0.0f), Quaternion(), Vector3F(1.0f)) {}

Transform3D::Transform3D(const Vector3F& t, const Quaternion& r, const Vector3F& s)
{
    this->Translation = t;
    this->Rotation = r;
    this->Scale = s;
}

Transform3D::Transform3D(const Matrix4F& transform)
{
    this->Translation = transform[3].xyz();

    this->Scale.x = Vector::Length(transform[0].xyz());
    this->Scale.y = Vector::Length(transform[1].xyz());
    this->Scale.z = Vector::Length(transform[2].xyz());
    
    Matrix3F rotation_matrix;
    rotation_matrix[0] = transform[0].xyz() * (1.0f / this->Scale.x);
    rotation_matrix[1] = transform[1].xyz() * (1.0f / this->Scale.y);
    rotation_matrix[2] = transform[2].xyz() * (1.0f / this->Scale.z);
    this->Rotation = Quaternion(rotation_matrix);
}

Matrix4F Transform3D::ToMatrix() const
{
    Matrix4F t = Matrix4F::Translate(this->Translation);
    Matrix4F r = this->Rotation.matrix();
    Matrix4F s = Matrix4F::Scale(this->Scale);
    return t * r * s;
}
