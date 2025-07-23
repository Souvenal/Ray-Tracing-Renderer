#pragma once

#include "ray.h"
#include "hitable.h"
#include "onb.h"

class material  {
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& alb, ray& scattered, float& pdf) const {
        return false;
    }
    virtual float scattering_pdf(const ray& r_in, const hit_record& rec, const ray& scattered) const {
        return 0.0;
    }
    virtual vec3 emitted(float u, float v, const vec3& p) const { return vec3(0,0,0); }
};