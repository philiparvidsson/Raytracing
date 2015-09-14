#ifndef lightsource_h_
#define lightsource_h_

#include "base/common.h"
#include "math/vector.h"
#include "optics/ray.h"

#include <stdlib.h>

typedef struct lightRayT (*lightFnT)(struct lightSourceT* l, struct intersectionT* i);

typedef struct lightSourceT {
    int num_samples;
    lightFnT light_fn;
    void* data;

    struct lightSourceT* next;
} lightSourceT;

typedef struct lightRayT {
    vec3 direction;
    float intensity;
    float distance;
} lightRayT;

lightSourceT* createLightSource(void);
void freeLightSource(lightSourceT* light_source);

#endif // light_h_
