#pragma once

#include "material.h"
#include "texture.h"

class metal : public material {
public:
    metal(texture* a, float f = 0) : albedo(a), fuzz(f < 1 ? f : 1.0) {}

    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const;
    
private:
    std::unique_ptr<texture> albedo;
    float fuzz;
};