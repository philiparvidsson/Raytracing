#ifndef ray_h_
#define ray_h_

#include "base/common.h"
#include "math/surface.h"
#include "math/vector.h"

typedef struct rayT {
    vec3 origin;    // Origin.
    vec3 direction; // Direction.
} rayT;

typedef struct intersectionT {
    struct surfaceT* surface;
    rayT ray;
    float t;

    vec3 position;
    vec3 normal;
} intersectionT;

#endif // ray_h_
