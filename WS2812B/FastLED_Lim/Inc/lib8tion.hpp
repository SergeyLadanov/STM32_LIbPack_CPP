/*
 * lib8tion.h
 *
 *  Created on: 5 февр. 2021 г.
 *      Author: serge
 */

#ifndef LED_VISUAL_LIB8TION_H_
#define LED_VISUAL_LIB8TION_H_


#include <cstdint>
#include <cstring>


#define LIB8STATIC __attribute__ ((unused)) static inline
#define LIB8STATIC_ALWAYS_INLINE __attribute__ ((always_inline)) static inline


/// ANSI unsigned short _Fract.  range is 0 to 0.99609375
///                 in steps of 0.00390625
typedef uint8_t   fract8;   ///< ANSI: unsigned short _Fract

///  ANSI: signed short _Fract.  range is -0.9921875 to 0.9921875
///                 in steps of 0.0078125
typedef int8_t    sfract7;  ///< ANSI: signed   short _Fract

///  ANSI: unsigned _Fract.  range is 0 to 0.99998474121
///                 in steps of 0.00001525878
typedef uint16_t  fract16;  ///< ANSI: unsigned       _Fract

///  ANSI: signed _Fract.  range is -0.99996948242 to 0.99996948242
///                 in steps of 0.00003051757
typedef int16_t   sfract15; ///< ANSI: signed         _Fract


// accumXY types should be interpreted as X bits of integer,
//         and Y bits of fraction.
//         E.g., accum88 has 8 bits of int, 8 bits of fraction

typedef uint16_t  accum88;  ///< ANSI: unsigned short _Accum.  8 bits int, 8 bits fraction
typedef int16_t   saccum78; ///< ANSI: signed   short _Accum.  7 bits int, 8 bits fraction
typedef uint32_t  accum1616;///< ANSI: signed         _Accum. 16 bits int, 16 bits fraction
typedef int32_t   saccum1516;///< ANSI: signed         _Accum. 15 bits int, 16 bits fraction
typedef uint16_t  accum124; ///< no direct ANSI counterpart. 12 bits int, 4 bits fraction
typedef int32_t   saccum114;///< no direct ANSI counterpart. 1 bit int, 14 bits fraction



LIB8STATIC_ALWAYS_INLINE uint8_t scale8( uint8_t i, fract8 scale)
{
    return (((uint16_t)i) * (1+(uint16_t)(scale))) >> 8;
}



LIB8STATIC uint8_t ease8InOutQuad( uint8_t i)
{
    uint8_t j = i;
    if( j & 0x80 ) {
        j = 255 - j;
    }
    uint8_t jj  = scale8(  j, j);
    uint8_t jj2 = jj << 1;
    if( i & 0x80 ) {
        jj2 = 255 - jj2;
    }
    return jj2;
}


///////////////////////////////////////////////////////////////////////
//
// linear interpolation, such as could be used for Perlin noise, etc.
//

// A note on the structure of the lerp functions:
// The cases for b>a and b<=a are handled separately for
// speed: without knowing the relative order of a and b,
// the value (a-b) might be overflow the width of a or b,
// and have to be promoted to a wider, slower type.
// To avoid that, we separate the two cases, and are able
// to do all the math in the same width as the arguments,
// which is much faster and smaller on AVR.

/// linear interpolation between two unsigned 8-bit values,
/// with 8-bit fraction
LIB8STATIC uint8_t lerp8by8( uint8_t a, uint8_t b, fract8 frac)
{
    uint8_t result;
    if( b > a) {
        uint8_t delta = b - a;
        uint8_t scaled = scale8( delta, frac);
        result = a + scaled;
    } else {
        uint8_t delta = a - b;
        uint8_t scaled = scale8( delta, frac);
        result = a - scaled;
    }
    return result;
}

#define memmove8 memmove
#define memcpy8 memcpy
#define memset8 memset



#endif /* LED_VISUAL_LIB8TION_H_ */
