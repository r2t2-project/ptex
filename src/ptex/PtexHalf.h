#ifndef PtexHalf_h
#define PtexHalf_h

/* 
   CONFIDENTIAL INFORMATION: This software is the confidential and
   proprietary information of Walt Disney Animation Studios ("Disney").
   This software is owned by Disney and may not be used, disclosed,
   reproduced or distributed for any purpose without prior written
   authorization and license from Disney. Reproduction of any section of
   this software must include this legend and all copyright notices.
   (c) Disney. All rights reserved.
*/

/* PtexHalf - Brent Burley, Sep 2006

   half-precision (16-bit) floating point type (following IEEE 754r).
   This type should be compatible with opengl, openexr, and the like.
   The range is [-65504.0, 65504.0] and the precision is about 1 part
   in 2000 (3.3 decimal places).

   From OpenGL spec 2.1.2:
   
    A 16-bit floating-point number has a 1-bit sign (S), a 5-bit
    exponent (E), and a 10-bit mantissa (M).  The value of a 16-bit
    floating-point number is determined by the following:

        (-1)^S * 0.0,                        if E == 0 and M == 0,
        (-1)^S * 2^-14 * (M/2^10),           if E == 0 and M != 0,
        (-1)^S * 2^(E-15) * (1 + M/2^10),    if 0 < E < 31,
        (-1)^S * INF,                        if E == 31 and M == 0, or
        NaN,                                 if E == 31 and M != 0,
*/

#include <stdint.h>

struct PtexHalf {
    uint16_t bits;
    PtexHalf() {}
    PtexHalf(float val) : bits(fromFloat(val)) {}
    operator float() const { return toFloat(bits); }
    PtexHalf& operator=(float val) { bits = fromFloat(val); return *this; }

    static float toFloat(uint16_t h)
    {
	union { uint32_t i; float f; } u;
	u.i = h2fTable[h];
	return u.f;
    }

    static uint16_t fromFloat(float val)
    {
	if (val==0) return 0;
	union { uint32_t i; float f; } u;
	u.f = val;
	int e = f2hTable[(u.i>>23)&0x1ff];
	if (e) return e + (((u.i&0x7fffff) + 0x1000) >> 13);
	return fromFloat_except(u.i);
    }

 private:
    static uint16_t fromFloat_except(uint32_t val);
    /* internal */ public:
    static uint32_t h2fTable[65536];
    static uint16_t f2hTable[512];
};

#endif
