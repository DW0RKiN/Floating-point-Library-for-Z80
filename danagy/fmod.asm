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

        RES     7, H                ;  2:8      HL = abs(HL), exp HL will be used for the result
        LD      A, B                ;  1:4
        AND     SIGN_MASK           ;  2:7
        LD      D, A                ;  1:4      Result sign only
        XOR     B                   ;  1:4
        SUB     H                   ;  1:4
        JR      c, FMOD_HL_GR       ;  2:12/7
        LD      E, A                ;  1:4      diff exp        
        LD      A, C                ;  1:4 
        JR      nz, FMOD_SUB        ;  2:12/7
        
        CP      L                   ;  1:4
        JR      z, FMOD_FPMIN       ;  2:12/7
        JR      c, FMOD_HL_GR       ;  2:12/7

FMOD_SUB:                           ;           BC_mantis - HL_mantis
        SUB     L                   ;  1:4
        JR      z, FMOD_FPMIN       ;  2:7/11   FPMIN
        JR      nc, FMOD_NORM       ;  2:7/11
        
; FMOD_ADD_HALF:           
        DEC     E                   ;  1:4       
        JP      m, FMOD_STOP        ;  3:10
        
        ADD     A, A                ;  1:4
        JR      nc, FMOD_X          ;  2:7/11
        ADD     A, L                ;  1:4
        JR      c, FMOD_SUB         ;  2:7/11
        DB      $06                 ;           LD B, $85
FMOD_X:
        ADD     A, L                ;  1:4
        
FMOD_NORM:
        ADD     A, A                ;  1:4
        DEC     E                   ;  1:4       
        JR      nc, FMOD_NORM       ;  2:7/11
        
        JP      p, FMOD_SUB         ;  3:10     E >= 0

        LD      L, A                ;  1:4
        LD      A, H                ;  1:4
        ADD     A, E                ;  1:4
        XOR     D                   ;  1:4
        LD      H, A                ;  1:4
        XOR     D                   ;  1:4
        RET     p                   ;  1:5/11
;       fall

; Input: D = sign only        
FMOD_UNDERFLOW:
        LD      H, D                ;  1:4
        LD      L, $00              ;  2:7
    if color_flow_warning
        CALL    UNDER_COL_WARNING   ;  3:17
    endif
    if carry_flow_warning
        SCF                         ;  1:4      carry = error
    endif        
        RET                         ;  1:10
        
FMOD_STOP:
        ADD     A, L                ;  1:4
        LD      L, A                ;  1:4
        LD      A, H                ;  1:4
        XOR     D                   ;  1:4
        LD      H, A                ;  1:4
        RET                         ;  1:10
        
FMOD_HL_GR:
    if carry_flow_warning
        OR      A                   ;  1:4
    endif
        LD      H, B                ;  1:4
        LD      L, C                ;  1:4
        RET                         ;  1:10

FMOD_FPMIN:                         ;           RET with reset carry
        LD      L, $00              ;  2:7
        LD      H, D                ;  1:4
        RET                         ;  1:10

    include "color_flow_warning.asm"

endif
