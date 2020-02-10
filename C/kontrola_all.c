
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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
    #define MAX_NUMBER 255
#else
    #error Byla nalezena neocekavana hodnota v promenne TARGET!
#endif

#include "../C/float.h"


int main( int argc, const char* argv[] ) 
{

    if ( ! ((argc >> 1) & 1)) {
        fprintf(stderr, "Potrebuji 1 nebo 2 parametry!\n");
        exit(1);
    }
    
    
    unsigned short ba, bb, bc;
    double fa, fb, fc;
    
    sscanf(argv[1], "%hi", &ba);  //ba &= BI_MASK;        
    fa = X_TO_DOUBLE( ba );
    printf("A: "); PRINT_FP_TO_X(stdout, fa);
    
    if ( argc == 3 ) {
        sscanf(argv[2], "%hi", &bb);  //bb &= BI_MASK;
        fb = X_TO_DOUBLE( bb );
        printf("B: "); PRINT_FP_TO_X(stdout, fb);
    }
    putchar('\n');  //----------------------------------------
    
    fc = fa * fa;
    printf("A * A = %f = %f^2\t\t\t", fc, fa);
    PRINT_FP_TO_X(stdout, fc);

    if ( fa >= 0 ) {    
        fc = sqrt(fa);
        printf("%f = sqrt(%f)\t\t\t", fc, fa);
        PRINT_FP_TO_X(stdout, fc);
    }
    
    if ( fa != 0 ) {
        fc = 1 / fa;
        printf("1 / A = %f = 1 / %f  \t\t", fc, fa);
        PRINT_FP_TO_X(stdout, fc);
    }

    fc = (int) fa;
    printf("INT(A) = %f = INT(%f)\t\t", fc, fa);
    PRINT_FP_TO_X(stdout, fc);

    fc = fa - (int) fa;
    printf("A - INT(A) = %f = %f - INT(A)\t", fc, fa);
    PRINT_FP_TO_X(stdout, fc);

    putchar('\n');  //-----------------------------------------
     
    if ( argc == 3 ) {
        fc = fb * fb;
        printf("B * B = %f = %f^2\t\t\t", fc, fb);
        PRINT_FP_TO_X(stdout, fc);

        if ( fb >= 0 ) {    
            fc = sqrt(fb);
            printf("%f = sqrt(%f)\t\t\t", fc, fb);
            PRINT_FP_TO_X(stdout, fc);
        }
    
        if ( fb != 0 ) {
            fc = 1 / fb;
            printf("1 / B = %f = 1 / %f  \t\t", fc, fb);
            PRINT_FP_TO_X(stdout, fc);
        }
        
        fc = (int) fb;
        printf("INT(B) = %f = INT(%f)\t\t", fc, fb);
        PRINT_FP_TO_X(stdout, fc);

        fc = fb - (int) fb;
        printf("B - INT(B) = %f = %f - INT(B)\t", fc, fb);
        PRINT_FP_TO_X(stdout, fc);
    
        putchar('\n');  //------------------------------------

        fc = fa + fb;
        printf("A + B = %f = %f + %f\t", fc, fa, fb);
        PRINT_FP_TO_X(stdout, fc);

        fc = fa - fb;
        printf("A - B = %f = %f - %f\t", fc, fa, fb);
        PRINT_FP_TO_X(stdout, fc);
        
        fc = fa * fb;
        printf("A * B = %f = %f * %f\t", fc, fa, fb);
        PRINT_FP_TO_X(stdout, fc);

        fc = fa / fb;
        printf("A / B = %f = %f / %f\t", fc, fa, fb);
        PRINT_FP_TO_X(stdout, fc);
        
        fc = fmod(fa, fb);
        printf("A %% B = %f = %f %% %f\t", fc, fa, fb);
        PRINT_FP_TO_X(stdout, fc);

    }      
}


