#ifndef directionallightsource_h_
#define directionallightsource_h_

#include "base/common.h"
#include "math/vector.h"
#include "optics/lightsource.h"

typedef struct directionalLightSourceT {
    vec3 direction;
} directionalLightSourceT;

lightSourceT* createDirectionalLightSource(vec3 direction);

#endif // directionallight_h_
