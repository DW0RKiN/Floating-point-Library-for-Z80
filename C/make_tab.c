
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
// #include <CL/cl.h>
#include <stdint.h>
#include <time.h> 

#include "float.h"


// #define _10bits 1

    
    
#define ZAOKROUHLENI 1
#define MAX_NUMBER 255
    
#if MAX_NUMBER == 1023
    const int pocet_bitu = 10;
    #warning Pro 10+1 bitovou mantisu
// 10+1 bits SEEE EEMM MMMM MMMM
// min 1024*1024 = ((1024+1024)^2)/4 =   1 048 576 = 0x100000 
// max 2047*2047 = ((2047+2047)^2)/4 =   4 190 209 = 0x3FF001 
// max  0x3FF001 =   11 1111 1111 0000 0000 0001 =>   _1 1111 1111 0... .... .... 2**11 * _11 1111 1110 
// min  0x100000 =    1 0000 0000 0000 0000 0000 =>    _ 0000 0000 00.. .... .... 2**10 * _00 0000 0000 
//                                  .. .... ....
    #define use_tab 1
    #define POSUN_TAB 2
    #if ZAOKROUHLENI 
    #include "orig_tab_mul11_7.h"
    int pricti = 0x800 >> POSUN_TAB;
//         int pricti =  0x03FF >> POSUN_TAB;
    #else
        #include "orig_tab_mul11_5.h"
        int pricti = 0;
    #endif

#elif MAX_NUMBER == 127
    const int pocet_bitu = 7;
    #warning Pro 7+1 bitovou mantisu
// 7+1 bits  EEEE EEEE SMMM MMMM
// min 128*128   = ((128+128)^2)/4   =   16 384 = 0x4000
// max 255*255   = ((255+255)^2)/4   =   65 025 = 0xFE01  
// max    0xFE01 =           1111 1110 0000 0001 =>           _111 1110 .... .... 2**8  * _111 1110 
// min    0x4000 =            100 0000 0000 0000 =>            _00 0000 0... .... 2**7  * _000 0000 
//                                      ... ....
    
    #define VYBER 5
        
#if VYBER == 1
    #define use_tab 1
    #include "orig_tab_mul7_2.h"
    #define POSUN_TAB 4
    int pricti =  0x7F >> POSUN_TAB;
#elif VYBER == 2
    #define use_tab 1
    #include "xmul7_16tab_2.h"
    #define POSUN_TAB 0
    int pricti =  0;
#elif VYBER == 3
    #define use_tab 1
    #include "mul7_16tab_1.h"
    #define POSUN_TAB 0
    int pricti =  0x7F >> POSUN_TAB;
#elif VYBER == 4
//     nejlepsi volba
    #define POSUN_TAB 0
    int pricti =  0x80;
#elif VYBER == 5
    #define POSUN_TAB 0

     int pricti =  0x1F;    // neni zaokrouhleno: 0 (0.000000%), preteceni: 0, podteceni: 0
//     int pricti =  0x0F;   // neni zaokrouhleno: 48 (0.292969%), preteceni: 14, podteceni: 34 ???
//     int pricti =  0x07;   // neni zaokrouhleno: 173 (1.055908%), preteceni: 45, podteceni: 128 ???
//     int pricti =  0x04;   // neni zaokrouhleno: 270 (1.647949%), preteceni: 90, podteceni: 180 ???
//     int pricti =  0x03;   // neni zaokrouhleno: 264 (1.611328%), preteceni: 78, podteceni: 186 ???
//     int pricti =  0x02;   // nemozne??
//     int pricti =  0x01;   // nemozne??
//     int pricti =  0xFF;   // nemozne
//     int pricti = 0x100;   // nemozne

#else
    #error "Neocekavana hodnota VYBER!"
#endif
    

#elif MAX_NUMBER == 255
    const int pocet_bitu = 8;    
    #warning Pro 8+1 bitovou mantisu
// 8+1 bits  SEEE EEEE MMMM MMMM
// min 256*256   = ((256+256)^2)/4   =   65 536 = 0x10000
// max 511*511   = ((511+511)^2)/4   =  261 121 = 0x3FC01  
// max   0x3FC01 =        11 1111 0000 0000 0100 =>        _1 1111 000. .... .... 2**9  * _111 1111 
// min   0x10000 =         1 0000 0000 0000 0000 =>         _ 0000 0000 .... .... 2**8  * _000 0000 
   
    #define VYBER 2
        
#if VYBER == 1
    
    #include "orig_tab_mul8.h"
    #define POSUN_TAB 2
    #define use_tab 1
    int pricti =  0xFF >> POSUN_TAB;
