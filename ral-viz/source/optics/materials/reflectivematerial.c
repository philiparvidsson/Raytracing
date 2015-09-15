#include "reflectivematerial.h"

#include <float.h>

__declspec(thread) static int depth = 0;


static vec3 calcMaterialColor(materialT* material, raytracerT* raytracer, intersectionT* intersection) {

    reflectiveMaterialT* m = material->data;

    vec3 color = { 0 };

    if (depth > 3)
        return (color);

    depth++;

    rayT ray = { 0 };

    
    ray.origin = intersection->position;

    for (int i = 0; i < m->num_samples; i++) {
        vec_reflect(&intersection->ray.direction, &intersection->normal, &ray.direction);

        float a = ((rand() / (float)RAND_MAX) * 3.141592653f * 0.5f) * m->reflectiveness;
        float b = (rand() / (float)RAND_MAX) * 3.141592653f * 2.0f;

        float x = 0.1f*((rand() / (float)RAND_MAX) - 0.5f);
        float y = 0.1f*((rand() / (float)RAND_MAX) - 0.5f);
        float z = 0.1f*((rand() / (float)RAND_MAX) - 0.5f);

        ray.direction.x += x;
        ray.direction.y += y;
        ray.direction.z += z;

        vec_normalize(&ray.direction, &ray.direction);

        intersectionT intersection2 = findIntersection(raytracer, &ray, intersection->surface, FLT_MAX);

        if (intersection2.t > 0.0f) {
            vec3 c2 = calcFinalColor(raytracer, &intersection2);
            vec_add(&color, &c2, &color);
        }
    }

    vec_scale(&color, 1.0f/m->num_samples, &color);

    depth--;
    return (color);
}

materialT* createReflectiveMaterial(float reflectiveness, int num_samples) {
    materialT* material = createMaterial();

    material->color_fn = calcMaterialColor;
    material->data     = malloc(sizeof(reflectiveMaterialT));

    ((reflectiveMaterialT*)material->data)->reflectiveness = clamp(1.0f-reflectiveness, 0.0f, 1.0f);
    ((reflectiveMaterialT*)material->data)->num_samples    = num_samples;

    return (material);
}