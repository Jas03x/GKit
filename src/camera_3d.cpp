#include <gk/camera_3d.hpp>

#include <assert.h>

Camera3D* Camera3D::Instance = nullptr;

Camera3D::Camera3D()
{
}

Camera3D::Camera3D(float fov, float width, float height, float zNear, float zFar)
{
	Projection = Matrix4F::Project(fov * M_PI / 180.0f, width, height, zNear, zFar);
	View = Matrix4F();
}

void Camera3D::Bind()
{
	Instance = this;
}

Camera3D* Camera3D::GetInstance()
{
	assert(Instance != nullptr);
	
	return Instance;
}

Matrix4F Camera3D::GetMatrix() const
{
	return Projection * View;
}

const Matrix4F& Camera3D::GetProjectionMatrix() const
{
	return Projection;
}

const Matrix4F& Camera3D::GetViewMatrix() const
{
	return View;
}

