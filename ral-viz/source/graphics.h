#ifndef graphics_h_
#define graphics_h_

#include "common.h"

bool initGraphics(int width, int height);
void exitGraphics(void);
void updateDisplay();

bool windowIsOpen(void);

#endif // graphics_h_
