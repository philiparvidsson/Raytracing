/*------------------------------------------------------------------------------
 * File: common.c
 * Created: September 12, 2015
 * Last changed: September 12, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Common includes, macros, functions etc.
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "common.h"

#include <stdlib.h>

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: wstrdup(str)
 *
 * Parameters:
 *   str  String to create a wide duplicate of.
 *
 * Description:
 *   Creates a multi-byte string duplicate of the specified string. Don't forget
 *   to call free() on the returned string when you are done with it!
 *
 * Usage:
 *   wchar_t* wstr = wstrdup(str);
 *   ...
 *   free(wstr);
 *------------------------------------*/
wchar_t* wstrdup(const string* str) {
    size_t   len  = mbstowcs(NULL, str, 0)+1;
    wchar_t* wstr = malloc(sizeof(wchar_t) * len);

    mbstowcs(wstr, str, len);

    return (wstr);
}
