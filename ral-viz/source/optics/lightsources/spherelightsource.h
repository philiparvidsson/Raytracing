#ifndef spherelightsource_h_
#define spherelightsource_h_

#include "base/common.h"
#include "math/vector.h"
#include "optics/lightsource.h"

typedef struct sphereLightSourceT {
    vec3 position;
    float radius;
    float intensity;
} sphereLightSourceT;

lightSourceT* createSphereLightSource(vec3 position, float radius, float intensity);

#endif // spherelightsource_h_
