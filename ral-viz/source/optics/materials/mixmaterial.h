#ifndef mixmaterial_h_
#define mixmaterial_h_

#include "base/common.h"
#include "optics/material.h"

typedef float(*mixFnT)(materialT* material, raytracerT* raytracer, intersectionT* intersection);

typedef struct mixMaterialT {
    mixFnT mix_fn;

    materialT* material1;
    materialT* material2;
} mixMaterialT;

materialT* createMixMaterial(materialT* material1, materialT* material2, mixFnT mix_fn);

#endif // mixmaterial_h_