#elif VYBER == 2
    #define POSUN_TAB 2
    int pricti =  0xFF >> POSUN_TAB;
#elif VYBER == 3
    #define POSUN_TAB 2
    int pricti =  0x04;
#elif VYBER == 4
//     nejlepsi volba
    #define POSUN_TAB 0
    int pricti =  0x80;
#elif VYBER == 5

#else
    #error "Neocekavana hodnota VYBER!"
#endif

//                                      ... ....
    
//     int pricti = 5;

#else
    #error "Neocekavana hodnota MAX_NUMBER"
#endif

const int top_bit = (2*(MAX_NUMBER+1)*(MAX_NUMBER+1)) >> POSUN_TAB;

const int START_LIMIT = MAX_NUMBER + 1;
const int STOP_LIMIT = MAX_NUMBER + MAX_NUMBER + 2;
const int POSUN = 0;
const int NEJVYSSI_BIT = 2 * (MAX_NUMBER + 1) * (MAX_NUMBER + 1);
    
const int NEJVYSSI = MAX_NUMBER + 1;
const int STOP_MINUS = MAX_NUMBER + 1;
const int STOP_PLUS = MAX_NUMBER + MAX_NUMBER + 1;

const int MASK_PLUS_TABLE = ((4*(MAX_NUMBER+1)*(MAX_NUMBER+1)) >> POSUN_TAB) - 1;
const int MASK_MINUS_TABLE = ((MAX_NUMBER+1)*(MAX_NUMBER+1)-1) >> (POSUN_TAB + 2);


#if ZAOKROUHLENI
    #define RULE (preteceni_zaokr  << PRETECENI_VAHA_ZAOKR) + ( podteceni_zaokr  << PODTECENI_VAHA_ZAOKR) + ( podteceni_presne << PODTECENI_VAHA_PRESNE)
#else
    #define RULE (preteceni_presne << PRETECENI_VAHA_PRESNE) + ( podteceni_presne << PODTECENI_VAHA_PRESNE)
#endif


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


float mul_int_opt(int i, int j)
{    
    float fi, fj;
    fi = make_float(0, 0, i << (23 - pocet_bitu));
    fj = make_float(0, 0, j << (23 - pocet_bitu));      
    fi = fi * fj;
 
    return X_TO_FLOAT( FLOAT_TO_X_OPT( fi ));
}


float mul_int(int i, int j)
{    
    float fi, fj;
    fi = make_float(0, 0, i << (23 - pocet_bitu));
    fj = make_float(0, 0, j << (23 - pocet_bitu));      
    fi = fi * fj;

    return X_TO_FLOAT( FLOAT_TO_X( fi ));
}


float mul_tab(int i, int j, int * plus, int * minus)
{
    int e = 0;
    int r = i - j;
    if ( r < 0 ) r = -r;

    int m = plus[i+j] - minus[r];
    
//     $80*$80 = $4000 = 0100 0000 0000 0000
//     $FF*$FF = $FE01 = 1111 1110 0000 0001 => 0x8000
//                     = 1..._.... ?xxx_xxxx,  if ((xxx_xxxx > 0) && (? > 0)) (1..._....)++; 
//     $100*$100 = $10000 = 01 0000 0000 0000 0000
//     $1FF*$1FF = $3FC01 = 11 1111 1100 0000 0001
//                        = 1._...._...? xxxx_xxxx,  if ((xxxx_xxxx > 0) && (? > 0)) (1._...._...)++; 

    
    if ( m & (2*top_bit)) {
        fprintf(stderr, "Argh!\n");
        fprintf(stderr, "plus[%i]:%X, minus[%i]: %X, ", i+j, plus[i+j], r, minus[r]);
        fprintf(stderr, "%i*%i,m:%X+%X >> %i, TOP:%X", i,j, m, pricti, pocet_bitu- POSUN_TAB, top_bit);
        fprintf(stderr, ", e+m: %i+%x\n", e, m);
        exit(5);
    }
    
    if (( m & (top_bit + top_bit/2)) == 0 )
    {
        fprintf(stderr, "...Argh! Podteceni pod minimalni hodnotu...\n");
        e--;
        m <<= 1;
    }
    
    if ( m & top_bit)
        e++;
    else
        m <<= 1;

    m += pricti;
    if (!(m & top_bit)) e++;
    m >>= 1 + pocet_bitu - POSUN_TAB;

    m &= MAX_NUMBER;
    
//     fprintf(stderr, ", e+m: %i+%x\n", e, m);
    
    return make_float(0, e, m << (23 - pocet_bitu));

}



