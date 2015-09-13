#ifndef sphere_surface_h_
#define sphere_surface_h_

#include "base/common.h"
#include "math/ray.h"
#include "math/surface.h"
#include "math/vector.h"

typedef struct sphereSurfaceT {
    vec3 center;
    float radius;
} sphereSurfaceT;

surfaceT* createSphereSurface(void);

#endif // sphere_surface_h_
