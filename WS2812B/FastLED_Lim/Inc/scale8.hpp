/*
 * scale8.h
 *
 *  Created on: 5 февр. 2021 г.
 *      Author: serge
 */

#ifndef LED_VISUAL_SCALE8_H_
#define LED_VISUAL_SCALE8_H_


LIB8STATIC_ALWAYS_INLINE uint8_t scale8_video( uint8_t i, fract8 scale)
{
    uint8_t j = (((int)i * (int)scale) >> 8) + ((i&&scale)?1:0);
    // uint8_t nonzeroscale = (scale != 0) ? 1 : 0;
    // uint8_t j = (i == 0) ? 0 : (((int)i * (int)(scale) ) >> 8) + nonzeroscale;
    return j;
}


/// This version of scale8 does not clean up the R1 register on AVR
/// If you are doing several 'scale8's in a row, use this, and
/// then explicitly call cleanup_R1.
LIB8STATIC_ALWAYS_INLINE uint8_t scale8_LEAVING_R1_DIRTY( uint8_t i, fract8 scale)
{
    return (((uint16_t)i) * ((uint16_t)(scale)+1)) >> 8;
}

/// Clean up the r1 register after a series of *LEAVING_R1_DIRTY calls
LIB8STATIC_ALWAYS_INLINE void cleanup_R1()
{

}


/// scale three one byte values by a fourth one, which is treated as
///         the numerator of a fraction whose demominator is 256
///         In other words, it computes r,g,b * (scale / 256), ensuring
/// that non-zero values passed in remain non zero, no matter how low the scale
/// argument.
///
///         THIS FUNCTION ALWAYS MODIFIES ITS ARGUMENTS IN PLACE
LIB8STATIC void nscale8x3_video( uint8_t& r, uint8_t& g, uint8_t& b, fract8 scale)
{
    uint8_t nonzeroscale = (scale != 0) ? 1 : 0;
    r = (r == 0) ? 0 : (((int)r * (int)(scale) ) >> 8) + nonzeroscale;
    g = (g == 0) ? 0 : (((int)g * (int)(scale) ) >> 8) + nonzeroscale;
    b = (b == 0) ? 0 : (((int)b * (int)(scale) ) >> 8) + nonzeroscale;
}


/// scale three one byte values by a fourth one, which is treated as
///         the numerator of a fraction whose demominator is 256
///         In other words, it computes r,g,b * (scale / 256)
///
///         THIS FUNCTION ALWAYS MODIFIES ITS ARGUMENTS IN PLACE

LIB8STATIC void nscale8x3( uint8_t& r, uint8_t& g, uint8_t& b, fract8 scale)
{
    uint16_t scale_fixed = scale + 1;
    r = (((uint16_t)r) * scale_fixed) >> 8;
    g = (((uint16_t)g) * scale_fixed) >> 8;
    b = (((uint16_t)b) * scale_fixed) >> 8;
}


/// scale a 16-bit unsigned value by a 16-bit value,
///         considered as numerator of a fraction whose denominator
///         is 65536. In other words, it computes i * (scale / 65536)

LIB8STATIC uint16_t scale16( uint16_t i, fract16 scale )
{
    uint16_t result;
    result = ((uint32_t)(i) * (1+(uint32_t)(scale))) / 65536;
    result = result >> 16;
    return result;
}


/// linear interpolation between two unsigned 16-bit values,
/// with 16-bit fraction
LIB8STATIC uint16_t lerp16by16( uint16_t a, uint16_t b, fract16 frac)
{
    uint16_t result;
    if( b > a ) {
        uint16_t delta = b - a;
        uint16_t scaled = scale16(delta, frac);
        result = a + scaled;
    } else {
        uint16_t delta = a - b;
        uint16_t scaled = scale16( delta, frac);
        result = a - scaled;
    }
    return result;
}


/// This version of scale8_video does not clean up the R1 register on AVR
/// If you are doing several 'scale8_video's in a row, use this, and
/// then explicitly call cleanup_R1.
LIB8STATIC_ALWAYS_INLINE uint8_t scale8_video_LEAVING_R1_DIRTY( uint8_t i, fract8 scale)
{
    uint8_t j = (((int)i * (int)scale) >> 8) + ((i&&scale)?1:0);
    // uint8_t nonzeroscale = (scale != 0) ? 1 : 0;
    // uint8_t j = (i == 0) ? 0 : (((int)i * (int)(scale) ) >> 8) + nonzeroscale;
    return j;
}



#endif /* LED_VISUAL_SCALE8_H_ */
