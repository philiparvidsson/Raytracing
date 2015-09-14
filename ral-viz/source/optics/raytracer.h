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
void addLightSource(struct raytracerT* raytracer, struct lightSourceT* light_source);
void addSurface(struct raytracerT* raytracer, struct surfaceT* surface);
struct intersectionT findIntersection(struct raytracerT* raytracer, struct rayT* ray, struct surfaceT* excluded_surface, float max_distance);
vec3 calcFinalColor(struct raytracerT* raytracer, struct intersectionT* intersection);
void raytraceAll(struct raytracerT* raytracer);
void raytraceLine(struct raytracerT* raytracer, int y);
void raytraceRect(raytracerT* raytracer, int x, int y, int w, int h);
#endif // raytracer_h_
