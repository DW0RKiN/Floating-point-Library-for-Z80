if not defined FDIV2

; Warning: must be included before first use!
;   Input: reg_hi, reg_lo
;  Output: number /= 2
FDIV2 MACRO reg_hi, reg_lo
    if EXP_PLUS_ONE > 1
        LD      A, reg_hi           ;  1:4          4*DEC reg_hi
        SUB     A, EXP_PLUS_ONE     ;  2:7
        .WARNING EXP muze podtect!
        LD      reg_hi, A           ;  1:4          number /= 2
    else
        DEC     reg_hi              ;  1:4          number /= 2
    endif
ENDM

endif
