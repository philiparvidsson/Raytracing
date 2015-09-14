#include "phongmaterial.h"

#include <float.h>

static vec3 calcMaterialColor(materialT* material, raytracerT* raytracer, intersectionT* intersection) {
    phongMaterialT* m = material->data;

    vec3 color = m->ambient_color;

    lightSourceT* light_source = raytracer->light_sources;
    while (light_source) {
        float mult = 1.0f / light_source->num_samples;

        for (int i = 0; i < light_source->num_samples; i++) {
            lightRayT light_ray = light_source->light_fn(light_source, intersection);

            rayT shadow_ray;
            shadow_ray.origin = intersection->position;
            shadow_ray.direction = light_ray.direction;

            intersectionT occlusion_intersection = findIntersection(raytracer, &shadow_ray, intersection->surface, light_ray.distance);

            if (occlusion_intersection.t <= 0.0f) {
                vec3 v = intersection->position;
                vec_sub(&v, &intersection->ray.origin, &v);
                vec_normalize(&v, &v);

                vec3 r;
                vec_reflect(&light_ray.direction, &intersection->normal, &r);

                float diffuse = max(0.0f, vec_dot(&light_ray.direction, &intersection->normal));
                float specular = powf(max(0.0f, vec_dot(&r, &v)), m->shininess);

                vec3 dc = m->diffuse_color;
                vec3 sc = m->specular_color;

                vec_scale(&dc, diffuse*mult, &dc);
                vec_scale(&sc, specular*mult, &sc);
                vec_add(&dc, &color, &color);
                vec_add(&sc, &color, &color);
            }
        }


        light_source = light_source->next;
    }

    return (color);
}

materialT* createPhongMaterial(vec3 ambient_color, vec3 diffuse_color,
    vec3 specular_color, float shininess) {
    materialT* material = createMaterial();

    material->color_fn = calcMaterialColor;
    material->data = malloc(sizeof(phongMaterialT));

    ((phongMaterialT*)material->data)->ambient_color = ambient_color;
    ((phongMaterialT*)material->data)->diffuse_color = diffuse_color;
    ((phongMaterialT*)material->data)->specular_color = specular_color;
    ((phongMaterialT*)material->data)->shininess = shininess;

    return (material);
}
