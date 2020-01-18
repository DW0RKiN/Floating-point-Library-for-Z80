; pasmo -I ../binary16 -d test_fbld.asm 24576.bin > test.asm ; grep "BREAKPOINT" test.asm
; randomize usr 57344

    INCLUDE "symbol.asm"

DATA_ADR    EQU     $6000   ; 24576
    
    ORG     DATA_ADR

    INCLUDE "test_fbld.dat"

; Stop MARK
    db $B1
    dw $DEAD

; Subroutines
    INCLUDE "fbld.asm"
    
_print_txt      EQU 1
_print_hex      EQU 1
_print_bin      EQU 1

    INCLUDE "print_fp.asm"

; Lookup tables

    
if ( $ > $E000 ) 
        .ERROR "Prilis dlouha data!"        
endif
        
    ORG     $E000         ; 57344


        LD      HL, DATA_ADR
        PUSH    HL
READ_DATA:
        POP     HL
        LD      A, (HL)
        LD      (OP1), A        
        INC     HL
        LD      C, (HL)
        INC     HL
        LD      B, (HL)
        INC     HL        
        PUSH    HL
        
BREAKPOINT:
; FUSE Debugger
; br 0xE00F
; HL = HL + DE
; HL = HL - DE
; HL = BC % HL
; HL = HL % 1
; HL = BC * DE
; HL = BC / HL
; HL = HL * HL
; HL = HL^0.5

        PUSH    AF
        CALL    FBLD            ; DE = FBYTE(A)
        POP     AF
        
IGNORE  EQU $+1
        LD      A, $00
        OR      A
        LD      A, $00
        LD      (IGNORE), A
        JR      nz, READ_DATA

        ; kontrola BC
        EX      DE, HL
        
        LD      A, L
        SUB     C
        JR      nz, TOLERANCE_P1
        LD      A, H
        SUB     B
        LD      A, COL_GREEN
if 1
        JR      z, READ_DATA
else
        JR      z, PRINT_OK
endif
                
TOLERANCE_P1:

; HL = eeee eeee smmm mmmm

        LD      A, L
        INC     A
        CP      C
        JR      nz, TOLERANCE_M1
        
        LD      A, L
        ADD     A, $01
        LD      A, H
        ADC     A, $00
        CP      B
        LD      A, COL_YELLOW
        JR      z, PRINT_OK
        
TOLERANCE_M1:
        LD      A, L
        DEC     A
        CP      C
        JR      nz, FAIL
        
        LD      A, L
        SUB     $01
        SBC     A, A
        ADD     A, H     
        CP      B
        LD      A, COL_AZURE
        JR      z, PRINT_OK

FAIL:
        LD      A, COL_RED
        CALL    PRINT_DATA

        POP     HL
        RET

PRINT_OK:
        CALL    PRINT_DATA
        
        JR      READ_DATA
        
        
OP1:
    dw  $0000


; BC = kontrolni
; HL = spocitana
PRINT_DATA:
        EX      DE, HL
        PUSH    AF
        LD      A, COL_WHITE
        CALL    PRINT_DOL_COL
        POP     AF
        LD      HL, (OP1)
        CALL    PRINT_HEX
        CALL    PRINT_ARITHMETIC
;         LD      HL, (OP2)
;         CALL    PRINT_HEX
;         CALL    PRINT_EQUAL
        CALL    PRINT_HEX_BC
        CALL    PRINT_QUESTION
        CALL    PRINT_DOL_COL
        EX      DE, HL        
        CALL    PRINT_HEX
        CALL    PRINT_ELN
        RET
        
        

PRINT_DOL_COL:
        CALL    PRINT_SET_COLOR
PRINT_DOL:
        CALL    PRINT_TXT
        defb    '$'

        
PRINT_ARITHMETIC:
        CALL    PRINT_TXT
        defb    INK, COL_WHITE, ' byte=>FP = $'        

PRINT_QUESTION:
        CALL    PRINT_TXT
        defb    INK, COL_WHITE, ' ? '

PRINT_ELN:
        CALL    PRINT_TXT
        defb    PRINT_NEW_LINE      
        defb    PRINT_STOP_MARK
            
