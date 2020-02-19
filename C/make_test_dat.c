
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


#define RANDOM  1

#if RANDOM == 0

#include "test_mul.h"
int max_mul = sizeof(data_mul)/sizeof(unsigned short);

#include "test_div.h"
int max_div = sizeof(data_div)/sizeof(unsigned short);

#include "test_add.h"
int max_add = sizeof(data_add)/sizeof(unsigned short);

#include "test_frac.h"
int max_frac = sizeof(data_frac)/sizeof(unsigned short);

#include "test_fint.h"
int max_fint = sizeof(data_fint)/sizeof(unsigned short);

#endif




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


// ; Round towards zero
// ; In: HL any floating-point number
// ; Out: HL same number rounded towards zero
// ; Pollutes: AF,B
float FINT(float x)
{
    int e = GET_FP_EXP(x);
    
// printf("exp(%f) =  %i\n", x, e);
//            1    
// seee eeee  emmm mmmm  mmmm mmmm  mmmm mmmm
    if ( e < 0 ) return 0.0;
    if ( e >= FP_MANT_SIZE ) return x;
    
    single m = FP_MASK_MANTISA >> e;
    single *px;
    
    px = (void *) &x;    
    *px &= -1 ^ m;
    
    return x;
}



// ; Round towards zero
// ; In: HL any floating-point number
// ; Out: HL same number rounded towards zero
// ; Pollutes: AF,B
double DINT(double num)
{
    long int e = GET_DO_EXP(num);
    
    if ( e < 0 ) return 0.0;
    if ( e >= DO_MANT_SIZE ) return num;
    
    binary64  m = DO_MASK_MANTISA >> e;
    binary64 *pnum;
    
if (sizeof(binary64) != sizeof(double))
{
    fprintf(stderr, "Neshoduji se velikosti binary64 a double!\n");
}
    pnum = (void *) &num;    
    
    double test = trunc(num);
    *pnum &= -1 ^ m;
    
    if ( test != num )
    {
        fprintf(stderr, "Neshoduji se hodnoty trunc!\n");        
    }    
    return num;
}


// ; Fractional part, remainder after division by 1
// ; In: HL any floating-point number
// ; Out: HL fractional part, with sign intact
// ; Pollutes: AF,AF',BC,DE
double FRAC(double x)
{
// printf("INT(%f)", x);
    x -= DINT(x); 
// printf(" =  %f\n", x);
    return x;
}





// ; Fractional part, remainder after division by 1
// ; In: HL any floating-point number
// ; Out: HL fractional part, with sign intact
// ; Pollutes: AF,AF',BC,DE
double DMOD(double a, double b)
{
    return  a - (DINT(a/b) * b); 
}



double bfloat2double_( __uint16_t num16 )
{
    double res = 2 * (1 + BF_MASK_MANTISA + (num16 & BF_MASK_MANTISA)) / ( 1 + BF_MASK_MANTISA);
    if ( num16 & BF_MASK_SIGN ) res = -res;
    int e = ((num16 >> 8) & 0xff) - BF_BIAS;
    if ( e > 0 )
    {
        while ( e-- ) res *= 2;
    }
    else if ( e < 0 )
    {
        while ( e++ ) res /= 2;
    }
    return res;
}


double bfloat2double( __uint16_t num16 )
{
    binary64 e = DO_BIAS + ((num16 & 0xff00) >> 8);
    e = e - BF_BIAS;
    e <<= DO_EXP_POS;
    
    binary64 m = num16 & BF_MASK_MANTISA;
    m <<= ( DO_MANT_SIZE - BF_MANT_SIZE );
    
    binary64 s = num16 & BF_MASK_SIGN;
    s <<= ( DO_SIGN_POS - BF_SIGN_POS );
    
// printf("%04x s:%x e: %02x 2*m: %02x", num16, num16 & 0x80, num16 >> 8, 2*(num16 & 0x7f));


    volatile binary64 num64 = m + e + s;
    volatile double *p;
    p = (void *) &num64;
// printf(" = %f\n", *p);
    return *p;
}



