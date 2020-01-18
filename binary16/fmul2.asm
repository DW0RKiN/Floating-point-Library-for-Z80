if not defined FMUL2

; Warning: must be included before first use!
;   Input: reg_hi, reg_lo
;  Output: number *= 2
FMUL2 MACRO reg_hi, reg_lo
        LD      A, reg_hi           ;  1:4
        ADD     A, EXP_PLUS_ONE     ;  2:7
        .WARNING EXP muze pretect!
        LD      reg_hi, A           ;  1:4          number *= 2
ENDM

endif
