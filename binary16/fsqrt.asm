if not defined @FSQRT

; Square root of floating-point number
; In: HL
; Out: HL = abs(HL)^0.5, reset carry
; Pollutes: AF

; Mantissas of square roots
; exp = 2*e
; (2^exp * mantisa)^0.5 = 2^e * mantisa^0.5
; exp = 2*e+1
; (2^exp * mantisa)^0.5 = 2^e * mantisa^0.5 * 2^0.5

; (2^-3 * mantisa)^0.5 = 2^-1 * 2^-0.5 * mantisa^0.5 = 2^-2 * 2^+0.5 ...
; (2^-2 * mantisa)^0.5 = 2^-1 *          mantisa^0.5 = 2^-1 *        ...
; (2^-1 * mantisa)^0.5 = 2^+0 * 2^-0.5 * mantisa^0.5 = 2^-1 * 2^+0.5 ...
; (2^+0 * mantisa)^0.5 = 2^+0 *          mantisa^0.5 = 2^+0 *        ...
; (2^+1 * mantisa)^0.5 = 2^+0 * 2^+0.5 * mantisa^0.5 = 2^+0 * 2^+0.5 ...
; (2^+2 * mantisa)^0.5 = 2^+1 *          mantisa^0.5 = 2^+1 *        ...
; (2^+3 * mantisa)^0.5 = 2^+1 * 2^+0.5 * mantisa^0.5 = 2^+1 * 2^+0.5 ...
@FSQRT:
if not defined FSQRT
; *****************************************
                    FSQRT                 ; *
; *****************************************
endif
        SLA     L                   ;  2:8      ABS(HL)
        LD      A, H                ;  1:4
        ADD     A, BIAS             ;  2:7
        RRA                         ;  1:4      A = (exp-bias)/2 + bias = (exp+bias)/2
                                    ;           carry => out = out * 2^0.5
        RR      L                   ;  2:8      RET with reset carry
        LD      H, SQR_TAB/256      ;  2:7
        LD      L, (HL)             ;  1:7
        LD      H, A                ;  1:4
        RET                         ;  1:10
endif
