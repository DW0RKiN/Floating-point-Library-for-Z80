if not defined @FMOD

; Remainder after division
;  In: BC dividend, HL modulus
; Out: HL remainder
; Pollutes: AF,AF',BC,DE
@FMOD:
if not defined FMOD
; *****************************************
                    FMOD                ; *
; *****************************************
endif
        PUSH    BC                  ;           Stack: dividend
        PUSH    HL                  ;           Stack: dividend, modulus
        CALL    @FDIV
        CALL    @FINT               ;           integer ratio
        EX      DE, HL              ;           DE = int(BC/HL)
        POP     BC                  ;           Stack: dividend; BC = modulus
        CALL    @FMUL               ;           Stack: dividend
        EX      DE, HL
        POP     HL
        ; continue with FSUB

if defined @FSUB
    .ERROR  You must exclude the file "fsub.asm" or include "fmod.asm" first
else
    include "fsub.asm"
endif

endif
