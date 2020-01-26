if not defined @FSQDIF

; Find the difference of two squares
; Input: HL=hypothenuse, DE=leg
; Output: HL = HL * HL - DE * DE computed as (HL - DE) * (HL + DE)
@FSQDIF:
if not defined FSQDIF
; *****************************************
                  FSQDIF                ; *
; *****************************************
endif
        PUSH    HL
        PUSH    DE
        CALL    FSUBP               ;           HL = HL - DE
        POP     DE
        EX      (SP), HL
        CALL    FADDP               ;           HL = HL + DE
        EX      DE, HL
        POP     BC
        ; continue with FMULP
if not defined @FMULP
    include "fmulp.asm"
else
        JP      @FMULP
endif

endif
