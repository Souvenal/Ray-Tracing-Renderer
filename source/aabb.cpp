#include "aabb.h"

bool aabb::hit(const ray& r, float tmin, float tmax) const {
    for (int a = 0; a < 3; a++) {
        float invD = 1.0f / r.direction()[a];
        float t0 = (_max[a] - r.origin()[a]) * invD;
        float t1 = (_min[a] - r.origin()[a]) * invD;
        if (invD < 0.0f) {
            std::swap(t0, t1);
        }
        tmin = (t0 > tmin)? t0 : tmin;
        tmax = (t1 < tmax)? t1 : tmax;
        if (tmax <= tmin)
            return false;
    }
    return true;
}

aabb surrounding_box(aabb box0, aabb box1){
    vec3 small(
        fmin(box0.min().x(), box1.min().x()),
        fmin(box0.min().y(), box1.min().y()),
        fmin(box0.min().z(), box1.min().z())
    );
    vec3 big(
        fmax(box0.max().x(), box1.max().x()),
        fmax(box0.max().y(), box1.max().y()),
        fmax(box0.max().z(), box1.max().z())
    );
    return aabb(small, big);
}