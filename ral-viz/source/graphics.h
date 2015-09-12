/*------------------------------------------------------------------------------
 * File: graphics.h
 * Created: September 12, 2015
 * Last changed: September 12, 2015
 *
 * Author(s): Philip Arvidsson (contact@philiparvidsson.com)
 *
 * Description:
 *   Simple graphics library for displaying memory bitmaps.
 *----------------------------------------------------------------------------*/

#ifndef graphics_h_
#define graphics_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "common.h"

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: initGraphics(title, width, height)
 *
 * Parameters:
 *   title   Window title.
 *   width   Width of the window client area, in pixels.
 *   height  Height of the window client area, in pixels.
 *
 * Description:
 *   Initializes the graphics library and creates a window of the specified
 *   size.
 *
 * Usage:
 *   bool r = initGraphics(1280, 720);
 *------------------------------------*/
bool initGraphics(const string* title, int width, int height);

/*--------------------------------------
 * Function: exitGraphics()
 *
 * Description:
 *   Exits graphics mode and closes the window.
 *
 * Usage:
 *   exitGraphics();
 *------------------------------------*/
void exitGraphics(void);

/*--------------------------------------
 * Function: updateDisplay()
 *
 * Description:
 *   Updates the window and its contents.
 *
 * Usage:
 *   updateDisplay();
 *------------------------------------*/
void updateDisplay(void);

/*--------------------------------------
 * Function: windowIsOpen()
 *
 * Description:
 *   Indicates whether the window has been closed by the user.
 *
 * Usage:
 *   bool open = windowIsOpen();
 *------------------------------------*/
bool windowIsOpen(void);

#endif // graphics_h_
