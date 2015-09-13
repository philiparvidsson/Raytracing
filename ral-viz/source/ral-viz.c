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
#include "graphics/raytracer.h"
#include "math/ray.h"
#include "math/material.h"
#include "math/materials/ambient_material.h"
#include "math/materials/diffuse_material.h"
#include "math/surface.h"
#include "math/surfaces/plane_surface.h"
#include "math/vector.h"
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

    //addSurface(raytracer, createPlaneSurface());
    surfaceT* sphere = createSphereSurface();
    sphere->material = createDiffuseMaterial(0.1f, 0.1f, 0.1f, 0.9f, 0.9f, 0.9f);

    surfaceT* plane = createPlaneSurface();
    plane->material = createAmbientMaterial(1.0f, 0.0f, 0.0f);

    addSurface(raytracer, sphere);
    addSurface(raytracer, plane);

    while (windowIsOpen()) {
        raytraceAll(raytracer);
        blitPixmap(raytracer->pixmap, 0, 0);

        updateDisplay();
    }

    free(raytracer);
    exitGraphics();

    printf("Bye!\n");

    return (0);
}
