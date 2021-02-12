#ifndef GK_DEBUG_DRAW_INTERFACE_HPP
#define GK_DEBUG_DRAW_INTERFACE_HPP

#include <cstdint>

#include <gk/math/aabb.hpp>
#include <gk/math/linear.hpp>

class DebugDrawInterface
{
public:
    enum Colour : uint8_t
    {
        BLACK = 0,
        WHITE = 1,
        BLUE  = 2,
        RED   = 3,
        GREEN = 4
    };

private:
    bool m_Enabled;

protected:
    DebugDrawInterface();

    void DrawLine(const Vector3F& v0, const Vector3F& v1, Colour colour);

    void DrawAABB(const AABB& aabb, Colour colour);
    void DrawSphere(const Vector3F& origin, float radius, Colour colour);

public:
    void EnableDebugDraw(bool enabled);
    bool DebugDrawEnabled() const;
};

#endif // GK_DEBUG_DRAW_INTERFACE_HPP