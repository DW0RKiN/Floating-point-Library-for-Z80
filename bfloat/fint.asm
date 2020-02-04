if not defined @FINT

; Round towards zero
; In: HL any floating-point number
; Out: HL same number rounded towards zero
; Pollutes: AF, B
@FINT:
if not defined FINT
; *****************************************
                    FINT                ; *
; *****************************************
endif
        LD      A, H                ;  1:4
        SUB     BIAS                ;  2:7
if defined FRAC_ZERO
        JR      c, FRAC_ZERO        ;  2:12/7   Completely fractional
else
        JR      c, FINT_ZERO        ;  2:12/7   Completely fractional
endif
        SUB     MANT_BITS           ;  2:7
        RET     nc                  ;  1:11/5   Already integer
        NEG                         ;  2:8      1..7

        LD      B, A                ;  1:4
        LD      A, $FF              ;  2:7
FINT_LOOP:                          ;           odmazani mantisy za plovouci radovou carkou
        ADD     A, A                ;  1:4
        DJNZ    FINT_LOOP           ;  2:13/8
        AND     L                   ;  1:4
        LD      L, A                ;  1:4
        RET
if not defined FRAC_ZERO
FINT_ZERO:
        LD      HL, FPMIN           ; -0???
        RET
endif
        
endif
