#include "raytracer.h"

#include "base/common.h"
#include "base/debug.h"
#include "graphics/pixmap.h"
#include "math/vector.h"
#include "optics/lightsource.h"
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

void addLightSource(raytracerT* raytracer, lightSourceT* light_source) {
    light_source->next = raytracer->light_sources;
    raytracer->light_sources = light_source;
}

void addSurface(raytracerT* raytracer, surfaceT* surface) {
    assert(surface->material != NULL);

    surface->next = raytracer->surfaces;
    raytracer->surfaces = surface;
}

intersectionT findIntersection(raytracerT* raytracer, rayT* ray, surfaceT* excluded_surface, float max_distance) {
    intersectionT intersection = { 0 };

    intersection.t = FLT_MAX;

    surfaceT* surface = raytracer->surfaces;
    while (surface) {
        if (surface == excluded_surface) {
            surface = surface->next;
            continue;
        }

        intersectionT intersect = surface->intersect_fn(ray, surface);

        // Find the intersection that is closest to the eye.
        if ((intersect.t > 0.0f) && (intersect.t < max_distance) && (intersect.t < intersection.t))
            intersection = intersect;

        surface = surface->next;
    }

    if (intersection.t == FLT_MAX)
        intersection.t = 0.0f;

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

            intersectionT intersection = findIntersection(raytracer, &ray, NULL, FLT_MAX);

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
        vec3 color = { 0 };

        for (int i = -7; i <= 7; i++) {
            for (int j = -7; j <= 7; j++) {
                float dx = 0.5f*(i/half_width)/7.0f;
                float dy = 0.5f*(j/half_height)/7.0f;

                ray.direction = (vec3) {  (x - half_width ) / half_width + dx,
                                         -(y - half_height) / half_height + dy,
                                         -1.0f };

                intersectionT intersection = findIntersection(raytracer, &ray, NULL, FLT_MAX);

                // It's safe to test equality against FLT_MAX here.
                if ((intersection.t < 0.01f) || (intersection.t > 10.0f)) {
                    // No intersection.
                    continue;
                }

                materialT* material = intersection.surface->material;

                vec3 c = material->color_fn(raytracer, &intersection);
                vec_add(&c, &color, &color);

            }
        }

        vec_scale(&color, 1.0f/255.0f, &color);
        setPixelf(raytracer->pixmap, x, y, color.x, color.y, color.z);
    }
}

vec3 calcFinalColor(raytracerT* raytracer, intersectionT* intersection) {
    materialT* material = intersection->surface->material;

    vec3 color = material->color_fn(raytracer, intersection);

    return (color);
}

void raytraceRect(raytracerT* raytracer, int x, int y, int w, int h) {
    int   width       = pixmapWidth (raytracer->pixmap);
    int   height      = pixmapHeight(raytracer->pixmap);
    float half_width  = (width  - 1) / 2.0f;
    float half_height = (height - 1) / 2.0f;

    rayT ray;
    ray.origin = (vec3) { 0.0f, 0.35f, 1.0f };

    for (int rx = x; rx < (x+w); rx++) {
        vec3 color = { 0 };
        for (int ry = y; ry < (y+h); ry++) {
            if (rx < 0 || rx >= width ) continue;
            if (ry < 0 || ry >= height) continue;

            for (int i = -7; i <= 7; i++) {
                for (int j = -7; j <= 7; j++) {
                    float dx = 0.5f*(i/half_width)/7.0f;
                    float dy = 0.5f*(j/half_height)/7.0f;

                    ray.direction = (vec3) {  (rx - half_width ) / half_width  + dx,
                                             -(ry - half_height) / half_height + dy,
                                             -1.0f };

                    intersectionT intersection = findIntersection(raytracer, &ray, NULL, FLT_MAX);

                    // It's safe to test equality against FLT_MAX here.
                    if ((intersection.t < 0.01f) || (intersection.t > 10.0f)) {
                        // No intersection.
                        continue;
                    }

                    vec3 c = calcFinalColor(raytracer, &intersection);
                    vec_add(&c, &color, &color);
                }
            }

            vec_scale(&color, 1.0f/225.0f, &color);
            setPixelf(raytracer->pixmap, rx, ry, color.x, color.y, color.z);
        }
    }
}
