#include "planesurface.h"

#include "base/common.h"
#include "optics/ray.h"
#include "optics/surface.h"

#include <stdlib.h>

static intersectionT findPlaneIntersection(rayT* ray, surfaceT* surface) {
    intersectionT intersection = { 0 };

    intersection.surface = surface;
    intersection.ray     = *ray;

    // Eq. for an XZ plane:
    // o_y+td_y=0
    // td_y=-o_y
    // t=-o_y/d_y

    float t = -ray->origin.y / ray->direction.y;
    if (t > 0.0f) {
        intersection.t        = t;
        intersection.position = (vec3) { ray->origin.x + t*ray->direction.x,
                                         ray->origin.y + t*ray->direction.y,
                                         ray->origin.z + t*ray->direction.z };
        intersection.normal   = (vec3) { 0.0f, 1.0f, 0.0f };
    }

    return (intersection);
}

surfaceT* createPlaneSurface(void) {
    surfaceT* surface = createSurface();

    surface->intersect_fn = findPlaneIntersection;
    surface->data         = malloc(sizeof(planeSurfaceT));

    return (surface);
}
