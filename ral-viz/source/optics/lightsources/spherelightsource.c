#include "spherelightsource.h"

#include "base/common.h"
#include "math/vector.h"
#include "optics/lightsource.h"
#include "optics/ray.h"

#include <math.h>
#include <stdlib.h>

static lightRayT calcLight(lightSourceT* l, intersectionT* i) {
    lightRayT light_ray;

    sphereLightSourceT* sphere = l->data;

    vec3 p = sphere->position;

    vec3 r = { 0 };
    r.x = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;
    r.y = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;
    r.z = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;
    vec_normalize(&r, &r);
    vec_scale(&r, sphere->radius, &r);

    vec_add(&r, &p, &p);


    light_ray.direction = p;
    vec_sub(&light_ray.direction, &i->position, &light_ray.direction);

    float distance_squared = vec_dot(&light_ray.direction, &light_ray.direction);

    light_ray.distance = sqrtf(distance_squared);

    vec_normalize(&light_ray.direction, &light_ray.direction);

    light_ray.intensity = 1.0f / distance_squared;

    return (light_ray);
}

lightSourceT* createSphereLightSource(vec3 position, float radius) {
    lightSourceT* light_source = createLightSource();

    light_source->num_samples = 1;
    light_source->light_fn    = calcLight;
    light_source->data        = malloc(sizeof(sphereLightSourceT));

    ((sphereLightSourceT*)light_source->data)->position = position;
    ((sphereLightSourceT*)light_source->data)->radius = radius;

    return (light_source);
}
