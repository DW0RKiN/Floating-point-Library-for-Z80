if not defined FMUL2

; Warning: must be included before first use!
;   Input: reg_hi, reg_lo
;  Output: number *= 2
FMUL2 MACRO reg_hi, reg_lo
        INC     reg_hi              ;  1:4          number *= 2
        .WARNING EXP muze pretect!
ENDM

endif
