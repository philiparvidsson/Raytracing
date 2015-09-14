#include "raytracer.h"

#include "base/common.h"
#include "base/debug.h"
#include "graphics/pixmap.h"
#include "math/vector.h"
#include "optics/surface.h"

#include <float.h>
#include <stdlib.h>

raytracerT* createRaytracer(int width, int height) {
    raytracerT* raytracer = calloc(1, sizeof(raytracerT));

    raytracer->pixmap = createPixmap(width, height);

    return (raytracer);
}

void freeRaytracer(raytracerT* raytracer) {
    free(raytracer->pixmap);
    free(raytracer);
}

void addSurface(raytracerT* raytracer, surfaceT* surface) {
    assert(surface->material != NULL);

    surface->next = raytracer->surfaces;
    raytracer->surfaces = surface;
}

intersectionT findIntersection(raytracerT* raytracer, rayT* ray) {
    intersectionT intersection = { 0 };

    intersection.t = FLT_MAX;

    surfaceT* surface = raytracer->surfaces;
    while (surface) {
        intersectionT intersect = surface->intersect_fn(ray, surface);

        // Find the intersection that is closest to the eye.
        if ((intersect.t > 0.0f) && (intersect.t < intersection.t))
            intersection = intersect;

        surface = surface->next;
    }

    return (intersection);
}

void raytraceAll(raytracerT* raytracer) {
    int   width       = pixmapWidth (raytracer->pixmap);
    int   height      = pixmapHeight(raytracer->pixmap);
    float half_width  = (width  - 1) / 2.0f;
    float half_height = (height - 1) / 2.0f;

    rayT ray;
    ray.origin = (vec3) { 0.0f, 0.35f, 1.0f };

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            ray.direction = (vec3) {  (x - half_width ) / half_width,
                                     -(y - half_height) / half_height,
                                     -1.0f };

            intersectionT intersection = findIntersection(raytracer, &ray);

            // It's safe to test equality against FLT_MAX here.
            if ((intersection.t < 0.01f) || (intersection.t > 10.0f)) {
                // No intersection - background color.
                setPixel(raytracer->pixmap, x, y, 0, 0, 0);
                continue;
            }

            materialT* material = intersection.surface->material;
            vec3       color    = material->color_fn(raytracer, &intersection);

            setPixelf(raytracer->pixmap, x, y, color.x, color.y, color.z);
        }
    }
}

void raytraceLine(raytracerT* raytracer, int y) {
    int   width       = pixmapWidth (raytracer->pixmap);
    int   height      = pixmapHeight(raytracer->pixmap);
    float half_width  = (width  - 1) / 2.0f;
    float half_height = (height - 1) / 2.0f;

    rayT ray;
    ray.origin = (vec3) { 0.0f, 0.35f, 1.0f };

    for (int x = 0; x < width; x++) {
        ray.direction = (vec3) {  (x - half_width ) / half_width,
                                    -(y - half_height) / half_height,
                                    -1.0f };

        intersectionT intersection = findIntersection(raytracer, &ray);

        // It's safe to test equality against FLT_MAX here.
        if ((intersection.t < 0.01f) || (intersection.t > 10.0f)) {
            // No intersection - background color.
            setPixel(raytracer->pixmap, x, y, 0, 0, 0);
            continue;
        }

        materialT* material = intersection.surface->material;
        vec3       color    = material->color_fn(raytracer, &intersection);

        setPixelf(raytracer->pixmap, x, y, color.x, color.y, color.z);
    }
}