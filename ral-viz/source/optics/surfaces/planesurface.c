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

    // Eq. for plane with normal:
    // (o-c+dt) . N = 0
    // (o_x - c_x + d_x*t) * N_x + (o_y - c_y + d_y*t) * N_y + (o_z - c_z + d_z*t) * N_z = 0
    // o_x*N_x - c_x*N_x + d_x*t*N_x + o_y*N_y - c_y*N_y + d_y*t*N_y + o_z*N_z - c_z*N_z + d_z*t*N_z = 0
    // o_x*N_x - c_x*N_x + o_y*N_y - c_y*N_y + o_z*N_z - c_z*N_z = 0 - d_x*t*N_x - d_y*t*N_y - d_z*t*N_z
    // o_x*N_x - c_x*N_x + o_y*N_y - c_y*N_y + o_z*N_z - c_z*N_z = -(d_x*t*N_x + d_y*t*N_y + d_z*t*N_z)
    // o_x*N_x - c_x*N_x + o_y*N_y - c_y*N_y + o_z*N_z - c_z*N_z = -(d_x*N_x + d_y*N_y + d_z*N_z) * t
    // (o_x - c_x)*N_x + (o_y - c_y)*N_y + (o_z - c_z)*N_z = -(d_x*N_x + d_y*N_y + d_z*N_z) * t
    // ((o_x - c_x)*N_x + (o_y - c_y)*N_y + (o_z - c_z)*N_z) / -(d_x*N_x + d_y*N_y + d_z*N_z) = t

    planeSurfaceT* s = (planeSurfaceT*)surface->data;
    vec3 p = s->pos;
    vec3 n = s->normal;
    vec3 d = ray->direction;
    vec3 o = ray->origin;

    vec3 r;
    vec_sub(&p, &o, &r);

    float t = vec_dot(&r, &n) / vec_dot(&d, &n);

    if (t > 0.0f) {
        intersection.t        = t;
        intersection.position = (vec3) { o.x + t*d.x,
                                         o.y + t*d.y,
                                         o.z + t*d.z };
        intersection.normal   = n;
    }

    //printf("%f\n", t);

    return (intersection);
}

surfaceT* createPlaneSurface(vec3 c, vec3 n) {
    surfaceT* surface = createSurface();

    surface->intersect_fn = findPlaneIntersection;
    surface->data         = malloc(sizeof(planeSurfaceT));

    planeSurfaceT* plane = (planeSurfaceT*)surface->data;

    plane->pos = c;
    plane->normal = n;

    return (surface);
}
