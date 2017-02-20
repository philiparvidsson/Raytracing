#ifndef planesurface_h_
#define planesurface_h_

#include "base/common.h"
#include "math/vector.h"
#include "optics/ray.h"
#include "optics/surface.h"

typedef enum planeSurfaceTypeT {
    XYPlane,
    XZPlane,
    YZPlane
} planeSurfaceTypeT;

typedef struct planeSurfaceT {
    vec3 pos;
    vec3 normal;
} planeSurfaceT;

#endif // planesurface_h_
