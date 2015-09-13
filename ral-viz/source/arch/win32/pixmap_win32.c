/*------------------------------------------------------------------------------
 * File: pixmap_win32.c
 * Created: September 13, 2015
 * Last changed: September 13, 2015
 *
 * Author(s): Philip Arvidsson (contact@philiparvidsson.com)
 *
 * Description:
 *   Windows implementation of the pixel map module.
 *----------------------------------------------------------------------------*/

#ifdef _WIN32

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "arch/win32/win32_private.h"
#include "base/common.h"
#include "base/debug.h"
#include "graphics/pixmap.h"

#include <windows.h>

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: pixmapT
 *------------------------------------*/
struct pixmapT {
    int width,       // Width, in pixels.
        height;      // Height, in pixels.

    HDC     hdc;     // Device context.
    HBITMAP hbmp;    // Bitmap handle.

    uint8_t* pixels; // Pixmap contents.
};

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: createPixmap(width, height)
 *------------------------------------*/
pixmapT* createPixmap(int width, int height) {
    BITMAPINFO bmi = { 0 };

    bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth       = width;
    bmi.bmiHeader.biHeight      = -height;
    bmi.bmiHeader.biPlanes      = 1;
    bmi.bmiHeader.biBitCount    = 24;
    bmi.bmiHeader.biCompression = BI_RGB;

    pixmapT* pixmap = malloc(sizeof(pixmapT));

    pixmap->width  = width;
    pixmap->height = height;
    pixmap->hdc    = CreateCompatibleDC(getWindowPtr()->hdc);
    pixmap->hbmp   = CreateDIBSection(pixmap->hdc, &bmi, DIB_RGB_COLORS, &pixmap->pixels, NULL, 0);

    assert(pixmap->hdc  != NULL);
    assert(pixmap->hbmp != NULL);

    SelectObject(pixmap->hdc, pixmap->hbmp);

    return (pixmap);
}

/*--------------------------------------
 * Function: freePixmap(pixmap)
 *------------------------------------*/
void freePixmap(pixmapT* pixmap) {
    DeleteObject(pixmap->hbmp);
    DeleteDC(pixmap->hdc);

    free(pixmap);
}

/*--------------------------------------
 * Function: getPixel(pixmap, x, y)
 *------------------------------------*/
pixelT getPixel(const pixmapT* pixmap, int x, int y) {
    int i = 3 * (x + y*pixmap->width);

    pixelT pixel;

    pixel.b = pixmap->pixels[i  ];
    pixel.g = pixmap->pixels[i+1];
    pixel.r = pixmap->pixels[i+2];

    return (pixel);
}

/*--------------------------------------
 * Function: setPixel(pixmap, x, y, r, g, b)
 *------------------------------------*/
void setPixel(pixmapT* pixmap, int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    int i = 3 * (x + y*pixmap->width);

    pixmap->pixels[i  ] = b;
    pixmap->pixels[i+1] = g;
    pixmap->pixels[i+2] = r;
}

/*--------------------------------------
 * Function: blitPixmap(pixmap, x, y)
 *------------------------------------*/
void blitPixmap(const pixmapT* pixmap, int x, int y) {
    if (!BitBlt(getWindowPtr()->hdc, x, y, pixmap->width, pixmap->height, pixmap->hdc, 0, 0, SRCCOPY))
        warn("BitBlt failed");
}

/*--------------------------------------
 * Function: pixmapWidth(pixmap)
 *------------------------------------*/
int pixmapWidth(const pixmapT* pixmap) {
    return (pixmap->width);
}

/*--------------------------------------
 * Function: pixmapHeight(pixmap)
 *------------------------------------*/
int pixmapHeight(const pixmapT* pixmap) {
    return (pixmap->height);
}

#endif // _WIN32
