#ifndef material_h_
#define material_h_

#include "base/common.h"
#include "math/vector.h"
#include "optics/ray.h"
#include "optics/raytracer.h"

typedef vec3(*materialColorFn)(const struct materialT*, const struct raytracerT*, const struct intersectionT*);

typedef struct materialT {
    materialColorFn color_fn;

    void* data;
} materialT;

materialT* createMaterial();
void freeMaterial(materialT* material);

#endif // material_h_
