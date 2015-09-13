#ifndef plane_surface_h_
#define plane_surface_h_

#include "base/common.h"
#include "math/ray.h"
#include "math/surface.h"
#include "math/vector.h"

typedef enum planeSurfaceTypeT {
    XYPlane,
    XZPlane,
    YZPlane
} planeSurfaceTypeT;

typedef struct planeSurfaceT {
    float lol;
} planeSurfaceT;

surfaceT* createPlaneSurface(void);

#endif // plane_surface_h_
