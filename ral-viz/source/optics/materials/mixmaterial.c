#include "mixmaterial.h"

#include "base/common.h"
#include "math/vector.h"
#include "optics/material.h"

static vec3 calcMaterialColor(raytracerT* raytracer, intersectionT* intersection) {
    mixMaterialT* m = intersection->surface->material->data;

    vec3 c1 = m->material1->color_fn(raytracer, intersection);
    vec3 c2 = m->material2->color_fn(raytracer, intersection);

    vec3 c = { 0 };

    c.x = (c1.x*m->a) + (c2.x*(1.0f-m->a));
    c.y = (c1.y*m->a) + (c2.y*(1.0f-m->a));
    c.z = (c1.z*m->a) + (c2.z*(1.0f-m->a));

    return (c);
}

materialT* createMixMaterial(materialT* material1, materialT* material2, float a) {
    materialT* material = createMaterial();

    material->color_fn = calcMaterialColor;
    material->data     = malloc(sizeof(mixMaterialT));

    ((mixMaterialT*)material->data)->a         = a;
    ((mixMaterialT*)material->data)->material1 = material1;
    ((mixMaterialT*)material->data)->material2 = material2;

    return (material);
}
