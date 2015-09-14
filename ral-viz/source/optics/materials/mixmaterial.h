#ifndef mixmaterial_h_
#define mixmaterial_h_

#include "base/common.h"
#include "optics/material.h"

typedef struct mixMaterialT {
    float a;

    materialT* material1;
    materialT* material2;
} mixMaterialT;

#endif // mixmaterial_h_
