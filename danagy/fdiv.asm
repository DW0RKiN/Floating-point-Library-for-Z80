if not defined @FDIV

    include "fdiv_pow2.asm"

; ---------------------------------------------
;  Input:  BC , HL
; Output: HL = BC / HL   =>   DE = 1 / HL   =>   HL = BC * DE
; if ( 1.m = 1.0 ) => 1/(2^x * 1.0) = 1/2^x * 1/1.0 = 2^-x * 1.0    
; if ( 1.m > 1.0 ) => 1/(2^x * 1.m) = 1/2^x * 1/1.m = 2^-x * 0.9999 .. 0.5001   =>   2^(-x-1) * 1.0002 .. 1.9998    
; Pollutes: AF, BC, DE
@FDIV:
if not defined FDIV
; *****************************************
                    FDIV                ; *
; *****************************************
endif
        LD      A, L                ;  1:4
        OR      A                   ;  1:4
        JR      z, FDIV_POW2        ;  2:12/7
        LD      A, H                ;               NegE - 1 = (0 - (E - BIAS)) + BIAS - 1 = 2*BIAS - E - 1 = 128 - E - 1 = 127 - E
        XOR     EXP_MASK            ;  2:7          NegE = 127 - E = $7F - E = $7F XOR E     
        LD      D, A                ;  1:4

        LD      H, DIVTAB/256       ;  2:7
        LD      E, (HL)             ;  1:7
    if not defined @FMUL
        ; continues with FMUL (HL = BC * DE), DE = 1 / HL
        
    include "fmul.asm"

    else
        JP      @FMUL               ;  3:10         continues with FMUL (HL = BC * DE), DE = 1 / HL

    endif
     
endif
        
