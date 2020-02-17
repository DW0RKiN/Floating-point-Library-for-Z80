if not defined FMUL2

; Warning: must be included before first use!
;   Input: reg_hi, reg_lo
;  Output: number *= 2
FMUL2 MACRO reg_hi, reg_lo
    if EXP_PLUS_ONE > 1
        LD      A, reg_hi           ;  1:4          4*INC reg_hi
        ADD     A, EXP_PLUS_ONE     ;  2:7
        .WARNING EXP muze pretect!
        LD      reg_hi, A           ;  1:4          number *= 2
    else
        INC     reg_hi              ;  1:4          number *= 2
        .WARNING EXP muze pretect!
    endif
ENDM

endif
