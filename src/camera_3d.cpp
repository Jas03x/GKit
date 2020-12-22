#include <gk/camera_3d.hpp>

#include <cassert>

#include <array>

Camera3D* Camera3D::Instance = nullptr;

Camera3D::Camera3D()
{
}

Camera3D::Camera3D(float fov, float width, float height, float zNear, float zFar) : Camera3D()
{
    m_FOV = fov * M_PI / 180.0f;
	m_NearPlane = zNear;
	m_FarPlane = zFar;
    m_AspectRatio = width / height;

    UpVector = Vector3F(0, 1, 0);
    Position = Vector3F(0, 0, 0);
    Target   = Vector3F(0, 0, 1);

	m_Projection = Matrix4F::Project(fov * M_PI / 180.0f, width, height, zNear, zFar);
	m_View = Matrix4F();
}

void Camera3D::Bind()
{
	Instance = this;
}

void Camera3D::Update()
{
	m_View = Matrix4F::View(Position, Target, UpVector);

    UpdateFrustum();
}

void Camera3D::UpdateFrustum()
{
	Matrix4F inv_vp = Matrix::Inverse(m_Projection * m_View);

	auto ClipToWorld = [inv_vp](const Vector3F& clip) -> Vector3F
	{
		Vector4F v = inv_vp * Vector4F(clip, 1.0f);
		return v.xyz() * (1.0f / v.w);
	};

	Vector3F ntl = ClipToWorld(Vector3F(-1, +1, -1)); // near-plane, top-left
	Vector3F ntr = ClipToWorld(Vector3F(+1, +1, -1)); // near-plane, top-right
	Vector3F nbl = ClipToWorld(Vector3F(-1, -1, -1)); // near-plane, bottom-left
	Vector3F nbr = ClipToWorld(Vector3F(+1, -1, -1)); // near-plane, bottom-right

	Vector3F ftl = ClipToWorld(Vector3F(-1, +1, +1)); // far-plane, top-left
	Vector3F ftr = ClipToWorld(Vector3F(+1, +1, +1)); // far-plane, top-right
	Vector3F fbl = ClipToWorld(Vector3F(-1, -1, +1)); // far-plane, bottom-left
	Vector3F fbr = ClipToWorld(Vector3F(+1, -1, +1)); // far-plane, bottom-rights

	auto DrawNormal = [this](const std::array<Vector3F, 4>& points, const Vector3F& normal) -> void
	{
		Vector3F v0 = (points[0] + points[1] + points[2] + points[3]) * 0.25f;
		Vector3F v1 = v0 + normal * 100;
		Vector3F v2 = v1 + normal * 10;

		if(DebugDrawEnabled())
		{
			DrawLine(v0, v1, Colour::BLUE);
			DrawLine(v1, v2, Colour::RED);
		}
	};

	auto CalculatePlane = [DrawNormal](const std::array<Vector3F, 4>& points) -> Frustum::Plane
	{
		Frustum::Plane p;
		p.n = Vector::Normalize(Vector::Cross(points[1] - points[0], points[2] - points[0]));
		p.d = Vector::Dot(-p.n, points[3]);

		DrawNormal(points, p.n);
		// printf("plane: n=(%f, %f, %f) d=%f\n", p.n.x, p.n.y, p.n.z, p.d);

		return p;
	};

	// NOTE: normals point *inwards* inside the frustum
	m_Frustum.planes[Frustum::FAR]    = CalculatePlane({ fbl, fbr, ftl, ftr });
	m_Frustum.planes[Frustum::NEAR]   = CalculatePlane({ ntl, ntr, nbl, nbr });
	m_Frustum.planes[Frustum::TOP]    = CalculatePlane({ ftl, ftr, ntl, ntr });
	m_Frustum.planes[Frustum::BOTTOM] = CalculatePlane({ nbl, nbr, fbl, fbr });
	m_Frustum.planes[Frustum::LEFT]   = CalculatePlane({ ntl, nbl, ftl, fbl });
	m_Frustum.planes[Frustum::RIGHT]  = CalculatePlane({ ftr, fbr, ntr, nbr });

	if(DebugDrawEnabled())
	{
		// near plane
		DrawLine(ntl, nbl, Colour::WHITE);
		DrawLine(ntr, nbr, Colour::WHITE);
		DrawLine(ntl, ntr, Colour::WHITE);
		DrawLine(nbl, nbr, Colour::WHITE);

		// far plane
		DrawLine(ftl, fbl, Colour::WHITE);
		DrawLine(ftr, fbr, Colour::WHITE);
		DrawLine(ftl, ftr, Colour::WHITE);
		DrawLine(fbl, fbr, Colour::WHITE);

		// top
		DrawLine(ntl, ftl, Colour::WHITE);
		DrawLine(ntr, ftr, Colour::WHITE);

		// bottom
		DrawLine(nbl, fbl, Colour::WHITE);
		DrawLine(nbr, fbr, Colour::WHITE);
	}

	// printf("\n");
}

Camera3D* Camera3D::GetInstance()
{
	assert(Instance != nullptr);
	
	return Instance;
}

Matrix4F Camera3D::GetMatrix() const
{
	return m_Projection * m_View;
}

const Matrix4F& Camera3D::GetProjectionMatrix() const
{
	return m_Projection;
}

const Matrix4F& Camera3D::GetViewMatrix() const
{
	return m_View;
}

float Camera3D::GetNearPlane() const
{
	return m_NearPlane;
}

float Camera3D::GetFarPlane() const
{
	return m_FarPlane;
}

ViewFrustum& Camera3D::GetFrustum()
{
	return m_Frustum;
}
