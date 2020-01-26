if not defined FDIV2

; Warning: must be included before first use!
;   Input: reg_hi, reg_lo
;  Output: number /= 2
FMUL2 MACRO reg_hi, reg_lo
        DEC     reg_hi              ;  1:4      number /= 2
        .WARNING EXP muze podtect!
ENDM

endif
