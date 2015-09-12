#include "common.h"
#include "graphics.h"

#include <stdio.h>

static void printIntroMessage(void) {
    printf("ral-viz early build\nnothing to see here yet...\n\n");
}

int main(int argc, char* argv[]) {
    printIntroMessage();

    initGraphics(1280, 720);

    while (windowIsOpen()) {
        updateDisplay();
    }

    exitGraphics();

    return (0);
}
