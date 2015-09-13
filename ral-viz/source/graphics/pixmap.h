/*------------------------------------------------------------------------------
 * File: pixmap.h
 * Created: September 13, 2015
 * Last changed: September 13, 2015
 *
 * Author(s): Philip Arvidsson (contact@philiparvidsson.com)
 *
 * Description:
 *   Provides a pixel map for drawing pixels.
 *----------------------------------------------------------------------------*/

#ifndef pixmap_h_
#define pixmap_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "base/common.h"

#include <stdint.h>
#include <stdlib.h>

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: pixelT
 *
 * Description:
 *   Represents a pixel in a pixel map.
 *------------------------------------*/
typedef struct pixelT {
    uint8_t r, g, b;
} pixelT;

/*--------------------------------------
 * Type: pixmapT
 *
 * Description:
 *   Represents a pixel map.
 *------------------------------------*/
typedef struct pixmapT pixmapT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: createPixmap(width, height)
 *
 * Parameters:
 *   width   Width, in pixels.
 *   height  Height, in pixels.
 *
 * Returns:
 *   A pixmap of the specified dimensions.
 *
 * Description:
 *   Creates a new 24-bit pixel map of the specified dimensions.
 *
 * Usage:
 *   pixmapT* pixmap = createPixmap(640, 480);
 *------------------------------------*/
pixmapT* createPixmap(int width, int height);

/*--------------------------------------
 * Function: freePixmap(pixmap)
 *
 * Parameters:
 *   pixmap  Pixmap to free.
 *
 * Description:
 *   Frees the specified pixmap from memory.
 *
 * Usage:
 *   freePixmap(my_pixmap);
 *------------------------------------*/
void freePixmap(pixmapT* pixmap);

/*--------------------------------------
 * Function: getPixel(pixmap, x, y)
 *
 * Parameters:
 *   pixmap  Pixmap to read a pixel from.
 *   x       X-coordinate of the pixel to read.
 *   y       Y-coordinate of the pixel to read.
 *
 * Returns:
 *   The color components of the specified pixel.
 *
 * Description:
 *   Reads the color of the pixel at the specified coordinates.
 *
 * Usage:
 *   pixelT pixel = getPixel(my_pixmap, 10, 10); // Get pixel at (10, 10).
 *------------------------------------*/
pixelT getPixel(const pixmapT* pixmap, int x, int y);

/*--------------------------------------
 * Function: setPixel(pixmap, x, y, r, g, b)
 *
 * Parameters:
 *   pixmap  Pixmap to read a pixel from.
 *   x       X-coordinate of the pixel to read.
 *   y       Y-coordinate of the pixel to read.
 *   r       Red color component (0..255).
 *   g       Green color component (0..255).
 *   b       Blue color component (0..255).
 *
 * Description:
 *   Writes the specified color to the specified coordinates in the pixmap.
 *
 * Usage:
 *   setPixel(my_pixmap, 10, 10, 255, 0, 0); // Set pixel at (10, 10) to red.
 *------------------------------------*/
void setPixel(pixmapT* pixmap, int x, int y, uint8_t r, uint8_t g, uint8_t b);

/*--------------------------------------
 * Function: setPixelf(pixmap, x, y, r, g, b)
 *
 * Parameters:
 *   pixmap  Pixmap to read a pixel from.
 *   x       X-coordinate of the pixel to read.
 *   y       Y-coordinate of the pixel to read.
 *   r       Red color component (0.0..1.0).
 *   g       Green color component (0.0..1.0).
 *   b       Blue color component (0.0..1.0).
 *
 * Description:
 *   Writes the specified color to the specified coordinates in the pixmap.
 *
 * Usage:
 *   setPixelf(my_pixmap, 10, 10, 1.0, 0, 0); // Set pixel at (10, 10) to red.
 *------------------------------------*/
static inline void setPixelf(pixmapT* pixmap, int x, int y,
                             float r, float g, float b)
{
    r = clamp(r, 0.0f, 1.0f);
    g = clamp(g, 0.0f, 1.0f);
    b = clamp(b, 0.0f, 1.0f);

    setPixel(pixmap, x, y, (uint8_t)(r*255), (uint8_t)(g*255), (uint8_t)(b*255));
}

/*--------------------------------------
 * Function: blitPixmap(pixmap, x, y)
 *
 * Parameters:
 *   pixmap  Pixmap to read a pixel from.
 *   x       X-coordinate of where to blit the pixmap.
 *   y       Y-coordinate of where to blit the pixmap.
 *
 * Description:
 *   Blits (draws) the pixmap to the window at the specified coordinates.
 *
 * Usage:
 *   blitPixmap(my_pixmap, 0, 0);
 *------------------------------------*/
void blitPixmap(const pixmapT* pixmap, int x, int y);

/*--------------------------------------
 * Function: pixmapWidth(pixmap)
 *
 * Parameters:
 *   pixmap  Pixmap to get width of.
 *
 * Returns:
 *   Width of the pixmap, in pixels.
 *
 * Description:
 *   Gets the width of a pixmap.
 *
 * Usage:
 *   int width = pixmapWidth(my_pixmap);
 *------------------------------------*/
int pixmapWidth(const pixmapT* pixmap);

/*--------------------------------------
 * Function: pixmapHeight(pixmap)
 *
 * Parameters:
 *   pixmap  Pixmap to get height of.
 *
 * Returns:
 *   Height of the pixmap, in pixels.
 *
 * Description:
 *   Gets the height of a pixmap.
 *
 * Usage:
 *   int height = pixmapHeight(my_pixmap);
 *------------------------------------*/
int pixmapHeight(const pixmapT* pixmap);

#endif // bitmap_h_
