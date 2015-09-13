/*------------------------------------------------------------------------------
 * File: utils_win32.h
 * Created: September 13, 2015
 * Last changed: September 13, 2015
 *
 * Author(s): Philip Arvidsson (contact@philiparvidsson.com)
 *
 * Description:
 *   Windows specific utility functions.
 *----------------------------------------------------------------------------*/

#ifndef win32util_h_
#define win32util_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "base/common.h"

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
wchar_t* wstrdup(const string* str);

#endif // win32util_h_
