#ifndef ambientmaterial_h_
#define ambientmaterial_h_

#include "base/common.h"
#include "math/vector.h"
#include "optics/material.h"

typedef struct ambientMaterialT {
    vec3 color;
} ambientMaterialT;

materialT* createAmbientMaterial(vec3 color);

#endif // ambientmaterial_h_