#define BYTE_TO_BINARY_PATTERN "%c%c%c%c %c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

void printf_bit(int num)
{
    if ( num >> 24 ) printf(BYTE_TO_BINARY_PATTERN"  ",BYTE_TO_BINARY( (num >> 24) & 0xFF ) );
    if ( num >> 16 ) printf(BYTE_TO_BINARY_PATTERN"  ",BYTE_TO_BINARY( (num >> 16) & 0xFF ) );
    if ( num >>  8 ) printf(BYTE_TO_BINARY_PATTERN"  ",BYTE_TO_BINARY( (num >>  8) & 0xFF ) );
    printf(BYTE_TO_BINARY_PATTERN"\n",BYTE_TO_BINARY( num & 0xFF ));
}


void printf_top_bit(int num)
{    
    int cifer = 0, i = num;
    while ( i ) {
        cifer++;
        i >>= 1;
    }

    cifer = (( cifer + 7 ) >> 3) << 3;
    i = cifer;
    int prah = 1 << ( cifer - 1);
    
    printf("; // ");
    
    while ( cifer ) {
        if (cifer > 1 + pocet_bitu - POSUN_TAB ) 
            putchar(( num & prah  ) ? '1' : '0');
        else
            putchar('.');
        cifer--;
        num <<= 1;
        if (( cifer & 0x3 ) == 0) putchar(' ');
        if (( cifer & 0x7 ) == 0) putchar(' ');
    }
    cifer = i;
    printf("\n; // ");


    num = pricti;
    char c = ' ';
    while ( cifer ) {
        if ( num & prah ) 
        { 
            putchar('1');
            c = '0';
        }
        else
            putchar(c);
        cifer--;
        num <<= 1;
        if (( cifer & 0x3 ) == 0) putchar(' ');
        if (( cifer & 0x7 ) == 0) putchar(' ');
    }
    cifer = i;
    printf("\n; // ");

    
    i = 1 + pocet_bitu - POSUN_TAB;
    while ( cifer ) {
        if (cifer > i + pocet_bitu )
            putchar(' ');
        else if (cifer > i + 10 )
            putchar('A' + cifer - i - 11);            
        else if (cifer > i ) 
            putchar('0' + cifer - i - 1);
        else
            putchar('.');
        cifer--;
        num <<= 1;
        if (( cifer & 0x3 ) == 0) putchar(' ');
        if (( cifer & 0x7 ) == 0) putchar(' ');
    }
    putchar('\n');
}





