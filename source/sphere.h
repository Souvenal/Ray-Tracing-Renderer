#pragma once

#include "hitable.h"

class sphere: public hitable {
public:
    sphere() {}
    sphere(vec3 cen, float r, material* mat) : center(cen), radius(r), mat_ptr(mat) {};
    bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const override;
    bool bounding_box(float t0, float t1, aabb& box) const override;

private:
    vec3 center;
    float radius;
    material *mat_ptr;
};