#include "raytracer.h"

#include "base/common.h"
#include "graphics/pixmap.h"
#include "math/surface.h"
#include "math/vector.h"

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
    surface->next = raytracer->surfaces;
    raytracer->surfaces = surface;
}

void raytraceAll(raytracerT* raytracer) {
    int   width       = pixmapWidth (raytracer->pixmap);
    int   height      = pixmapHeight(raytracer->pixmap);
    float half_width  = (width  - 1) / 2.0f;
    float half_height = (height - 1) / 2.0f;

    rayT ray;
    ray.origin = (vec3) { 0.0f, 0.35f, 2.0f };

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            ray.direction = (vec3) {  (x - half_width ) / half_width,
                                     -(y - half_height) / half_height,
                                     -1.0f };   

            intersectionT intersection = { 0 };

            intersection.t = FLT_MAX;

            surfaceT* surface = raytracer->surfaces;
            while (surface) {
                intersectionT intersect = surface->intersect_fn(&ray, surface);

                // Find the intersection that is closest to the eye.
                if (intersect.t >= 1.0f && intersect.t < intersection.t)
                    intersection = intersect;

                surface = surface->next;
            }

            // It's safe to test equality against FLT_MAX here.
            if (intersection.t == FLT_MAX) {
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
