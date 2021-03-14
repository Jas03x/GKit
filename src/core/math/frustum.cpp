#include <gk/core/math/frustum.hpp>

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

    /*
    float radius = Vector::Length(aabb.radius);

    float d_n = Vector::Dot(aabb.origin, planes[NEAR].n) + planes[NEAR].d;
    float d_f = Vector::Dot(aabb.origin, planes[FAR].n) + planes[FAR].d;
    float d_t = Vector::Dot(aabb.origin, planes[TOP].n) + planes[TOP].d;
    float d_b = Vector::Dot(aabb.origin, planes[BOTTOM].n) + planes[BOTTOM].d;
    float d_l = Vector::Dot(aabb.origin, planes[LEFT].n) + planes[LEFT].d;
    float d_r = Vector::Dot(aabb.origin, planes[RIGHT].n) + planes[RIGHT].d;
    
    bool x = (d_l > 0 && d_r > 0) || fabs(d_l) <= radius || fabs(d_r) <= radius;
    bool y = (d_t > 0 && d_b > 0) || fabs(d_t) <= radius || fabs(d_b) <= radius;
    bool z = (d_f > 0 && d_n > 0) || fabs(d_f) <= radius || fabs(d_n) <= radius;

    return x && y && z;
    */
}
