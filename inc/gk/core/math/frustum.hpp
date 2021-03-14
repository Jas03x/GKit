#ifndef GK_FRUSTUM_HPP
#define GK_FRUSTUM_HPP

#include <gk/core/math/aabb.hpp>
#include <gk/core/math/linear.hpp>

struct Frustum
{
    enum
    {
        NEAR   = 0,
        FAR    = 1,
        TOP    = 2,
        BOTTOM = 3,
        LEFT   = 4,
        RIGHT  = 5,
        COUNT  = 6
    };

    struct Plane
    {
        Vector3F n;
        float    d;
    };

    Plane planes[COUNT];

    bool InView(const AABB& aabb) const;
};

#endif // GK_FRUSTUM_HPP
