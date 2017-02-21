#ifndef texturedmaterial_h_
#define texturedmaterial_h_

#include "base/common.h"
#include "math/vector.h"
#include "optics/material.h"

typedef vec3(*texFnT)(materialT* material, raytracerT* raytracer, intersectionT* intersection);

typedef struct texturedMaterialT {
    texFnT ambient_tex,
         diffuse_tex,
         specular_tex;

    float shininess;
} texturedMaterialT;

materialT* createTexturedMaterial(texFnT ambient_tex,
	texFnT diffuse_tex,
	texFnT specular_tex, float shininess);

#endif // texturedmaterial_h_
