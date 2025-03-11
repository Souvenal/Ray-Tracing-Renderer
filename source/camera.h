#pragma once

#include "ray.h"

vec3 random_in_unit_disk();

class camera {
    public:
        // new:  add t0 and t1
        camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist,
               float t0, float t1);
    
        // new: add time to construct ray
        ray get_ray(float s, float t);
    
        vec3 origin;
        vec3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
        vec3 u, v, w;
        float time0, time1;  // new variables for shutter open/close times
        float lens_radius;
};