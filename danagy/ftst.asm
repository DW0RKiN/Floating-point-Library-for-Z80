if not defined FTST

; Warning: must be included before first use!
;   Input: reg_hi, reg_lo
;  Output: zero flag if floating-point number is positive
FTST MACRO reg_hi, reg_lo
    if SIGN_BIT > 7
        BIT     SIGN_BIT - 8, reg_hi;  2:8
    else
        BIT     SIGN_BIT, reg_lo    ;  2:8
    endif
ENDM

endif
