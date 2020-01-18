if not defined @FSUB

        ; continue from @FMOD (if it was included)
        
; Subtract two floating-point numbers
;  In: HL, DE numbers to subtract, no restrictions
; Out: HL = HL - DE
; Pollutes: AF, BC, DE
@FSUB:
if not defined FSUB
; *****************************************
                    FSUB                ; *
; *****************************************
endif
        LD      A, D                ;  1:4
        XOR     SIGN_MASK           ;  2:7
        LD      D, A                ;  1:4      DE = -DE
        ; continue with FADD

if defined @FADD
    .ERROR  You must exclude the file "fadd.asm" or include "fsub.asm" first
else
    include "fadd.asm"
endif

endif
