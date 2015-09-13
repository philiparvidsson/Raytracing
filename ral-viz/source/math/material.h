#ifndef material_h_
#define material_h_

#include "base/common.h"
#include "graphics/raytracer.h"
#include "math/ray.h"
#include "math/vector.h"

typedef vec3(*materialColorFn)(const struct raytracerT*, const struct intersectionT*);

typedef struct materialT {
    materialColorFn color_fn;

    void* data;
} materialT;

materialT* createMaterial();
void freeMaterial(materialT* material);

#endif // material_h_
