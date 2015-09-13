#ifndef ambient_material_h_
#define ambient_material_h_

#include "base/common.h"
#include "math/material.h"
#include "math/vector.h"

typedef struct ambientMaterialT {
    vec3 color;
} ambientMaterialT;

materialT* createAmbientMaterial(float r, float g, float b);

#endif // ambient_material_h_
