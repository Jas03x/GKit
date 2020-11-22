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
    Vector3F ftl = aabb.origin + Vector3F(+aabb.radius.x, +aabb.radius.y, +aabb.radius.z);
    Vector3F ftr = aabb.origin + Vector3F(-aabb.radius.x, +aabb.radius.y, +aabb.radius.z);
    Vector3F fbl = aabb.origin + Vector3F(+aabb.radius.x, -aabb.radius.y, +aabb.radius.z);
    Vector3F fbr = aabb.origin + Vector3F(-aabb.radius.x, -aabb.radius.y, +aabb.radius.z);

    Vector3F btl = aabb.origin + Vector3F(+aabb.radius.x, +aabb.radius.y, -aabb.radius.z);
    Vector3F btr = aabb.origin + Vector3F(-aabb.radius.x, +aabb.radius.y, -aabb.radius.z);
    Vector3F bbl = aabb.origin + Vector3F(+aabb.radius.x, -aabb.radius.y, -aabb.radius.z);
    Vector3F bbr = aabb.origin + Vector3F(-aabb.radius.x, -aabb.radius.y, -aabb.radius.z);

    if(m_Enabled && DebugDrawer::Enabled())
    {
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
