if not defined @FRAC
    
; Fractional part, remainder after division by 1
;  In: HL any floating-point number
; Out: HL fractional part, with sign intact
; Pollutes: AF, B
@FRAC:
if not defined FRAC
; *****************************************
                    FRAC                ; *
; *****************************************
endif
        LD      A, H                ;
        AND     EXP_MASK            ;  2:7      delete sign
        CP      BIAS + MANT_BITS    ;
if defined FINT_ZERO
        JR      nc, FINT_ZERO       ;           Already integer
else
        JR      nc, FRAC_ZERO       ;           Already integer
endif
        SUB     BIAS                ;
        RET     c                   ;           Pure fraction

        INC     A                   ; 1:4       2^0*1.xxx > 1
        LD      B, A                ; 1:4
        LD      A, L                ; 2:7
FRAC_LOOP:                          ;           odmazani mantisy pred plovouci radovou carkou
        DEC     H                   ; 1:4
        ADD     A, A                ; 1:4
        DJNZ    FRAC_LOOP           ; 2:13/8

        LD      L, A                ; 1:4
        RET     c                   ; 1:11/5

if defined FINT_ZERO
        JR      z, FINT_ZERO        ;
else
        JR      z, FRAC_ZERO        ;
endif
                    
FRAC_NORM:                          ;           normalizace cisla
        DEC     H                   ; 4:1
        ADD     A, A                ; 4:1
        JR      nc, FRAC_NORM       ; 2:12/7

        LD      L, A                ; 1:4
        RET                         ; 1:10

if not defined FINT_ZERO
FRAC_ZERO:
        LD      HL, FPMIN           ; -0???
        RET                         ;
endif

endif
