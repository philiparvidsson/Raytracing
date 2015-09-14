#ifndef pointlightsource_h_
#define pointlightsource_h_

#include "base/common.h"
#include "math/vector.h"
#include "optics/lightsource.h"

typedef struct pointLightSourceT {
    vec3 position;
} pointLightSourceT;

lightSourceT* createPointLightSource(vec3 position);

#endif // pointlightsource_h_
