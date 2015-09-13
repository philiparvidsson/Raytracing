#ifndef diffuse_material_h_
#define diffuse_material_h_

#include "base/common.h"
#include "math/material.h"
#include "math/vector.h"

typedef struct diffuseMaterialT {
    vec3 ambient_color,
         diffuse_color;
} diffuseMaterialT;

materialT* createDiffuseMaterial(float ambient_r,
                                 float ambient_g,
                                 float ambient_b,
                                 float diffuse_r,
                                 float diffuse_g,
                                 float diffuse_b);

#endif // diffuse_material_h_
