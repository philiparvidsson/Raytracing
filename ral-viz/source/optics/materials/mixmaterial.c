#include "mixmaterial.h"

#include "base/common.h"
#include "math/vector.h"
#include "optics/material.h"

static vec3 calcMaterialColor(materialT* material, raytracerT* raytracer, intersectionT* intersection) {
    mixMaterialT* m = material->data;

    vec3 c1 = m->material1->color_fn(m->material1, raytracer, intersection);
    vec3 c2 = m->material2->color_fn(m->material2, raytracer, intersection);

    vec3 c = { 0 };

    float a = m->mix_fn(material, raytracer, intersection);

    c.x = (c1.x*a) + (c2.x*(1.0f-a));
    c.y = (c1.y*a) + (c2.y*(1.0f-a));
    c.z = (c1.z*a) + (c2.z*(1.0f-a));

    return (c);
}

materialT* createMixMaterial(materialT* material1, materialT* material2, mixFnT mix_fn) {
    materialT* material = createMaterial();

    material->color_fn = calcMaterialColor;
    material->data     = malloc(sizeof(mixMaterialT));

    ((mixMaterialT*)material->data)->mix_fn   = mix_fn;
    ((mixMaterialT*)material->data)->material1 = material1;
    ((mixMaterialT*)material->data)->material2 = material2;

    return (material);
}
