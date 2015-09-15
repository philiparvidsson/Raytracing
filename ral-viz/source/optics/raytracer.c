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

vec3 calcFinalColor(raytracerT* raytracer, intersectionT* intersection) {
    materialT* material = intersection->surface->material;

    vec3 color = material->color_fn(material, raytracer, intersection);

    return (color);
}

void raytraceRectFast(raytracerT* raytracer, int x, int y, int w, int h) {
    int   width = pixmapWidth(raytracer->pixmap);
    int   height = pixmapHeight(raytracer->pixmap);
    float half_width = (width - 1) / 2.0f;
    float half_height = (height - 1) / 2.0f;

    rayT ray;
    ray.origin = (vec3) { 0.0f, 0.35f, 1.0f };

    for (int rx = x; rx < (x + w); rx += 2) {
        for (int ry = y; ry < (y + h); ry += 2) {
            vec3 color = { 0 };
            if (rx < 0 || rx >= width) continue;
            if (ry < 0 || ry >= height) continue;

            ray.direction = (vec3) {  (rx - half_width ) / half_width,
                                        -(ry - half_height) / half_height,
                                        -1.0f };

            intersectionT intersection = findIntersection(raytracer, &ray, NULL, FLT_MAX);

            if ((intersection.t < 0.01f) || (intersection.t > 10.0f)) {
                // No intersection.
                continue;
            }

            vec3 c = calcFinalColor(raytracer, &intersection);
            vec_add(&c, &color, &color);

            color.x = sqrtf(color.x);
            color.y = sqrtf(color.y);
            color.z = sqrtf(color.z);

            int rx1 = rx;
            int rx2 = rx + 1;
            int ry1 = ry;
            int ry2 = ry+1;

            if ((rx1 >= 0 && rx1 < width) && (ry1 >= 0 && ry1 < height)) setPixelf(raytracer->pixmap, rx1, ry1, color.x, color.y, color.z);
            if ((rx2 >= 0 && rx2 < width) && (ry1 >= 0 && ry1 < height)) setPixelf(raytracer->pixmap, rx2, ry1, color.x, color.y, color.z);
            if ((rx1 >= 0 && rx1 < width) && (ry2 >= 0 && ry2 < height)) setPixelf(raytracer->pixmap, rx1, ry2, color.x, color.y, color.z);
            if ((rx2 >= 0 && rx2 < width) && (ry2 >= 0 && ry2 < height)) setPixelf(raytracer->pixmap, rx2, ry2, color.x, color.y, color.z);
        }
    }
}


void raytraceRect(raytracerT* raytracer, int x, int y, int w, int h) {
    int   width       = pixmapWidth (raytracer->pixmap);
    int   height      = pixmapHeight(raytracer->pixmap);
    float half_width  = (width  - 1) / 2.0f;
    float half_height = (height - 1) / 2.0f;


    vec3 origin = (vec3) { 0.0f, 0.35f, 1.0f };

    int filter_size = 7;
    int num_aperture_samples = 16;
    float aperture_size = 0.05f;
    float focal_dist = 1.0f;

    for (int rx = x; rx < (x+w); rx++) {
        for (int ry = y; ry < (y+h); ry++) {
            if (rx < 0 || rx >= width ) continue;
            if (ry < 0 || ry >= height) continue;
            vec3 color = { 0 };

            for (int n = 0; n < num_aperture_samples; n++) {
                float a = (rand() / (float)RAND_MAX) * 3.141592653f * 2.0f;
                float b = (rand() / (float)RAND_MAX) * aperture_size;

                float ax = cosf(a) * aperture_size;
                float ay = sinf(a) * aperture_size;

                for (int i = -filter_size; i <= filter_size; i++) {
                    for (int j = -filter_size; j <= filter_size; j++) {
                        float fx = 0.5f*(i/half_width)/(float)filter_size;
                        float fy = 0.5f*(j/half_height)/(float)filter_size;


                        rayT ray;
                        ray.origin = (vec3) { origin.x + ax, origin.y + ay, origin.z };
                        ray.direction = (vec3) {  (rx - half_width ) / half_width  + fx - ax/focal_dist,
                                                 -(ry - half_height) / half_height + fy - ay/focal_dist,
                                                 -1.0f };

                        intersectionT intersection = findIntersection(raytracer, &ray, NULL, FLT_MAX);

                        if ((intersection.t < 0.01f) || (intersection.t > 10.0f)) {
                            // No intersection.
                            continue;
                        }

                        vec3 c = calcFinalColor(raytracer, &intersection);
                        vec_add(&c, &color, &color);
                    }
                }
            }

            vec_scale(&color, 1.0f/(num_aperture_samples*(2*filter_size+1)*(2*filter_size+1)), &color);

            color.x = sqrtf(color.x);
            color.y = sqrtf(color.y);
            color.z = sqrtf(color.z);
            setPixelf(raytracer->pixmap, rx, ry, color.x, color.y, color.z);
        }
    }
}
