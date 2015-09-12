/*------------------------------------------------------------------------------
 * File: graphics_win32.c
 * Created: September 12, 2015
 * Last changed: September 12, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Windows implementation of the graphics library.
 *----------------------------------------------------------------------------*/

#ifdef _WIN32

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "common.h"
#include "graphics.h"

#include <windows.h>

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

/*--------------------------------------
 * Constant: ClassName
 *
 * Description:
 *   Name of the class used to create the window.
 *------------------------------------*/
#define ClassName ("ral-viz__")

/*--------------------------------------
 * Constant: WindowStyle
 *
 * Description:
 *   Window styles.
 *------------------------------------*/
#define WindowStyle (~WS_MAXIMIZEBOX & WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME)

/*--------------------------------------
 * Constant: WindowStyleEx
 *
 * Description:
 *   Extended window styles.
 *------------------------------------*/
#define WindowStyleEx (WS_EX_LEFT)

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
    int           width,      // Window width, in pixels.
                  height;     // Window height, in pixels.

    // Below are platform specific system fields.

    HWND  hwnd;  // Window handle.
} windowT;

/*------------------------------------------------
 * GLOBALS
 *----------------------------------------------*/

/*--------------------------------------
 * Variable: window
 *
 * Description:
 *   Pointer to the window, if any.
 *------------------------------------*/
static windowT* window = NULL;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: WindowProc()
 *
 * Description:
 *   http://en.wikipedia.org/wiki/WindowProc
 *------------------------------------*/
static LRESULT CALLBACK WindowProc(_In_ HWND   hwnd,
                                   _In_ UINT   uMsg,
                                   _In_ WPARAM wParam,
                                   _In_ LPARAM lParam)
{
    switch (uMsg) {

    case WM_CLOSE: {
        window->hwnd = NULL;
        break;
    }

    };

    return (DefWindowProc(hwnd, uMsg, wParam, lParam));
}

/*--------------------------------------
 * Function: registerWindowClass()
 *------------------------------------*/
static void registerWindowClass(void) {
    WNDCLASSEXW wcx = { 0 };

    wcx.cbSize        = sizeof(WNDCLASSEXW);
    wcx.style         = CS_HREDRAW | CS_VREDRAW;
    wcx.lpfnWndProc   = WindowProc;
    wcx.cbClsExtra    = 0;
    wcx.cbWndExtra    = 0;
    wcx.hInstance     = GetModuleHandleW(NULL);
    wcx.hIcon         = LoadIconW(NULL, IDI_APPLICATION);
    wcx.hCursor       = LoadCursorW(NULL, IDC_ARROW);
    wcx.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wcx.lpszMenuName  = NULL;
    wcx.lpszClassName = ClassName;
    wcx.hIconSm       = NULL;

    assert(RegisterClassExW(&wcx));
}

/*--------------------------------------
 * Function: initGraphics(width, height)
 *------------------------------------*/
bool initGraphics(int width, int height) {
    return (false);
}

/*--------------------------------------
 * Function: exitGraphics()
 *------------------------------------*/
void exitGraphics(void) {

}

/*--------------------------------------
 * Function: updateDisplay()
 *------------------------------------*/
void updateDisplay(void) {

}

/*--------------------------------------
 * Function: windowIsOpen()
 *------------------------------------*/
bool windowIsOpen(void) {
    return (false);
}

#endif // _WIN32
