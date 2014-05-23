//===-- lib/fixtfsi.c - Quad-precision -> integer conversion ------*- C -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements quad-precision to integer conversion for the
// compiler-rt library.  No range checking is performed; the behavior of this
// conversion is undefined for out of range values in the C standard.
//
//===----------------------------------------------------------------------===//

#define QUAD_PRECISION
#include "fp_lib.h"

#if defined(CRT_HAS_128BIT) && defined(CRT_LDBL_128BIT)
COMPILER_RT_ABI int __fixtfsi(fp_t a) {

    // Break a into sign, exponent, significand
    const rep_t aRep = toRep(a);
    const rep_t aAbs = aRep & absMask;
    const int sign = aRep & signBit ? -1 : 1;
    const int exponent = (aAbs >> significandBits) - exponentBias;
    const rep_t significand = (aAbs & significandMask) | implicitBit;

    // If 0 <= exponent < significandBits, right shift to get the result.
    if ((unsigned int)exponent < significandBits) {
        // value exceed INT_MAX, return INT_MAX
        if (exponent >= sizeof(int) * CHAR_BIT){
            return sign == 1 ? INT_MAX : INT_MIN;
        }
        else {
            return sign * (significand >> (significandBits - exponent));
        }
    }
    // If exponent is negative, the result is zero.
    else if (exponent < 0) {
        return 0;
    }
    // exponent > significandBits(must > 32)
    else {
        return sign == 1 ? INT_MAX : INT_MIN;
    }
}

#endif
