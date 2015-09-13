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

#endif // common_h_
