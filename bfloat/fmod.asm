if not defined @FMOD
        
; Remainder after division.
;  In: BC dividend, HL modulus
; Out: HL = BC % HL = BC - int(BC/HL) * HL = frac(BC/HL) * HL  => does not return correct results with larger exponent difference
; Pollutes: AF, BC, DE
@FMOD:
if not defined FMOD
; *****************************************
                    FMOD                ; *
; *****************************************
endif
        include "mcmpa.asm"
        MCMPA   H, L, B, C          ;           flag: abs(HL) - abs(BC)
        JP      c, FMOD_BC_GR       ;  3:10
        
        LD      H, B                ;  1:4
        LD      L, C                ;  1:4
        RET     nz                  ;  1:11/5
        LD      H, A                ;  1:4
        LD      A, C                ;  1:4
        AND     SIGN_MASK           ;  2:7
        LD      L, A                ;  1:4      FPMIN or FMMIN   
        RET                         ;  1:10
        
FMOD_BC_GR:
        LD      D, H                ;           D = old H exp
        LD      H, B                ;           H = new B exp

        LD      A, L                ;  1:4
        OR      SIGN_MASK           ;  2:7
        LD      E, A                ;  1:4      E = 0 1mmm mmmm

        LD      A, C                ;
        AND     SIGN_MASK           ;
        LD      L, A                ;           L = sign only
        
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
    if carry_flow_warning
        OR      A                   ;
    endif
        LD      L, A                ;
        RET                         ;
                
FMOD_SAME_EXP:
        SLA     E                   ;  2:8      E = 2*E           
        CP      E                   ;
        JR      c, FMOD_EXIT        ;

        SUB     E                   ;
        JR      nz, FMOD_NORM       ;
                                    ;           BC = 2^n * HL a bohuzel jsme odcitali stale poloviny (BC-HL/2) az jsme se dostali sem...
        LD      H, A                ;           FPMIN or FMMIN
        RET                         ;

; Input: H = 0, L = sign only        
FMOD_UNDERFLOW:
    if color_flow_warning
        CALL    UNDER_COL_WARNING   ;  3:17
    endif
    if carry_flow_warning
        SCF                         ;  1:4      carry = error
    endif        
        RET
        
    include "color_flow_warning.asm"

endif
