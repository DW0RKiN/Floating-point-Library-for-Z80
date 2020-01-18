; pasmo -I ../binary16 -d test_fwld.asm 24576.bin > test.asm ; grep "BREAKPOINT" test.asm
; randomize usr 57344

    INCLUDE "symbol.asm"

DATA_ADR    EQU     $6000   ; 24576
    
    ORG     DATA_ADR

    INCLUDE "test_fwld0.dat"
;     INCLUDE "test_fwld1.dat"
;     INCLUDE "test_fwld2.dat"
;     INCLUDE "test_fwld3.dat"
;     INCLUDE "test_fwld4.dat"

; Stop MARK
    dw $DEAD

; Subroutines
    INCLUDE "fwld.asm"
    
_print_txt      EQU 1
_print_hex      EQU 1
_print_bin      EQU 1

    INCLUDE "print_fp.asm"

; Lookup tables   

    
if ( $ > $E000 ) 
        .ERROR "Prilis dlouha data!"        
endif
        
    ORG     $E000         ; 57344

        LD      DE, OP1
        LD      HL, DATA_ADR
        LDI
        LDI                     ; zkopirovani pocatku, aby se program dal spoustet vickrat
        JR      START
READ_DATA:
        LD      HL, (OP1)
        INC     HL
        LD      (OP1), HL
        POP     HL   
START:
        LD      C, (HL)         ;  1:7
        INC     HL              ;  1:6
        LD      B, (HL)         ;  1:7
        INC     HL              ;  1:6
        PUSH    HL

        LD      HL, (OP1)
BREAKPOINT:
; FUSE Debugger
; br 0xE01C
; HL = HL + DE
; HL = HL - DE
; HL = BC % HL
; HL = HL % 1
; HL = BC * DE
; HL = BC / HL
; HL = HL * HL
; HL = HL^0.5

        PUSH    HL
        CALL    FWLD            ; HL = 1.0 * HL
        POP     AF
;     kontrola        
        
IGNORE  EQU $+1
        LD      A, $00
        OR      A
        LD      A, $00
        LD      (IGNORE), A
        JR      nz, READ_DATA
                
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
        defb    INK, COL_WHITE, ' * 1.0 = $'        

PRINT_QUESTION:
        CALL    PRINT_TXT
        defb    INK, COL_WHITE, ' ? '

PRINT_ELN:
        CALL    PRINT_TXT
        defb    PRINT_NEW_LINE      
        defb    PRINT_STOP_MARK
            
