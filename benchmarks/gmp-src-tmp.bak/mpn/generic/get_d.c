/* mpn_get_d -- limbs to double conversion.

   THE FUNCTIONS IN THIS FILE ARE FOR INTERNAL USE ONLY.  THEY'RE ALMOST
   CERTAIN TO BE SUBJECT TO INCOMPATIBLE CHANGES OR DISAPPEAR COMPLETELY IN
   FUTURE GNU MP RELEASES.

Copyright 2003, 2004, 2007, 2009, 2010 Free Software Foundation, Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library.  If not, see http://www.gnu.org/licenses/.  */

#include "gmp.h"
#include "gmp-impl.h"
#include "longlong.h"

#ifndef _GMP_IEEE_FLOATS
#define _GMP_IEEE_FLOATS 0
#endif

#if ! _GMP_IEEE_FLOATS
/* dummy definition, just to let dead code compile */
union ieee_double_extract {
  struct {
    int manh, manl, sig, exp;
  } s;
  double d;
};
#endif

/* To force use of the generic C code for testing, put
   "#define _GMP_IEEE_FLOATS 0" at this point.  */



/* In alpha gcc prior to 3.4, signed DI comparisons involving constants are
   rearranged from "x < n" to "x+(-n) < 0", which is of course hopelessly
   wrong if that addition overflows.

   The workaround here avoids this bug by ensuring n is not a literal
   constant.  Note that this is alpha specific.  The offending transformation
   is/was in alpha.c alpha_emit_conditional_branch() under "We want to use
   cmpcc/bcc".

   Bizarrely, it turns out this happens also with Cray cc on
   alphaev5-cray-unicosmk2.0.6.X, and has the same solution.  Don't know why
   or how.  */

#if HAVE_HOST_CPU_FAMILY_alpha				\
  && ((defined (__GNUC__) && ! __GMP_GNUC_PREREQ(3,4))	\
      || defined (_CRAY))
static volatile const long CONST_1024 = 1024;
static volatile const long CONST_NEG_1023 = -1023;
static volatile const long CONST_NEG_1022_SUB_53 = -1022 - 53;
#else
#define CONST_1024	      (1024)
#define CONST_NEG_1023	      (-1023)
#define CONST_NEG_1022_SUB_53 (-1022 - 53)
#endif



/* Return the value {ptr,size}*2^exp, and negative if sign<0.
   Must have size>=1, and a non-zero high limb ptr[size-1].

   {ptr,size} is truncated towards zero.  This is consistent with other gmp
   conversions, like mpz_set_f or mpz_set_q, and is easy to implement and
   test.

   In the past conversions had attempted (imperfectly) to let the hardware
   float rounding mode take effect, but that gets tricky since multiple
   roundings need to be avoided, or taken into account, and denorms mean the
   effective precision of the mantissa is not constant.  (For reference,
   mpz_get_d on IEEE systems was ok, except it operated on the absolute
   value.  mpf_get_d and mpq_get_d suffered from multiple roundings and from
   not always using enough bits to get the rounding right.)

   It's felt that GMP is not primarily concerned with hardware floats, and
   really isn't enhanced by getting involved with hardware rounding modes
   (which could even be some weird unknown style), so something unambiguous
   and straightforward is best.


   The IEEE code below is the usual case, it knows either a 32-bit or 64-bit
   limb and is done with shifts and masks.  The 64-bit case in particular
   should come out nice and compact.

   The generic code works one bit at a time, which will be quite slow, but
   should support any binary-based "double" and be safe against any rounding
   mode.  Note in particular it works on IEEE systems too.


   Traps:

   Hardware traps for overflow to infinity, underflow to zero, or
   unsupported denorms may or may not be taken.  The IEEE code works bitwise
   and so probably won't trigger them, the generic code works by float
   operations and so probably will.  This difference might be thought less
   than ideal, but again its felt straightforward code is better than trying
   to get intimate with hardware exceptions (of perhaps unknown nature).


   Not done:

   mpz_get_d in the past handled size==1 with a cast limb->double.  This
   might still be worthwhile there (for up to the mantissa many bits), but
   for mpn_get_d here, the cost of applying "exp" to the resulting exponent
   would probably use up any benefit a cast may have over bit twiddling.
   Also, if the exponent is pushed into denorm range then bit twiddling is
   the only option, to ensure the desired truncation is obtained.


   Other:

   For reference, note that HPPA 8000, 8200, 8500 and 8600 trap FCNV,UDW,DBL
   to the kernel for values >= 2^63.  This makes it slow, and worse the kernel
   Linux (what versions?) apparently uses untested code in its trap handling
   routines, and gets the sign wrong.  We don't use such a limb-to-double
   cast, neither in the IEEE or generic code.  */


