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
