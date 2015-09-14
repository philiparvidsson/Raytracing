#ifndef spherelightsource_h_
#define spherelightsource_h_

#include "base/common.h"
#include "math/vector.h"
#include "optics/lightsource.h"

typedef struct sphereLightSourceT {
    vec3 position;
    float radius;
} sphereLightSourceT;

lightSourceT* createSphereLightSource(vec3 position, float radius);

#endif // spherelightsource_h_
