/*------------------------------------------------------------------------------
 * File: common.h
 * Created: September 12, 2015
 * Last changed: September 12, 2015
 *
 * Author(s): Philip Arvidsson (contact@philiparvidsson.com)
 *
 * Description:
 *   Common includes, macros, functions etc.
 *----------------------------------------------------------------------------*/

#ifndef common_h_
#define common_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include <stdbool.h>
#include <wchar.h>

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

/*--------------------------------------
 * Constant: Authors
 *
 * Description:
 *   The program authors.
 *------------------------------------*/
#define Authors ("Philip Arvidsson (contact@philiparvidsson.com)")

/*------------------------------------------------
 * MACROS
 *----------------------------------------------*/

/*--------------------------------------
 * Macro: string
 *
 * Description:
 *   String type macro.
 *------------------------------------*/
#define string stringT

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: stringT
 *
 * Description:
 *   String type definition.
 *------------------------------------*/
typedef char stringT;

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

#endif // common_h_
