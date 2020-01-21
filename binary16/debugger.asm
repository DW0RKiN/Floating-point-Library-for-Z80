if not defined @DEBUG

    INCLUDE "print_fp.asm"

@DEBUG

; HL = HL + DE
FADD:
        CALL    @FADD               ;  3:17
        PUSH    HL                  ;  1:11
        LD      HL, '+'+' ' * 256   ;  3:10     "+ "
        LD      A, COL_BLUE         ;  2:7
        JP      PRINT_XFP           ;  3:10


; HL = HL - DE
FSUB:
        CALL    @FSUB               ;  3:17
        PUSH    HL                  ;  1:11
        LD      HL, '-'+' ' * 256   ;  3:10     "- "
        LD      A, COL_BLUE         ;  2:7
        JP      PRINT_XFP           ;  3:10


if 0
; HL = BC % HL
FMOD:
        CALL    @FMOD               ;  3:17
        PUSH    HL                  ;  1:11
        LD      HL, '%'+' ' * 256   ;  3:10     "% "
        LD      A, COL_BLUE         ;  2:7
        JP      PRINT_XFP           ;  3:10
endif

; HL = HL % 1
FRAC:
        CALL    @FRAC               ;  3:17
        PUSH    HL                  ;  1:11
        LD      HL, '%'+'1' * 256   ;  3:10     "%1"
        LD      A, COL_BLUE         ;  2:7
        JP      PRINT_XFP           ;  3:10

        
; HL = BC * DE
FMUL:
        CALL    @FMUL                   ;  3:17
        PUSH    HL                      ;  1:11
        LD      HL, '*'+' ' * 256       ;  3:10     "* "
        LD      A, COL_BLUE             ;  2:7
        JP      PRINT_XFP               ;  3:10


; HL = BC / HL
FDIV:
        CALL    @FDIV                   ;  3:17
        PUSH    HL                      ;  1:11
        LD      HL, '/'+' ' * 256       ;  3:10     "/ "
        LD      A, COL_BLUE             ;  2:7
        JP      PRINT_XFP               ;  3:10


; HL = HL * HL
FPOW2:
        CALL    @FPOW2                  ;  3:17
        PUSH    HL                      ;  1:11
        LD      HL, $325E               ;  3:10     "↑2"
        LD      A, COL_BLUE             ;  2:7
        JP      PRINT_XFP               ;  3:10


; HL = HL^0.5
FSQRT:
        CALL    @FSQRT
        PUSH    HL                      ;  1:11
        LD      HL, 'S'+'Q' * 256       ;  3:10     "SQ"
        LD      A, COL_BLUE             ;  2:7
        JP      PRINT_XFP               ;  3:10

endif
