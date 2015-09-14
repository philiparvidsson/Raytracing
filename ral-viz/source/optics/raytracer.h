#ifndef raytracer_h_
#define raytracer_h_

#include "base/common.h"
#include "graphics/pixmap.h"
#include "optics/lightsource.h"
#include "optics/surface.h"

typedef struct raytracerT {
    pixmapT* pixmap;

    struct surfaceT* surfaces;
    struct lightT* light_sources;
} raytracerT;

struct raytracerT* createRaytracer(int width, int height);
void freeRaytracer(struct raytracerT* raytracer);
void addSurface(struct raytracerT* raytracer, struct surfaceT* surface);
void raytraceAll(struct raytracerT* raytracer);
void raytraceLine(struct raytracerT* raytracer, int y);

#endif // raytracer_h_
