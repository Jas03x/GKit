#ifndef GK_FRUSTUM_HPP
#define GK_FRUSTUM_HPP

#include <gk/aabb_collider.hpp>
#include <gk/matrix.hpp>

struct Frustum
{
    enum
    {
        NEAR_P = 0,
        FAR_P  = 1,
        TOP    = 2,
        BOTTOM = 3,
        LEFT   = 4,
        RIGHT  = 5,
        COUNT  = 6
    };

    struct Plane
    {
        Vector3F point;
        Vector3F normal;
    };

    Plane planes[COUNT];

    bool InView(const AABB& aabb) const;
};

#endif // GK_FRUSTUM_HPP
