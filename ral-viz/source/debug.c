/*------------------------------------------------------------------------------
 * File: debug.c
 * Created: September 12, 2015
 * Last changed: September 12, 2015
 *
 * Author(s): Philip Arvidsson (contact@philiparvidsson.com)
 *
 * Description:
 *   Macros and functions to make debugging easier.
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "common.h"
#include "debug.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#endif

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

#ifdef _WIN32
static void printLastErrorWin32(void) {
    int last_error = GetLastError();

    if (last_error == NO_ERROR)
        return;

    LPWSTR error_text = NULL;
    FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER
                   | FORMAT_MESSAGE_FROM_SYSTEM
                   | FORMAT_MESSAGE_IGNORE_INSERTS,
                   NULL,
                   last_error,
                   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                   (LPWSTR)&error_text,
                   0,
                   NULL);

    wprintf(L"GetLastError() reports %d: %ls\n", last_error, error_text);

    if (error_text)
        LocalFree(error_text);
}
#endif // _WIN32

static void setTextColor(const string* color) {
#ifdef _WIN32
    HANDLE std_out = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD   attr    = 0;

    if (!color)
        color = "";

    if (strcmp(color, "magenta") == 0) {
        attr = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    }
    else if (strcmp(color, "red") == 0) {
        attr = FOREGROUND_RED | FOREGROUND_INTENSITY;
    }
    else if (strcmp(color, "white") == 0) {
        attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
             | FOREGROUND_INTENSITY;
    }
    else if (strcmp(color, "yellow") == 0) {
        attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    }
    else {
        attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    }

    SetConsoleTextAttribute(std_out, attr);
#endif // _WIN32
}

/*--------------------------------------
 * Function: errorFunc(msg, func_name, line, ...)
 *------------------------------------*/
void errorFunc(const string* msg, const string* func_name, int line, ...) {
    va_list ap;

    string s[1024];
    va_start(ap, line);
    vsprintf(s, msg, ap);
    va_end(ap);

    printf("\n----------------------------------------\n");
    setTextColor("red");
    printf("error: %s\n\tin %s() on line %d.\n\n", s, func_name, line);
    setTextColor(NULL);
    printf("This program will now exit.\n");

#ifdef _WIN32
    printLastErrorWin32();
#endif // _WIN32

#if defined(_DEBUG) && defined(_MSC_VER)
    printf("Press ENTER to debug...");
    getchar();
    __debugbreak();
#else
    printf("Press ENTER to exit...");
    getchar();
#endif
    exit(EXIT_FAILURE);
}

void traceFunc(const string* msg, ...) {
    va_list ap;

    string s[1024];
    va_start(ap, msg);
    vsprintf(s, msg, ap);
    va_end(ap);

    setTextColor(NULL);
    printf("%s\n", s);
}

void warnFunc(const string* msg, ...) {
    va_list ap;

    string s[1024];
    va_start(ap, msg);
    vsprintf(s, msg, ap);
    va_end(ap);

    setTextColor("yellow");
    printf("warning: %s\n", s);
    setTextColor(NULL);
}
