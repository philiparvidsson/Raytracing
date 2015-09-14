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
    lightSourceT* light_source1 = createSphereLightSource((vec3) { -0.3f, 0.1f, 0.0f }, 0.05f);
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

    surfaceT* sphere1 = createSphereSurface((vec3) { 0.2f, 0.4f, 0.2f }, 0.1f);
    /*sphere1->material = createPhongMaterial((vec3) { 0.2f, 0.0f, 0.0f },
                                            (vec3) { 0.8f, 0.0f, 0.0f },
                                            (vec3) { 1.0f, 1.0f, 1.0f }, 50.0f);
*/
    sphere1->material = createReflectiveMaterial(1.0);
    surfaceT* sphere2 = createSphereSurface((vec3) { -0.3f, 0.6f, 0.0f }, 0.3f);
    sphere2->material = createPhongMaterial((vec3) { 0.2f, 0.2f, 0.2f },
                                            (vec3) { 0.8f, 0.8f, 0.8f },
                                            (vec3) { 0.8f, 0.8f, 0.8f }, 20.0f);

    surfaceT* plane = createPlaneSurface();
    plane->material = createDiffuseMaterial((vec3) { 0.2f, 0.0f, 0.0f },
                                            (vec3) { 0.9f, 0.3f, 0.3f });

    surfaceT* sphere3 = createSphereSurface((vec3) { 0.0f, 0.0f, 0.0f }, 8.0f);
    sphere3->material = createDiffuseMaterial((vec3) { 0.7f, 0.8f, 0.9f }, (vec3) { 0.3f, 0.2f, 0.1f });

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
        args->size = 4;
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
