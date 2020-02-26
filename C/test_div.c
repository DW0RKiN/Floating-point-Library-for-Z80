
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// gcc -DTARGET=0
#ifndef TARGET
    #define TARGET 0
#endif

#if TARGET == 0
    #warning Target: bfloat
    #define MAX_NUMBER 127
#elif TARGET == 1
    #warning Target: danagy
    #define MAX_NUMBER 255
#elif TARGET == 2
    #warning Target: binary16
    #define MAX_NUMBER 1023
#else
    #error Byla nalezena neocekavana hodnota v promenne TARGET!
#endif

#include "../C/float.h"


__uint16_t inc_x(__uint16_t num) {
    __uint16_t sign = num & MASK_SIGN;
    num++;
    if ( sign != ( num & MASK_SIGN )) num += MASK_SIGN;
    return num;
}


__uint16_t dec_x(__uint16_t num){
    __uint16_t sign = num & MASK_SIGN;
    num--;
    if ( sign != ( num & MASK_SIGN )) num -= MASK_SIGN;
    return num;
}


__uint16_t test_div(__uint16_t a, __uint16_t b)
{    
    double db = 1/X_TO_DOUBLE(b);

    while ( (DOUBLE_TO_X_OPT(db) & (MASK_EXP + MASK_MANTISA)) == 0 )
    {
        if ( (a & MASK_EXP) == 0 ) {
            return (( a ^ b ) & MASK_SIGN);
        }
        b -= 1 << EXP_POS;
        a -= 1 << EXP_POS;
        db = 1/X_TO_DOUBLE(b);
    }

    double da = X_TO_DOUBLE(a);

    b = DOUBLE_TO_X_OPT(db);
    db = X_TO_DOUBLE(b);
    
    return DOUBLE_TO_X_OPT(da*db);
}

int main( int argc, const char* argv[] ) 
{

    
    long rozdil_1 = 0, rozdil_2 = 0, rozdil_3 = 0;
    long chyb = 0;
    long sum = 0;
    
    for (int d = 0; d <= 0xffff; d+=256 ) {
        printf("%i\n",d >> 8);
    for (int c = 0; c <= 0xff; c++ ) {
        int a = d+c;
    for (int b = 0; b <= 0xffff; b++ ) {
             
        sum++;
        __uint16_t r = DOUBLE_TO_X_OPT(X_TO_DOUBLE(a) / X_TO_DOUBLE(b));
        __uint16_t x = test_div(a, b);
        
        __uint16_t xp1 = inc_x(x);

        if ( x != r ) {
            
            __uint16_t xm1 = dec_x(x);
            __uint16_t xp2 = inc_x(xp1);
            __uint16_t xm2 = dec_x(xm1);
            __uint16_t xp3 = inc_x(xp2);
            __uint16_t xm3 = dec_x(xm2);

            int dif = (r & (MASK_MANTISA + MASK_EXP)) - (x & (MASK_MANTISA + MASK_EXP)); 
            if (dif >  MASK_SIGN) dif -= MASK_SIGN;
            if (dif < -MASK_SIGN) dif += MASK_SIGN;
                        
            if (( r & MASK_SIGN ) != ( x & MASK_SIGN)) {
                chyb++;
                printf("; $%04x / $%04x = $%04x <> $%04x, rozdíl: %i\n", a, b, r, x, dif);                
            }
            else if ( r == xp1 || r == xm1 ) rozdil_1++;
            else if ( r == xp2 || r == xm2 ) rozdil_2++;
            else if ( r == xp3 || r == xm3 ) {
                rozdil_3++;
                printf("; $%04x / $%04x = $%04x <> $%04x, rozdíl: %i\n", a, b, r, x, dif);                
            }
            else {
                chyb++;
                printf("; $%04x / $%04x = $%04x <> $%04x, rozdíl: %i\n", a, b, r, x, dif);                
            }
        }
    }}}

    printf("\n; sum: %li, rozdíl: má být mínus jest, nemohu se splést...\n", sum);
    printf("; nepřesnost o 1: %li (%.3f%%)\n", rozdil_1, (100.0 * rozdil_1 )/ sum);
    printf("; nepřesnost o 2: %li (%.3f%%)\n", rozdil_2, (100.0 * rozdil_2 )/ sum);
    printf("; nepřesnost o 3: %li (%.3f%%)\n", rozdil_3, (100.0 * rozdil_3 )/ sum);
    printf(";       chyb: %li (%.3f%%)\n", chyb, (100.0 * chyb )/ sum);
    
}