double
mpn_get_d (mp_srcptr up, mp_size_t size, mp_size_t sign, long exp)
{
  int lshift, nbits;
  union ieee_double_extract u;
  mp_limb_t x, mhi, mlo;

  ASSERT (size >= 0);
  ASSERT_MPN (up, size);
  ASSERT (size == 0 || up[size-1] != 0);

  if (size == 0)
    return 0.0;

  /* Adjust exp to a radix point just above {up,size}, guarding against
     overflow.  After this exp can of course be reduced to anywhere within
     the {up,size} region without underflow.  */
  if (UNLIKELY ((unsigned long) (GMP_NUMB_BITS * size)
		> (unsigned long) (LONG_MAX - exp)))
    {
      if (_GMP_IEEE_FLOATS)
	goto ieee_infinity;

      /* generic */
      exp = LONG_MAX;
    }
  else
    {
      exp += GMP_NUMB_BITS * size;
    }

  up += size;

#if ! _GMP_IEEE_FLOATS
you lose
#endif

#if GMP_LIMB_BITS == 64
  mlo = up[-1];
  count_leading_zeros (lshift, mlo);

  exp -= (lshift - GMP_NAIL_BITS) + 1;
  mlo <<= lshift;

  nbits = GMP_LIMB_BITS - lshift;

  if (nbits < 53 && size > 1)
    {
      x = up[-2];
      x <<= GMP_NAIL_BITS;
      x >>= nbits;
      mlo |= x;
      nbits += GMP_NUMB_BITS;

      if (LIMBS_PER_DOUBLE >= 3 && nbits < 53 && size > 2)
	{
	  x = up[-3];
	  x <<= GMP_NAIL_BITS;
	  x >>= nbits;
	  mlo |= x;
	  nbits += GMP_NUMB_BITS;
	}
    }
  mhi = mlo >> (32 + 11);
  mlo = mlo >> 11;		/* later implicitly truncated to 32 bits */
#endif
#if GMP_LIMB_BITS == 32
  x = *--up;
  count_leading_zeros (lshift, x);

  exp -= (lshift - GMP_NAIL_BITS) + 1;
  x <<= lshift;
  mhi = x >> 11;

  if (lshift < 11)		/* FIXME: never true if NUMB < 20 bits */
    {
      /* All 20 bits in mhi */
      mlo = x << 21;
      /* >= 1 bit in mlo */
      nbits = GMP_LIMB_BITS - lshift - 21;
    }
  else
    {
      if (size > 1)
	{
	  nbits = GMP_LIMB_BITS - lshift;

	  x = *--up, size--;
	  x <<= GMP_NAIL_BITS;
	  mhi |= x >> nbits >> 11;

	  mlo = x << GMP_LIMB_BITS - nbits - 11;
	  nbits = nbits + 11 - GMP_NAIL_BITS;
	}
      else
	{
	  mlo = 0;
	  goto done;
	}
    }

  /* Now all needed bits in mhi have been accumulated.  Add bits to mlo.  */

  if (LIMBS_PER_DOUBLE >= 2 && nbits < 32 && size > 1)
    {
      x = up[-1];
      x <<= GMP_NAIL_BITS;
      x >>= nbits;
      mlo |= x;
      nbits += GMP_NUMB_BITS;

      if (LIMBS_PER_DOUBLE >= 3 && nbits < 32 && size > 2)
	{
	  x = up[-2];
	  x <<= GMP_NAIL_BITS;
	  x >>= nbits;
	  mlo |= x;
	  nbits += GMP_NUMB_BITS;

	  if (LIMBS_PER_DOUBLE >= 4 && nbits < 32 && size > 3)
	    {
	      x = up[-3];
	      x <<= GMP_NAIL_BITS;
	      x >>= nbits;
	      mlo |= x;
	      nbits += GMP_NUMB_BITS;
	    }
	}
    }

 done:;

#endif
  if (UNLIKELY (exp >= CONST_1024))
    {
      /* overflow, return infinity */
    ieee_infinity:
      mhi = 0;
      mlo = 0;
      exp = 1024;
    }
  else if (UNLIKELY (exp <= CONST_NEG_1023))
    {
      int rshift;

      if (LIKELY (exp <= CONST_NEG_1022_SUB_53))
	return 0.0;	 /* denorm underflows to zero */

      rshift = -1022 - exp;
      ASSERT (rshift > 0 && rshift < 53);
#if GMP_LIMB_BITS > 53
      mlo >>= rshift;
      mhi = mlo >> 32;
#else
      if (rshift >= 32)
	{
	  mlo = mhi;
	  mhi = 0;
	  rshift -= 32;
	}
      lshift = GMP_LIMB_BITS - rshift;
      mlo = (mlo >> rshift) | (rshift == 0 ? 0 : mhi << lshift);
      mhi >>= rshift;
#endif
      exp = -1023;
    }
  u.s.manh = mhi;
  u.s.manl = mlo;
  u.s.exp = exp + 1023;
  u.s.sig = (sign < 0);
  return u.d;
}
