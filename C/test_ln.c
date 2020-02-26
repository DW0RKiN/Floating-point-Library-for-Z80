
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

__uint16_t _sqrt(__uint16_t num16)
{
    double n = X_TO_DOUBLE(num16);
    
    double x = n, old = 0, old2 = 0;
    double a, b, xx = x*x;
    while ( 1 )
    {
        a = (n - xx) / (2*x);
        b = x + a;
        x = b - a*a/(2*b);
        xx = x*x;
//         printf("sqrt(%f) =  %e, chyba: %f, a: %e\n", n, x, xx-n, a);
//         if ( a >= 0 ) break;
        if ( x == old ) break;
        if ( x == old2 ) break;
        old2 = old;
        old = x;
    }
    num16 = DOUBLE_TO_X_OPT(x);
    return num16; 
}


__uint16_t test_ln(int exp, int mant)
{
    double e = log(2.0);
    e *= exp;
    
    double m = mant / (MASK_MANTISA+1.0);
    m = log(1.0+m);
    
    __uint16_t se = DOUBLE_TO_X_OPT(e);
    __uint16_t sm = DOUBLE_TO_X_OPT(m);
    
    if ( se == 0 ) return sm;
    if ( sm == 0 ) return se;
    e = X_TO_DOUBLE(se);    
    m = X_TO_DOUBLE(sm);
    
    se = DOUBLE_TO_X_OPT(e+m);
//     if ( exp == -1 ) se += 1;
    return se;
}




void print256_tab(int * tab, int idx, int posun, int stop)
{
    printf(";   _0  _1  _2  _3  _4  _5  _6  _7  _8  _9  _A  _B  _C  _D  _E  _F\n");

    for ( int i = 0; i < 256 && i < stop; i += 16 ) {
        printf("db ");
        for ( int j = i; j < i + 16 && j < stop; j++ ) {
            if (j != i ) putchar(',');
            printf("$%02x", (tab[idx+j] >> posun) & 0xFF);
        }
        printf("   ; %X_  ", (i >> 4) & 0xF );

        if ( ! posun ) {
            printf("   " );
            for ( int j = i; j < i + 16 && j < stop; j++ ) {
                if (j != i ) putchar(',');
                printf("%03x", (tab[idx+j]));
            }
            printf(" %X_", (i >> 4) & 0xF );
        }

        printf("\n");
    }
}



void print256word_tab(int * tab, int idx, int stop)
{
    printf(";   _0    _1    _2    _3    _4    _5    _6    _7    _8    _9    _A    _B    _C    _D    _E    _F\n");

    for ( int i = 0; i < 256 && i < stop; i += 16 ) {
        printf("dw ");
        for ( int j = i; j < i + 16 && j < stop; j++ ) {
            if (j != i ) putchar(',');
            printf("$%04x", (tab[idx+j]) & 0xFFFF);
        }
        printf("   ; %X_  ", (i >> 4) & 0xF );
        printf("\n");
    }
}



int main( int argc, const char* argv[] ) 
{

#ifdef TEST
    
    int rozdil_1 = 0, rozdil_2 = 0, rozdil_3 = 0;
    int chyb = 0;
    int sum = 0;
    
    for (int e = 0; e <= (MASK_EXP >> EXP_POS); e++ )
    for (int m = 0; m <= MASK_MANTISA; m++ ) {
             
        sum++;
#if 0
    #if TARGET == 0
        if ( e == 0x7D || e == 0x7E ) continue;
    #elif TARGET == 1
        if ( e == 0x3F ) continue;
    #else
        if ( e == (0x3A >> 2) || e == (0x3B >> 2) ) continue;
    #endif
#endif
        
        __uint16_t s = MAKE_X( 0,e-BIAS,m );
        double f = X_TO_DOUBLE( s );
        f = log(f);
        __uint16_t r = DOUBLE_TO_X_OPT(f);
        __uint16_t x = test_ln(e-BIAS, m);
 
        
        if ( x != r ) {
            
            __uint16_t xp1 = inc_x(x);
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
                printf("; exp($%04x) = $%04x, e: %i, m: $%04x, má být: $%04x, jest: $%04x, rozdíl: %i, opačná znaménka!\n", s, r, e-BIAS, m, r, x, dif);                
            }
            else if ( r == xp1 || r == xm1 ) rozdil_1++;
            else if ( r == xp2 || r == xm2 ) rozdil_2++;
            else if ( r == xp3 || r == xm3 ) {
                rozdil_3++;
                printf("; exp($%04x) = $%04x, e: %i, m: $%04x, má být: $%04x, jest: $%04x, rozdíl: %i\n", s, r, e-BIAS, m, r, x, dif);
            }
            else {
                chyb++;
                printf("; exp($%04x) = $%04x, e: %i, m: $%04x, má být: $%04x, jest: $%04x, rozdíl: %i\n", s, r, e-BIAS, m, r, x, dif);
            }
        }
    }

    printf("\n; sum: %i, rozdíl: má být mínus jest, nemohu se splést...\n", sum);
    printf("; nepřesnost o 1: %i (%.3f%%)\n", rozdil_1, (100.0 * rozdil_1 )/ sum);
    printf("; nepřesnost o 2: %i (%.3f%%)\n", rozdil_2, (100.0 * rozdil_2 )/ sum);
    printf("; nepřesnost o 3: %i (%.3f%%)\n", rozdil_3, (100.0 * rozdil_3 )/ sum);
    printf(";       chyb: %i (%.3f%%)\n", chyb, (100.0 * chyb )/ sum);
    
#else
    #if MAX_NUMBER == 127
        int tab[256];
        int i;

        for ( i = 0; i <= 255; i++ )
        {
            __uint16_t s;
            if ( i & MASK_SIGN ) 
                s = 0x7d80 + i;
            else 
                s = 0x7d00 + i;
            double f = X_TO_DOUBLE( s );
            f = log( f );
            s = DOUBLE_TO_X_OPT( f );
            tab[i] = s;
        }

        printf(";\n");
        print256word_tab(tab, 0, MAX_NUMBER+1 );
        printf(";\n");
        print256word_tab(tab, 128, MAX_NUMBER+1 );

    #elif MAX_NUMBER == 255

        int tab[256];
        int i;

        for ( i = 0; i <= 255; i++ )
        {
            __uint16_t s = 0x3f00 + i;
            double f = X_TO_DOUBLE( s );
            f = log( f );
            s = DOUBLE_TO_X_OPT( f );
            tab[i] = s;
        }

        printf("; lo\n");
        print256_tab(tab, 0, 0, MAX_NUMBER+1 );
        printf("; hi\n");
        print256_tab(tab, 0, 8, MAX_NUMBER+1 );
    
    #elif MAX_NUMBER == 1023
        int tab[512];
        int i;
    
        for ( i = 0; i <= 511; i++ )
        {
            __uint16_t s = 0x3A00 + i;
            double f = X_TO_DOUBLE( s );
            f = log( f );
            s = DOUBLE_TO_X_OPT( f );
            tab[i] = s;
        }

        for ( i = 0; i <= 256; i += 256 )
        {
            printf("; lo\n");
            print256_tab(tab, i, 0, 256 );
        }
        for ( i = 0; i <= 256; i += 256 )
        {
            printf("; hi\n");
            print256_tab(tab, i, 8, 256 );
        }
    #else
        #error   Neocekavana hodnota MAX_NUMBER!
    #endif

#endif
}
