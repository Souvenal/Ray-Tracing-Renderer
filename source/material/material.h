#pragma once

#include "ray.h"
#include "hitable.h"

class material {
    public:
        virtual ~material() {}
        virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
        virtual vec3 emitted(float u, float v, const vec3& p) const {
            return vec3(0, 0, 0); // Default implementation returns no emission
        }
};