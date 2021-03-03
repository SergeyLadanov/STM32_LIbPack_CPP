/*
 * math8.h
 *
 *  Created on: 5 февр. 2021 г.
 *      Author: serge
 */

#ifndef LED_VISUAL_MATH8_H_
#define LED_VISUAL_MATH8_H_


#include <cstdint>

LIB8STATIC_ALWAYS_INLINE uint8_t qadd8( uint8_t i, uint8_t j)
{

    unsigned int t = i + j;
    if( t > 255) t = 255;
    return t;

}

LIB8STATIC_ALWAYS_INLINE int8_t avg7( int8_t i, int8_t j)
{
    return ((i + j) >> 1) + (i & 0x1);
}

LIB8STATIC_ALWAYS_INLINE uint8_t qsub8( uint8_t i, uint8_t j)
{
    int t = i - j;
    if( t < 0) t = 0;
    return t;
}

/// saturating 8x8 bit multiplication, with 8 bit result
/// @returns the product of i * j, capping at 0xFF
LIB8STATIC_ALWAYS_INLINE uint8_t qmul8( uint8_t i, uint8_t j)
{
    int p = ((int)i * (int)(j) );
    if( p > 255) p = 255;
    return p;
}

///         square root for 16-bit integers
///         About three times faster and five times smaller
///         than Arduino's general sqrt on AVR.
LIB8STATIC uint8_t sqrt16(uint16_t x)
{
    if( x <= 1) {
        return x;
    }

    uint8_t low = 1; // lower bound
    uint8_t hi, mid;

    if( x > 7904) {
        hi = 255;
    } else {
        hi = (x >> 5) + 8; // initial estimate for upper bound
    }

    do {
        mid = (low + hi) >> 1;
        if ((uint16_t)(mid * mid) > x) {
            hi = mid - 1;
        } else {
            if( mid == 255) {
                return 255;
            }
            low = mid + 1;
        }
    } while (hi >= low);

    return low - 1;
}



#endif /* LED_VISUAL_MATH8_H_ */
