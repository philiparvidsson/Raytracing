#ifndef diffusematerial_h_
#define diffusematerial_h_

#include "base/common.h"
#include "math/vector.h"
#include "optics/material.h"

typedef struct diffuseMaterialT {
    vec3 ambient_color,
         diffuse_color;
} diffuseMaterialT;

materialT* createDiffuseMaterial(vec3 ambient_color, vec3 diffuse_color);

#endif // diffusematerial_h_
