#ifndef GK_FRUSTUM_HPP
#define GK_FRUSTUM_HPP

#include <gk/matrix.hpp>

struct Frustum
{
    struct Plane
    {
        Vector3F point;
        Vector3F normal;
    };

    Plane front;
    Plane back;
    Plane top;
    Plane bottom;
    Plane right;
    Plane left;
};

#endif // GK_FRUSTUM_HPP
