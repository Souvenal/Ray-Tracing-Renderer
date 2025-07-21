#pragma once

#include "hitables.h"
#include "materials.h"
#include "textures.h"

hitable* two_spheres();
hitable* two_perlin_spheres();
hitable* random_scene();
hitable *simple_light();
hitable *cornell_box();
hitable *cornell_smoke();
hitable *final();