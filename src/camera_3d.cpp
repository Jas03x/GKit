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
	float half_angle = tanf(m_FOV / 2.0f);

	float h_near = 2 * half_angle * m_NearPlane; // height near plane
	float w_near = m_AspectRatio * h_near;       // width near plane

	float h_far = 2 * half_angle * m_FarPlane; // height far plane
	float w_far = m_AspectRatio * h_far;       // width far plane

	Vector3F cf = Vector::Normalize(Target - Position);     // camera forward vector
	Vector3F cu = Vector::Normalize(UpVector);              // camera up vector
	Vector3F cr = Vector::Normalize(Vector::Cross(cf, cu)); // camera right vector

	Vector3F npc = Position + cf * m_NearPlane;                         // near-plane, center
	Vector3F ntl = npc + (+cu * h_near * 0.5f) + (-cr * w_near * 0.5f); // near-plane, top-left
	Vector3F ntr = npc + (+cu * h_near * 0.5f) + (+cr * w_near * 0.5f); // near-plane, top-right
	Vector3F nbl = npc + (-cu * h_near * 0.5f) + (-cr * w_near * 0.5f); // near-plane, bottom-left
	Vector3F nbr = npc + (-cu * h_near * 0.5f) + (+cr * w_near * 0.5f); // near-plane, bottom-right

	Vector3F fpc = Position + cf * m_FarPlane;                        // far-plane, center
	Vector3F ftl = fpc + (+cu * h_far * 0.5f) + (-cr * w_far * 0.5f); // far-plane, top-left
	Vector3F ftr = fpc + (+cu * h_far * 0.5f) + (+cr * w_far * 0.5f); // far-plane, top-right
	Vector3F fbl = fpc + (-cu * h_far * 0.5f) + (-cr * w_far * 0.5f); // far-plane, bottom-left
	Vector3F fbr = fpc + (-cu * h_far * 0.5f) + (+cr * w_far * 0.5f); // far-plane, bottom-rights

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
		p.n = Vector::Cross(points[1] - points[0], points[2] - points[0]);
		p.d = Vector::Dot(p.n, points[3]);

		float f = 1.0f / Vector::Length(p.n);
		p.n *= f;
		p.d *= f;

		DrawNormal(points, p.n);
		// printf("plane: n=(%f, %f, %f) d=%f\n", p.n.x, p.n.y, p.n.z, p.d);

		return p;
	};

#if 1
	m_Frustum.planes[Frustum::FAR]    = CalculatePlane({ ftl, ftr, fbl, fbr });
	m_Frustum.planes[Frustum::NEAR]   = CalculatePlane({ nbl, ntr, ntl, nbr });
	m_Frustum.planes[Frustum::TOP]    = CalculatePlane({ ntl, ntr, ftl, ftr });
	m_Frustum.planes[Frustum::BOTTOM] = CalculatePlane({ fbl, nbr, nbl, fbr });
	m_Frustum.planes[Frustum::LEFT]   = CalculatePlane({ ftl, nbl, ntl, fbl });
	m_Frustum.planes[Frustum::RIGHT]  = CalculatePlane({ ntr, nbr, ftr, fbr });
#else
	// NOTE: normals point *inwards* inside the frustum
	m_Frustum.planes[Frustum::FAR]    = CalculatePlane({ fbl, ftr, ftl, fbr });
	m_Frustum.planes[Frustum::NEAR]   = CalculatePlane({ ntl, ntr, nbl, nbr });
	m_Frustum.planes[Frustum::TOP]    = CalculatePlane({ ftl, ntr, ntl, ftr });
	m_Frustum.planes[Frustum::BOTTOM] = CalculatePlane({ nbl, nbr, fbl, fbr });
	m_Frustum.planes[Frustum::LEFT]   = CalculatePlane({ ntl, nbl, ftl, fbl });
	m_Frustum.planes[Frustum::RIGHT]  = CalculatePlane({ ftr, nbr, ntr, fbr });
#endif

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
