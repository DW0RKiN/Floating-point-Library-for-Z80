if not defined @FWLD

include "fbld.asm"
include "faddp.asm"

; Load Word. Convert unsigned 16-bit integer into floating-point number
;  In: HL = Word to convert
; Out: HL = floating point representation
; Pollutes: AF
@FWLD:
if not defined FWLD
; *****************************************
                    FWLD                ; *
; *****************************************
endif
        PUSH    DE                  ;  1:11        
        LD      A, H                ;  1:4
        OR      A                   ;  1:4
        JR      z, FWLD_ONLY_LO     ;  2:12/7

        CALL    FBLD                ;  3:17     DE = 1.0 * A
    if 1
        LD      A, EXP_PLUS_ONE << 3;  2:7
        ADD     A, D                ;  1:4
        LD      H, A                ;  1:4
        LD      A, L                ;  1:4
        LD      L, E                ;  1:4        
                                    ;  6:23
    else
        LD      A, L                ;  1:4        
        LD      HL, EXP_PLUS_ONE<<11;  3:10
        ADD     HL, DE              ;  1:11
                                    ;  5:25
    endif
        OR      A                   ;  1:4
        JR      z, FWLD_ONLY_HI     ;  2:12/7
        CALL    FBLD                ;  3:17     DE = 1.0 * A
        PUSH    BC                  ;  1:11
        CALL    FADDP               ;  3:17     HL = HL + DE
        POP     BC                  ;  1:10
FWLD_ONLY_HI:
        POP     DE                  ;  1:10
        RET                         ;  1:10
                                    ; 27/26:152/154 + 2*FBLD + FADDP

FWLD_ONLY_LO:
        LD      A, L                ;  1:4
        CALL    FBLD                ;  3:17     DE = 1.0 * A
        EX      DE, HL              ;  1:4
        POP     DE                  ;  1:10
        RET                         ;  1:10
                                    ; 33:
endif
