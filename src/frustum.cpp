#include <gk/frustum.hpp>

bool Frustum::InView(const AABB& aabb) const
{
    bool in_view = true;

#if 1
    for(unsigned int i = 0; i < COUNT; i++)
    {
        const Plane& p = planes[i];
        float radius = Vector::Length(aabb.radius);

        // sphere test
        if(Vector::Dot(aabb.origin, p.n) + p.d <= -radius)
        {
            in_view = false;
            break;
        }
    }
#else
    const Plane& p = planes[NEAR];
    float radius = Vector::Length(aabb.radius);

    // sphere test
    if(Vector::Dot(aabb.origin, p.n) + p.d < -radius)
    {
        in_view = false;
    }
#endif

    return in_view;
}

