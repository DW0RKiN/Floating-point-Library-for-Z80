if not defined FTST

; Warning: must be included before first use!
;   Input: reg_hi, reg_lo
;  Output: zero flag if floating-point number is positive
FTST MACRO reg_hi, reg_lo
        BIT     7, reg_lo           ;  2:8
ENDM

endif
