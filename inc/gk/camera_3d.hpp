#ifndef GK_CAMERA_3D_H
#define GK_CAMERA_3D_H

#include <gk/frustum.hpp>
#include <gk/matrix.hpp>

class Camera3D
{
public:
	enum FOV : int {
		THIRD_PERSON = 60,
		FIRST_PERSON = 90
	};

private:
	static Camera3D* Instance;

    float m_FOV;
	float m_NearPlane;
	float m_FarPlane;
    float m_AspectRatio;

    Matrix4F m_View;
	Matrix4F m_Projection;

	Frustum m_Frustum;

public:
    Vector3F UpVector;
    Vector3F Position;
    Vector3F Target;

public:
	Camera3D();
	Camera3D(float fov, float width, float height, float zNear, float zFar);

	static Camera3D* GetInstance();

	void Bind();
    void Update();
	
	Matrix4F GetMatrix() const;
	const Matrix4F& GetViewMatrix() const;
	const Matrix4F& GetProjectionMatrix() const;

	float GetNearPlane() const;
	float GetFarPlane() const;
    const Frustum& GetViewFrustum() const;
};

#endif // GK_CAMERA_3D_H
