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
#include "optics/materials/ambientmaterial.h"
#include "optics/materials/diffusematerial.h"
#include "optics/raytracer.h"
#include "optics/surfaces/planesurface.h"
#include "optics/surfaces/spheresurface.h"
#include "optics/lightsource.h"
#include "optics/material.h"
#include "optics/ray.h"
#include "optics/surface.h"
#include "graphics.h"

#include <stdio.h>

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

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

    lightSourceT* light_source1 = createDirectionalLightSource((vec3) { 1.0f, 1.0f, 0.0f });
    lightSourceT* light_source2 = createPointLightSource((vec3) { 0.3f, 0.4f, 0.0f });

    addLightSource(raytracer, light_source1);
    addLightSource(raytracer, light_source2);

    //surfaceT* sphere1 = createSphereSurface((vec3) { 0.0f, 0.0f, 0.0f }, 8.0f);
    surfaceT* sphere1 = createSphereSurface((vec3) { 0.3f, 0.9f, 0.2f }, 0.1f);
    sphere1->material = createDiffuseMaterial((vec3) { 0.7f, 0.8f, 1.0f },
                                              (vec3) { 0.3f, 0.2f, 0.0f });

    surfaceT* sphere2 = createSphereSurface((vec3) { -0.3f, 0.4f, 0.0f }, 0.3f);
    sphere2->material = createDiffuseMaterial((vec3) { 0.2f, 0.2f, 0.2f },
                                              (vec3) { 0.8f, 0.8f, 0.8f });

    surfaceT* plane = createPlaneSurface();
    plane->material = createDiffuseMaterial((vec3) { 0.0f, 0.0f, 0.0f },
                                            (vec3) { 0.9f, 0.3f, 0.3f });

    sphere1->material = sphere2->material;
    addSurface(raytracer, sphere1);
    addSurface(raytracer, sphere2);
    addSurface(raytracer, plane);

    int y = 0;
    while (windowIsOpen()) {
        if (y < 720) {
            raytraceLine(raytracer, y++);
            blitPixmap(raytracer->pixmap, 0, 0);
        }

        updateDisplay();
    }

    free(raytracer);
    exitGraphics();

    printf("Bye!\n");

    return (0);
}
