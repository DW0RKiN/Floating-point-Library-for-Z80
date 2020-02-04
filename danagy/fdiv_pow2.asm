if not defined @FDIV_POW2

include "color_flow_warning.asm"

;  Input:  BC, HL with a mantissa equal to 1.0 (eeee eeee s000 0000)
; Output: HL = BC / HL = BC / (1.0 * 2^HL_exp) = BC * 1.0 * 2^-HL_exp, if ( overflow or underflow ) set carry
; Pollutes: AF, BC, DE

; if ( 1.m = 1.0 ) => 1/(2^x * 1.0) = 1/2^x * 1/1.0 = 2^-x * 1.0    
; New_sign = BC_sign ^ HL_sign
; New_exp  = (BC_exp - BIAS) + ( BIAS - HL_exp ) + BIAS = BIAS + BC_exp - HL_exp
; New_mant = BC_mant * 1.0 = BC_mant
@FDIV_POW2:
if not defined FDIV_POW2
; *****************************************
                 FDIV_POW2              ; *
; *****************************************
endif
        LD      A, B                ;  1:4      BC_exp
        SUB     H                   ;  1:4     -HL_exp
        ADD     A, BIAS             ;  2:7
        LD      L, A                ;  1:4
        XOR     H                   ;  1:4      xor sign
        XOR     B                   ;  1:4      xor sign
        JP      m, FDIV_POW2_FLOW   ;  3:10
        LD      H, L                ;  1:4
        LD      L, C                ;  1:4
        RET                         ;  1:10
FDIV_POW2_FLOW:        
        BIT     6, L                ;  2:8      sign+($00..$3F)=overflow, sign+($41..$7F)=underflow
        JR      z, FDIV_POW2_OVER   ;  2:12/7
        
FDIV_POW2_UNDER:      
        LD      A, L                ;  1:4
        CPL                         ;  1:4
        AND     SIGN_MASK           ;  2:7
        LD      H, A                ;  1:4
        LD      L, $00              ;  2:7
    if color_flow_warning
        CALL    UNDER_COL_WARNING   ;  3:17
    endif
    if carry_flow_warning
        SCF                         ;  1:4      carry = error
    endif
        RET                         ;  1:10

FDIV_POW2_OVER:
        LD      A, L                ;  1:4
        CPL                         ;  1:4
        OR      EXP_MASK            ;  2:7
        LD      H, A                ;  1:4
        LD      L, $FF              ;  2:7
    if color_flow_warning
        CALL    OVER_COL_WARNING    ;  3:17
    endif
    if carry_flow_warning
        SCF                         ;  1:4      carry = error
    endif
        RET                         ;  1:10
     
endif
