/*------------------------------------------------------------------------------
 * File: ral-viz.c
 * Created: September 12, 2015
 * Last changed: September 12, 2015
 *
 * Author(s): Philip Arvidsson (contact@philiparvidsson.com)
 *
 * Description:
 *   Main program file. Contains the main method etc.
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "base/common.h"
#include "graphics/pixmap.h"
#include "math/vector.h"
#include "optics/lightsources/directionallightsource.h"
#include "optics/lightsources/pointlightsource.h"
#include "optics/lightsources/spherelightsource.h"
#include "optics/materials/ambientmaterial.h"
#include "optics/materials/diffusematerial.h"
#include "optics/materials/mixmaterial.h"
#include "optics/materials/phongmaterial.h"
#include "optics/materials/reflectivematerial.h"
#include "optics/raytracer.h"
#include "optics/surfaces/planesurface.h"
#include "optics/surfaces/spheresurface.h"
#include "optics/lightsource.h"
#include "optics/material.h"
#include "optics/ray.h"
#include "optics/surface.h"
#include "graphics.h"
#include "thread.h"

#include <stdio.h>

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

typedef struct tracerThreadArgsT {
    raytracerT* raytracer;
    int index;
    int step;
    int size;
} tracerThreadArgsT;

void tracerThread(void* arg) {
    tracerThreadArgsT* args = arg;

    int y = args->index * args->size;
    while (y < pixmapHeight(args->raytracer->pixmap)) {
        int x = 0;

        while (x < pixmapWidth(args->raytracer->pixmap)) {
            raytraceRect(args->raytracer, x, y, args->size, args->size);
            x += args->size;
        }

        y += args->step * args->size;
    }

    free(args);
}

/*--------------------------------------
 * Function: printIntroMessage()
 *
 * Description:
 *   Prints an introductory message.
 *
 * Usage:
 *   printIntroMessage();
 *------------------------------------*/
static void printIntroMessage(void) {
    printf("ral-viz early build\nnothing to see here yet...\n\n");
}

static inline float absf(float f) {
    if (f < 0.0f)
        return (-f);
    return (f);
}

float mixFunc(materialT* material, raytracerT* raytracer, intersectionT* intersection) {
    lightSourceT* light_source = raytracer->light_sources;
    

    //float mix = 0.0f;
    /*while (light_source) {
        float mult = 1.0f / light_source->num_samples;

        for (int i = 0; i < light_source->num_samples; i++) {
            lightRayT light_ray = light_source->light_fn(light_source, intersection);

            rayT shadow_ray;
            shadow_ray.origin = intersection->position;
            shadow_ray.direction = light_ray.direction;

            intersectionT occlusion_intersection = findIntersection(raytracer, &shadow_ray, intersection->surface, light_ray.distance);

            if (occlusion_intersection.t <= 0.0f) {
                vec3 v = intersection->position;
                vec_sub(&v, &intersection->ray.origin, &v);
                vec_normalize(&v, &v);

                vec3 r;
                vec_reflect(&light_ray.direction, &intersection->normal, &r);

                float specular = powf(absf(vec_dot(&r, &v)), 20.0f);

                mix += specular * mult;
            }
        }


        light_source = light_source->next;
    }*/

    //mix = powf(mix, 1.0f/3.0f);
    vec3 v = intersection->position;
    vec_sub(&v, &intersection->ray.origin, &v);
    vec_normalize(&v, &v);
    vec3 r;
    vec_reflect(&v, &intersection->normal, &r);

    float mix = powf(clamp(vec_dot(&r, &v), 0.0f, 1.0f), 2.5f);

    return (0.4f * clamp(mix, 0.0f, 1.0f) + 0.03f);
}

/*--------------------------------------
 * Function: main(argc, argv)
 *
 * Parameters:
 *   argc  Number of command line arguments.
 *   argv  Command line arguments.
 *
 * Description:
 *   Main program function.
 *
 * Usage:
 *   ---
 *------------------------------------*/
