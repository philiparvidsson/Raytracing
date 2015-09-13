#include "ambient_material.h"

#include "base/common.h"
#include "math/material.h"
#include "math/vector.h"

static vec3 calcAmbienetColor(raytracerT* raytracer, intersectionT* intersection) {
    ambientMaterialT* material = intersection->surface->material->data;

    return (material->color);
}

materialT* createAmbientMaterial(float r, float g, float b) {
    materialT* material = createMaterial();

    material->color_fn = calcAmbienetColor;
    material->data     = malloc(sizeof(ambientMaterialT));

    ((ambientMaterialT*)material->data)->color = (vec3) { r, g, b };

    return (material);
}
