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

#ifdef _MSC_VER

// The inline keyword was introduced in C99 so MSVC++ doesn't support it.
// However, it supports the __inline keyword which exactly the same, so we can
// just alias it on MSVC++.
#define inline __inline

// The _Noreturn keyword was introduced in C11.
#define _Noreturn  __declspec(noreturn)

#endif // _MSC_VER

/*--------------------------------------
 * Macro: string
 *
 * Description:
 *   String type macro.
 *------------------------------------*/
#define string stringT

#define clamp(x, a, b) max(min(x, b), a)
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

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
