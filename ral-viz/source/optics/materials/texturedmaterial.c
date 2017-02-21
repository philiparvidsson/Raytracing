#include "texturedmaterial.h"

#include <float.h>
__declspec(thread) static int depth = 0;

static vec3 calcMaterialColor(materialT* material, raytracerT* raytracer, intersectionT* intersection) {
    texturedMaterialT* m = material->data;

    vec3 color = m->ambient_tex(material, raytracer, intersection);
	vec3 diffuse_color = m->diffuse_tex(material, raytracer, intersection);
	vec3 specular_color = m->specular_tex(material, raytracer, intersection);

	depth++;

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

                vec3 dc = diffuse_color;
                vec3 sc = specular_color;

                vec_scale(&dc, diffuse*mult, &dc);
                vec_scale(&sc, specular*mult, &sc);
                vec_add(&dc, &color, &color);
                vec_add(&sc, &color, &color);
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

materialT* createTexturedMaterial(texFnT ambient_tex,
	texFnT diffuse_tex,
	texFnT specular_tex, float shininess) {
    materialT* material = createMaterial();

    material->color_fn = calcMaterialColor;
    material->data = malloc(sizeof(texturedMaterialT));

    ((texturedMaterialT*)material->data)->ambient_tex = ambient_tex;
	((texturedMaterialT*)material->data)->diffuse_tex = diffuse_tex;
	((texturedMaterialT*)material->data)->specular_tex = specular_tex;
    ((texturedMaterialT*)material->data)->shininess = shininess;

    return (material);
}
