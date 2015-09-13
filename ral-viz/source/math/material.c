#include "material.h"

materialT* createMaterial() {
    materialT* material = calloc(1, sizeof(materialT));

    return (material);
}

void freeMaterial(materialT* material) {
    if (material->data)
        free(material->data);

    free(material);
}
