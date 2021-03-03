/*
 * noise.cpp
 *
 *  Created on: 5 февр. 2021 г.
 *      Author: serge
 */

#include "noise.hpp"

#include "lib8tion.hpp"
#include "math8.hpp"


#define FL_PROGMEM
#define FL_PGM_READ_BYTE_NEAR(x)  (*((const  uint8_t*)(x)))
#define FL_PGM_READ_WORD_NEAR(x)  (*((const uint16_t*)(x)))
#define FL_PGM_READ_DWORD_NEAR(x) (*((const uint32_t*)(x)))

#define P(x) FL_PGM_READ_BYTE_NEAR(p + x)

#define EASE8(x)  (ease8InOutQuad(x) )
#define EASE16(x) (ease16InOutQuad(x))



FL_PROGMEM static uint8_t const p[] = { 151,160,137,91,90,15,
   131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
   190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
   88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
   77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
   102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
   135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
   5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
   223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
   129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
   251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
   49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
   138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,151
   };



static int8_t inline __attribute__((always_inline)) lerp7by8( int8_t a, int8_t b, fract8 frac)
{
    // int8_t delta = b - a;
    // int16_t prod = (uint16_t)delta * (uint16_t)frac;
    // int8_t scaled = prod >> 8;
    // int8_t result = a + scaled;
    // return result;
    int8_t result;
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


static int8_t inline __attribute__((always_inline)) selectBasedOnHashBit(uint8_t hash, uint8_t bitnumber, int8_t a, int8_t b) {
	int8_t result;
	result = (hash & (1<<bitnumber)) ? a : b;
	return result;
}

static int8_t  inline __attribute__((always_inline)) grad8(uint8_t hash, int8_t x, int8_t y, int8_t z) {
#if 0
  switch(hash & 0xF) {
    case  0: return (( x) + ( y))>>1;
    case  1: return ((-x) + ( y))>>1;
    case  2: return (( x) + (-y))>>1;
    case  3: return ((-x) + (-y))>>1;
    case  4: return (( x) + ( z))>>1;
    case  5: return ((-x) + ( z))>>1;
    case  6: return (( x) + (-z))>>1;
    case  7: return ((-x) + (-z))>>1;
    case  8: return (( y) + ( z))>>1;
    case  9: return ((-y) + ( z))>>1;
    case 10: return (( y) + (-z))>>1;
    case 11: return ((-y) + (-z))>>1;
    case 12: return (( y) + ( x))>>1;
    case 13: return ((-y) + ( z))>>1;
    case 14: return (( y) + (-x))>>1;
    case 15: return ((-y) + (-z))>>1;
  }
#else

  hash &= 0xF;

  int8_t u, v;
  //u = (hash&8)?y:x;
  u = selectBasedOnHashBit( hash, 3, y, x);

#if 1
  v = hash<4?y:hash==12||hash==14?x:z;
#else
  // Verbose version for analysis; generates idenitical code.
  if( hash < 4) { // 00 01 02 03
	  v = y;
  } else {
      if( hash==12 || hash==14) { // 0C 0E
		  v = x;
	  } else {
		  v = z; // 04 05 06 07   08 09 0A 0B   0D  0F
	  }
  }
#endif

  if(hash&1) { u = -u; }
  if(hash&2) { v = -v; }

  return avg7(u,v);
#endif
}


int8_t inoise8_raw(uint16_t x, uint16_t y, uint16_t z)
{
  // Find the unit cube containing the point
  uint8_t X = x>>8;
  uint8_t Y = y>>8;
  uint8_t Z = z>>8;

  // Hash cube corner coordinates
  uint8_t A = P(X)+Y;
  uint8_t AA = P(A)+Z;
  uint8_t AB = P(A+1)+Z;
  uint8_t B = P(X+1)+Y;
  uint8_t BA = P(B) + Z;
  uint8_t BB = P(B+1)+Z;

  // Get the relative position of the point in the cube
  uint8_t u = x;
  uint8_t v = y;
  uint8_t w = z;

  // Get a signed version of the above for the grad function
  int8_t xx = ((uint8_t)(x)>>1) & 0x7F;
  int8_t yy = ((uint8_t)(y)>>1) & 0x7F;
  int8_t zz = ((uint8_t)(z)>>1) & 0x7F;
  uint8_t N = 0x80;

  u = EASE8(u); v = EASE8(v); w = EASE8(w);

  int8_t X1 = lerp7by8(grad8(P(AA), xx, yy, zz), grad8(P(BA), xx - N, yy, zz), u);
  int8_t X2 = lerp7by8(grad8(P(AB), xx, yy-N, zz), grad8(P(BB), xx - N, yy - N, zz), u);
  int8_t X3 = lerp7by8(grad8(P(AA+1), xx, yy, zz-N), grad8(P(BA+1), xx - N, yy, zz-N), u);
  int8_t X4 = lerp7by8(grad8(P(AB+1), xx, yy-N, zz-N), grad8(P(BB+1), xx - N, yy - N, zz - N), u);

  int8_t Y1 = lerp7by8(X1,X2,v);
  int8_t Y2 = lerp7by8(X3,X4,v);

  int8_t ans = lerp7by8(Y1,Y2,w);

  return ans;
}

uint8_t inoise8(uint16_t x, uint16_t y, uint16_t z) {
//  return scale8(76+(inoise8_raw(x,y,z)),215)<<1;
    int8_t n = inoise8_raw( x, y, z);  // -64..+64
    n+= 64;                            //   0..128
    uint8_t ans = qadd8( n, n);        //   0..255
    return ans;
}
