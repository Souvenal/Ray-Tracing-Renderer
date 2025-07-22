#pragma once

#include "hitables.h"
#include "materials.h"
#include "textures.h"
#include "camera.h"

hitable* two_spheres();
hitable* two_perlin_spheres();
hitable* random_scene();
hitable *simple_light();
void cornell_box(hitable **scene, camera **cam, float aspect);
hitable *cornell_smoke();
hitable *final();