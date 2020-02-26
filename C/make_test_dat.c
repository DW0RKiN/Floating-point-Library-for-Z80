
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



__uint16_t plus_inf() {
    return MASK_EXP + MASK_MANTISA;     // + inf
}

__uint16_t minus_inf() {
    return 0xffff;                      // - inf
}

__uint16_t rand_positive() {
    return rand() & ( 0xFFFF - MASK_SIGN ); 
}

__uint16_t rand_negative() {
    return (rand() & ( 0xFFFF )) | MASK_SIGN; 
}

__uint16_t rand_all() {
    return rand() & ( 0xFFFF ); 
}

#define LIMIT 100

void both_positive(int pos, __uint16_t *a, __uint16_t *b) {
    a[pos] = rand_positive(); 
    if ( pos < LIMIT ) 
        b[pos] = plus_inf();
    else
        b[pos] = (rand() & MASK_MANTISA) + ((pos << EXP_POS) & MASK_EXP);
}
    
void both_same(int pos, __uint16_t *a, __uint16_t *b) {
    if ( pos & 1 ) {               // jen kladna cisla
        a[pos] = rand_positive();        
        if ( pos < LIMIT ) 
            b[pos] = plus_inf();
        else
            b[pos] = (rand() & MASK_MANTISA) + ((pos << EXP_POS) & MASK_EXP);
    } else {                    // jen zaporna cisla
        a[pos] = (rand() & ( 0xFFFF - MASK_SIGN )) + MASK_SIGN;        
        if ( pos < LIMIT ) 
            b[pos] = 0xFFFF;                       // - inf
        else
            b[pos] = (rand() & MASK_MANTISA) + ((pos << EXP_POS) & MASK_EXP) + MASK_SIGN;
    }
}
    
void both_all(int pos, __uint16_t *a, __uint16_t *b) {

    a[pos] = rand_all();        
    if ( pos < LIMIT ) 
        b[pos] = (rand() & MASK_SIGN) + MASK_EXP + MASK_MANTISA;
    else
        b[pos] = rand() & ( MASK_SIGN + MASK_MANTISA ) + ((pos << EXP_POS) & MASK_EXP);
}
    

int check_duplicate(int pos, __uint16_t *pole_a, __uint16_t *pole_b) {
    int i = pos - 1;
    while ( i > 0 ) {
        if ( pole_a[i] == pole_a[pos] && pole_b[i] == pole_b[pos]) return 1;
        i--;
    }
    return 0;
}


void ukonci_radek(__uint16_t num) {
    if ( num ==  0 ) printf("\t = 0");
    if ( num ==  MASK_SIGN ) printf("\t =-0");
    if ( num ==  0xFFFF ) printf("\t = -INF");
    if ( num ==  0xFFFF - MASK_SIGN ) printf("\t = +INF");
    printf("\n");
}

    

