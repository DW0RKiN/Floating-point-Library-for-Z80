
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// gcc ./print_tab.c -DMUL -DSOURCE=\"orig_tab_mul7_0x80.h\" -lm

#ifndef SOURCE

// #define SOURCE "orig_tab_mul7_0x80.h"
#define SOURCE "orig_tab_mul11_2.h"
// #define SOURCE "orig_tab_mul8_3.h"
// #define SOURCE "orig_tab_mul7.h"
// #define SOURCE "mul7_16tab_1.h"

#endif

#include SOURCE

#include "float.h"

int const START_LIMIT = MAX_NUMBER+1;
int const NEJVYSSI = MAX_NUMBER+1;


#if MAX_NUMBER == 1023
    #define PRINT_FP_TO_X  fprint_fp_to_binary16
    #define FLOAT_TO_X     float_to_binary16
    #define FLOAT_TO_X_OPT float_to_binary16_opt
    #define X_TO_FLOAT     binary16_to_float

#elif MAX_NUMBER == 127
    #define PRINT_FP_TO_X  fprint_fp_to_bfloat16
    #define FLOAT_TO_X     float_to_bfloat16
    #define FLOAT_TO_X_OPT float_to_bfloat16_opt
    #define X_TO_FLOAT     bfloat16_to_float

#elif MAX_NUMBER == 255
    #define PRINT_FP_TO_X  fprint_fp_to_danagy16
    #define FLOAT_TO_X     float_to_danagy16
    #define FLOAT_TO_X_OPT float_to_danagy16_opt
    #define X_TO_FLOAT     danagy16_to_float

#else
    #error "Neocekavana hodnota MAX_NUMBER!"
#endif


float vynasob(int a, int b)
{
    float fa = make_float(0,0,a << (23-POCET_BITU));
    float fb = make_float(0,0,b << (23-POCET_BITU));
    float fc = fa*fb;

    return X_TO_FLOAT( FLOAT_TO_X_OPT(fc));
// float(single): Seee eeee emmm mmmm mmmm mmmm mmmm mmmm
//                           MMM MMMM MMM. .... .... ....
// binary16:                          Seee eeMM MMMM MMMM
// float(single): Seee eeee emmm mmmm mmmm mmmm mmmm mmmm
//                           MMM MMMM .... .... .... ....
// bfloat16:                          eeee eeee SMMM MMMM
// float(single): Seee eeee emmm mmmm mmmm mmmm mmmm mmmm
//                           MMM MMMM .... .... .... ....
// danagy16:                          Seee eeee MMMM MMMM

}





int nic = 0;
int pricti = 0;
int pricti_pretece = 0;

#define NEG 1
#define HALF_PRICTI 1

#if NEG == 1
    #if NEG_PRICTI == 1
        #define xNEG_COUNT(x) (2*TOP_BIT - tab_minus[x] + (PRICTI >> 1))
    #else
        #define xNEG_COUNT(x) (2*TOP_BIT - tab_minus[x])
    #endif
#else
    #if NEG_PRICTI == 1
        #define xNEG_COUNT(x) ((PRICTI >> 1) - tab_minus[x])
    #else
        #define xNEG_COUNT(x) (- tab_minus[x])
    #endif

#endif




