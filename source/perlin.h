#pragma once

#include "vec3.h"

float trilinear_interp(float c[2][2][2], float u, float v, float w);

class perlin {
public:
    float noise(const vec3& p) const;
    float turb(const vec3& p, int depth = 7) const;

    // static float *ranfloat;
    /** 
     * because min and max depend on integer x/y/z,
     * we should use random unit vectors on lattice points,
     * and use a dot product to remove the max and min off the lattice.
     */
    static vec3 *ranvec;
    static int *perm_x;
    static int *perm_y;
    static int *perm_z;
};