int main(int argc, char* argv[]) {
    printIntroMessage();

    initGraphics("ral-viz pre-alpha", 720, 720);

    raytracerT* raytracer = createRaytracer(720, 720);

    //lightSourceT* light_source1 = createDirectionalLightSource((vec3) { 1.0f, 1.0f, 0.0f });
    lightSourceT* light_source1 = createSphereLightSource((vec3) { -0.3f, 0.8f, 0.4f }, 0.05f);
    lightSourceT* light_source2 = createSphereLightSource((vec3) { -0.4f, 1.6f, 0.7f }, 0.2f);
    lightSourceT* light_source3 = createSphereLightSource((vec3) { 0.3f, 1.8f, -0.4f }, 0.1f);

    /*surfaceT* light_sphere1 = createSphereSurface((vec3) { -0.8f, 0.2f, 1.3f }, 0.2f);
    surfaceT* light_sphere2 = createSphereSurface((vec3) { -0.4f, 1.6f, 0.7f }, 0.2f);
    surfaceT* light_sphere3 = createSphereSurface((vec3) { 0.3f, 1.8f, -0.4f }, 0.1f);

    materialT* material = createAmbientMaterial((vec3) { 1.0f, 1.0f, 1.0f });
    light_sphere1->material = material;
    light_sphere2->material = material;
    light_sphere3->material = material;*/

    //addSurface(raytracer, light_sphere1);
    //addSurface(raytracer, light_sphere2);
    //addSurface(raytracer, light_sphere3);

    addLightSource(raytracer, light_source1);
    addLightSource(raytracer, light_source2);
    addLightSource(raytracer, light_source3);

    surfaceT* sphere1 = createSphereSurface((vec3) { 0.3f, 0.2f, 0.3f }, 0.2f);
    /*sphere1->material = createPhongMaterial((vec3) { 0.2f, 0.0f, 0.0f },
                                            (vec3) { 0.8f, 0.0f, 0.0f },
                                            (vec3) { 1.0f, 1.0f, 1.0f }, 50.0f);
*/
    sphere1->material = createMixMaterial(
                            createReflectiveMaterial(1.0, 8),
                            createPhongMaterial((vec3) { 0.0f, 0.1f, 0.2f },
                                                (vec3) { 0.0f, 0.5f, 1.0f },
                                                (vec3) { 1.0f, 1.0f, 1.0f }, 90.0f),
                            mixFunc);
    //sphere1->material = createReflectiveMaterial(1.0, 1);

    surfaceT* sphere2 = createSphereSurface((vec3) { -0.3f, 0.3f, 0.0f }, 0.3f);
    //sphere2->material = createPhongMaterial((vec3) { 0.2f, 0.2f, 0.2f },
    //                                        (vec3) { 0.8f, 0.8f, 0.8f },
    //                                        (vec3) { 0.8f, 0.8f, 0.8f }, 20.0f);
    sphere2->material = createMixMaterial(
                            createReflectiveMaterial(1.0, 8),
                            createPhongMaterial((vec3) { 0.2f, 0.05f, 0.0f },
                                                (vec3) { 0.8f, 0.2f, 0.0f },
                                                (vec3) { 4.0f, 4.0f, 4.0f }, 180.0f),
                            mixFunc);
    surfaceT* plane = createPlaneSurface();
    plane->material = createDiffuseMaterial((vec3) { 0.5f, 0.5f, 0.5f },
                                            (vec3) { 1.0f, 1.0f, 1.0f });

    surfaceT* sphere3 = createSphereSurface((vec3) { 0.0f, 0.0f, 0.0f }, 8.0f);
    sphere3->material = createAmbientMaterial((vec3) { 1.0f, 1.0f, 1.0f });

    //sphere1->material = sphere2->material;
    addSurface(raytracer, sphere1);
    addSurface(raytracer, sphere2);
    addSurface(raytracer, sphere3);
    addSurface(raytracer, plane);


    int num_threads = 4;
    for (int i = 0; i < num_threads; i++) {
        tracerThreadArgsT* args = malloc(sizeof(tracerThreadArgsT));
        args->index = i;
        args->step = num_threads;
        args->raytracer = raytracer;
        args->size = 16;
        createThread(tracerThread, args);
    }

    int y = 0;
    while (windowIsOpen()) {
        blitPixmap(raytracer->pixmap, 0, 0);
        updateDisplay();
    }

    free(raytracer);
    exitGraphics();

    printf("Bye!\n");

    return (0);
}