float vypocitej(int a, int b)
{

    if ( a > MAX_NUMBER || b > MAX_NUMBER )
    {
        fprintf(stderr, "Chyba ve fci vypocitej()\n");
        exit(1);
    }

    int e = 0, r = a - b, m;
    if ( r < 0 ) r = -r;

#if 0
    m = tab_minus[r];
    m &= 2*TOP_BIT - 1;
    m += tab_plus[a+b];
    m &= 2*TOP_BIT - 1;

    if ( m != tab_plus[a+b] - tab_minus[r] + (PRICTI >> 1)) {
        fprintf(stderr, "Chyba no. 1 ve fci %s! %i != %i - %i = %i\n", __FUNCTION__, m, tab_plus[a+b], tab_minus[r], tab_plus[a+b] - tab_minus[r]);
        exit(13);
    }
    if ( m & TOP_BIT ) {

int old = m;
        m += PRICTI - ( PRICTI >> 1 );
        e++;
        pricti++;
        if (!(m & TOP_BIT)) {
//         m >>= 1;   // neni potreba protoze to jsou jen nuly a preteceny nejvyssi bit...
            e++;
            pricti_pretece++;
        }
// fprintf(stderr, "%X => %X, %X => %X\n", old, m, old >> POSUN_VPRAVO, m >> POSUN_VPRAVO);

    }
    else if ( m & (TOP_BIT >> 1)) {
        m <<= 1;
        nic++;
    }
    else {
        fprintf(stderr, "Chyba ve fci %s! m: %X nema TOP_BIT (%X) ani nizsi bit nastaveny na 1!\n", __FUNCTION__, m, TOP_BIT);
        exit(14);
    }
    m >>= POSUN_VPRAVO;
#else

#if NEG
    m = tab_plus[a+b] + tab_minus[r];

    if ( m > TOP_BIT + TOP_BIT - 1 ) {
        fprintf(stderr, "Chyba no. 1 ve fci %s! m($%X) > $%X!\n", __FUNCTION__, m, TOP_BIT + TOP_BIT - 1);
        fprintf(stderr, "a: %i, b: %i, TOP_BIT: $%X, plus[] = $%X, minus[] = $%X, POSUN_VPRAVO: %i\n", a, b, TOP_BIT, tab_plus[a+b], tab_minus[r], POSUN_VPRAVO);
        exit(13);
    }

    m &= TOP_BIT + TOP_BIT - 1;

#else
    m = tab_plus[a+b] - tab_minus[r];

    if ( m > TOP_BIT + TOP_BIT - 1 ) {
        fprintf(stderr, "Chyba no. 1 ve fci %s! m($%X) > $%X!\n", __FUNCTION__, m, TOP_BIT + TOP_BIT - 1);
        fprintf(stderr, "a: %i, b: %i, TOP_BIT: $%X, plus[] = $%X, minus[] = $%X, POSUN_VPRAVO: %i\n", a, b, TOP_BIT, tab_plus[a+b], tab_minus[r], POSUN_VPRAVO);
        exit(13);
    }

    m &= TOP_BIT + TOP_BIT - 1;
#endif

    if ( m & TOP_BIT ) {
        e++;
        pricti++;
#if HALF_PRICTI
        m += PRICTI - (PRICTI >> 1);
#else
        m += PRICTI;
#endif

        if (!(m & TOP_BIT)) {
//         m >>= 1;   // neni potreba protoze to jsou jen nuly a preteceny nejvyssi bit...
            e++;
            pricti_pretece++;
        }
    }
    else if ( m & (TOP_BIT >> 1)) {
#if HALF_PRICTI
        m <<= 1;
        nic++;
#else
        pricti++;
        m += PRICTI >> 1;
        m <<= 1;
        if (!(m & TOP_BIT)) {
//         m >>= 1;   // neni potreba protoze to jsou jen nuly a preteceny nejvyssi bit...
            e++;
            pricti_pretece++;
        }
#endif
    }
    else
    {
        fprintf(stderr, "Chyba no. 2 ve fci %s! m($%X) < $%X!\n", __FUNCTION__, m, TOP_BIT >> 1);
        fprintf(stderr, "a: %i, b: %i, TOP_BIT: $%X, plus[] = $%X, minus[] = $%X, POSUN_VPRAVO: %i\n", a, b, TOP_BIT, tab_plus[a+b], tab_minus[r], POSUN_VPRAVO);
        exit(15);
    }

    m >>= POSUN_VPRAVO;
#endif
    return make_float(0,e,(m << (FP_MANT_SIZE - POCET_BITU)) & FP_MASK_MANTISA);
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


int max_tab_minus = 0;

void init()
{
    int i;
#if NEG
    for ( i = 0; i <= MAX_NUMBER; i++ )
        if ( max_tab_minus < tab_minus[i]) max_tab_minus = tab_minus[i];

//     if ( max > 0x1000000 ) ;
//     else if ( max >> 16 )
//         max = 0x1000000;
//     else if ( max >> 8 )
//         max = 0x10000;
//     else
//         max = 0x100;

    printf("; tab_minus je zvyseno o $%X, a tab_plus zase snizeno o $%X\n", max_tab_minus, max_tab_minus);

    for ( i = 0; i <= MAX_NUMBER; i++ )
        tab_minus[i] = max_tab_minus - tab_minus[i];
#endif

#if NEG
        char c = '+';
#else
        char c = '-';
#endif
    
#if HALF_PRICTI
    printf("; (ApB)%c(AmB) >= $%X => pricti: $%X\n", c, TOP_BIT, PRICTI - (PRICTI >> 1));
    printf("; (ApB)%c(AmB) >= $%X => pricti: $%X\n", c, TOP_BIT/2, 0);
#else
    printf("; (ApB)%c(AmB) >= $%X => pricti: $%X\n", c, TOP_BIT, PRICTI);
    printf("; (ApB)%c(AmB) >= $%X => pricti: $%X\n", c, TOP_BIT/2, PRICTI);
#endif

    for ( i = 0; i <= MAX_NUMBER + MAX_NUMBER; i++ ) {
#if HALF_PRICTI
        tab_plus[i] += PRICTI >> 1;
#endif
#if NEG
        tab_plus[i] -= max_tab_minus;
#endif
    }
}




int main () {

#if MAX_NUMBER == 127
    #define STOP 128
#elif MAX_NUMBER >= 255
    #define STOP 256
#else
    #error "Neocekavana hodnota MAX_NUMBER!"
#endif


#if MAX_NUMBER > 255
    #if NEG
        #define FORMAT_TABM "_%i:\t; $%X - tab_minus[i]\n", j >> 8, max_tab_minus
    #else
        #define FORMAT_TABM "_%i:\n", j >> 8
    #endif
#else
    #if NEG
        #define FORMAT_TABM ":\t; $%X - tab_minus[i]\n", max_tab_minus
    #else
        #define FORMAT_TABM ":\n"
    #endif
#endif


#if MAX_NUMBER + MAX_NUMBER > 255
    #if NEG
        #define FORMAT_TABP "_%i:\t; tab_plus[i] - $%X\n", j >> 8, max_tab_minus
    #else
        #define FORMAT_TABP "_%i:\n", j >> 8
    #endif
#else
    #if NEG
        #define FORMAT_TABP ":\t; tab_plus[i] - $%X\n", max_tab_minus
    #else
        #define FORMAT_TABP ":\n"
    #endif
#endif


    int i,j;
    float odchylka, spoctene, vynasobene;

    init();

// 10+1 bits SEEE EEMM MMMM MMMM
// max 2047*2047 = 4 190 209 = 0x3FF001 = 0011 1111 1111 0000 0000 0001 => 00(1)1 1111 1111 0... .... .... 2**11 x (1)11 1111 1110
// min 1024*1024 = 1 048 576 = 0x100000 = 0001 0000 0000 0000 0000 0000 => 000(1) 0000 0000 00.. .... .... 2**10 x (1)00 0000 0000
//                                                         .. .... ....
// 7+1 bits  EEEE EEEE SMMM MMMM
// max 255*255   =    65 025 =   0xFE01 =           1111 1110 0000 0001 =>           (1)111 1110 .... .... 2**8  x (1)111 1110
// min 128*128   =    16 384 =   0x4000 =           0100 0000 0000 0000 =>           0(1)00 0000 0... .... 2**7  x (1)000 0000
//                                                             ... ....

// gcc -DMUL
#ifdef MUL
    j = 0;
    int n;

    while ( j < MAX_NUMBER + 1) {
        
#if MAX_NUMBER == 127
        int tab[256];
        for ( i = 0; i < 256; i++ ) {
            if ( i < 128 ) tab[i] = tab_minus[i];
            else if ( i == 128 )
                tab[128] = 0;
            else
                tab[i] = tab_minus[256-i];
        }
        printf("\nTab_AmB_lo" FORMAT_TABM);
        print256_tab(tab,j,0,256);
        printf( ";Tab_AmB_hi" FORMAT_TABM);
        print256_tab(tab,j,8,256);
#else
        printf("\nTab_AmB_lo" FORMAT_TABM);
        print256_tab(tab_minus,j,0,STOP);        
        printf( ";Tab_AmB_hi" FORMAT_TABM);
        print256_tab(tab_minus,j,8,STOP);
#endif
        j += 256;
    }


    j = 0;
    while ( j < MAX_NUMBER + MAX_NUMBER + 1 ) {
        printf("\nTab_ApB_lo" FORMAT_TABP);
        print256_tab(tab_plus,j,0, 256);

#if (TOP_BIT >> 16)
        printf(";Tab_ApB_med" FORMAT_TABP);
        print256_tab(tab_plus,j,8, 256);

        printf(";Tab_ApB_hi" FORMAT_TABP);
        print256_tab(tab_plus,j,16,256);
#else
        printf(";Tab_ApB_hi" FORMAT_TABP);
        print256_tab(tab_plus,j,8,256);
#endif
        j += 256;
    }



    int chyb = 0, sum = 0, neni_presne = 0;

    for ( i = 0; i <= MAX_NUMBER; i++ )
    for ( j = 0; j <= MAX_NUMBER; j++ )
    {
            sum++;
            spoctene  = vypocitej(i, j);
            vynasobene= vynasob(i, j);

            odchylka = spoctene - vynasobene;
            if ( odchylka < 0 ) odchylka = - odchylka;
            if ( spoctene != vynasobene || odchylka > 2.0 / (MAX_NUMBER+1))
            {
                chyb++;

//                 printf("%")

                PRINT_FP_TO_X(stdout, spoctene);
                PRINT_FP_TO_X(stdout, vynasobene);

                int m, n, k;
                m = (i + MAX_NUMBER + 1)*(j + MAX_NUMBER + 1);

                k = 0;
                n = m;
                while ( n ) {
                    k++;
                    n >>= 1;
                }
                n = (m + (1 << (k - POCET_BITU - 2)) - 1) >> (k - POCET_BITU - 1);
                n &= MAX_NUMBER;

                printf("%i*%i = $%X(m:$%X) %f", i, j, m, n, vynasobene);

                m = i - j;
                if ( m < 0 ) m = -m;
                m = tab_plus[i+j] - tab_minus[m];
                n = m;
                if (!(n & TOP_BIT)) n <<= 1;
                n = n ^ TOP_BIT;
                n >>= POSUN_VPRAVO;
                printf(", tab = $%X(m:$%X) %f\n", m, n, spoctene);

            }

            if ( spoctene != vynasobene ) {
                neni_presne++;
// exit(0);
            }
    }

    printf("\n; nic nemusi: %i(%f\%%), musi pricitat $%X: %i(%f\%%), pretece pricteni: %i\n", nic, 100.0*nic/(nic+pricti), PRICTI - (PRICTI >> 1), pricti, 100.0*pricti/(nic+pricti),pricti_pretece);
    printf("; neni presne: %i (%f%%), chyb: %i, sum: %i\n", neni_presne, 100.0*neni_presne/sum, chyb, sum);
#endif



// gcc -DSQR_TAB
#ifdef SQR_TAB
    {
        /****************** SQRTAB ******************/

        int tab[256];

        printf("\n; Mantissas of square roots\n");
        
        printf("; (2**-3 * mantisa)**0.5 = 2**-1 * mantisa**0.5 * 2**-0.5 = 2**-2 * 2**0.5\n\
; (2**-2 * mantisa)**0.5 = 2**-1 * mantisa**0.5\n\
; (2**-1 * mantisa)**0.5 = 2**+0 * mantisa**0.5 * 2**-0.5 = 2**-1 * 2**0.5\n\
; (2**+0 * mantisa)**0.5 = 2**+0 * mantisa**0.5\n\
; (2**+1 * mantisa)**0.5 = 2**+0 * mantisa**0.5 * 2**0.5\n\
; (2**+2 * mantisa)**0.5 = 2**+1 * mantisa**0.5\n\
; (2**+3 * mantisa)**0.5 = 2**+1 * mantisa**0.5 * 2**0.5\n\n");
        
        printf("; exp = 2*e\n");
        printf("; (2**exp * mantisa)**0.5 = 2**e * mantisa**0.5\n");
        printf("; exp = 2*e+1\n");
        printf("; (2**exp * mantisa)**0.5 = 2**e * mantisa**0.5 * 2**0.5\n");
        printf("\nSQR_TAB:\n");

        j = 0;
        while ( j < MAX_NUMBER ) {
            for ( i = 0; i < 256; i++ ) {
                float f = make_float(0,0,(j+i) << (23-POCET_BITU));
                f = sqrt(f);
                tab[i] = ((unsigned int) FLOAT_TO_X_OPT(f)) & MAX_NUMBER;
            }
            printf("; lo exp=2*x\n");
            print256_tab(tab, 0, 0, STOP );
            #if MAX_NUMBER > 255
            printf("; hi exp=2*x\n");
            print256_tab(tab, 0, 8, STOP );
            #endif
            j += 256;
        }

        j = 0;
        while ( j < MAX_NUMBER ) {
            for ( i = 0; i < 256; i++ ) {
                float f = make_float(0,1,(j+i) << (23-POCET_BITU));
                f = sqrt(f);
                tab[i] = ((unsigned int) FLOAT_TO_X_OPT(f)) & MAX_NUMBER;
            }
            printf("; lo exp=2*x+1\n");
            print256_tab(tab, 0, 0, STOP );
            #if MAX_NUMBER > 255
            printf("; hi exp=2*x+1\n");
            print256_tab(tab, 0, 8, STOP );
            #endif
            j += 256;
        }

    }
#endif


// gcc -DPOW2TAB
#ifdef POW2TAB
    {
        /****************** POW2TAB ******************/

        int tab[STOP];

        printf("\n; Mantissas of power 2\n");
        printf("; e = 2*exp\n");
        printf("; mantisa < 2**0.5\n");
        printf("; (2**exp * mantisa)**2 = 2**e * mantisa**2\n");
        printf("; mantisa > 2**0.5\n");
        printf("; (2**exp * mantisa)**2 = 2**e * (mantisa**2)/2 * 2 = 2**(e+1) * (mantisa**2)/2\n");
        printf("\nPOW2TAB:\n");

        int predel = 0;

        j = 0;
        while ( j < MAX_NUMBER ) {
            for ( i = 0; i < STOP; i++ ) {
                float f = make_float(0,0,(j+i) << (23-POCET_BITU));
                f = f*f;
                tab[i] = ((unsigned int) FLOAT_TO_X_OPT(f)) & MAX_NUMBER;
                if ( ! tab[i] ) predel = j+i;
            }
            printf("; lo\n");
            print256_tab(tab, 0, 0, STOP );
            #if MAX_NUMBER > 255
            printf("; hi\n");
            print256_tab(tab, 0, 8, STOP );
            #endif
            j += 256;
        }

        printf("\nPREDEL_POW2      EQU $%2X\n", (int) (sqrt(2.0)*MAX_NUMBER) - MAX_NUMBER);
        printf("; mantisa $00..$%2X (0..%i)\t --> 2*exp\n; mantisa $%2X..$%2X (%i..%i)\t --> 2*exp+1\n", predel-1, predel-1, predel, MAX_NUMBER, predel, MAX_NUMBER);
    }
#endif


// gcc -DPOW2TAB_OLD
#ifdef POW2TAB_OLD
    {
        /****************** POW2TAB old style ******************/

        int tab[256];
        j = 0;
        while ( j <= MAX_NUMBER ) {
            tab[j] = j*j;
            j++;
        }
        printf("POW2TAB:\n");

        j = 0;
        while ( j <= MAX_NUMBER ) {
            printf("; lo\n");
            print256_tab(tab, 0, 0, STOP );
            printf("; hi\n");
            print256_tab(tab, 0, 8, STOP );
            j += 256;
        }        
    }
#endif


// gcc -DDIVTAB
#ifdef DIVTAB
    {
        /****************** DIVTAB ******************/

        int tab[256];

        #define EX -0
        printf("\n; mantisa = 1\n");
        printf("; 1 / ( 2**exp * mantisa ) = 2**(-exp) * 1\n");
        printf("; // mantisa = 1.01 .. 1.99\n");
        printf("; 1 / ( 2**exp * mantisa ) = 2**(-exp-1) * 2*1/mantisa\n");
        printf("DIVTAB:\n");

        j = 0;
        while ( j < MAX_NUMBER ) {
            for ( i = 0; i < STOP; i++ ) {
                float f = make_float(0,EX,(j+i) << (23-POCET_BITU));
                f = 1/f;
                tab[i] = ((unsigned int) FLOAT_TO_X_OPT(f)) & MAX_NUMBER;
            }
            printf("; lo\n");
            print256_tab(tab, 0, 0, STOP );
            #if MAX_NUMBER > 255
            printf("; hi\n");
            print256_tab(tab, 0, 8, STOP );
            #endif
            j += 256;
        }
        
#if MAX_NUMBER == 127
        for ( i = 0; i < STOP; i++ ) tab[i] += 128;
        printf("; negative\n");
        print256_tab(tab, 0, 0, STOP );
#endif
    }
#endif


// gcc -DBYTE
#ifdef BYTE
    /****************** BYTE TABLE ******************/

    // byte to float
    for ( i = -256; i < 257; i++ ) {

        if ( i )
            PRINT_FP_TO_X(stdout, (float) i);
        else {
            for ( i = -9; i < 10; i++ ) {
                if ( i ) PRINT_FP_TO_X(stdout, ((float) i)/10.0);
            }
            i = 0;
        }
    }

#endif


// gcc -DCONST
#ifdef CONST
        /****************** KONSTANTY ******************/

// float NAN = 0.0/0.0;

// konstanty


float konst[] = {
+1.414213562373095, // SQRT(2)
+0.0/+1.0,      // POS_ZERO
+0.0/-1.0,      // NEG_ZERO
-1.0/0.0,       // NEG_INF
+1.0/0.0,       // POS_INF

-0.01875,       // -3/160
-0.0375,        // -3/80
-0.0625,        // -1/16
-0.075,         // -3/40
-0.15,          // -3/20
-0.25,          // -1/4

+0.00015625,    // 1/6400
+0.00140625,    // 0.0375*0.0375 = 3*3 / (80*80)
+0.0025,        // 1/400
+0.01,          // 0.1*0.1
+0.0125,        // 1/80
+0.0225,        // 0.15*0.15
+0.03125,       // 1/32
+0.0375,        // 3/80
+0.04,          // 0.2*0.2
+0.05,          // 1/20
+0.09,          // 0.3*0.3
+0.125,         // 1/8
+0.15,          // 3/20
+0.1875,        // 3/16
+0.25,          // 1/4
+0.75,          // 3/4
+0.36,          // 0.6*0.6

+1.5,
+18.75,
+300 };

    int sum_konst = sizeof(konst)/sizeof(float);

    printf("ROOT2F ");
    PRINT_FP_TO_X(stdout, konst[0]);
    printf(" Square root of 2 (1.41421356)\n");
    printf("FPMIN           EQU FP0\nFMMIN           EQU FM0\n");
    
    for ( i = 1; i < sum_konst; i++ )
    {
        PRINT_FP_TO_X(stdout, konst[i]);
    }

#endif


// gcc -DBYTE
#ifdef LN
    /****************** LN TABLE ******************/

    {
        printf("; ln(2^exp*man) = ln(2^exp) + ln(man) = ln(2)*exp + ln(man) = ln2_exp[e] + ln_m[m]\n");
        printf("LN_M:\n");

        
    #if MAX_NUMBER == 127
        for ( i = 0; i <= MAX_NUMBER; i++ )
        {
            double f = make_double(0,0,((binary64) i) << (DO_EXP_POS-POCET_BITU));
            double ln = log(f);
            printf("dw $%04x\t; ln(%1.4f) = %f\n", ((unsigned int) DOUBLE_TO_X_OPT(ln)), f, ln);
        }
        
        printf("\n\n; LN(2)*(-1) = $7eb1 => but $7eb2 is sometimes better...\nLN2_EXP:\n");
        
        int tab[256];
        for ( i = 0; i <= 255; i++ )
        {
            double f = (i - BIAS) * log((double) 2.0);
            tab[i] = ((unsigned int) DOUBLE_TO_X_OPT(f));
            
//             if ( i == 0x7E ) tab[i]++;
        }
        
        printf("; lo\n");
        print256_tab(tab, 0, 0, 256 );
        printf("; hi\n");
        print256_tab(tab, 0, 8, 256 );
    #elif MAX_NUMBER == 255
        int tab[256];
        for ( i = 0; i <= 255; i++ )
        {
            double f = make_double(0,0,((binary64) i) << (DO_EXP_POS-POCET_BITU));
            double ln = log(f);
            tab[i] = ((unsigned int) DOUBLE_TO_X_OPT(ln));
        }
        
        printf("; lo\n");
        print256_tab(tab, 0, 0, 256 );
        printf("; hi\n");
        print256_tab(tab, 0, 8, 256 );

        printf("\n\n; Numbers with exponent -1 ($3f) have lower result accuracy\nLN2_EXP:\n");

        for ( i = 0; i < 128; i++ )
        {
            double f = (i - BIAS) * log((double) 2.0);
            printf("dw $%04x\t; %i*ln(2) = %1.4f\n", ((unsigned int) DOUBLE_TO_X_OPT(f)), i-BIAS, f);
        }
    #elif MAX_NUMBER == 1023
    
        int tab[1024];
        for ( i = 0; i <= MAX_NUMBER; i++ )
        {
            double f = make_double(0,0,((binary64) i) << (DO_EXP_POS-POCET_BITU));
            double ln = log(f);
            tab[i] = ((unsigned int) DOUBLE_TO_X_OPT(ln));
        }

        j = 0;
        while ( j <= MAX_NUMBER ) {
            printf("; lo\n");
            print256_tab(tab, j, 0, STOP );
            printf("; hi\n");
            print256_tab(tab, j, 8, STOP );
            j += 256;
        }

        printf("\n\n; Numbers with exponent -1 ($3c) have lower result accuracy\nLN2_EXP:\n");

        for ( i = 0; i < 32; i++ )
        {
            double f = (i - BIAS) * log((double) 2.0);
            printf("dw $%04x\t; %i*ln(2) = %1.4f\n", ((unsigned int) DOUBLE_TO_X_OPT(f)), i-BIAS, f);
        }

    #else    
        #error Neocekacana hodnota MAX_NUMBER!        
    #endif    
    }

#endif


}


