#ifndef raytracer_h_
#define raytracer_h_

#include "base/common.h"
#include "graphics/pixmap.h"
#include "math/surface.h"

typedef struct raytracerT {
    pixmapT* pixmap;

    surfaceT* surfaces;
} raytracerT;

raytracerT* createRaytracer(int width, int height);
void freeRaytracer(raytracerT* raytracer);
void addSurface(raytracerT* raytracer, surfaceT* surface);
void raytraceAll(raytracerT* raytracer);

#endif // raytracer_h_
