#include "diffusematerial.h"

#include "base/common.h"
#include "math/vector.h"
#include "optics/material.h"

static vec3 calcMaterialColor(raytracerT* raytracer, intersectionT* intersection) {
    diffuseMaterialT* material = intersection->surface->material->data;

    vec3 color = material->ambient_color;

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

                color.x += f*material->diffuse_color.x*mult;
                color.y += f*material->diffuse_color.y*mult;
                color.z += f*material->diffuse_color.z*mult;
            }
        }


        light_source = light_source->next;
    }

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
