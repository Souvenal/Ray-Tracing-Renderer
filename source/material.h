#pragma once

#include "ray.h"
#include "hitable.h"

class material {
    public:
        virtual ~material() {}
        virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};