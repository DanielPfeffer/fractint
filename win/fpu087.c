/* Fpu087.c
 * This file contains routines to replace fpu087.asm.
 *
 * This file Copyright 1991 Ken Shirriff.  It may be used according to the
 * fractint license conditions, blah blah blah.
 */

#include <math.h>

#include "port.h"
#include "prototyp.h"

#if defined(__BORLANDC__)
#include <float.h>

static int isnan(LDBL x)
{
    return _isnanl(x);
}

static int isinf(LDBL x)
{
    return !isnan(x) && !_finitel(x);
}

static int islessequal(LDBL x, LDBL y)
{
    return x <= y;
}
#endif



/*
 *----------------------------------------------------------------------
 *
 * xxx086 --
 *
 *	Simulate integer math routines using floating point.
 *	This will of course slow things down, so this should all be
 *	changed at some point.
 *
 *----------------------------------------------------------------------
 */

void FPUcplxmul(_CMPLX *x, _CMPLX *y, _CMPLX *z)
{
    LDBL tx, ty;
    tx = (LDBL)x->x * (LDBL)y->x - (LDBL)x->y * (LDBL)y->y;
    ty = (LDBL)x->x * (LDBL)y->y + (LDBL)x->y * (LDBL)y->x;
    if (isnan(ty) || isinf(ty))
      z->y = HUGE_VAL;
    else
      z->y = (LDBL)ty;
    if (isnan(tx) || isinf(tx))
      z->x = HUGE_VAL;
    else
      z->x = (LDBL)tx;
}

void FPUcplxdiv(_CMPLX *x, _CMPLX *y, _CMPLX *z)
{
    LDBL mod,tx,yxmod,yymod, yx, yy;
    yx = y->x;
    yy = y->y;
    mod = yx * yx + yy * yy;
    if (mod == 0.0 || fabsl(mod) <= LDBL_MIN) {
      z->x = HUGE_VAL;
      z->y = HUGE_VAL;
      overflow = 1;
    }
    else {
        yxmod = yx/mod;
        yymod = - yy/mod;
        tx = x->x * yxmod - x->y * yymod;
        z->y = (LDBL)(x->x * yymod + x->y * yxmod);
        z->x = (LDBL)tx;
    }
}

void FPUsincos(LDBL *Angle, LDBL *Sin, LDBL *Cos)
{
  if (isnan(*Angle) || isinf(*Angle)){
    *Sin = 0.0;
    *Cos = 1.0;
  } else {
    *Sin = (LDBL)sinl(*Angle);
    *Cos = (LDBL)cosl(*Angle);
  }
}

void FPUsinhcosh(LDBL *Angle, LDBL *Sinh, LDBL *Cosh)
{
  if (isnan(*Angle) || isinf(*Angle)){
    *Sinh = 1.0;
    *Cosh = 1.0;
  } else {
    *Sinh = (LDBL)sinhl(*Angle);
    *Cosh = (LDBL)coshl(*Angle);
  }
  if (isnan(*Sinh) || isinf(*Sinh))
    *Sinh = 1.0;
  if (isnan(*Cosh) || isinf(*Cosh))
    *Cosh = 1.0;
}

void FPUcplxlog(_CMPLX *x, _CMPLX *z)
{
    LDBL mod, xx, xy;
    xx = x->x;
    xy = x->y;
    if (xx == 0.0 && xy == 0.0) {
        z->x = z->y = 0.0;
        return;
        }
    mod = xx*xx + xy*xy;
    if (isnan(mod) || islessequal(mod,0) || isinf(mod))
      z->x = 0.5;
    else
      z->x = (LDBL)(0.5 * logl(mod));
    if (isnan(xx) || isnan(xy) || isinf(xx) || isinf(xy))
      z->y = 1.0;
    else
      z->y = (LDBL)atan2l(xy,xx);
}

void FPUcplxexp387(_CMPLX *x, _CMPLX *z)
{
    LDBL pwr,y;
    y = x->y;
    pwr = expl(x->x);
    if (isnan(pwr) || isinf(pwr))
      pwr = 1.0;
    z->x = (LDBL)(pwr*cosl(y));
    z->y = (LDBL)(pwr*sinl(y));
}

/* Integer Routines */
#define em2float(l) (*(float *)&(l))
#define float2em(f) (*(long *)&(f))

/*
 * Input is a 16 bit offset number.  Output is shifted by Fudge.
 */
unsigned long ExpFudged(x, Fudge)
long x;
int Fudge;
{
    return (long) (exp((double)x/(double)(1<<16))*(double)(1<<Fudge));
}

/* This multiplies two e/m numbers and returns an e/m number. */
long r16Mul(x,y)
long x,y;
{
    float f;
    f = em2float(x)*em2float(y);
    return float2em(f);
}

/* This takes an exp/mant number and returns a shift-16 number */
long LogFloat14(x)
unsigned long x;
{
    return log((double)em2float(x))*(1<<16);
}

/* This divides two e/m numbers and returns an e/m number. */
long RegDivFloat(x,y)
long x,y;
{
    float f;
    f = em2float(x)/em2float(y);
    return float2em(f);
}

/*
 * This routine on the IBM converts shifted integer x,FudgeFact to
 * the 4 byte number: exp,mant,mant,mant
 * Instead of using exp/mant format, we'll just use floats.
 * Note: If sizeof(float) != sizeof(long), we're hosed.
 */
long RegFg2Float(long x, int FudgeFact)
{
    float f;
    long l;
    f = x/(float)(1<<FudgeFact);
    l = float2em(f);
    return l;
}

/*
 * This converts em to shifted integer format.
 */
long RegFloat2Fg(long x, int Fudge)
{
    return em2float(x)*(float)(1<<Fudge);
}

long RegSftFloat(long x, int Shift)
{
    float f;
    f = em2float(x);
    if (Shift>0) {
	f *= (1 << Shift);
    } else {
	f /= (1 << -Shift);
    }
    return float2em(f);
}
