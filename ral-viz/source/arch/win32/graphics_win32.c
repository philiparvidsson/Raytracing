/*------------------------------------------------------------------------------
 * File: graphics_win32.c
 * Created: September 12, 2015
 * Last changed: September 12, 2015
 *
 * Author(s): Philip Arvidsson (contact@philiparvidsson.com)
 *
 * Description:
 *   Windows implementation of the graphics library.
 *----------------------------------------------------------------------------*/

#ifdef _WIN32

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "common.h"
#include "debug.h"
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
    int           width,  // Window width, in pixels.
                  height; // Window height, in pixels.

    // Below are platform specific fields..

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
    wcx.lpszClassName = wstrdup(ClassName);
    wcx.hIconSm       = NULL;

    if (!RegisterClassExW(&wcx))
        error("could not register window class");
}

/*--------------------------------------
 * Function: registerWindowClass()
 *------------------------------------*/
static void unregisterWindowClass(void) {
    if (!UnregisterClassW(ClassName, GetModuleHandleW(NULL)))
        warn("could not unregister window class");
}

static void createWindow(const string* title, int width, int height) {
    RECT window_rect = { 0 };

    window_rect.right  = width;
    window_rect.bottom = height;

    AdjustWindowRectEx(&window_rect, WindowStyle, FALSE, WindowStyleEx);

    int window_width  = window_rect.right  - window_rect.left;
    int window_height = window_rect.bottom - window_rect.top;

    registerWindowClass();

    window = malloc(sizeof(windowT));

    wchar_t *window_name = wstrdup(title);

    window->hwnd = CreateWindowExW(WindowStyleEx,
                                   ClassName,
                                   window_name,
                                   WindowStyle,
                                   CW_USEDEFAULT,
                                   CW_USEDEFAULT,
                                   window_width,
                                   window_height,
                                   HWND_DESKTOP,
                                   NULL,
                                   GetModuleHandleW(NULL),
                                   NULL);

    free(window_name);

    assert(window->hwnd != NULL);

    RECT desktop_rect;
    GetClientRect(GetDesktopWindow(), &desktop_rect);

    MoveWindow(window->hwnd,
               (desktop_rect.right  - desktop_rect.left - window_width ) / 2,
               (desktop_rect.bottom - desktop_rect.top  - window_height) / 2,
               window_width,
               window_height,
               FALSE);

    //window->hdc = GetDC(window->hwnd);

    //assert(window->hdc != NULL);

    window->width  = width;
    window->height = height;
}

/*--------------------------------------
 * Function: initGraphics(title, width, height)
 *------------------------------------*/
bool initGraphics(const string* title, int width, int height) {
    if (window)
        error("graphics already initialized");

    registerWindowClass();
    createWindow(title, width, height);

    return (false);
}

/*--------------------------------------
 * Function: exitGraphics()
 *------------------------------------*/
void exitGraphics(void) {
    if (!window)
        return;

    unregisterWindowClass();
}

/*--------------------------------------
 * Function: updateDisplay()
 *------------------------------------*/
void updateDisplay(void) {
    if (!window)
        error("graphics not initialized");

    MSG msg;
    while (window && PeekMessageW(&msg, window->hwnd, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}

/*--------------------------------------
 * Function: windowIsOpen()
 *------------------------------------*/
bool windowIsOpen(void) {
    return (window);
}

#endif // _WIN32
