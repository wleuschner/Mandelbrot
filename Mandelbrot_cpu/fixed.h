#ifndef __FIXED_H_
#define __FIXED_H_
#include<stdint.h>

#define BITS 55LL
#define PP 1LL<<BITS

typedef long long FIXED;
//typedef double FIXED;

FIXED fp_fromInt(long long a);
FIXED fp_fromFloat(double a);

FIXED fp_add(FIXED a,FIXED b);
FIXED fp_sub(FIXED a,FIXED b);
FIXED fp_mul(FIXED a,FIXED b);
FIXED fp_div(FIXED a,FIXED b);

float fp_toFloat(FIXED a);

#endif
