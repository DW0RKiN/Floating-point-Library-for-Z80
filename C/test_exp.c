
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

const double lud = 2.7182818284590452353602875;

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

// ; e^((2^exp)*man) = e^( (2^exp) * (1.0+(man-1)) ) = e^(2^exp) * e^(man-1)
__uint16_t test_exp(int ie, int im)
{
    double e = pow(2,ie);    
    e = exp(e);
    
    __uint16_t se = DOUBLE_TO_X_OPT(e);
    
    if ( se == 0xFFFF - MASK_SIGN ) return se;
    
    double m = exp(pow(2,ie)*( im / (MASK_MANTISA+1.0)));
    __uint16_t sm = DOUBLE_TO_X_OPT(m);
    e = X_TO_DOUBLE(se);    
    m = X_TO_DOUBLE(sm);

    double r=e*m;
    return DOUBLE_TO_X_OPT(e*m);
}



// ; e^((2^exp)*man) = 
// ; e^( (2^exp) +   (2^exp-1) +   (2^exp-2) +   (2^exp-3) +   (2^exp-4) + ... ) = 
// ;   e^(2^exp) * e^(2^exp-1) * e^(2^exp-2) * e^(2^exp-3) * e^(2^exp-4) * ... 
__uint16_t test2_exp(int sign, int ie, int im)
{
    int mask     = MASK_MANTISA + 1;
    double temp;
    double res;

    if ( sign ) sign = -1; else sign = 1;
    res = pow(lud,sign*pow(2,ie));    // e^(2^exp))
// printf("e: %i ($%02x), $%04x\n", ie, ie+BIAS, DOUBLE_TO_X_OPT( res ));
    
    while ( im ) {

        
        ie--;
        mask >>= 1;
        if ( mask & im ) {
            __uint16_t r = DOUBLE_TO_X_OPT( res );
            res = X_TO_DOUBLE( r );
            r = DOUBLE_TO_X_OPT( pow(lud,sign*pow(2,ie)) );
// printf("e: %i ($%02x), * $%04x\n", ie, ie+BIAS, r );
            res *= X_TO_DOUBLE( r );
// printf("= $%04x\n",DOUBLE_TO_X_OPT( res ));

            im -= mask;
        }
        
    }
// printf("\n");
    return DOUBLE_TO_X_OPT(res);
}


__uint16_t test3_exp(int ie, int im)
{
    //2^x = 2^(i+f) = 2^i * 2^f , i is integer part , f is fractional part, with x=i+f
    //2^i is (1<<i), 
    //2^f use (1+x*len(2)/n)^n = 2^x with n=8 bellow:
    
    double p = 1.4426950408889634073599247*pow(2,ie)*(1.0+((double) im)/(MASK_MANTISA+1.0));
    
    long i = p;  //i is the integer part of p        
    p = (p-i) * 0.00270760617406228636491106297445+1; //LN2/256 = 0.00270760617406228636491106297445 , (p-i) is the fractional part
    p *=p;
    p *=p;
    p *=p;
    p *=p;
    p *=p;
    p *=p;
    p *=p;
    p =  p*p*((long) 1<<i);
    return  DOUBLE_TO_X_OPT( p );
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

    for (int sign = 0; sign <= 1; sign++ ) 
    for (int e = 0; e <= (MASK_EXP >> EXP_POS); e++ ) 
    for (int m = 0; m <= MASK_MANTISA; m++ ) {
        
        sum++;
        
        __uint16_t s = MAKE_X( sign,e-BIAS,m );
        double in = X_TO_DOUBLE( s );
        double f = exp(in);
        __uint16_t r = DOUBLE_TO_X_OPT(f);

        __uint16_t x = test2_exp(sign,e-BIAS, m);

        
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

    printf("\n; rozdíl: má být mínus jest, nemohu se splést...\n");
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
            double f;
            if ( i < 0x70 ) f = pow(lud,pow(2,0x70 + i-BIAS));
            else f = pow(lud,-pow(2,-0x10 + i-BIAS));
            s = DOUBLE_TO_X_OPT( f );
            tab[i] = s;
        }

        printf("EXP_TAB:\n; lo plus,  exp-$70\n");
        print256_tab(tab, 0, 0, 0x70 );
        printf("; lo minus, exp-$70+$80\n");
        print256_tab(tab, 0x70, 0, 256 - 0x70);
        printf("; hi plus,  exp-$70\n");
        print256_tab(tab, 0, 8, 0x70 );
        printf("; hi minus, exp-$70+$80\n");        
        print256_tab(tab, 0x70, 8, 256 - 0x70);

    #elif MAX_NUMBER == 255

        int tab[256];
        int i;

        for ( i = 0; i <= 255; i++ )
        {
            __uint16_t s;
            double f;
            if ( i < 0x80 ) f = pow(lud,pow(2,i-BIAS));
            else f = pow(lud,-pow(2,i-0x80-BIAS));
            s = DOUBLE_TO_X_OPT( f );
            tab[i] = s;
        }

        printf("EXP_TAB:\n; plus\n");
        print256word_tab(tab, 0, 0x80 );
        printf("; minus\n");
        print256word_tab(tab, 0x80, 256 - 0x80);


    #elif MAX_NUMBER == 1023
        int tab[64];
        int i;

        for ( i = 0; i < 64; i++ )
        {
            __uint16_t s;
            double f;
            if ( i < 0x20 ) f = pow(lud,pow(2,i-15));
            else f = pow(lud,-pow(2,i-0x20-15));
            s = DOUBLE_TO_X_OPT( f );
            tab[i] = (s >> 8) + ((s+1) << 8);

        }

        printf("EXP_TAB:\n; plus, big endian (hi lo+1)\n");
        print256word_tab(tab, 0, 0x20 );
        printf("; minus, big endian (hi lo+1)\n");
        print256word_tab(tab, 0x20, 0x20);
    #else
        #error   Neocekavana hodnota MAX_NUMBER!
    #endif

#endif
}
