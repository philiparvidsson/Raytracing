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
int current_region_lol; // global int, msdc hax :<
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

void tracerThread2(void* arg) {
    tracerThreadArgsT* args = arg;

    int num_regions = args->number_of_regions;
    while (current_region_lol < num_regions) {
        regionT region = args->regions[current_region_lol++];
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



static void renderRegion(raytracerT* raytracer, regionT region) {
    int num_cpu = processorCount();

    regionT* regs = malloc(sizeof(regionT) * 64 * 64);

    int k = 0;
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            if (region.x + j >= pixmapWidth(raytracer->pixmap)) continue;
            if (region.y + i >= pixmapHeight(raytracer->pixmap)) continue;

            setPixel(raytracer->pixmap, region.x + j, region.y + i, 255, 255, 255);

            regs[k].sizeX = 1;
            regs[k].sizeY = 1;
            regs[k].x = region.x + j;
            regs[k].y = region.y + i;
            k++;
        }
    }

    tracerThreadArgsT* args = malloc(sizeof(tracerThreadArgsT) * num_cpu);

    current_region_lol = 0;
    for (int i = 0; i < num_cpu; i++) {
        args[i].raytracer = raytracer;
        args[i].regions = regs;
        args[i].number_of_regions = k;
        args[i].thread_exit = false;
        createThread(tracerThread2, &args[i]);
        //printf("%d\n", 1337);
    }

    while (true) {
        blitPixmap(raytracer->pixmap, 0, 0);
        updateDisplay();

        bool done = true;

        for (int i = 0; i < num_cpu; i++) {
            if (!args[i].thread_exit)
                done = false;
        }

        if (done)
            break;
    }

    free(args);
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

    initGraphics("ral-viz pre-alpha", 480, 480);

    raytracerT* raytracer = createRaytracer(480, 480);






    surfaceT* sphere = createSphereSurface((vec3) { 0.0f, 0.0f, 5.0f }, 8.0f);
    surfaceT* sphere2 = createSphereSurface((vec3) { 0.0f, 1.1f, -2.0f }, 0.5f);

    sphere2->material = createPhongMaterial((vec3) { 0.0f, 0.0f, 0.0f },
                                            (vec3) { 0.0f, 0.0f, 0.0f },
                                            (vec3) { 1.0f, 1.0f, 1.0f }, 30.0);

    sphere2->material = createMixMaterial(createReflectiveMaterial(1.0, 8), sphere2->material, mixFunc);

    sphere->material = createAmbientMaterial((vec3) { 1.0f, 1.0f, 1.0f });

    float size = 3.0f;
    float offs = 1.5f;
    float zoffs = -2.0f;
    surfaceT* plane1 = createPlaneSurface((vec3) { -size, 0.0f+offs, 0.0f+zoffs }, (vec3) { 1.0f, 0.0f, 0.0f });
    surfaceT* plane2 = createPlaneSurface((vec3) { size, 0.0f+offs, 0.0f+zoffs }, (vec3) { -1.0f, 0.0f, 0.0f });
    surfaceT* plane3 = createPlaneSurface((vec3) { 0.0f, size+offs, 0.0f+zoffs }, (vec3) { 0.0f, -1.0f, 0.0f });
    surfaceT* plane4 = createPlaneSurface((vec3) { 0.0f, -size+offs, 0.0f+zoffs }, (vec3) { 0.0f, 1.0f, 0.0f });
    surfaceT* plane6 = createPlaneSurface((vec3) { 0.0f, 0.0f+offs, -size+zoffs }, (vec3) { 0.0f, 0.0f, 1.0f });

    addLightSource(raytracer, createSphereLightSource((vec3) { 0.0f, size+offs-0.05f, zoffs }, size, 20.0f));
	addLightSource(raytracer, createSphereLightSource((vec3) { 0.5f, -size + offs + 0.9f, 0.0f }, 0.9f, 4.0f));
	addLightSource(raytracer, createSphereLightSource((vec3) { 0.0f, offs, size+zoffs}, 3.0f, 40.0f));


    plane1->material = createDiffuseMaterial((vec3) { 0.0f, 0.0f, 0.0f },
                                             (vec3) { 1.0f, 0.0f, 0.0f });

    plane2->material = createDiffuseMaterial((vec3) { 0.0f, 0.0f, 0.0f },
                                             (vec3) { 0.0f, 1.0f, 0.0f });

    plane3->material = createDiffuseMaterial((vec3) { 0.0f, 0.0f, 0.0f },
                                             (vec3) { 1.0f, 1.0f, 1.0f });

    plane4->material = createDiffuseMaterial((vec3) { 0.0f, 0.0f, 0.0f },
                                             (vec3) { 1.0f, 1.0f, 1.0f });


    plane6->material = createDiffuseMaterial((vec3) { 0.0f, 0.0f, 0.0f },
                                             (vec3) { 1.0f, 1.0f, 1.0f });

    addSurface(raytracer, plane1);
    addSurface(raytracer, plane2);
    addSurface(raytracer, plane3);
    addSurface(raytracer, plane4);
    addSurface(raytracer, plane6);

    surfaceT* s = createSphereSurface((vec3) { -1.1f, -size+offs+0.85, zoffs }, 0.85f);
    s->material = createDiffuseMaterial((vec3) { 0.0f, 0.0f, 0.0f },
                                      (vec3) { 0.0f, 0.5f, 1.0f }
                                      );
    addSurface(raytracer, s);

    s = createSphereSurface((vec3) { 1.2f, -size+offs+0.65, 1.0f +zoffs}, 0.65f);
    s->material = createDiffuseMaterial((vec3) { 0.0f, 0.0f, 0.0f },
                                      (vec3) { 1.0f, 0.15f, 0.15f }
                                      );
    addSurface(raytracer, s);

    s = createSphereSurface((vec3) { 0.3f, -size+offs+1.45, -2.0f +zoffs}, 1.45f);
    s->material = createDiffuseMaterial((vec3) { 0.0f, 0.0f, 0.0f },
                                      (vec3) { 1.0f, 1.0f, 0.0f }
                                      );
	s->material = createMixMaterial(s->material, createReflectiveMaterial(0.5f, 16), mixFunc);
    addSurface(raytracer, s);

    s = createSphereSurface((vec3) { 0.0f, 0.0f, 0.0 }, 14.0f);
    s->material = createAmbientMaterial((vec3) { 1.0f, 1.0f, 1.0f });
    addSurface(raytracer, s);

	s = createSphereSurface((vec3) { -0.2f, -size+offs+0.45, 1.0f +zoffs}, 0.45f);
    s->material = createDiffuseMaterial((vec3) { 0.0f, 0.0f, 0.0f },
                                      (vec3) { 0.2f, 0.9f, 0.15f }
                                      );
	addSurface(raytracer, s);

    // create regions
    int sizeX, sizeY, width, height, num_regionsX, num_regionsY;
    sizeX = 64;
    sizeY = 64;

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

    for (int i = 0; i < num_regionsX*num_regionsY; i++) {
        int j = rand() % (num_regionsX*num_regionsY);
        regionT reg = regions[i];
        regions[i] = regions[j];
        regions[j] = reg;
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
    /*for (int i = 0; i < num_threads; i++) {
        args[i].raytracer = raytracer;
        args[i].regions = regions;
        args[i].number_of_regions = num_regionsX*num_regionsY;
        args[i].thread_exit = false;
        createThread(tracerThread, &args[i]);
    }*/

    int y = 0;
    bool lol = false;
    while (windowIsOpen()) {
        blitPixmap(raytracer->pixmap, 0, 0);
        updateDisplay();

        /*if (render_running) {
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
        }*/

        if (current_region < num_regionsX*num_regionsY) {
            renderRegion(raytracer, regions[current_region++]);
        }
        else {
            if (!lol) {
                trace("render time: %f.2s", ((float)(clock() - t)) / CLOCKS_PER_SEC);
                lol = true;
            }
        }
    }

    free(raytracer);
    exitGraphics();

    printf("Bye!\n");

    return (0);
}
