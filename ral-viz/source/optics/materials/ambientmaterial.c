#include "ambientmaterial.h"

#include "base/common.h"
#include "math/vector.h"
#include "optics/material.h"

static vec3 calcMaterialColor(materialT* material, raytracerT* raytracer, intersectionT* intersection) {
    ambientMaterialT* amb = material->data;

    return (amb->color);
}

materialT* createAmbientMaterial(vec3 color) {
    materialT* material = createMaterial();

    material->color_fn = calcMaterialColor;
    material->data     = malloc(sizeof(ambientMaterialT));

    ((ambientMaterialT*)material->data)->color = color;

    return (material);
}
