#include "diffuse_material.h"

#include "base/common.h"
#include "math/material.h"
#include "math/vector.h"

static vec3 calcDiffuseColor(raytracerT* raytracer, intersectionT* intersection) {
    diffuseMaterialT* material = intersection->surface->material->data;

    vec3 color = { 0 };

    vec3 l = (vec3) { 1.0f, 1.0f, 2.0f };

    vec_normalize(&l, &l);
    float f = vec_dot(&l, &intersection->normal);
    f = clamp(f, 0.0f, 1.0f);

    color.x += material->ambient_color.x + f*material->diffuse_color.x;
    color.y += material->ambient_color.y + f*material->diffuse_color.y;
    color.z += material->ambient_color.z + f*material->diffuse_color.z;

    return (color);
}

materialT* createDiffuseMaterial(float ar, float ag, float ab,
                                 float dr, float dg, float db)
{
    materialT* material = createMaterial();

    material->color_fn = calcDiffuseColor;
    material->data     = malloc(sizeof(diffuseMaterialT));

    ((diffuseMaterialT*)material->data)->ambient_color = (vec3) { ar, ag, ab };
    ((diffuseMaterialT*)material->data)->diffuse_color = (vec3) { dr, dg, db };

    return (material);
}
