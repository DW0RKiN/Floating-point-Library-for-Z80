if not defined @FMOD

    include "fint.asm"
    include "fdiv.asm"

;  Input: HL, BC
; Output: HL = abs(HL), flag => abs(BC) - abs(HL)
FCOMP:
        RES     7, L            ;  2:8
        LD      A, B            ;  1:4
        SUB     H               ;  1:4
        RET     nz              ;  1:11/5       if ( carry ) { H > B } else { H < B }
        LD      A, C            ;  1:4
        AND     MANT_MASK       ;  2:7
        SUB     L               ;  1:4
        RET                     ;  1:10
        
; Remainder after division
;  In: BC dividend, HL modulus
; Out: HL = BC % HL = BC - int(BC/HL) * HL = frac(BC/HL) * HL  => does not return correct results with larger exponent difference
; Pollutes: AF, BC, DE
@FMOD:
if not defined FMOD
; *****************************************
                    FMOD                ; *
; *****************************************
endif
        CALL    FCOMP               ;           abs(BC) - abs(HL), HL = abs(HL)
        JP      z, FMOD_FPMIN       ;
        JR      nc, FMOD_BC_GR      ;
        LD      H, B                ;   
        LD      L, C                ;
        RET                         ;   
FMOD_BC_GR:
        LD      D, H                ;           D = old H exp
        LD      H, B                ;           D = new H exp

        LD      A, L                ;  1:4
        OR      SIGN_MASK           ;  2:7
        LD      E, A                ;  1:4      E = 0 1mmm mmmm = HL_mantis >> 1

        LD      A, C                ;
        AND     SIGN_MASK           ;
        LD      L, A                ;           sign only
        
        LD      A, C                ;
        ADD     A, A                ;
        LD      C, A                ;           C = save BC_mantis = 1 mmmm mmm0

        LD      A, D                ;
        SUB     H                   ;
        LD      A, C                ;           A = load BC_mantis = 1 mmmm mmm0
        JR      z, FMOD_SAME_EXP    ;
FMOD_SUB:                           ;           BC_mantis - HL_mantis/2
        SUB     E                   ;
        JR      nc, FMOD_SUB        ;
        JR      z, FMOD_FPMIN       ;           FPMIN

FMOD_NORM:
        INC     H
FMOD_NORM_LOOP:
        DEC     H                   ;            
    if color_flow_warning = 0 && carry_flow_warning = 0
        RET     z                   ;           
    else
        JR      z, FMOD_UNDERFLOW   ;
    endif        
        ADD     A, A                ;
        JR      nc, FMOD_NORM_LOOP  ;
        DEC     H                   ;
        
        LD      C, A                ;           C = save BC_mantis = 1 mmmm mmmm
        LD      A, D                ;
        CP      H                   ;           
        LD      A, C                ;           A = load BC_mantis = 1 mmmm mmmm

        JR      c, FMOD_SUB         ;           if (old H_exp - new H_exp < 0) again
        JR      z, FMOD_SAME_EXP    ;

FMOD_EXIT:
        RL      L                   ;  2:8      sign out
        RRA                         ;           A = smmm mmmm
        LD      L, A                ;
        RET                         ;
                
FMOD_SAME_EXP:
        SLA     E                   ;  2:8      E = 2*E           
        CP      E                   ;
        JR      c, FMOD_EXIT        ;

        SUB     E                   ;
        JR      nz, FMOD_NORM       ;
        ; fall
        
FMOD_FPMIN:                         ;           RET with reset carry
        LD      H, A                ;
        RET

        
; Input: H = 0, L = sign only        
FMOD_UNDERFLOW:
    if color_flow_warning
        CALL    UNDER_COL_WARNING   ;  3:17
    endif
    if carry_flow_warning
        SCF                         ;  1:4      carry = error
    endif        
        RET

endif
