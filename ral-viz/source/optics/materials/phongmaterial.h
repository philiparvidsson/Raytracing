#ifndef phongmaterial_h_
#define phongmaterial_h_

#include "base/common.h"
#include "math/vector.h"
#include "optics/material.h"

typedef struct phongMaterialT {
    vec3 ambient_color,
         diffuse_color,
         specular_color;

    float shininess;
} phongMaterialT;

materialT* createPhongMaterial(vec3 ambient_color, vec3 diffuse_color,
                               vec3 specular_color, float shininess);

#endif // phongmaterial_h_
