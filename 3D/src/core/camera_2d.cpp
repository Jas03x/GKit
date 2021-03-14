#include <gk/3d/core/camera_2d.hpp>

#include <cassert>
#include <cstdio>

Camera2D* Camera2D::Instance = NULL;

Camera2D::Camera2D()
{
}

Camera2D::Camera2D(unsigned int width, unsigned int height)
{
	Projection[0] = Vector3F( 2.0f / static_cast<float>(width), 0.0f,   0.0f);
	Projection[1] = Vector3F( 0.0f,  2.0f / static_cast<float>(height), 0.0f);
	Projection[2] = Vector3F(-1.0f, -1.0f, 1.0f);

	View = Matrix3F(1.0f);
}

Camera2D::~Camera2D()
{
}

void Camera2D::Bind()
{
	Instance = this;
}

Camera2D* Camera2D::GetInstance()
{
	assert(Instance != nullptr);
	
	return Instance;
}

Matrix3F Camera2D::GetMatrix() const {
	return Projection * View;
}

const Matrix3F& Camera2D::GetViewMatrix() const {
	return View;
}

const Matrix3F& Camera2D::GetProjectionMatrix() const {
	return Projection;
}
