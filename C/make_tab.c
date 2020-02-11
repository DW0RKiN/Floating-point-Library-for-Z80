
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
                    
                if ( rand() & 0x03 ) {    // vetsi sance zmenit o 1 plus_tabulku, ktera ma vetsi hodnoty
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


