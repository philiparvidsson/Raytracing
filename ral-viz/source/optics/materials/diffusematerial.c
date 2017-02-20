#include "diffusematerial.h"

#include "base/common.h"
#include "math/vector.h"
#include "optics/material.h"

#include <float.h>
__declspec(thread) static int depth = 0;

static vec3 calcMaterialColor(materialT* material, raytracerT* raytracer, intersectionT* intersection) {
    diffuseMaterialT* difmat = material->data;

    vec3 color = difmat->ambient_color;

    depth++;

    lightSourceT* light_source = raytracer->light_sources;
    while (light_source) {
        float mult = 1.0f / light_source->num_samples;

        for (int i = 0; i < light_source->num_samples; i++) {
            lightRayT light_ray = light_source->light_fn(light_source, intersection);

            rayT shadow_ray;
            shadow_ray.origin = intersection->position;
            shadow_ray.direction = light_ray.direction;
            //vec_flip(&shadow_ray.direction, &shadow_ray.direction);

            intersectionT occlusion_intersection = findIntersection(raytracer, &shadow_ray, intersection->surface, light_ray.distance);

            if (occlusion_intersection.t <= 0.0f) {
                float f = vec_dot(&intersection->normal, &light_ray.direction);

                f *= light_ray.intensity;

                f = clamp(f, 0.0f, 1.0f);

                color.x += f*difmat->diffuse_color.x*mult;
                color.y += f*difmat->diffuse_color.y*mult;
                color.z += f*difmat->diffuse_color.z*mult;
            }
        }


        light_source = light_source->next;
    }

    vec3 c3 = { 0 };
    rayT ray = { 0 };

    if (depth < 2) {
        ray.origin = intersection->position;
        int num_samples = 8;
        for (int i = 0; i < num_samples; i++) {
            //vec_reflect(&intersection->ray.direction, &intersection->normal, &ray.direction);
            ray.direction = intersection->normal;

            float x = 1.8f*((rand() / (float)RAND_MAX) - 0.5f);
            float y = 1.8f*((rand() / (float)RAND_MAX) - 0.5f);
            float z = 1.8f*((rand() / (float)RAND_MAX) - 0.5f);

            ray.direction.x += x;
            ray.direction.y += y;
            ray.direction.z += z;

            vec_normalize(&ray.direction, &ray.direction);

            intersectionT intersection2 = findIntersection(raytracer, &ray, intersection->surface, FLT_MAX);

            if (intersection2.t > 0.0f) {
                vec3 c2 = calcFinalColor(raytracer, &intersection2);
                vec_add(&c3, &c2, &c3);
            }
        }

        vec_scale(&c3, 1.0f / num_samples, &c3);

        float a = 0.6f;
        color.x = color.x*a + c3.x*(1.0f - a);
        color.y = color.y*a + c3.y*(1.0f - a);
        color.z = color.z*a + c3.z*(1.0f - a);
    }

    depth--;


    return (color);
}

materialT* createDiffuseMaterial(vec3 ambient_color, vec3 diffuse_color) {
    materialT* material = createMaterial();

    material->color_fn = calcMaterialColor;
    material->data     = malloc(sizeof(diffuseMaterialT));

    ((diffuseMaterialT*)material->data)->ambient_color = ambient_color;
    ((diffuseMaterialT*)material->data)->diffuse_color = diffuse_color;

    return (material);
}
