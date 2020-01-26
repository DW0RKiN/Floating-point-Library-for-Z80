if not defined @FDIV

    include "fdiv_pow2.asm"

    if not defined @FMUL
FDIV_UNDERFLOW:                     ;
        LD      A, E                ;  1:4
        XOR     C                   ;  1:4
        AND     SIGN_MASK           ;  2:7
        LD      H, D                ;  1:4
        LD      L, A                ;  1:4
        SCF                         ;  1:4          carry = error
        RET                         ;  1:10         HL = 0000 0000 s000 0000
    endif
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
        ADD     A, A                ;  1:4
        JR      z, FDIV_POW2        ;  2:12/7
                                    ;               NegE - 1 = (0 - (E - BIAS)) + BIAS - 1 = 2*BIAS - E - 1 = 254 - E - 1 = 253 - E
        LD      A, 253              ;  2:7               
        SUB     H                   ;  1:4          NegE = 253 - E
        LD      H, DIVTAB/256       ;  2:7
        LD      E, (HL)             ;  1:7
        LD      D, A                ;  1:4
        JR      nc, @FMUL           ;  2:7/12       continues with FMUL (HL = BC * DE), DE = 1 / HL

        ; A = $FE, $FF = -2, -1
        ADD     A, B                ;  1:4      
        LD      B, A                ;  1:4
        LD      D, $00              ;  2:7
    if not defined @FMUL
        JR      nc, FDIV_UNDERFLOW  ;  2:7/12       
        ; continues with FMUL (HL = BC * DE), DE = 1 / HL
        
    include "fmul.asm"

    else
        JR      c, @FMUL            ;  2:7/12       continues with FMUL (HL = BC * DE), DE = 1 / HL

FDIV_UNDERFLOW:                     ;
        LD      A, E                ;  1:4
        XOR     C                   ;  1:4
        AND     SIGN_MASK           ;  2:7
        LD      H, D                ;  1:4
        LD      L, A                ;  1:4
        SCF                         ;  1:4          carry = error
        RET                         ;  1:10         HL = 0000 0000 s000 0000
    endif
     
endif
        
