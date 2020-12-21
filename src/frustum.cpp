#include <gk/frustum.hpp>

bool Frustum::InView(const AABB& aabb) const
{
    bool in_view = true;
    float radius = Vector::Length(aabb.radius);

    for(unsigned int i = 0; i < COUNT; i++)
    {
        const Plane& p = planes[i];

        // sphere test
        if(Vector::Dot(aabb.origin, p.n) + p.d + radius < 0)
        {
            in_view = false;
            break;
        }
    }

    return in_view;
}
