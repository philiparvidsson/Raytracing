#include "directionallightsource.h"

#include "base/common.h"
#include "math/vector.h"
#include "optics/lightsource.h"
#include "optics/ray.h"

#include <float.h>
#include <stdlib.h>

static lightRayT calcLight(lightSourceT* l, intersectionT* i) {
    lightRayT light_ray;

    light_ray.direction = ((directionalLightSourceT*)l->data)->direction;
    light_ray.distance  = FLT_MAX;
    light_ray.intensity = 1.0f;

    return (light_ray);
}

lightSourceT* createDirectionalLightSource(vec3 direction) {
    lightSourceT* light_source = createLightSource();

    light_source->light_fn = calcLight;
    light_source->data     = malloc(sizeof(directionalLightSourceT));

    vec_normalize(&direction, &direction);
    ((directionalLightSourceT*)light_source->data)->direction = direction;

    return (light_source);
}