int main( int argc, const char* argv[] ) 
{

    if ( RAND_MAX < 0xFFFF)
    {
        fprintf(stderr,"RAND_MAX = %X < 0xFFFF\n", RAND_MAX);
        return 1;
    }

    srand(time(NULL)); // randomize seed




#define MAX_LINE 65536
    
    __uint16_t input1[MAX_LINE];
    __uint16_t input2[MAX_LINE];
    __uint16_t output[MAX_LINE];
        
    int pos = 0;
    double da, db, dc;
        
    while ( 1 ) {

#if defined FADDP
        #warning FADDP
        both_same(pos, input1, input2);
        if ( check_duplicate( pos, input1, input2 )) continue;
        da = X_TO_DOUBLE(input1[pos]);
        db = X_TO_DOUBLE(input2[pos]);
        dc = da + db;
        output[pos] = DOUBLE_TO_X_OPT( dc );
        printf("dw $%04x, $%04x, $%04x\t\t; %+.3e + %+.3e = %+.3e", input1[pos], input2[pos], output[pos], da, db, dc);
        ukonci_radek(output[pos]);
        pos++;
        if ( pos > SPACE/6 ) break;
#elif defined  FSUBP
        #warning FSUBP
        both_same( pos, input1, input2);
        if ( check_duplicate( pos, input1, input2 )) continue;
        da = X_TO_DOUBLE(input1[pos]);
        db = X_TO_DOUBLE(input2[pos]);
        dc = da - db;
        output[pos] = DOUBLE_TO_X_OPT( dc );
        printf("dw $%04x, $%04x, $%04x\t\t; %+.3e - %+.3e = %+.3e", input1[pos], input2[pos], output[pos], da, db, dc);
        ukonci_radek(output[pos]);
        pos++;
        if ( pos > SPACE/6 ) break;

#elif defined  FMUL
        #warning FMUL
        both_all( pos, input1, input2);
        if ( check_duplicate( pos, input1, input2 )) continue;
        da = X_TO_DOUBLE(input1[pos]);
        db = X_TO_DOUBLE(input2[pos]);
        dc = da * db;
        output[pos] = DOUBLE_TO_X_OPT( dc );
        printf("dw $%04x, $%04x, $%04x\t\t; %+.3e * %+.3e = %+.3e", input1[pos], input2[pos], output[pos], da, db, dc);
        ukonci_radek(output[pos]);
        pos++;
        if ( pos > (SPACE-TABLE_MUL)/6 ) break;
#elif defined  FDIV
        #warning FDIV
        both_all( pos, input1, input2);
        if ( check_duplicate( pos, input1, input2 )) continue;
        da = X_TO_DOUBLE(input1[pos]);
        db = X_TO_DOUBLE(input2[pos]);
        dc = da / db;
        output[pos] = DOUBLE_TO_X_OPT( dc );
        printf("dw $%04x, $%04x, $%04x\t\t; %+.3e / %+.3e = %+.3e", input1[pos], input2[pos], output[pos], da, db, dc);
        ukonci_radek(output[pos]);
        pos++;
        if ( pos > (SPACE-TABLE_DIV)/6 ) break;

#elif defined  FMOD
        #warning FMOD
        both_all( pos, input1, input2);
        if ( check_duplicate( pos, input1, input2 )) continue;
        if ( ( (input1[pos] | MASK_SIGN) < (input2[pos] | MASK_SIGN) ) && ( rand() & 0x3 ) != 0 ) continue; 
        da = X_TO_DOUBLE(input1[pos]);
        db = X_TO_DOUBLE(input2[pos]);
        dc = fmod( da, db );
        output[pos] = DOUBLE_TO_X_OPT( dc );
        printf("dw $%04x, $%04x, $%04x\t\t; %+.3e %% %+.3e = %+.3e", input1[pos], input2[pos], output[pos], da, db, dc);
        ukonci_radek(output[pos]);
        pos++;
        if ( pos > SPACE/6 ) break;
#elif defined FRAC
        // frac
        #warning FRAC
        if ( pos < 4+MANT_SIZE ) {
            input1[pos] = MAKE_X( rand() & 1, pos-2, MASK_MANTISA );         
        }
        else
            input1[pos] = rand_all();
        da = X_TO_DOUBLE(input1[pos]);
        dc = fmod(da, 1.0);
        if ( dc == 0  && (rand() & 0x3f) != 0 && pos >= 4+MANT_SIZE  ) continue;
        if ( da == dc && (rand() & 0x3f) != 0 && pos >= 4+MANT_SIZE  ) continue;
        
        output[pos] = DOUBLE_TO_X_OPT( dc );
        if ( check_duplicate( pos, input1, output )) continue;        
        printf("dw $%04x, $%04x\t\t; %+12g %% 1 = %+.3e", input1[pos], output[pos], da, dc);
        if ( input1[pos] == output[pos] ) printf(" =");
        ukonci_radek(output[pos]);
        pos++;
        if ( pos > SPACE/4 ) break;
        
#elif defined  FDIV_POW2
        #warning FDIV_POW2
        both_all( pos, input1, input2);
        input2[pos] &= MASK_SIGN + MASK_EXP; 
        if ( check_duplicate( pos, input1, input2 )) continue;
        da = X_TO_DOUBLE(input1[pos]);
        db = X_TO_DOUBLE(input2[pos]);
        dc = da / db;
        output[pos] = DOUBLE_TO_X_OPT( dc );
        printf("dw $%04x, $%04x, $%04x\t\t; %+.3e / %+.3e = %+.3e", input1[pos], input2[pos], output[pos], da, db, dc);
        ukonci_radek(output[pos]);
        pos++;
        if ( pos > SPACE/6 ) break;

#elif defined  FPOW2
    // pow2()
        input1[pos] = rand_all();
        da = X_TO_DOUBLE(input1[pos]);
        dc = da * da;
        output[pos] = DOUBLE_TO_X_OPT( dc );
        if ( check_duplicate( pos, input1, output )) continue;
        printf("dw $%04x, $%04x\t\t; (%+.3e)^2 = %+.3e", input1[pos], output[pos], da, dc);
        ukonci_radek(output[pos]);
        pos++;
        if ( pos > (SPACE-TABLE_SQRT)/4 ) break;
#elif defined  FSQRT
        #warning FSQRT
    // fsqrt()
        input1[pos] = rand_positive();
        da = X_TO_DOUBLE(input1[pos]);
        dc = pow(da, 0.5);
        output[pos] = DOUBLE_TO_X_OPT( dc );
        if ( check_duplicate( pos, input1, output )) continue;
        printf("dw $%04x, $%04x\t\t; (%+.3e)^0.5 = %+.3e", input1[pos], output[pos], da, dc);
        ukonci_radek(output[pos]);
        pos++;
        if ( pos > (SPACE-TABLE_SQRT)/4 ) break;
#elif defined  FLN
        #warning FLN
    // ln()
        input1[pos] = rand_all();
        da = X_TO_DOUBLE(input1[pos]);
        if ( da < 0 ) da = -da;
        dc = log(da);
        output[pos] = DOUBLE_TO_X_OPT( dc );
        if ( check_duplicate( pos, input1, output )) continue;
        printf("dw $%04x, $%04x\t\t; ln(%+.3e) = %+.3e", input1[pos], output[pos], da, dc);
        ukonci_radek(output[pos]);
        pos++;
        if ( pos > (SPACE-TABLE_LN)/4 ) break;    
#elif defined  FEXP
        #warning FEXP
    // exp()
        input1[pos] = rand_all();
        da = X_TO_DOUBLE(input1[pos]);
        dc = pow(2.7182818284590452353602875,da);
        if (( dc == 0 || dc == 0xffff - MASK_SIGN || dc == BIAS * 256) && (rand() & 0xff) != 0) continue;
        output[pos] = DOUBLE_TO_X_OPT( dc );
        if ( check_duplicate( pos, input1, output )) continue;
        printf("dw $%04x, $%04x\t\t; exp(%+.3e) = %+.3e", input1[pos], output[pos], da, dc);
        ukonci_radek(output[pos]);
        pos++;
        if ( pos > (SPACE-TABLE_MUL)/4 ) break;    

#elif defined  FWLD
        #warning FWLD
    // load world
        input1[pos] = rand() & 0xffff;
        da = input1[pos];
        dc = da;
        output[pos] = DOUBLE_TO_X_OPT( dc );
        if ( check_duplicate( pos, input1, output )) continue;
        printf("dw $%04x, $%04x\t\t; 1.0 * %i = %+.3e", input1[pos], output[pos], input1[pos], dc);
        ukonci_radek(output[pos]);
        pos++;
        if ( pos > SPACE/4 ) break;    

#elif defined FWST
        #warning FWST
    // store world
        input1[pos] = rand_all();
        da = X_TO_DOUBLE(input1[pos]);
        if ( da < 0 ) dc = -da; else dc = da;
            
        if ( dc > 65535 ) {
            dc = 65535;
            if ((rand() & 0x7F) != 0) continue;
        }
        else if ( dc <= 0.5 ) {
            dc = 0;
            if ((rand() & 0x7F) != 0) continue;
        }
        else if ( dc < 1 ) 
            dc = 1;
        else 
            dc += 0.5;
        
        output[pos] = (int) dc;
        if ( check_duplicate( pos, input1, output )) continue;
        printf("dw $%04x, $%04x\t\t; (int) (%+.3e) = %i", input1[pos], output[pos], da, output[pos]);
        ukonci_radek(output[pos]);
        pos++;
        if ( pos > SPACE/4 ) break;

#else
    #error Necekana hodnota VARIANTY!
#endif

    }
    
    printf("; lines: %i\n", pos);
    
}


