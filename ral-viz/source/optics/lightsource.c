#include "lightsource.h"

lightSourceT* createLightSource(void) {
    lightSourceT* light_source = calloc(1, sizeof(lightSourceT));

    return (light_source);
}

void freeLightSource(lightSourceT* light_source) {
    if (light_source->data)
        free(light_source->data);

    free(light_source);
}
