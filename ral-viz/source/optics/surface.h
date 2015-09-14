#ifndef surface_h_
#define surface_h_

#include "base/common.h"
#include "optics/material.h"
#include "optics/ray.h"

typedef struct intersectionT (*intersectFnT)(struct rayT*, struct surfaceT*);

typedef struct surfaceT {
    intersectFnT intersect_fn;

    struct materialT* material;

    void* data;

    struct surfaceT* next;
} surfaceT;

surfaceT* createSurface();
void freeSurface();

#endif // surface_h_
