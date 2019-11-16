#ifndef GK_CAMERA_2D_H
#define GK_CAMERA_2D_H

#include <gk/matrix.hpp>

class Camera2D
{
private:
	static Camera2D* Instance;

	Matrix3F Projection;

public:
	Matrix3F View;

public:
	Camera2D();
	Camera2D(unsigned int width, unsigned int height);
	~Camera2D();

	void Bind();
	static Camera2D* GetInstance();

	Matrix3F GetMatrix() const;
	const Matrix3F& GetViewMatrix() const;
	const Matrix3F& GetProjectionMatrix() const;
};

#endif // GK_CAMERA_2D_H