if not defined @FMOD

    include "color_flow_warning.asm"
    
;  Input: HL, BC
; Output: abs(HL), flag => abs(BC) - abs(HL)
FCOMP:
        RES     7, H            ;  2:8  
        LD      A, B            ;  1:4
        AND     $FF - SIGN_MASK ;  2:7
        SUB     H               ;  1:4
        RET     nz              ;  1:11/5       if ( carry ) { H > B } else { H < B }
        LD      A, C            ;  1:4
        SUB     L               ;  1:4
        RET                     ;  1:10
    
    
; Remainder after division
;  In: BC dividend, HL modulus
; Out: HL remainder, HL = BC % HL       
; BC % HL = BC - int(BC/HL) * HL = frac(BC/HL) * HL  => does not return correct results with larger exponent difference
; Pollutes: AF, BC, DE

@FMOD:
if not defined FMOD
; *****************************************
                    FMOD                ; *
; *****************************************
endif        

        CALL    FCOMP               ;           BC - HL
        JP      z, FMOD_UNDERFLOW   ;
        JR      nc, FMOD_BC_GR      ;
        LD      H, B                ;   
        LD      L, C                ;
        RET                         ;   
FMOD_BC_GR:
        EX      DE, HL              ;
        LD      H, B                ;
        LD      L, C                ;           HL % DE

        LD      A, D                ;
        AND     EXP_MASK            ;
        LD      B, A                ;

        LD      A, H                ;
        AND     EXP_MASK            ;
        SUB     B                   ;
        RRCA                        ;
        RRCA                        ;
        LD      C, A                ;

        LD      A, H                ;
        AND     SIGN_MASK           ;
        OR      B                   ;
        LD      B, A                ;
        
        LD      A, $02              ;  2:7
        RR      H                   ;
        RR      L                   ;
        RRA                         ;           reset carry
        RR      H                   ;
        RR      L                   ;
        RRA                         ;           set carry
        LD      H, L                ;
        RR      H                   ;
        RRA                         ;
        LD      L, A                ;           HL = 1mmm mmmm mmm0 0000
        
        LD      A, $02              ;  2:7
        RR      D                   ;
        RR      E                   ;
        RRA                         ;           reset carry
        RR      D                   ;
        RR      E                   ;
        RRA                         ;           set carry
        LD      D, E                ;
        RR      D                   ;
        RRA                         ;
        LD      E, A                ;           DE = 1mmm mmmm mmm0 0000
        
        JR      FMOD_SUB            ;
FMOD_BACK:
        ADD     HL, DE              ;
FMOD_SRL_DE:        
        SRL     D                   ;
        RR      E                   ;

FMOD_SUB:                           ;           HL - DE
if 0
        LD      A, H
        CP      D
        JR      c, FMOD_SRL_DE      ;
else
        OR      A                   ;
endif
        SBC     HL, DE              ;
        JR      c, FMOD_BACK        ;
        RET     z                   ;           FPMIN

FMOD_SHIFT_HL:
        DEC     C                   ;
        ADC     HL, HL              ;
        JP      p, FMOD_SHIFT_HL    ;
        
        LD      A, D                ;  1:4
        ADD     A, A                ;  1:4
        JR      c, FMOD_DE_OK       ;  2:12/7
        SLA     E                   ;  2:8
        RL      D                   ;  2:8
FMOD_DE_OK:

        LD      A, C                ;
        OR      A                   ;
        JP      z, FMOD_SAME_EXP    ;
        JP      p, FMOD_SUB         ;
        
        ADD     A, A                ;           -2x
        ADD     A, A                ;           -4x
        ADD     A, B                ;
        XOR     B                   ;
        JP      m, FMOD_UNDERFLOW   ;
        XOR     B                   ;
        LD      B, A                ;

FMOD_NORM:                          ;           HL  = 1mmm mmmm mmmm mmmm
        ADD     HL, HL              ;           HL  = mmmm mmmm mmmm mmm0
        ADD     HL, HL              ;           HL  = mmmm mmmm mmmm mm00
        LD      A, $00              ;
        ADC     A, A                ;           AHL = 0000 000m mmmm mmmm mmmm mm00
        ADD     HL, HL              ;            HL = mmmm mmmm mmmm m000
        ADC     A, A                ;           AHL = 0000 00mm mmmm mmmm mmmm m000
        OR      B                   ;           RET with reset carry
        LD      L, H                ;
        LD      H, A                ;
        RET                         ;

FMOD_SAME_EXP:
        SBC     HL, DE              ;
        RET     z                   ;           FPMIN

        JR      nc, FMOD_SHIFT_HL   ;
        
        ADD     HL, DE              ;
        JR      FMOD_NORM

FMOD_UNDERFLOW:
        LD      A, B                ;
        AND     SIGN_MASK           ;
        LD      H, A                ;
        LD      L, $00              ;
    if color_flow_warning
        CALL    UNDER_COL_WARNING   ;  3:17
    endif
    if carry_flow_warning
        SCF                         ;  1:4      carry = error
    endif        
        RET                         ;

endif
