#ifndef reflectivematerial_h_
#define reflectivematerial_h_

#include "base/common.h"
#include "math/vector.h"
#include "optics/material.h"

typedef struct reflectiveMaterialT {
    float reflectiveness;
} reflectiveMaterialT;

materialT* createReflectiveMaterial(float reflectiveness);

#endif // reflectivematerial_h_
