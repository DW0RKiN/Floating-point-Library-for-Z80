; Raytracer
; Input: B = y coordinate from top = 0..191, C = x coordinate from left = 0..255
; Output: flag C = light

; Start ray
TRACE:
        PUSH    BC
    if defined print_register
        CALL    PRINT_BC            ;  3:17
    endif
        LD      A, C
        CALL    FCE
        LD      HL, STRED_X         ;           -128
    if defined print_register
        CALL    PRINT_HL            ;  3:17
        CALL    PRINT_DE            ;  3:17
    endif
        CALL    FADD
    if defined print_variables
        CALL PRINT_DX               ;  3:17
    endif
        LD      (DX), HL        
        POP     BC
        
        LD      A, B 
        CALL    FCE
        LD      HL, STRED_Y         ;           -96
        CALL    FADD
    if defined print_variables
        CALL PRINT_DY
    endif
        LD      (DY), HL

        LD      HL, STRED_Z         ;           +300
    if defined print_variables
        CALL    PRINT_DZ
    endif
        LD      (DZ), HL
        
        LD      HL, DX
        LD      B, $03
        CALL    FLEN                ;           dot.asm Square norm of a vector
    if defined print_variables
        CALL    PRINT_DD
    endif
        LD      (DD), HL
        CALL    FSQRT
    if defined print_variables
        CALL    PRINT_DL
    endif
        LD      (DL), HL
        
        ; Kopirovani GROUNDC, BALL1C, BALL2C z spectrum.asm do GC, B1C, B2C ve variables.asm
        LD      A, (POS)            ;           A = 3
        LD      HL, GROUNDC
        LD      DE, GC
POSL:
        LD      BC, $06
        LDIR                        ;           (DE++) = (HL++)
        DEC     A
        JR      nz, POSL
        LD      (BOUNCE), A         ;           lo Bounce = 0
TRACEL:
        LD      HL, BOUNCE          ;           HL = adr Bounce
        DEC     (HL)                ; 
        RET     z

;         fall to spectrum.asm
