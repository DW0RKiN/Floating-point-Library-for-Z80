if not defined @FADD

        ; continue from @FSUB (if it was included)
        
; Add two floating-point numbers
;  In: HL, DE numbers to add, no restrictions
; Out: HL = HL + DE
; Pollutes: AF, BC, DE
@FADD:
if not defined FADD
; *****************************************
                    FADD                ; *
; *****************************************
endif
        LD      A, H                ;  1:4
        XOR     D                   ;  1:4
        JP      m, FSUBP_FADD_OP_SGN;  3:10
        ; continue with FADDP

if defined @FADDP
    .ERROR  You must exclude the file "faddp.asm" or include "fadd.asm" first
else
    include "faddp.asm"
endif

    include "fsubp.asm"

endif
