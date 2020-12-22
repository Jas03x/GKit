#include <gk/debug_draw_interface.hpp>

#include <gk/debug_drawer.hpp>

DebugDrawInterface::DebugDrawInterface()
{
    m_Enabled = false;
}

void DebugDrawInterface::EnableDebugDraw(bool enabled)
{
    m_Enabled = enabled;
}

bool DebugDrawInterface::DebugDrawEnabled() const
{
    return m_Enabled;
}

void DebugDrawInterface::DrawLine(const Vector3F& v0, const Vector3F& v1, Colour colour)
{
    if(m_Enabled && DebugDrawer::Enabled())
    {
        DebugDrawer::DrawLine({ v0, colour }, { v1, colour });
    }
}

void DebugDrawInterface::DrawAABB(const AABB& aabb, Colour colour)
{
    if (m_Enabled && DebugDrawer::Enabled())
    {
        Vector3F ftl = aabb.origin + Vector3F(+aabb.radius.x, +aabb.radius.y, +aabb.radius.z);
        Vector3F ftr = aabb.origin + Vector3F(-aabb.radius.x, +aabb.radius.y, +aabb.radius.z);
        Vector3F fbl = aabb.origin + Vector3F(+aabb.radius.x, -aabb.radius.y, +aabb.radius.z);
        Vector3F fbr = aabb.origin + Vector3F(-aabb.radius.x, -aabb.radius.y, +aabb.radius.z);

        Vector3F btl = aabb.origin + Vector3F(+aabb.radius.x, +aabb.radius.y, -aabb.radius.z);
        Vector3F btr = aabb.origin + Vector3F(-aabb.radius.x, +aabb.radius.y, -aabb.radius.z);
        Vector3F bbl = aabb.origin + Vector3F(+aabb.radius.x, -aabb.radius.y, -aabb.radius.z);
        Vector3F bbr = aabb.origin + Vector3F(-aabb.radius.x, -aabb.radius.y, -aabb.radius.z);
        
        // top face
        DebugDrawer::DrawLine({ ftl, colour }, { ftr, colour });
        DebugDrawer::DrawLine({ btl, colour }, { btr, colour });
        DebugDrawer::DrawLine({ ftl, colour }, { btl, colour });
        DebugDrawer::DrawLine({ ftr, colour }, { btr, colour });

        // bottom face
        DebugDrawer::DrawLine({ fbl, colour }, { fbr, colour });
        DebugDrawer::DrawLine({ bbl, colour }, { bbr, colour });
        DebugDrawer::DrawLine({ fbl, colour }, { bbl, colour });
        DebugDrawer::DrawLine({ fbr, colour }, { bbr, colour });

        // front face
        DebugDrawer::DrawLine({ ftl, colour }, { fbl, colour });
        DebugDrawer::DrawLine({ ftr, colour }, { fbr, colour });

        // back face
        DebugDrawer::DrawLine({ btl, colour }, { bbl, colour });
        DebugDrawer::DrawLine({ btr, colour }, { bbr, colour });
    }
}

void DebugDrawInterface::DrawSphere(const Vector3F& origin, float radius, Colour colour)
{
    if (m_Enabled && DebugDrawer::Enabled())
    {
        const float step = M_PI / 8.0f;
        for (float f = 0.0f; f < M_PI * 2.0f; f += step)
        {
            float c_0 = cosf(f);
            float s_0 = sinf(f);
            Matrix3F rot_x_0 = {
                { 1,    0,    0 },
                { 0,  c_0, -s_0 },
                { 0,  s_0,  c_0 }
            };
            Matrix3F rot_z_0 = {
                { c_0, -s_0, 0 },
                { s_0,  c_0, 0 },
                {    0,   0, 1}
            };

            float c_1 = cosf(f + step);
            float s_1 = sinf(f + step);
            Matrix3F rot_x_1 = {
                { 1,    0,    0 },
                { 0,  c_1, -s_1 },
                { 0,  s_1,  c_1 }
            };
            Matrix3F rot_z_1 = {
                { c_1, -s_1, 0 },
                { s_1,  c_1, 0 },
                {    0,   0, 1}
            };

            DebugDrawer::DrawLine(
                { origin + rot_x_0 * Vector3F(0, 0, radius), colour },
                { origin + rot_x_1 * Vector3F(0, 0, radius), colour }
            );

            DebugDrawer::DrawLine(
                { origin + rot_z_0 * Vector3F(radius, 0, 0), colour },
                { origin + rot_z_1 * Vector3F(radius, 0, 0), colour }
            );
        }
    }
}
