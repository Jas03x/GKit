#include <gk/camera_3d.hpp>

#include <cassert>

#include <array>

Camera3D* Camera3D::Instance = nullptr;

Camera3D::Camera3D()
{
}

Camera3D::Camera3D(float fov, float width, float height, float zNear, float zFar)
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

Frustum Camera3D::GetViewFrustum() const
{
    float half_angle = tanf(m_FOV / 2.0f);

    float h_near = 2 * half_angle * m_NearPlane; // height near plane
    float w_near = m_AspectRatio * h_near;       // width near plane

    float h_far = 2 * half_angle * m_FarPlane; // height far plane
    float w_far = m_AspectRatio * h_far;       // width far plane

    Vector3F cf = Vector::Normalize(Target - Position);     // camera forward vector
    Vector3F cu = UpVector;                                 // camera up vector
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
    Vector3F fbr = fpc + (-cu * h_far * 0.5f) + (+cr * w_far * 0.5f); // far-plane, bottom-right

    auto CalculatePlane = [](const std::array<Vector3F, 4>& points) -> Frustum::Plane
    {
        Frustum::Plane plane;
        plane.point = (points[0] + points[1] + points[2] + points[3]) * 0.25f;
        plane.normal = Vector::Cross(points[1] - points[0], points[2] - points[0]);
        return plane;
    };

    Frustum frustum = {};
    frustum.front  = CalculatePlane({ ftl, ftr, fbl, ftl });
    frustum.back   = CalculatePlane({ ntl, ntr, nbl, ntl });
    frustum.top    = CalculatePlane({ ntl, ntr, ftl, ftr });
    frustum.bottom = CalculatePlane({ nbl, nbr, fbl, fbr });
    frustum.left   = CalculatePlane({ ntl, nbl, ftl, fbl });
    frustum.right  = CalculatePlane({ ntr, nbr, ftr, fbr });

    return frustum;
}

