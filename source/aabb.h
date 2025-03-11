#pragma once

#include "ray.h"

inline float ffmin(float a, float b) { return a < b? a : b; }
inline float ffmax(float a, float b) { return a > b? a : b; }

class aabb {
    public:
        aabb() {}
        aabb(const vec3& a, const vec3& b) { _min = a; _max = b;}

        vec3 min() const {return _min; }
        vec3 max() const {return _max; }

        bool hit(const ray& r, float tmin, float tmax) const;

        vec3 _min;
        vec3 _max;
};

aabb surrounding_box(aabb box0, aabb box1);