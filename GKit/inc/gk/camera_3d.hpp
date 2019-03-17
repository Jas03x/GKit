#ifndef GK_CAMERA_3D_H
#define GK_CAMERA_3D_H

#include <gk/matrix.hpp>

class Camera3D
{
private:
	static Camera3D* Instance;

	Matrix4F Projection;

public:
	Matrix4F View;
	
public:
	Camera3D();
	Camera3D(float fov, float width, float height, float zNear, float zFar);

	static Camera3D* GetInstance();

	void Bind();
	
	Matrix4F GetMatrix() const;
	const Matrix4F& GetViewMatrix() const;
	const Matrix4F& GetProjectionMatrix() const;
};

#endif // GK_CAMERA_3D_H