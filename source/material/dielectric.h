#pragma once

#include "material.h"

class dielectric : public material {
public:
    dielectric(float ri) : ref_idx(ri) {}
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& alb, ray& scattered, float& pdf) const;
    
private:
    float ref_idx;
}; 