int main( int argc, const char* argv[] ) 
{

    int i;
    unsigned short ba, bb, bc;
    double da, db, dc;

    
#if RANDOM

if ( RAND_MAX < 0xFFFF)
{
    fprintf(stderr,"RAND_MAX = %X < 0xFFFF\n", RAND_MAX);
    return 1;
}


srand(time(NULL)); // randomize seed



#if MAX_NUMBER == 1023

    #define TABLE_DIV           ((8+32)*256)    // DIV+MUL 10240
    #define TABLE_MUL           (32*256)        // MUL      8192
    #define TABLE_SQRT          (16*256)        // FSQRT    4096
    #define TABLE_LN            (9*256)         // LN       2112

    #define SPACE           30000

#elif MAX_NUMBER == 255

    #define TABLE_DIV           ((2+8)*256)     // DIV+MUL  2560
    #define TABLE_MUL           (8*256)         // MUL      2048
    #define TABLE_SQRT          (1*256)         // FSQRT     256
    #define TABLE_LN            (3*256)         // LN        768

    #define SPACE           30000

#elif MAX_NUMBER == 127

    #define TABLE_DIV           ((1+4)*256)     // DIV+MUL  1280
    #define TABLE_MUL           (4*256)         // MUL      1024
    #define TABLE_SQRT          (1*256)         // FSQRT     256
    #define TABLE_LN            (3*256)         // LN        768

    #define SPACE           30000

#else
    #error Neocekavana hodnota MAX_NUMBER! Povoleny jsou 127, 1023
#endif



#if 0
// fbyte

#define CAST 5
#define MAX_CAST    5

int MIN = (256*256*CAST)/MAX_CAST;
int MAX = (256*256*(CAST+1))/MAX_CAST;

printf("dw $%04x\t\t; %5i .. %5i\n", MIN, MIN, MAX-1);

for (i = MIN; i < MAX; i++ ) {

    fc = 1.0 *i;
    bc = FLOAT_TO_X_OPT( fc );
    fc = X_TO_FLOAT( bc );
    printf("dw $%04x\t\t; 1.0 * $%04X = 1.0 * %5i = %+.6e, s:%i e:%+03i: m: " BYTE_TO_BINARY_PATTERN "b\n", bc, i, i, fc, GET_SIGN(bc), GET_EXP(bc), BYTE_TO_BINARY(GET_MANTISA(bc)));
}
#endif


#if 0

    #define BYTE_PER_LINE   (2*2)
    #define MAX_LINE        (SPACE/BYTE_PER_LINE)


// frac
for (i = -2; i < 2+MANT_SIZE; i++ ) {

    ba = MAKE_X( rand() & 1, i, MASK_MANTISA );         
    da = X_TO_DOUBLE( ba );    
    dc = FRAC(da);
    bc = DOUBLE_TO_X_OPT( dc );
    printf("dw $%04x, $%04x\t\t; %+12g %% 1 = %+.6e\n", ba, bc, da, dc);
}
printf("\n");

int nula = 0;
int shodne = 0;

for (i = 0; i < MAX_LINE; i++ ) {

    do {
        ba = rand() & (0xFFFF - MASK_SIGN);           // jen kladna cisla     
        da = X_TO_DOUBLE( ba );
        dc = FRAC(da);
        bc = DOUBLE_TO_X_OPT( dc );
        
        if ( da == dc ) 
        {
            if ( shodne < (i / 32))
            {
                shodne++;
                break;
            }
        } else if ( dc == 0 ) 
        {
            if ( nula < (i / 32))
            {
                nula++;
                break;
            }
        } else 
            break;
        
    } while (1);
    
    char c = ' ';
    if ( da == dc ) c = '=';
    if ( dc ==  0 ) c = '0';
    printf("dw $%04x, $%04x\t\t; %+12g %% 1 = %+.6e %c\n", ba, bc, da, dc, c);
}
#endif


#if 1

int exp = 0;

#define POS     1
#define SAME    2
#define ALL     4

// gcc -DVARIANTA=1
#ifndef VARIANTA
    #define VARIANTA 1
#endif

#if   VARIANTA == 1
    #define RAND SAME
    #define BYTE_PER_LINE   (3*2)
    #define MAX_LINE        (SPACE/BYTE_PER_LINE)
    char c = '+';       // *
#elif VARIANTA == 2
    #define RAND SAME
    #define BYTE_PER_LINE   (3*2)
    #define MAX_LINE        (SPACE/BYTE_PER_LINE)
    char c = '-';       // *
#elif VARIANTA == 3
    #define RAND ALL
    #define BYTE_PER_LINE   (3*2)
    #define MAX_LINE        ((SPACE-TABLE_MUL)/BYTE_PER_LINE)
    char c = '*';       // *
#elif VARIANTA == 4
    #define RAND ALL
    #define BYTE_PER_LINE   (3*2)
    #define MAX_LINE        ((SPACE-TABLE_DIV)/BYTE_PER_LINE)
    char c = '/';       // *
#elif VARIANTA == 5
    #define RAND ALL
    #define BYTE_PER_LINE   (3*2)
    #define MAX_LINE        ((SPACE-TABLE_DIV)/BYTE_PER_LINE)
    char c = '%';       // *
#elif VARIANTA == 6
    #define RAND ALL
    #define BYTE_PER_LINE   (3*2)
    #define MAX_LINE        (SPACE/BYTE_PER_LINE)
    char c = '/';       // * x / 2^x
#elif VARIANTA == 7
    // pow2()
    #define RAND ALL
    #define BYTE_PER_LINE   (2*2)
    #define MAX_LINE        ((SPACE-TABLE_SQRT)/BYTE_PER_LINE)
#elif VARIANTA == 8
    // fsqrt()
    #define RAND POS
    #define BYTE_PER_LINE   (2*2)
    #define MAX_LINE        ((SPACE-TABLE_SQRT)/BYTE_PER_LINE)
#elif VARIANTA == 9
    // load world
    #define RAND POS
    #define BYTE_PER_LINE   (2*2)
    #define MAX_LINE        (SPACE/BYTE_PER_LINE)
#elif VARIANTA == 10
    // ln()
    #define RAND POS
    #define BYTE_PER_LINE   (2*2)
    #define MAX_LINE        ((SPACE-TABLE_LN)/BYTE_PER_LINE)

#else
    #error Necekana hodnota VARIANTY!
#endif


#if 1
for (i = 0; i < MAX_LINE; i++ ) {

    
#if RAND == POS
    ba = rand() & ( 0xFFFF - MASK_SIGN );        
    if ( i < MAX_LINE / 10 ) 
        bb = MASK_EXP + MASK_MANTISA;      // + inf
    else
        bb = (rand() & MASK_MANTISA) + ((exp++ << EXP_POS) & MASK_EXP);
    
#elif RAND == SAME
    if ( i & 1 ) {               // jen kladna cisla
        ba = rand() & ( 0xFFFF - MASK_SIGN );        
        if ( i < MAX_LINE / 10 ) 
            bb = MASK_EXP + MASK_MANTISA;      // + inf
        else
            bb = (rand() & MASK_MANTISA) + ((exp++ << EXP_POS) & MASK_EXP);
    } else {                    // jen zaporna cisla
        ba = (rand() & ( 0xFFFF - MASK_SIGN )) + MASK_SIGN;        
        if ( i < MAX_LINE / 10 ) 
            bb = 0xFFFF;                       // - inf
        else
            bb = (rand() & MASK_MANTISA) + ((exp++ << EXP_POS) & MASK_EXP) + MASK_SIGN;
    }
    
#elif RAND == ALL
    ba = rand() & 0xFFFF;        
    if ( i < MAX_LINE / 10 ) 
        bb = (rand() & MASK_SIGN) + MASK_EXP + MASK_MANTISA;
    else
        bb = rand() & ( MASK_SIGN + MASK_MANTISA ) + ((exp++ << EXP_POS) & MASK_EXP);
#else
    #error Neocekavana hodnota RAND
#endif
    
    da = X_TO_DOUBLE( ba );
    db = X_TO_DOUBLE( bb );
// *************************
    
#if   VARIANTA == 1
    dc   = da + db;     // *
#elif VARIANTA == 2
    dc   = da - db;     // *
#elif VARIANTA == 3
    dc   = da * db;     // *
#elif VARIANTA == 4
    dc   = da / db;     // *
#elif VARIANTA == 5
    dc = 1;
    if ( da < 0 ) 
    {  
       if ( db  < 0 && -da < -db ) dc = 0;
       if ( db >= 0 && -da <  db ) dc = 0;
    } 
    else 
    {  
       if ( db  < 0 &&  da < -db ) dc = 0;
       if ( db >= 0 &&  da <  db ) dc = 0;
    } 
    if ( dc == 0 && (rand() & 0x3) )
    {
        dc = da; da = db; db = dc;
        bc = ba; ba = bb; bb = bc;
    }
   
#if 0
    dc = DMOD(da, db);  // *
#else
    dc = fmod(da, db);  // *
#endif
    
#if 0
//     compile with -lm
    if ( dc != fmod( da, db ))
    {
        fprintf(stderr,"%f != %f\n", dc, fmod(da,db));
    }
#endif
#elif VARIANTA == 6
    bb = bb & ( 0xFFFF - MASK_MANTISA ); 
    db = X_TO_DOUBLE( bb );
    dc   = da / db;     // *
#elif VARIANTA == 7
    bb = ba; 
    db = X_TO_DOUBLE( bb );
    dc   = da * db;     // *
#elif VARIANTA == 8
    bc   = _sqrt(ba);   // *
    dc = X_TO_DOUBLE( bc );
    
#elif VARIANTA == 9
    ba = rand() & 0xFFFF; 
    dc = 1.0 * ba;
#elif VARIANTA == 10
    dc  = log(da);     // *

#else
    #error Necekana hodnota VARIANTY!
#endif
    
// *************************
    bc = DOUBLE_TO_X_OPT( dc );

// printf("  a: %lx = %g\n", *((binary64 *) &da), da);    
// printf("  b: %lx = %g\n", *((binary64 *) &db), db);    
// printf("res: %lx = %g\n", *((binary64 *) &dc), dc);    

#if VARIANTA == 7
    printf("dw $%04x, $%04x\t\t; %+.3e^2 = %+.3e", ba, bc, da, dc);
#elif VARIANTA == 8
    printf("dw $%04x, $%04x\t\t; %+.3e^0.5 = %+.3e", ba, bc, da, dc);
#elif VARIANTA == 9
    printf("dw $%04x, $%04x\t\t; %i * 1.0 = %+.3e", ba, bc, ba, dc);
#elif VARIANTA == 10
    printf("dw $%04x, $%04x\t\t; ln(%+.3e) = %+.3e", ba, bc, da, dc);
#else
    printf("dw $%04x, $%04x, $%04x\t\t; %+.3e %c %+.3e = %+.3e", ba, bb, bc, da, c, db, dc);
#endif
    if ( bc ==  0 ) printf("\t = 0");
    if ( bc ==  MASK_SIGN ) printf("\t =-0");
    if ( bc ==  0xFFFF ) printf("\t = -INF");
    if ( bc ==  0xFFFF - MASK_SIGN ) printf("\t = +INF");
    printf("\n");    

}
#endif
#endif

#endif // RANDOM
    
    
#if 0    
    for (i = 0; i < max_mul; i+=3 ) {
        
#if 1
        fa = binary16_to_float( data_mul[i+0] );
        ba = FLOAT_TO_X_OPT( fa );
        fb = binary16_to_float( data_mul[i+1] );
        bb = FLOAT_TO_X_OPT( fb );
#else
        ba = data_mul[i+0];
        bb = data_mul[i+1];
#endif
        fa = X_TO_FLOAT( ba );
        fb = X_TO_FLOAT( bb );

        fc = fa * fb;        
        bc = FLOAT_TO_X_OPT( fc );

        printf("dw $%04x, $%04x, $%04x\t\t; %+.6e * %+.6e = %+.6e\n", ba, bb, bc, fa, fb, fc);
    }
#endif

#if 0
    for (i = 0; i < max_div; i+=3 ) {
        
#if 1
        fa = binary16_to_float( data_div[i+0] );
        ba = FLOAT_TO_X_OPT( fa );
        fb = binary16_to_float( data_div[i+1] );
        bb = FLOAT_TO_X_OPT( fb );
#else
        ba = data_div[i+0];
        bb = data_div[i+1];
#endif
        fa = X_TO_FLOAT( ba );
        fb = X_TO_FLOAT( bb );

        fc = fa / fb;        
        bc = FLOAT_TO_X_OPT( fc );

        printf("dw $%04x, $%04x, $%04x\t\t; %+.6e / %+.6e = %+.6e\n", ba, bb, bc, fa, fb, fc);
    }
#endif



#if 0
    for (i = 0; i < max_add; i+=3 ) {
        
        ba = data_add[i+0];
        bb = data_add[i+1];        
        
#if 0
        fa = binary16_to_float( ba );
        ba = FLOAT_TO_X_OPT( fa );
        fb = binary16_to_float( bb );
        bb = FLOAT_TO_X_OPT( fb );
#endif
        
        fa = X_TO_FLOAT( ba );
        fb = X_TO_FLOAT( bb );

        fc = fa + fb;        
        bc = FLOAT_TO_X_OPT( fc );

        printf("dw $%04x, $%04x, $%04x\t\t; %+.6e + %+.6e = %+.6e\n", ba, bb, bc, fa, fb, fc);
    }
#endif


#if 0
    for (i = 0; i < max_frac; i+=3 ) {

        ba = data_frac[i+0];
#if 1
        fa = binary16_to_float( ba );
        ba = FLOAT_TO_X_OPT( fa );
#endif
        
        fa = X_TO_FLOAT( ba );

        fc = fa - (int) fa;        
        bc = FLOAT_TO_X_OPT( fc );

        printf("dw $%04x, $%04x\t\t; INT(%+.6e) = %+.6e\n", ba, bc, fa, fc);
    }
#endif

#if 0
    for (i = 0; i < max_fint; i+=2 ) {

        ba = data_fint[i+0];
#if 0
        fa = binary16_to_float( ba );
        ba = FLOAT_TO_X_OPT( fa );
#endif
        fa = X_TO_FLOAT( ba );

        fc = (int) fa;        
        bc = FLOAT_TO_X_OPT( fc );

        printf("dw $%04x, $%04x\t\t; INT(%+.6e) = %+.6e\n", ba, bc, fa, fc);
    }
#endif

}


