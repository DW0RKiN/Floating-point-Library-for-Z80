
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
    #define MAX_NUMBER 1023
#else
    #error Byla nalezena neocekavana hodnota v promenne TARGET!
#endif

#include "../C/float.h"


void zarovnej(char * veta)
{
    int i = 0, j = 0;
    while ( veta[j] ) {
        veta[i] = veta[j];
        if (veta[j] == ' ' && veta[j+1] == ' ') ;
        else if (veta[j] == '0' && veta[j-1] != '.') 
        {
            int k = j;
            while ( veta[++k] == '0' ) ;
            if ( ( veta[k] > '0' && veta[k] <= '9') || veta[k] == '.' ) i++;
        }
        else
            i++;
        j++;
    }
    do 
        veta[i++] = ' ';
    while ( i < 48 );
    veta[i] = 0;
    printf("%s", veta);
}


int main( int argc, const char* argv[] ) 
{

    if ( ! ((argc >> 1) & 1)) {
        fprintf(stderr, "Potrebuji 1 nebo 2 parametry!\n");
        exit(1);
    }
    
    
    unsigned short ba, bb, bc;
    double fa, fb, fc;
    char veta[1024];
    
    if ((argv[1][1] & 0xDF) == 'X' ) {
        sscanf(argv[1], "%hi", &ba);  //ba &= BI_MASK;        
        fa = X_TO_DOUBLE( ba );
    } else {
        sscanf(argv[1], "%lf", &fa);          
        ba = DOUBLE_TO_X_OPT( fa );        
    }
    
    printf("A: "); PRINT_FP_TO_X(stdout, fa);
    
    if ( argc == 3 ) {

        if ((argv[2][1] & 0xDF) == 'X' ) {
            sscanf(argv[2], "%hi", &bb);  //bb &= BI_MASK;        
            fb = X_TO_DOUBLE( bb );
        } else {
            sscanf(argv[2], "%lf", &fb);          
            bb = DOUBLE_TO_X_OPT( fb );        
        }
        printf("B: "); PRINT_FP_TO_X(stdout, fb);
    }
    putchar('\n');  //----------------------------------------
    
    fc = fa * fa;
    sprintf(veta, "A * A = %f = %f^2", fc, fa);
    zarovnej(veta);
    PRINT_FP_TO_X(stdout, fc);

    if ( fa >= 0 ) {    
        fc = sqrt(fa);
        sprintf(veta, "%f = sqrt(%f)", fc, fa);
        zarovnej(veta);
        PRINT_FP_TO_X(stdout, fc);
    }
    
    if ( fa != 0 ) {
        fc = 1 / fa;
        sprintf(veta,"1 / A = %f = 1 / %f", fc, fa);
        zarovnej(veta);
        PRINT_FP_TO_X(stdout, fc);
    }

    fc = (int) fa;
    sprintf(veta,"INT(A) = %f = INT(%f)", fc, fa);
    zarovnej(veta);
    PRINT_FP_TO_X(stdout, fc);

    fc = fa - (int) fa;
    sprintf(veta,"A - INT(A) = %f = %f - INT(A)", fc, fa);
    zarovnej(veta);
    PRINT_FP_TO_X(stdout, fc);

    putchar('\n');  //-----------------------------------------
     
    if ( argc == 3 ) {
        fc = fb * fb;
        sprintf(veta,"B * B = %f = %f^2", fc, fb);
        zarovnej(veta);
        PRINT_FP_TO_X(stdout, fc);

        if ( fb >= 0 ) {    
            fc = sqrt(fb);
            sprintf(veta,"%f = sqrt(%f)", fc, fb);
            zarovnej(veta);
            PRINT_FP_TO_X(stdout, fc);
        }
    
        if ( fb != 0 ) {
            fc = 1 / fb;
            sprintf(veta,"1 / B = %f = 1 / %f", fc, fb);
            zarovnej(veta);
            PRINT_FP_TO_X(stdout, fc);
        }
        
        fc = (int) fb;
        sprintf(veta,"INT(B) = %f = INT(%f)", fc, fb);
        zarovnej(veta);
        PRINT_FP_TO_X(stdout, fc);

        fc = fb - (int) fb;
        sprintf(veta,"B - INT(B) = %f = %f - INT(B)", fc, fb);
        zarovnej(veta);
        PRINT_FP_TO_X(stdout, fc);
    
        putchar('\n');  //------------------------------------

        fc = fa + fb;
        sprintf(veta,"A + B = %f = %f + %f", fc, fa, fb);
        zarovnej(veta);
        PRINT_FP_TO_X(stdout, fc);

        fc = fa - fb;
        sprintf(veta,"A - B = %f = %f - %f", fc, fa, fb);
        zarovnej(veta);
        PRINT_FP_TO_X(stdout, fc);
        
        fc = fa * fb;
        sprintf(veta,"A * B = %f = %f * %f", fc, fa, fb);
        zarovnej(veta);
        PRINT_FP_TO_X(stdout, fc);

        fc = fa / fb;
        sprintf(veta,"A / B = %f = %f / %f", fc, fa, fb);
        zarovnej(veta);
        PRINT_FP_TO_X(stdout, fc);
        
        fc = fmod(fa, fb);
        sprintf(veta,"A %% B = %f = %f %% %f", fc, fa, fb);
        zarovnej(veta);
        PRINT_FP_TO_X(stdout, fc);

    }      
}


