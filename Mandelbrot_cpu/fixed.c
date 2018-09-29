#include"fixed.h"

FIXED fp_fromInt(long long a)
{
    return a<<BITS;
    //return a;
}

FIXED fp_fromFloat(double a)
{
    return a*(1LL<<BITS);
    //return a;
}

FIXED fp_add(FIXED a,FIXED b)
{
    return a+b;
}

FIXED fp_sub(FIXED a,FIXED b)
{
    return a-b;
}

FIXED fp_mul(FIXED a,FIXED b)
{
    return (int64_t)((__int128_t)a*b >> BITS);
    //return a*b;
}

FIXED fp_div(FIXED a,FIXED b)
{
    return (a/(b>>BITS));
    //return a/b;
}

float fp_toFloat(FIXED a)
{
    float scale=1LL<<BITS;
    return a/scale;
}