int main () {

    srand(time(0)); 
    
    const int tolerance = 0xffff;
    float f_tolerance = 2.0 / (MAX_NUMBER+1);
    
    int i,j;
    float odchylka, odchylka_presne, jest;

    int sum, best = 2*MAX_NUMBER*MAX_NUMBER, horsi = 0, bez_restartu = 0;
    int iteraci = 0;
    int preteceni, podteceni, neni_presne, last_neni_presne = best;
#if ZAOKROUHLENI
    int neni_zaokrouhleno;
    float odchylka_zaokr;
    #define PRETECENI_VAHA_ZAOKR 2
    #define PODTECENI_VAHA_ZAOKR 0
#endif

#define PRETECENI_VAHA_PRESNE 0
#define PODTECENI_VAHA_PRESNE 0
    
    int roz_tab = 0;
    int abs_roz_tab = 0;
    
    int  plus_power_div4_orig[STOP_PLUS];     // originalni
    int  plus_power_div4_edit[STOP_PLUS];     // editovana
    int  plus_power_div4_best[STOP_PLUS];     // editovana

    int minus_power_div4_orig[STOP_MINUS];    // originalni
    int minus_power_div4_edit[STOP_MINUS];    // editovana
    int minus_power_div4_best[STOP_MINUS];    // editovana
    

// (a+b)^2 = aa+2ab+bb
// (a-b)^2 = aa-2ab+bb
// (a+b)^2 - (a-b)^2 = 4ab
// ((a+b)^2)/4 -((a-b)^2)/4 = ab

#ifdef use_tab
    for ( i = 0; i < STOP_PLUS; i++ ) {
        plus_power_div4_orig[i] = (( 2*NEJVYSSI+i)*(2*NEJVYSSI+i)/4) >> POSUN_TAB;  // 16 bit number in table
        plus_power_div4_edit[i] = tab_plus[i];  // 24 bits number in table
        plus_power_div4_best[i] = tab_plus[i];
    }

    for ( i = 0; i < STOP_MINUS; i++ ) {
        minus_power_div4_orig[i] = ((i*i)/4) >> POSUN_TAB;  // 16 bit number in table
        minus_power_div4_edit[i] = tab_minus[i];  // 16 bits number in table
        minus_power_div4_best[i] = tab_minus[i];
    }
#else
    for ( i = 0; i < STOP_PLUS; i++ ) {
        plus_power_div4_orig[i] = (( 2*NEJVYSSI+i)*(2*NEJVYSSI+i)/4) >> POSUN_TAB;  // 16 bit number in table
        plus_power_div4_edit[i] = plus_power_div4_orig[i];
    }

    for ( i = 0; i < STOP_MINUS; i++ ) {
        minus_power_div4_orig[i] = ((i*i)/4) >> POSUN_TAB;  // 16 bit number in table
        minus_power_div4_edit[i] = minus_power_div4_orig[i];
    }
#endif
    
    
    for ( i = 0; i < STOP_PLUS; i++ ) {
            j = plus_power_div4_orig[i] - plus_power_div4_edit[i];
            roz_tab += j;
            if ( j < 0 ) j = -j;
            abs_roz_tab += j;
    }

    for ( i = 0; i < STOP_MINUS; i++ ) {
            j = minus_power_div4_orig[i] - minus_power_div4_edit[i];
            roz_tab += j;
            if ( j < 0 ) j = -j;
            abs_roz_tab += j;        
    }

    goto spocitej_chyby;

// *******************************************************************************
    while ( 1 ) {
        
        int chyba = 0;


        for ( i = 0; i <= MAX_NUMBER; i++ ) 
        for ( j = 0; j <= MAX_NUMBER; j++ ) 
        {
            
            jest = mul_tab( i, j, plus_power_div4_edit, minus_power_div4_edit);
#if ZAOKROUHLENI
            odchylka_zaokr  = mul_int_opt(i, j) - jest;
            odchylka = odchylka_zaokr;
#else
            odchylka_presne = mul_int(i, j) - jest;
            odchylka = odchylka_presne;
#endif

#if 0
            if ( odchylka != 0 ) 
            {                
                float f = odchylka;
                if ( f < 0 ) f = -f;
                if ( f > f_tolerance ) {
                    fprintf(stderr,"f_tolerance < %f \n",f);
#if 1
                    PRINT_FP_TO_X( stderr,odchylka + jest );
                    PRINT_FP_TO_X( stderr,jest );
#endif
                    int r = i-j;
                    if ( r < 0 ) r = -r;
                    fprintf(stderr, "%i*%i=%f ? %f, odchylka:%f",i+NEJVYSSI,j+NEJVYSSI, odchylka + jest, jest, odchylka);
                    fprintf(stderr, ", pe[%i]:%X, me[%i]:%X", i+j, plus_power_div4_edit[i+j], r, minus_power_div4_edit[r]);
                    fprintf(stderr, ", pe-me = %X>>%i", plus_power_div4_edit[i+j] - minus_power_div4_edit[r], pocet_bitu - POSUN_TAB);
                    fprintf(stderr, "=%X\n\n", plus_power_div4_edit[i+j] - minus_power_div4_edit[r] >> (pocet_bitu - POSUN_TAB));
// exit(3);

                }
            }
        
#endif
 
            int soucet = i + j;
            int rozdil = i - j;
            if ( rozdil < 0 ) rozdil = - rozdil; 
            
            if ( ! neni_presne ) 
            {
                if ( plus_power_div4_edit[soucet] > plus_power_div4_orig[soucet] && rand() & 1) plus_power_div4_edit[soucet]--;
                if ( plus_power_div4_edit[soucet] < plus_power_div4_orig[soucet] && rand() & 1) plus_power_div4_edit[soucet]++;
                if ( minus_power_div4_edit[rozdil] > minus_power_div4_orig[rozdil] && rand() & 1) minus_power_div4_edit[rozdil]--;
                if ( minus_power_div4_edit[rozdil] < minus_power_div4_orig[rozdil] && rand() & 1) minus_power_div4_edit[rozdil]++;
            }
                
                
                
            int x = 0;
            while ( odchylka != 0 ) 
            {
// exit(1);
                if ( x++  > 15*tolerance) break;
                    
                if ( rand() & 0x03 ) {    // vetsi sance zmenit o 1 plus_tabulku,ktera ma vetsi hodnoty
                    if ( odchylka > 0 ) {  // musime pridat
                        if (plus_power_div4_edit[soucet] < MASK_PLUS_TABLE && 
                            plus_power_div4_edit[soucet] <= 5*tolerance + plus_power_div4_orig[soucet]) {
                            plus_power_div4_edit[soucet]++;
                            break;
                            }
                        }
                        else {
                            if (plus_power_div4_edit[soucet] > 0 && 
                            plus_power_div4_edit[soucet] >= -5*tolerance + plus_power_div4_orig[soucet]) {
                            plus_power_div4_edit[soucet]--;
                            break;
                        }
                    }
                }
                else // mensi sance menit minus_tabulku obsahujici mensi hodnoty
                {
                    if ( odchylka > 0 ) { // musime pridat
                        if (minus_power_div4_edit[rozdil] > 0 && 
                            minus_power_div4_edit[rozdil] >= -tolerance + minus_power_div4_orig[rozdil]) {
                            minus_power_div4_edit[rozdil]--;
                            break;
                        }
                    }
                    else {
                        if (minus_power_div4_edit[rozdil] < MASK_MINUS_TABLE && 
                            minus_power_div4_edit[rozdil] <= tolerance + minus_power_div4_orig[rozdil] ) {
                            minus_power_div4_edit[rozdil]++;
                            break;
                        }
                    }
                }
            }
            
            
        }
        
// **************************************************************************
spocitej_chyby:

        preteceni = 0, podteceni = 0, sum = 0, neni_presne = 0, chyba = 0;

#if ZAOKROUHLENI
        int podteceni_zaokr = 0;
        int preteceni_zaokr = 0;
#endif
        int preteceni_presne = 0;
        int podteceni_presne = 0;
        
        for ( i = 0; i <= MAX_NUMBER; i++ ) 
        for ( j = 0; j <= MAX_NUMBER; j++ ) 
        {
            sum++;
            int nova_chyba = 0;

            jest = mul_tab( i, j, plus_power_div4_edit, minus_power_div4_edit );
#if ZAOKROUHLENI            
            odchylka_zaokr  = mul_int_opt(i, j) - jest;
            if ( odchylka_zaokr  > 0 ) podteceni_zaokr++;
            if ( odchylka_zaokr  < 0 ) preteceni_zaokr++;
#endif
            odchylka_presne = mul_int(i, j) - jest;            
            if ( odchylka_presne > 0 ) podteceni_presne++;
            if ( odchylka_presne < 0 ) preteceni_presne++;

#if ZAOKROUHLENI
            odchylka = odchylka_zaokr;
#else
            odchylka = odchylka_presne;
#endif
            
            if ( odchylka > 0 ) {
                neni_presne++;
                podteceni++;
                if (  odchylka > f_tolerance ) nova_chyba++;
            }
            if ( odchylka < 0 ) {
                neni_presne++;
                preteceni++;
                if ( -odchylka > f_tolerance ) nova_chyba++;
            }
            chyba += nova_chyba;
            
            
            if ( odchylka != 0 ) {
                
#if 1           
                if ( nova_chyba ) 
                {
                    PRINT_FP_TO_X( stderr, odchylka + jest );
                    PRINT_FP_TO_X( stderr,jest );
                    
                    int r = i-j;
                    if ( r < 0 ) r = -r;
                    fprintf(stderr, "%i*%i=%f ? %f, odchylka:%f",i+NEJVYSSI,j+NEJVYSSI, odchylka + jest, jest, odchylka);
                    fprintf(stderr, ", pe[%i]:%X, me[%i]:%X", i+j, plus_power_div4_edit[i+j], r, minus_power_div4_edit[r]);
                    fprintf(stderr, ", pe-me = %X>>%i", plus_power_div4_edit[i+j] - minus_power_div4_edit[r], pocet_bitu - POSUN_TAB);
                    fprintf(stderr, "=%X\n\n", plus_power_div4_edit[i+j] - minus_power_div4_edit[r] >> (pocet_bitu - POSUN_TAB));
// exit(3);

                }
#endif
                                        
            }
        }
        
        if ( neni_presne > last_neni_presne ) 
        {
            horsi++;
            bez_restartu++;
        }
        else if ( neni_presne < last_neni_presne )
            if ( horsi ) horsi--;
        
            
        int aktualne = RULE;

            
        if ( !chyba && best >= aktualne ) {
            
            int r = 0;
            int ar = 0;            
            
            for ( i = 0; i < STOP_PLUS; i++ ) {
                j = plus_power_div4_orig[i] - plus_power_div4_edit[i];
                r += j;
                if ( j < 0 ) j = -j;
                ar += j;
            }

            for ( i = 0; i < STOP_MINUS; i++ ) {
                j = minus_power_div4_orig[i] - minus_power_div4_edit[i];
                r += j;
                if ( j < 0 ) j = -j;
                ar += j;        
            }
            
            
            
            if ( best > aktualne ||  abs_roz_tab > ar ) {

                best = aktualne;                
                abs_roz_tab = ar;
                roz_tab = r;
            
                for ( i = 0; i < STOP_PLUS; i++ )
                    plus_power_div4_best[i] = plus_power_div4_edit[i];
        
                for ( i = 0; i < STOP_MINUS; i++ ) 
                    minus_power_div4_best[i] = minus_power_div4_edit[i];
                
                printf("#define MAX_NUMBER %i\n", MAX_NUMBER);
                printf("#define TOP_BIT 0x%X\n", top_bit);
                printf("#define PRICTI 0x%X\n", pricti );
                printf("#define POSUN_VPRAVO %i\n", 1 + pocet_bitu - POSUN_TAB );
                printf("#define POCET_BITU %i\n\n", pocet_bitu);
                

                printf_top_bit(top_bit);
                printf("\n");
                
//           .... 1...   .... ....   .... ....
//           xxxx x987   6543 210x   xxxx xxxx 
                
                printf("; //       neni presne: %i (%f%%), preteceni: %i, podteceni: %i\n", (preteceni_presne+podteceni_presne), 100.0*(preteceni_presne+podteceni_presne)/sum, preteceni_presne, podteceni_presne);
#if ZAOKROUHLENI
                printf("; // neni zaokrouhleno: %i (%f%%), preteceni: %i, podteceni: %i\n", (preteceni_zaokr+podteceni_zaokr), 100.0*(preteceni_zaokr+podteceni_zaokr)/sum, preteceni_zaokr, podteceni_zaokr);
#endif
                printf("; // sum(tab_dif[]): %i, sum(abs(tab_dif[])): %i\n", r, ar );


                printf("; // (( %i + a ) * ( %i + b )) >> %i = (tab_plus[a+b] - tab_minus[a-b]) >> %i = ", NEJVYSSI, NEJVYSSI, pocet_bitu - POSUN_TAB, pocet_bitu - POSUN_TAB);
                
                printf("(1");
                i = pocet_bitu;
                while ( i ) {
                    if (!((i+1) & 0x03)) putchar(' ');
                    putchar('m');
                    i--;
                }
                printf(".) OR (0");
                i = pocet_bitu;
                if (!((i+1) & 0x03)) putchar(' ');
                putchar('1');
                while ( i ) {
                    if (!(i & 0x03)) putchar(' ');
                    putchar('m');
                    i--;
                }
                printf(")\n; // 0 <= a <= %i, 0 <= b <= %i\n\n", MAX_NUMBER, MAX_NUMBER);
                
                int tol = 0;
                for ( i = 0; i < STOP_MINUS; i++ ) {
                    j = minus_power_div4_orig[i] - minus_power_div4_edit[i];
                    if ( j < 0) j = -j;
                    if ( j > tol ) tol = j;
                }   
                printf("; // tab_minus[i] = (i*i)/4 +- 0x%X\n", tol);

                printf("int tab_minus[MAX_NUMBER+1] = {  // [0..%i]",MAX_NUMBER);
                for ( i = 0; i < STOP_MINUS; i++ ) {
                    if ( i % 16 == 0 ) 
                    {
                        if ( i ) putchar(',');
                        printf("\n0x%04X", minus_power_div4_edit[i]);
                    }
                    else 
                        printf( ",0x%04X", minus_power_div4_edit[i]);
                }   
                printf("};\n");

                tol = 0;
                for ( i = 0; i < STOP_PLUS; i++ ) {
                    j = plus_power_div4_orig[i] - plus_power_div4_edit[i];
                    if ( j < 0) j = -j;
                    if ( j > tol ) tol = j;
                }   
                printf("\n; // tab_plus[i] = ((%i + %i + i) * (%i + %i + i))/4 +- 0x%X\n", NEJVYSSI, NEJVYSSI, NEJVYSSI, NEJVYSSI, tol);    
                printf("int tab_plus[MAX_NUMBER+MAX_NUMBER+1] = {  // [0..%i]",MAX_NUMBER+MAX_NUMBER);
                for ( i = 0; i < STOP_PLUS; i++ ) {
                    if ( i % 16 == 0 ) 
                    {
                        if ( i ) putchar(',');
                        printf("\n0x%04X", plus_power_div4_edit[i]);
                    }
                    else 
                        printf( ",0x%04X", plus_power_div4_edit[i]);
                }   
                printf("};\n\n");
                fflush(stdout);
            }
            fprintf(stderr, "\n+ %i. best: %i, neni presne: %i (%f%%), preteceni: %i, podteceni: %i, tab_dif: %i, abs(tab_dif): %i\n", iteraci, best, neni_presne, 100.0*neni_presne/sum, preteceni, podteceni, r, ar );

        } 
        
        
        if ( chyba > 0 ||  (((rand() & 0xfff) < (neni_presne - best) ) &&  horsi > 20 )) {  // reset!
            
            fprintf(stderr, "\nReset!");
            if ( chyba ) fprintf(stderr, " S chybou!");
            putc('\n', stderr );
            
            horsi = 0;
            neni_presne = 2*MAX_NUMBER*MAX_NUMBER;
            bez_restartu = 0;
            
            for ( i = 0; i < STOP_PLUS; i++ )
                if ( rand() & 0x03 )
                    plus_power_div4_edit[i] = plus_power_div4_best[i];
                #ifdef use_tab
                else if ( rand() & 1 )
                    plus_power_div4_edit[i] = ( plus_power_div4_best[i] + tab_plus[i] ) >> 1;
                else if ( rand() & 1 )
                    plus_power_div4_edit[i] = tab_plus[i];
                #endif
                else if ( rand() & 1 )
                    plus_power_div4_edit[i] = ( plus_power_div4_best[i] + plus_power_div4_orig[i] ) >> 1;
                else 
                    plus_power_div4_edit[i] = plus_power_div4_orig[i];
                    
    
            for ( i = 0; i < STOP_MINUS; i++ ) 
                if ( rand() & 0x03 )
                    minus_power_div4_edit[i] = minus_power_div4_best[i];
                #ifdef use_tab
                else if ( rand() & 1 )
                    minus_power_div4_edit[i] = ( minus_power_div4_best[i] + tab_minus[i] ) >> 1;
                else if ( rand() & 1 )
                    minus_power_div4_edit[i] = tab_minus[i];
                #endif
                else if ( rand() & 1 )
                    minus_power_div4_edit[i] = ( minus_power_div4_best[i] + minus_power_div4_orig[i] ) >> 1;
                else 
                    minus_power_div4_edit[i] = minus_power_div4_orig[i];
        }
         
         
         
        if ( (++iteraci & 0x03f) == 0 ) 
            fprintf(stderr, "\n- %i. best: %i, neni presne: %i (%f%%), preteceni: %i, podteceni: %i, tab_dif: %i, abs(tab_dif): %i, bez_restartu: %i, horsi: %i\n", iteraci, best, neni_presne, 100.0*neni_presne/sum, preteceni, podteceni, roz_tab, abs_roz_tab, bez_restartu, horsi );
      
        last_neni_presne = neni_presne;
 
    }
          
}



