/*------------------------------------------------------------------------------
 * File: win32_private.h
 * Created: September 13, 2015
 * Last changed: September 13, 2015
 *
 * Author(s): Philip Arvidsson (contact@philiparvidsson.com)
 *
 * Description:
 *   Windows platform internals.
 *----------------------------------------------------------------------------*/

#ifndef win32_private_h_
#define win32_private_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "base/common.h"

#include <windows.h>

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: windowT
 *
 * Description:
 *   Represents a window.
 *------------------------------------*/
typedef struct windowT {
    int width,  // Window width, in pixels.
        height; // Window height, in pixels.

    // Below are platform specific fields..

    HWND hwnd; // Window handle.
    HDC  hdc;  // Device context.
} windowT;
/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: getWindowPtr()
 *
 * Returns:
 *   A pointer to the window.
 *
 * Description:
 *   Retrieves the window pointer.
 *
 * Usage:
 *   windowT* window = getWindowPtr();
 *------------------------------------*/
windowT* getWindowPtr(void);

#endif // win32_private_h_
