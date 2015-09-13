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

    initGraphics("ral-viz pre-alpha", 1280, 720);

    pixmapT* pixmap = createPixmap(1280, 720);

    int y = 0;
    while (windowIsOpen()) {

        if (y < 720) {
            for (int x = 0; x < 1280; x++) {
                vec3 e = (vec3) { 0.0f, 1.0f, 2.0f };
                vec3 s = (vec3) { 0.0f, 0.0f, 0.0f };

                float wx = (x - 639.5f) / 639.5f;
                float wy = (y - 359.5f) / 359.5f;

                vec3 r = (vec3) { wx, wy, -e.z };

                // plane: y = 0, t > 1.0
                // e.y + t*r.y = 0
                // t*r.y = -e.y
                // t = -e.y/r.y

                float p = -e.y / r.y;
                if (p >= 1.0f) {
                    setPixelf(pixmap, x, 719-y, 1.0f/p, 0.0f, 0.0f);
                }
                else {
                    setPixelf(pixmap, x, 719-y, 1.0f, 1.0f, 1.0f);
                }

                // sphere: x^2+y^2+z^2=r^2
                // (e.x+t*r.x)^2 + (e.y+t*r.y)^2 + (e.z+t*r.z)^2 = r^2
            }
        }

        y++;

        blitPixmap(pixmap, 0, 0);

        updateDisplay();
    }

    freePixmap(pixmap);
    exitGraphics();

    printf("Bye!\n");

    return (0);
}
