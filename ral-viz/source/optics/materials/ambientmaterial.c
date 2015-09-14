#include "ambientmaterial.h"

#include "base/common.h"
#include "math/vector.h"
#include "optics/material.h"

static vec3 calcAmbientColor(raytracerT* raytracer, intersectionT* intersection) {
    ambientMaterialT* material = intersection->surface->material->data;

    return (material->color);
}

materialT* createAmbientMaterial(vec3 color) {
    materialT* material = createMaterial();

    material->color_fn = calcAmbientColor;
    material->data     = malloc(sizeof(ambientMaterialT));

    ((ambientMaterialT*)material->data)->color = color;

    return (material);
}