/*
 
 ; tab_mul[a] = (a*a)/4 +- 64
 int tab_mul[512] = {
$0000,$0000,$0001,$0001,$0002,$0003,$0003,$0005,$0003,$0007,$0005,$0009,$0007,$000D,$0009,$0011,
$0016,$001F,$0029,$002D,$003C,$0043,$004D,$0082,$0083,$0088,$0088,$0090,$009D,$00AA,$00B7,$00C5,
$00D4,$0106,$0106,$010F,$011C,$012B,$0144,$0153,$0183,$018B,$0191,$01A9,$01C1,$01D1,$0204,$020C,
$0216,$022C,$024C,$0284,$0287,$029D,$02B5,$02CF,$0303,$030E,$0324,$0342,$0382,$038A,$039B,$03BE,
$03DA,$0409,$041B,$043D,$0482,$048B,$04A4,$04C1,$0502,$050F,$0535,$055D,$0587,$05A5,$05CC,$0607,
$061A,$0642,$0684,$069D,$06C3,$0705,$0714,$0742,$0782,$079D,$07CB,$0807,$0823,$085D,$0886,$08A9,
$08EB,$090D,$093B,$0985,$09A3,$09DC,$0A0F,$0A42,$0A7F,$0A9D,$0AD3,$0B0D,$0B4B,$0B87,$0BB4,$0C03,
$0C1A,$0C5E,$0C95,$0CDC,$0D15,$0D57,$0D94,$0DD2,$0E0F,$0E49,$0E88,$0EC5,$0F03,$0F41,$0F81,$0FC0,
$1000,$1040,$1081,$10C2,$1104,$1146,$1189,$11CC,$1210,$1254,$1299,$12DE,$1324,$136A,$13B1,$13F8,
$1440,$1488,$14D1,$151A,$1564,$15AE,$15F9,$1644,$1690,$16DC,$1729,$1776,$17C4,$1812,$1861,$18B0,
$1900,$1950,$19A1,$19F2,$1A44,$1A96,$1AE9,$1B3C,$1B90,$1BE4,$1C39,$1C8E,$1CE4,$1D3A,$1D91,$1DE8,
$1E40,$1E98,$1EF1,$1F4A,$1FA4,$1FFE,$2059,$20B4,$2110,$216C,$21C9,$2226,$2284,$22E2,$2341,$23A0,
$2400,$2460,$24C1,$2522,$2584,$25E6,$2649,$26AC,$2710,$2774,$27D9,$283E,$28A4,$290A,$2971,$29D8,
$2A40,$2AA8,$2B11,$2B7A,$2BE4,$2C4E,$2CB9,$2D24,$2D90,$2DFC,$2E69,$2ED6,$2F44,$2FB2,$3021,$3090,
$3100,$3170,$31E1,$3252,$32C4,$3336,$33A9,$341C,$3490,$3504,$3579,$35EE,$3664,$36DA,$3751,$37C8,
$3840,$38B8,$3931,$39AA,$3A24,$3A9E,$3B19,$3B94,$3C10,$3C8C,$3D09,$3D86,$3E04,$3E82,$3F01,$3F80,
$4000,$4080,$4101,$4182,$4204,$4286,$4309,$438C,$4410,$4494,$4519,$459E,$4624,$46AA,$4731,$47B8,
$4840,$4903,$4983,$4A07,$4A85,$4B0D,$4B89,$4C11,$4C96,$4D2D,$4DBD,$4E43,$4F01,$4F85,$5005,$508D,
$5109,$519F,$5229,$52C5,$5381,$5407,$5487,$5511,$559D,$562D,$56C4,$5785,$5806,$5891,$591D,$59AF,
$5A4D,$5B07,$5B88,$5C1F,$5CB7,$5D53,$5E04,$5E8F,$5F1D,$5FB8,$6082,$610B,$6196,$6230,$6300,$6389,
$6411,$64B0,$6580,$6609,$669B,$673E,$6802,$688D,$6924,$69C8,$6A84,$6B13,$6BB8,$6C83,$6D08,$6DAB,
$6E4D,$6F0B,$6FA4,$7050,$7106,$71A6,$724C,$730A,$73A6,$7452,$7507,$75AA,$7681,$770E,$77B6,$7885,
$7914,$79C4,$7A85,$7B29,$7C01,$7C8F,$7D43,$7E07,$7EA8,$7F81,$800F,$80C2,$8186,$822C,$8303,$839D,
$8453,$8510,$85CB,$868C,$873B,$8808,$88B4,$8986,$8A28,$8B04,$8BA3,$8C82,$8D1B,$8E00,$8E9A,$8F5E,
$9015,$90DE,$919A,$9280,$931B,$9402,$94A3,$9584,$9627,$9706,$97B4,$9888,$993B,$9A0C,$9ACB,$9B90,
$9C53,$9D1E,$9E03,$9EAC,$9F86,$A042,$A110,$A201,$A2A8,$A387,$A443,$A50F,$A601,$A6A9,$A785,$A844,
$A914,$AA05,$AAB5,$AB8E,$AC81,$AD2A,$AE07,$AED2,$AFA7,$B08A,$B14C,$B225,$B306,$B3CF,$B4A4,$B58B,
$B64F,$B72B,$B808,$B903,$B9B8,$BA93,$BB84,$BC48,$BD24,$BE0D,$BF02,$BFBE,$C09B,$C189,$C280,$C32F,
$C411,$C509,$C600,$C6AE,$C796,$C88B,$C982,$CA33,$CB1D,$CC0F,$CD04,$CDD4,$CEB7,$CF9F,$D088,$D187,
$D24D,$D32D,$D41D,$D511,$D606,$D705,$D7C4,$D8AD,$D99D,$DA91,$DB87,$DC87,$DD81,$DE47,$DF29,$E01F,
$E109,$E20D,$E305,$E405,$E501,$E5C3,$E6BC,$E7AD,$E896,$E991,$EA89,$EB8D,$EC85,$ED87,$EE83,$EF83,
$F040,$F138,$F231,$F32A,$F424,$F51E,$F619,$F714,$F810,$F90C,$FA09,$FB06,$FC04,$FD02,$FE01,$FF00,
}

 
 
 */
