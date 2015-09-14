#ifndef lightsource_h_
#define lightsource_h_

#include "base/common.h"
#include "math/vector.h"

typedef struct lightSourceT {
    void* data;

    struct lightSourceT* next;
} lightSourceT;

#endif // light_h_
