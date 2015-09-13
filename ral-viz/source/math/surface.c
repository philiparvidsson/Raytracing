#include "surface.h"

#include <stdlib.h>

surfaceT* createSurface() {
    surfaceT* surface = calloc(1, sizeof(surfaceT));

    return (surface);
}

void freeSurface(surfaceT* surface) {
    if (surface->data)
        free(surface->data);

    free(surface);
}
