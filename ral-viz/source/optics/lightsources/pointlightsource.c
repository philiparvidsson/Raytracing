#include "pointlightsource.h"

#include "base/common.h"
#include "math/vector.h"
#include "optics/lightsource.h"
#include "optics/ray.h"

#include <math.h>
#include <stdlib.h>

static lightRayT calcLight(lightSourceT* l, intersectionT* i) {
    lightRayT light_ray;

    light_ray.direction = ((pointLightSourceT*)l->data)->position;
    vec_sub(&light_ray.direction, &i->position, &light_ray.direction);
    //vec_flip(&light_ray.direction, &light_ray.direction);

    float distance_squared = vec_dot(&light_ray.direction, &light_ray.direction);

    light_ray.distance = sqrtf(distance_squared);

    vec_normalize(&light_ray.direction, &light_ray.direction);

    light_ray.intensity = 0.1f / distance_squared;

    return (light_ray);
}

lightSourceT* createPointLightSource(vec3 position) {
    lightSourceT* light_source = createLightSource();

    light_source->light_fn = calcLight;
    light_source->data     = malloc(sizeof(pointLightSourceT));

    ((pointLightSourceT*)light_source->data)->position = position;

    return (light_source);
}
