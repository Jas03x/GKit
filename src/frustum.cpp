#include <gk/frustum.hpp>

bool Frustum::InView(const AABB& aabb) const
{
    bool in_view = false;

    for(unsigned int i = 0; i < COUNT; i++)
    {
        const Plane& p = planes[i];
        float radius = Vector::Length(aabb.length);

        // sphere test
        if(Vector::Dot(aabb.origin, p.n) - p.d <= radius)
        {
            in_view = true;
            break;
        }
    }

    return in_view;
}

