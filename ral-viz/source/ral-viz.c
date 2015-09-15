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

#include "base/debug.h"
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

#include <time.h>
#include <stdio.h>

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

typedef struct regionT {
    int x;
    int y;
    int sizeX;
    int sizeY;
} regionT;

typedef struct tracerThreadArgsT {
    raytracerT* raytracer;
    regionT *regions;
    int number_of_regions;
    bool thread_exit;
} tracerThreadArgsT;

int current_region; // global int, msdc hax :<
int current_region_reverse;

void tracerThreadFast(void* arg) {
    tracerThreadArgsT* args = arg;

    int num_regions = args->number_of_regions;
    while (current_region < num_regions) {
        regionT region = args->regions[current_region++];
        raytraceRectFast(args->raytracer, region.x, region.y, region.sizeX, region.sizeY);
    }

    args->thread_exit = true;
}

void tracerThread(void* arg) {
    tracerThreadArgsT* args = arg;

    int num_regions = args->number_of_regions;
    while (current_region < num_regions) {
        regionT region = args->regions[current_region++];
        raytraceRect(args->raytracer, region.x, region.y, region.sizeX, region.sizeY);
    }

    args->thread_exit = true;
}

void tracerThreadReverse(void* arg) {
    tracerThreadArgsT* args = arg;

    int num_regions = args->number_of_regions;
    while (current_region_reverse >= 0) {
        regionT region = args->regions[current_region_reverse--];
        raytraceRect(args->raytracer, region.x, region.y, region.sizeX, region.sizeY);
    }

    args->thread_exit = true;
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
    printf("ral-viz early build\nnothing to see here yet...\ncuriously, \"nothing\" is precisely equal to mental_difference(phil, wannabe_nub)\nhowever, we shan't forget a more important function for understanding real world phenomena: matt_noob_level() which is not defined - but rather empirically observed - to always return infinity.\n\n\n\n");
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
    lightSourceT* light_source1 = createSphereLightSource((vec3) { -0.3f, 0.1f, 0.4f }, 0.05f, 1.0f);
    lightSourceT* light_source2 = createSphereLightSource((vec3) { -0.4f, 1.6f, 0.7f }, 0.2f, 1.0f);
    lightSourceT* light_source3 = createSphereLightSource((vec3) { 0.3f, 1.8f, -0.4f }, 0.1f, 1.0f);
    lightSourceT* light_source4 = createSphereLightSource((vec3) { 0.05f, 0.1f, 0.0f }, 0.025f, 0.5f);

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
    addLightSource(raytracer, light_source4);

    surfaceT* sphere1 = createSphereSurface((vec3) { 0.3f, 0.2f, 0.0f }, 0.2f);
    /*sphere1->material = createPhongMaterial((vec3) { 0.2f, 0.0f, 0.0f },
                                            (vec3) { 0.8f, 0.0f, 0.0f },
                                            (vec3) { 1.0f, 1.0f, 1.0f }, 50.0f);
*/
    sphere1->material = createMixMaterial(
                            createReflectiveMaterial(1.0, 8),
                            createPhongMaterial((vec3) { 0.0f, 0.1f, 0.2f },
                                                (vec3) { 0.0f, 0.3f, 0.8f },
                                                (vec3) { 1.0f, 1.0f, 1.0f }, 90.0f),
                            mixFunc);
    //sphere1->material = createReflectiveMaterial(1.0, 1);

    surfaceT* sphere2 = createSphereSurface((vec3) { -0.3f, 0.3f, 0.0f }, 0.3f);
    //sphere2->material = createPhongMaterial((vec3) { 0.2f, 0.2f, 0.2f },
    //                                        (vec3) { 0.8f, 0.8f, 0.8f },
    //                                        (vec3) { 0.8f, 0.8f, 0.8f }, 20.0f);
    sphere2->material = createMixMaterial(
                            createReflectiveMaterial(1.0, 8),
                            createPhongMaterial((vec3) { 0.2f, 0.0f, 0.0f },
                                                (vec3) { 0.9f, 0.0f, 0.0f },
                                                (vec3) { 4.0f, 4.0f, 4.0f }, 90.0f),
                            mixFunc);
    surfaceT* plane = createPlaneSurface();
    plane->material = createDiffuseMaterial((vec3) { 0.0f, 0.0f, 0.0f },
                                            (vec3) { 1.0f, 1.0f, 1.0f });

    surfaceT* sphere3 = createSphereSurface((vec3) { 0.0f, 0.0f, 0.0f }, 8.0f);
    sphere3->material = createAmbientMaterial((vec3) { 1.0f, 1.0f, 1.0f });

    surfaceT* sphere4 = createSphereSurface((vec3) { 0.0f, 0.9f, -1.2f }, 0.9f);
    sphere4->material = createDiffuseMaterial((vec3) { 0.0f, 0.0f, 0.0f },
                                              (vec3) { 1.0f, 0.0f, 0.0f });

    surfaceT* sphere5 = createSphereSurface((vec3) { 0.5f, 0.2f, 0.45f }, 0.2f);
    sphere5->material = createPhongMaterial((vec3) { 0.2f, 0.15f, 0.0f },
                                            (vec3) { 0.9f, 0.7f, 0.3f },
                                            (vec3) { 4.0f, 4.0f, 4.0f }, 90.0f);

    surfaceT* sphere6 = createSphereSurface((vec3) { -0.4f, 0.1f, 0.35f }, 0.1f);
    sphere6->material = createPhongMaterial((vec3) { 0.1f, 0.3f, 0.1f },
                                            (vec3) { 0.3f, 0.9f, 0.3f },
                                            (vec3) { 4.0f, 4.0f, 4.0f }, 90.0f);

    //sphere1->material = sphere2->material;
    addSurface(raytracer, sphere1);
    addSurface(raytracer, sphere2);
    addSurface(raytracer, sphere3);
    addSurface(raytracer, sphere4);
    addSurface(raytracer, sphere5);
    addSurface(raytracer, sphere6);
    addSurface(raytracer, plane);

    // create regions
    int sizeX, sizeY, width, height, num_regionsX, num_regionsY;
    sizeX = 16;
    sizeY = 16;

    width = pixmapWidth(raytracer->pixmap);
    height = pixmapHeight(raytracer->pixmap);

    if (width % sizeX == 0)
        num_regionsX = width / sizeX;
    else
        num_regionsX = width / sizeX + 1;

    if (height % sizeY == 0)
        num_regionsY = height / sizeY;
    else
        num_regionsY = height / sizeY + 1;

    regionT *regions = malloc(sizeof(regionT) * num_regionsX * num_regionsY);
    for (int i = 0; i < num_regionsY; i++) {
        for (int j = 0; j < num_regionsX; j++) {
            regions[i * num_regionsX + j].x = j * sizeX;
            regions[i * num_regionsX + j].y = (num_regionsY-i-1) * sizeY;
            regions[i * num_regionsX + j].sizeX = sizeX;
            regions[i * num_regionsX + j].sizeY = sizeY;
        }
    }

    for (int i = 0; i < 5000; i++) {
        int a = rand() % (num_regionsX * num_regionsY);
        int b = rand() % (num_regionsX * num_regionsY);

        regionT reg = regions[a];
        regions[a] = regions[b];
        regions[b] = reg;
    }


    clock_t t;
    bool render_running = true;

    int num_threads = processorCount();
    trace("using %d render threads...", num_threads)
    current_region = 0;

    // code copy-paste-change hax by philster
    tracerThreadArgsT* args = malloc(sizeof(tracerThreadArgsT) * num_threads);
    for (int i = 0; i < num_threads; i++) {
        args[i].raytracer = raytracer;
        args[i].regions = regions;
        args[i].number_of_regions = num_regionsX*num_regionsY;
        args[i].thread_exit = false;
        createThread(tracerThreadFast, &args[i]);
    }

    while (render_running && windowIsOpen) {
        blitPixmap(raytracer->pixmap, 0, 0);
        updateDisplay();

        bool threads_exited = true;
        for (int i = 0; i < num_threads; i++)
            if (!args[i].thread_exit) {
                threads_exited = false;
                break;
            }

        if (threads_exited) {
            render_running = false;
        }
    }

    // FINAL RENDER PASS
    render_running = true;
    
    current_region = 0;
    current_region_reverse = current_region - 1;

    t = clock();
    for (int i = 0; i < num_threads; i++) {
        args[i].raytracer = raytracer;
        args[i].regions = regions;
        args[i].number_of_regions = num_regionsX*num_regionsY;
        args[i].thread_exit = false;
        createThread(tracerThread, &args[i]);
    }

    int y = 0;
    while (windowIsOpen()) {
        blitPixmap(raytracer->pixmap, 0, 0);
        updateDisplay();

        if (render_running) {
            bool threads_exited = true;
            for (int i = 0; i < num_threads; i++)
                if (!args[i].thread_exit) {
                    threads_exited = false;
                    break;
                }

            if (threads_exited) {
                render_running = false;
                trace("render time: %f.2s", ((float)(clock() - t)) / CLOCKS_PER_SEC);

                free(args);
            }
        }
    }

    free(raytracer);
    exitGraphics();

    printf("Bye!\n");

    return (0);
}
