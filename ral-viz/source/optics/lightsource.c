#include "lightsource.h"

lightSourceT* createLightSource(void) {
    lightSourceT* light_source = calloc(1, sizeof(lightSourceT));

    light_source->num_samples = 1;

    return (light_source);
}

void freeLightSource(lightSourceT* light_source) {
    if (light_source->data)
        free(light_source->data);

    free(light_source);
}
