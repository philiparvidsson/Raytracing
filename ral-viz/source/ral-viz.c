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
    setPixelf(pixmap, 10, 10, 1.0f, 0.0f, 0.0f);

    int num = 0;
    while (windowIsOpen()) {
        int x = rand() % 1280;
        int y = rand() % 720;
        int r = rand() & 255;
        int g = rand() & 255;
        int b = rand() & 255;
        setPixel(pixmap, x, y, r, g, b);

        if ((num++ % 10000) == 0)
            blitPixmap(pixmap, 0, 0);
        updateDisplay();
    }

    freePixmap(pixmap);
    exitGraphics();

    printf("Bye!\n");

    return (0);
}
