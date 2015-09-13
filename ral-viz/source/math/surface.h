#ifndef surface_h_
#define surface_h_

#include "base/common.h"
#include "math/material.h"
#include "math/ray.h"

typedef struct intersectionT(*intersectFnT)(struct rayT*, struct surfaceT*);

typedef struct surfaceT {
    intersectFnT intersect_fn;

    struct materialT* material;

    void* data;

    struct surfaceT* next;
} surfaceT;

surfaceT* createSurface();
void freeSurface();

#endif // surface_h_
