if not defined FORSGN

; Warning: must be included before first use!
;   Input: reg1_hi, reg1_lo, reg2_hi, reg2_lo
;  Output: (BIT 7, A) = reg1_sign or reg2_sign
FORSGN MACRO reg1_hi, reg1_lo, reg2_hi, reg2_lo
    if SIGN_BIT > 7
        LD      A, reg1_hi
        OR      reg2_hi             ; sign or sign
    else
        LD      A, reg1_lo
        OR      reg2_lo             ; sign or sign
    endif
ENDM

endif
