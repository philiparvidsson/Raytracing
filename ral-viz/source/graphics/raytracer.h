#ifndef raytracer_h_
#define raytracer_h_

#include "base/common.h"
#include "graphics/pixmap.h"
#include "math/surface.h"

typedef struct raytracerT {
    pixmapT* pixmap;

    struct surfaceT* surfaces;
} raytracerT;

struct raytracerT* createRaytracer(int width, int height);
void freeRaytracer(struct raytracerT* raytracer);
void addSurface(struct raytracerT* raytracer, struct surfaceT* surface);
void raytraceAll(struct raytracerT* raytracer);

#endif // raytracer_h_
