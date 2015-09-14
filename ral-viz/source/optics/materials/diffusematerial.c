#include "diffusematerial.h"

#include "base/common.h"
#include "math/vector.h"
#include "optics/material.h"

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

materialT* createDiffuseMaterial(vec3 ambient_color, vec3 diffuse_color) {
    materialT* material = createMaterial();

    material->color_fn = calcDiffuseColor;
    material->data     = malloc(sizeof(diffuseMaterialT));

    ((diffuseMaterialT*)material->data)->ambient_color = ambient_color;
    ((diffuseMaterialT*)material->data)->diffuse_color = diffuse_color;

    return (material);
}
