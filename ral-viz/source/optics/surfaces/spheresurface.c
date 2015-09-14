#include "spheresurface.h"

#include "base/common.h"
#include "math/vector.h"
#include "optics/ray.h"
#include "optics/surface.h"

#include <stdlib.h>

static inline float square(float f) {
    return (f*f);
}

static intersectionT findSphereIntersection(rayT* ray, surfaceT* surface) {
    intersectionT   intersection = { 0 };
    sphereSurfaceT* sphere       = surface->data;

    intersection.surface = surface;
    intersection.ray     = *ray;

    // Sphere equation: x^2+y^2+z^2=r^2
    // Or, with a position: (x-c_x)^2+(y-c_y)^2+(z-c_z)^2 = r^2
    //
    // Intersecting it with a parametric line:
    //
    //   (o_x+td_x-c_x)^2+(o_y+td_y-c_y)^2+(o_z+td_z-c_z)^2 = r^2
    //
    // Solving for t:
    //   (o_x+td_x-c_x)^2+(o_y+td_y-c_y)^2+(o_z+td_z-c_z)^2-r^2 = 0
    //
    //   ((o_x-c_x)+td_x)^2+((o_y-c_y)+td_y)^2+((o_z-c_z)+td_z)^2-r^2 = 0
    //
    //   (o_x-c_x)^2+2*(o_x-c_x)*t*d_x+t^2d_x^2 +
    //   (o_y-c_y)^2+2*(o_y-c_y)*t*d_y+t^2d_y^2 +
    //   (o_z-c_z)^2+2*(o_z-c_z)*t*d_y+t^2d_y^2 - r^2 = 0
    //
    //   (o_x-c_x)^2+(o_y-c_y)^2+(o_z-c_z)^2 +
    //   2t((o_x-c_x)*d_x+(o_y-c_y)*d_y+(o_z-c_z)*d_y) +
    //   t^2(d_x^2+d_y^2+d_z^2) - r^2 = 0
    //
    // This gives us a quadratic equation of the form ax^2+bx+c=0:
    //
    //   a = (d_x^2+d_y^2+d_z^2)
    //   b = 2((o_x-c_x)*d_x+(o_y-c_y)*d_y+(o_z-c_z)*d_y)
    //   c = (o_x-c_x)^2+(o_y-c_y)^2+(o_z-c_z)^2-r
    //
    //   t^2+(b/a)t+(c/a) = 0
    //
    // Completing the square:
    //
    //   t^2+(b/a)t = -c/a
    //   (t+0.5(b/a))^2 = -c/a+0.25(b/a)^2
    //   t+0.5(b/a) = +-sqrt(-c/a+0.25(b/a)^2)
    //   t = +-sqrt(-c/a+0.25(b/a)^2) - 0.5(b/a)

    float a = square(ray->direction.x)
            + square(ray->direction.y)
            + square(ray->direction.z);

    float b = 2.0f * ((ray->origin.x - sphere->center.x) * ray->direction.x +
                      (ray->origin.y - sphere->center.y) * ray->direction.y +
                      (ray->origin.z - sphere->center.z) * ray->direction.z);

    float c = square(ray->origin.x - sphere->center.x) +
              square(ray->origin.y - sphere->center.y) +
              square(ray->origin.z - sphere->center.z) -
              square(sphere->radius);

    float d = -c/a + 0.25f*square(b/a);

    if (d >= 0.0f) {
        float t0     = -sqrtf(d) - 0.5*(b/a);
        float t1     =  sqrtf(d) - 0.5*(b/a);
        float t      =  t0;
        bool  inside =  false;

        if (t <= 0.0f) {
            // We're inside the sphere.
            t      = t1;
            inside = true;
        }

        intersection.t        = t;
        intersection.position = (vec3) { ray->origin.x + t*ray->direction.x,
                                         ray->origin.y + t*ray->direction.y,
                                         ray->origin.z + t*ray->direction.z };
        intersection.normal   = intersection.position;
        vec_sub(&intersection.normal, &sphere->center, &intersection.normal);
        vec_normalize(&intersection.normal, &intersection.normal);

        if (inside)
            vec_flip(&intersection.normal, &intersection.normal);
    }

    return (intersection);
}

surfaceT* createSphereSurface(vec3 center, float radius) {
    surfaceT* surface = createSurface();

    surface->intersect_fn = findSphereIntersection;
    surface->data = malloc(sizeof(sphereSurfaceT));

    sphereSurfaceT* sphere = surface->data;
    
    sphere->center = center;
    sphere->radius = radius;

    return (surface);
}
