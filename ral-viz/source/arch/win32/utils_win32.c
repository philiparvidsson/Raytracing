/*------------------------------------------------------------------------------
 * File: utils_win32.c
 * Created: September 13, 2015
 * Last changed: September 13, 2015
 *
 * Author(s): Philip Arvidsson (contact@philiparvidsson.com)
 *
 * Description:
 *   Windows specific utility functions.
 *----------------------------------------------------------------------------*/

#ifdef _WIN32

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "utils_win32.h"

#include "base/common.h"

#include <stdlib.h>
#include <wchar.h>

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: wstrdup(str)
 *------------------------------------*/
wchar_t* wstrdup(const string* str) {
    size_t   len  = mbstowcs(NULL, str, 0)+1;
    wchar_t* wstr = malloc(sizeof(wchar_t) * len);

    mbstowcs(wstr, str, len);

    return (wstr);
}

#endif // _WIN32
