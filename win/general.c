/* generalasm.c
 * This file contains routines to replace general.asm.
 *
 * This file Copyright 1991 Ken Shirriff.  It may be used according to the
 * fractint license conditions, blah blah blah.
 */

#include <limits.h>

#include "port.h"
#include "prototyp.h"

int overflow = 0;

//int boxx[2304], boxy[1024];
//int boxvalues[512];
//char tstack[4096];
//BYTE dacbox[256][3];
//BYTE olddacbox[256][3];

//extern int tabmode;

int cpu=0;		/* cpu type: 86, 186, 286, or 386 */
int fpu=0;		/* fpu type: 0, 87, 287, 387 */

SEGTYPE extraseg=0;		/* extra 64K segment (allocated by init) */

/*
;
;       32-bit integer multiply routine with an 'n'-bit shift.
;       Overflow condition returns 0x7fffh with overflow = 1;
;
;       long x, y, z, multiply();
;       int n;
;
;       z = multiply(x,y,n)
;
*/

/*
 * 32 bit integer multiply with n bit shift.
 * Note that we fake integer multiplication with floating point
 * multiplication.
 */
long multiply(long x, long y, int n)
{
    double retval = ((double)x) * ((double) y) / (1L << n);
    if (retval > LONG_MAX)
    {
        retval = LONG_MAX;
        overflow = 1;
    }
    if (retval < LONG_MIN)
    {
        retval = LONG_MIN;
        overflow = 1;
    }
    return (long)retval;
}

/*
;
;       32-bit integer divide routine with an 'n'-bit shift.
;       Overflow condition returns 0x7fffh with overflow = 1;
;
;       z = divide(x,y,n);       z = x / y;
*/
long divide(long x, long y, int n)
{
    double retval;
    if (y == 0)
    {
        if (x >= 0)
        {
            //  for x == 0, the result should be a NaN, but we have no way to
            //  represent it. We therefore return +Inf instead.
            retval = LONG_MAX;
            overflow = 1;
        }
        if (x < 0)
        {
            retval = LONG_MIN;
            overflow = 1;
        }
    }
    else
    {
        retval = ((double)x) * ((double) y) / (1L << n);
        if (retval > LONG_MAX)
        {
            retval = LONG_MAX;
            overflow = 1;
        }
        if (retval < LONG_MIN)
        {
            retval = LONG_MIN;
            overflow = 1;
        }
    }
    return (long)retval;
}

