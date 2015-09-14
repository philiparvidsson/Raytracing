#ifndef spheresurface_h_
#define spheresurface_h_

#include "base/common.h"
#include "math/vector.h"
#include "optics/ray.h"
#include "optics/surface.h"

typedef struct sphereSurfaceT {
    vec3  center;
    float radius;
} sphereSurfaceT;

surfaceT* createSphereSurface(vec3 center, float radius);

#endif // spheresurface_h_
