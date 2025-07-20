#pragma once

#include "material.h"
#include "texture.h"

class lambertian : public material {
    public:
        lambertian(texture* a) : albedo(a) {}
        virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const;
        
        std::unique_ptr<texture> albedo;
};