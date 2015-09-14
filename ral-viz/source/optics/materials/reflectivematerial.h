#ifndef reflectivematerial_h_
#define reflectivematerial_h_

#include "base/common.h"
#include "math/vector.h"
#include "optics/material.h"

typedef struct reflectiveMaterialT {
    float reflectiveness;
    int num_samples;
} reflectiveMaterialT;

materialT* createReflectiveMaterial(float reflectiveness, int num_samples);

#endif // reflectivematerial_h_
