#include "reflectivematerial.h"

#include <float.h>

static vec3 calcMaterialColor(raytracerT* raytracer, intersectionT* intersection) {
    reflectiveMaterialT* material = intersection->surface->material->data;

    vec3 color = { 0 };

    rayT ray = { 0 };
    vec_reflect(&intersection->ray.direction, &intersection->normal, &ray.direction);
    ray.origin = intersection->position;

    intersectionT intersection2 = findIntersection(raytracer, &ray, intersection->surface, FLT_MAX);

    if (intersection2.t > 0.0f) {
        vec3 c2 = calcFinalColor(raytracer, &intersection2);
        vec_add(&color, &c2, &color);
    }


    return (color);
}

materialT* createReflectiveMaterial(float reflectiveness) {
    materialT* material = createMaterial();

    material->color_fn = calcMaterialColor;
    material->data     = malloc(sizeof(reflectiveMaterialT));

    ((reflectiveMaterialT*)material->data)->reflectiveness = reflectiveness;

    return (material);
}