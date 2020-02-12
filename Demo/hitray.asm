; Move the origin by D
; Pollutes: AF, AF', BC, DE, HL
HITRAY:        
        LD      A, (POS)
        LD      B, A
        LD      HL, GC
    if defined print_name_fce
        CALL    PRINT_HITRAY
    endif
HRL:       
        PUSH    BC
        LD      E, (HL)
        INC     HL
        LD      D, (HL)
        PUSH    HL
        LD      HL, (DX)
    if defined print_variables
        CALL    PRINT_DX
    endif
        EX      DE, HL
    if defined print_variables
        CALL    PRINT_HL
    endif
        CALL    FSUB                ;           HL = HL - DE          
        EX      DE, HL
        POP     HL
        LD      (HL), D
        DEC     HL
        LD      (HL), E             ;           [GC|B1C|B2C] = [GC|B1C|B2C] - [DZ]
        INC     HL
        INC     HL
        LD      E, (HL)
        INC     HL
        LD      D, (HL)
        PUSH    HL
        LD      HL, (DY)
    if defined print_variables
        CALL    PRINT_DY
    endif
        EX      DE, HL
    if defined print_variables
        CALL    PRINT_HL
    endif        
        CALL    FSUB                ;           HL = HL - DE          
        EX      DE, HL
        POP     HL
        LD      (HL), D
        DEC     HL
        LD      (HL), E             ;           [GC|B1C|B2C+2] = [GC|B1C|B2C+2] - [DZ]
        INC     HL
        INC     HL
        LD      E, (HL)
        INC     HL
        LD      D, (HL)
        PUSH    HL
        LD      HL, (DZ)
    if defined print_variables
        CALL    PRINT_DZ
endif
        EX      DE, HL
    if defined print_variables
        CALL    PRINT_HL
    endif        
        CALL    FSUB                ;           HL = HL - DE          
        EX      DE, HL
        POP     HL
        LD      (HL), D
        DEC     HL
        LD      (HL), E             ;           [GC|B1C|B2C+4] = [GC|B1C|B2C+4] - [DZ]                  
        INC     HL
        INC     HL                  ;           HL = GC=>B1C=>B2C         
        POP     BC
        DJNZ    HRL
        LD      HL, (GC)
    if defined print_variables
        CALL    PRINT_GX
    endif
        CALL    FRAC
        LD      (GC), HL
        LD      HL, (GC+4)
    if defined print_variables
        CALL    PRINT_GZ
    endif
        CALL    FRAC
        LD      (GC+4), HL
        RET

; Scale D by DE
; Input: DE=scale factor
; Output: HL=DZ
; Pollutes: AF, AF', BC
; LET dx=dx*s: LET dy=dy*s: LET dz=dz*s
SCALED:
        PUSH    DE
    if defined print_name_fce
        CALL    PRINT_SCALED
    endif
    if defined print_register
        CALL    PRINT_DE
    endif
        LD      BC, (DX)
    if defined print_register
        CALL    PRINT_BC
    endif
        CALL    FMUL                ;           HL = DE * BC
        LD      (DX), HL
        POP     DE
        PUSH    DE
        LD      BC, (DY)
    if defined print_register
        CALL    PRINT_BC
    endif
        CALL    FMUL                ;           HL = DE * BC
        LD      (DY), HL
        POP     DE
        PUSH    DE
        LD      BC, (DZ)
    if defined print_register
        CALL    PRINT_BC
    endif
        CALL    FMUL                ;           HL = DE * BC
        LD      (DZ), HL
        POP     DE
        RET

; DL by DE and DD by DE*DE
; Input: DE=scale factor
; Output: HL=DD
; Pollutes: AF, AF', BC, DE
SCALEDL:
        LD      BC, (DL)
        CALL    FMUL                ;           HL = DE * BC
        LD      (DL), HL
        CALL    FPOW2               ;           HL = HL * HL
        LD      (DD), HL
        RET
