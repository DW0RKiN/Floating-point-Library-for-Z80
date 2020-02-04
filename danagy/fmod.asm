if not defined @FMOD


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
        RES     7, H                ;  2:8
        LD      A, B                ;  1:4
        AND     SIGN_MASK           ;
        LD      D, A                ;           sign only
        XOR     B                   ;           
        SUB     H                   ;
        JR      nc, FMOD_BC_GR_EXP  ;
FMOD_HL_GR:
        LD      H, B                ;   
        LD      L, C                ;
        RET                         ;   
FMOD_BC_GR_EXP:
        LD      E, A                ;           diff exp        
        LD      A, C                ;  1:4 
        JR      nz, FMOD_SUB        ;
        CP      L                   ;  1:4
        JR      z, FMOD_FPMIN       ;
        JR      c, FMOD_HL_GR       ;

FMOD_SUB:                           ;           BC_mantis - HL_mantis
        SUB     L                   ;
        JR      z, FMOD_FPMIN       ;           FPMIN
        JR      nc, FMOD_NORM       ;
        
; FMOD_ADD_HALF:           
        DEC     E                   ;            
        JP      m, FMOD_STOP        ;
        
        ADD     A, A                ;
        JR      nc, FMOD_X          ;
        ADD     A, L                ;
        JR      c, FMOD_SUB         ;
        DB      $06                 ;           LD B, $85
FMOD_X:
        ADD     A, L                ;
        
FMOD_NORM:
        ADD     A, A                ;
        DEC     E                   ;            
        JR      nc, FMOD_NORM       ;
        
        JP      p, FMOD_SUB         ;           E >= 0

        LD      L, A                ;
        LD      A, H                ;
        ADD     A, E                ;
        XOR     D                   ;
        LD      H, A                ;
        XOR     D                   ;
        RET     p                   ;
;       fall

; Input: D = sign only        
FMOD_UNDERFLOW:
        LD      H, D                ;
        LD      L, $00              ;
    if color_flow_warning
        CALL    UNDER_COL_WARNING   ;  3:17
    endif
    if carry_flow_warning
        SCF                         ;  1:4      carry = error
    endif        
        RET                         ;
        
FMOD_STOP:
        ADD     A, L                ;
        LD      L, A                ;
        LD      A, H                ;
        XOR     D                   ;
        LD      H, A                ;
        RET                         ;
        
FMOD_FPMIN:                         ;           RET with reset carry
        LD      L, $00              ;
        LD      H, D                ;
        RET                         ;

    include "color_flow_warning.asm"

endif
