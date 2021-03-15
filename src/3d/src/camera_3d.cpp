#include <gk/3d/camera_3d.hpp>

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
}

void Camera3D::GetFrustum(Frustum& frustum)
{
#if 0
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
#else
	float half_angle = tanf(m_FOV / 2.0f);

	// dimensions of near and far planes
	Vector2F d_far = Vector2F(m_AspectRatio * 2 * half_angle * m_FarPlane, 2 * half_angle * m_FarPlane) * 0.5f;
	Vector2F d_near = Vector2F(m_AspectRatio * 2 * half_angle * m_NearPlane, 2 * half_angle * m_NearPlane) * 0.5f;

	Vector3F cf = Vector::Normalize(Target - Position);           // camera forward vector
	Vector3F cr = Vector::Normalize(Vector::Cross(cf, UpVector)); // camera right vector
	Vector3F cu = Vector::Cross(cr, cf);                          // camera up vector

	Vector3F npc = Position + cf * m_NearPlane;               // near-plane, center
	Vector3F ntl = npc + (+cu * d_near.y) + (-cr * d_near.x); // near-plane, top-left
	Vector3F ntr = npc + (+cu * d_near.y) + (+cr * d_near.x); // near-plane, top-right
	Vector3F nbl = npc + (-cu * d_near.y) + (-cr * d_near.x); // near-plane, bottom-left
	Vector3F nbr = npc + (-cu * d_near.y) + (+cr * d_near.x); // near-plane, bottom-right

	Vector3F fpc = Position + cf * m_FarPlane;              // far-plane, center
	Vector3F ftl = fpc + (+cu * d_far.y) + (-cr * d_far.x); // far-plane, top-left
	Vector3F ftr = fpc + (+cu * d_far.y) + (+cr * d_far.x); // far-plane, top-right
	Vector3F fbl = fpc + (-cu * d_far.y) + (-cr * d_far.x); // far-plane, bottom-left
	Vector3F fbr = fpc + (-cu * d_far.y) + (+cr * d_far.x); // far-plane, bottom-rights
#endif

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
	frustum.planes[Frustum::FAR]    = CalculatePlane({ fbl, fbr, ftl, ftr });
	frustum.planes[Frustum::NEAR]   = CalculatePlane({ ntl, ntr, nbl, nbr });
	frustum.planes[Frustum::TOP]    = CalculatePlane({ ftl, ftr, ntl, ntr });
	frustum.planes[Frustum::BOTTOM] = CalculatePlane({ nbl, nbr, fbl, fbr });
	frustum.planes[Frustum::LEFT]   = CalculatePlane({ ntl, nbl, ftl, fbl });
	frustum.planes[Frustum::RIGHT]  = CalculatePlane({ ftr, fbr, ntr, nbr });

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
