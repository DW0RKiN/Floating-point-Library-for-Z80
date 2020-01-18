if not defined @FSQRT

; Square root of a positive floating-point number
; In: HL
; Out: HL = HL^0.5
; Pollutes: AF

; Mantissas of square roots
; exp = 2*e
; (2**exp * mantisa)**0.5 = 2**e * mantisa**0.5
; exp = 2*e+1
; (2**exp * mantisa)**0.5 = 2**e * mantisa**0.5 * 2**0.5
@FSQRT:
if not defined FSQRT
; *****************************************
                    FSQRT                 ; *
; *****************************************
endif
        LD      A, H            ;  1:4
        AND     EXP_MASK        ;  2:7  $7C  (delete sign)
        ADD     A, BIAS         ;  2:7  +$3C = + 0011 1100      (x-bias)/2 + bias = (x-bias+2*bias)/2 = (x+bias)/2
                                ;  $7C + $3C = $B8 => clear carry
        
        PUSH    BC              ;  1:11
        LD      C, A            ;  1:4         ???? ??00
        
        RRA                     ;  1:4
        AND     EXP_MASK        ;  2:7
        LD      B, A            ;  1:4
        
        LD      A, H            ;  1:4
        AND     MANT_MASK_HI    ;  2:7
        ADD     A, C            ;  1:4
        AND     2*MANT_MASK_HI+1;  2:7       maska nejniziho dale ztraceneho bitu exponentu, 1 => 0.5 or 0 => 0
        ADD     A, A
        ADD     A, 1+Tab_Even_Sqrt_lo_0/256     ;  2:7
        LD      H, A            ;  1:4
        
        LD      A, (HL)         ;  1:7   hi
        DEC     H               ;  1:4
        LD      L, (HL)         ;  1:7   lo
        OR      B               ;  1:4
        LD      H, A            ;  1:4
        
        POP     BC              ;  1:10
        RET

endif
