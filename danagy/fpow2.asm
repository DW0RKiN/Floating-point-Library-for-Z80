if not defined @FPOW2

; e = 2*exp
; mantisa < 2**0.5
; (2**exp * mantisa)**2 = 2**e * mantisa**2
; mantisa > 2**0.5
; (2**exp * mantisa)**2 = 2**e * (mantisa**2)/2 * 2 = 2**(e+1) * (mantisa**2)/2

; Square of a floating-point number
; In: HL = number to square
; Out: HL = HL * HL
; Pollutes: AF
@FPOW2:
if not defined FPOW2
; *****************************************
                    FPOW2                ; *
; *****************************************
endif
        ; A = (EXP-BIAS + EXP-BIAS) + BIAS = EXP-BIAS + EXP

        LD      A, PREDEL_POW2      ;  2:7
        CP      L                   ;  1:4
        LD      A, H                ;  1:4
        ADC     A, A                ;  1:4      2*exp+carry, abs(hl)
        LD      H, POW2TAB/256      ;  2:7
        LD      L, (HL)             ;  1:7
        SUB     BIAS                ;  2:7
        LD      H, A                ;  1:4
        RET     p                   ;  1:11/5

        JR      nc, FPOW2_OVERFLOW  ;  2:12/7   $BE..$80
        
FPOW2_UNDERFLOW:                    ;           $FF..$C0
        LD      HL, FPMIN           ;  3:10
    if color_flow_warning
        CALL    UNDER_COL_WARNING   ;  3:17
    endif
        RET                         ;  1:10
                
FPOW2_OVERFLOW:
        LD      HL, FPMAX           ;  3:10
    if color_flow_warning
        CALL    OVER_COL_WARNING    ;  3:17
    endif
    if carry_flow_warning
        SCF                         ;  1:4
    endif
        RET                         ;  1:10


if defined FMUL
; Power 2 of a floating-point number
; In: HL = number to square
; Out: HL = HL * HL
; Pollutes: AF
FPOW2_USE_FMUL:
        PUSH    BC                  ;  1:11
        PUSH    DE                  ;  1:11
        LD      B, H                ;  1:4
        LD      C, L                ;  1:4        
        EX      DE, HL              ;  1:4
        call    FMUL                ;           HL = (DE*BC)
        POP     DE                  ;  1:10
        POP     BC                  ;  1:10
        RET
endif

    include "color_flow_warning.asm"
    
endif
