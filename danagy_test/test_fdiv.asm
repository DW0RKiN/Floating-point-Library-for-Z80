; pasmo -I ../danagy -d test_fdiv.asm 24576.bin > test.asm ; grep "BREAKPOINT" test.asm
; randomize usr 57344

    INCLUDE "finit.asm"

    color_flow_warning  EQU     1
    carry_flow_warning  EQU     1
    DATA_ADR    EQU     $6000       ; 24576
    TEXT_ADR    EQU     $E000       ; 57344

    ORG     DATA_ADR

;     dw  FP24, FP8, FP3-1
;     dw  FP24, FP8, FP3+1
    
    INCLUDE "test_fdiv.dat"

    dw $BABE, $CAFE, $DEAD          ; Stop MARK

; Subroutines
    INCLUDE "fdiv.asm"
    INCLUDE "fequals.asm"
    INCLUDE "print_txt.asm"
    INCLUDE "print_hex.asm"

; Lookup tables
    INCLUDE "fdiv.tab"

    
    if ( $ > TEXT_ADR ) 
        .ERROR "Prilis dlouha data!"        
    endif
        
    ORG     TEXT_ADR

        LD      HL, DATA_ADR
        PUSH    HL
READ_DATA:
        POP     HL
        LD      BC, $0006
        LD      DE, OP1
        LDIR
        PUSH    HL
        
; HL = HL + DE
; HL = HL - DE

; HL = BC * DE

; HL = BC % HL
; HL = BC / HL

; HL = HL * HL
; HL = HL % 1
; HL = HL^0.5

        LD      BC, (OP1)
        LD      HL, (OP2)
BREAKPOINT:
; FUSE Debugger
; br 0xE015

        PUSH    BC
        PUSH    HL
        CALL    FDIV                    ; HL = BC / HL
        POP     BC
        POP     BC
        
;     kontrola
        LD      BC, (RESULT)
        
IGNORE  EQU $+1
        LD      A, $00
        OR      A
        LD      A, $00
        LD      (IGNORE), A
        JR      nz, READ_DATA

        CALL    FEQUALS
if 1
        JR      nc, READ_DATA
        JR      z, PRINT_OK
else
        JR      z, PRINT_OK
endif
        CALL    PRINT_DATA

if 1
        OR      A
        LD      HL, $DEAD
        SBC     HL, BC
        JR      nz, READ_DATA
endif
        POP     HL
        RET                         ; exit

PRINT_OK:
        CALL    PRINT_DATA
        JR      READ_DATA
        
        
OP1:
        defs    2
OP2:
        defs    2
RESULT:
        defs    2


; BC = kontrolni
; HL = spocitana
PRINT_DATA:
        LD      (DATA_COL), A
        
        LD      DE, DATA_4
        CALL    WRITE_HEX
        
        LD      HL, (OP1)
        LD      DE, DATA_1
        CALL    WRITE_HEX

        LD      HL, (OP2)
        LD      DE, DATA_2
        CALL    WRITE_HEX
        
        LD      H, B
        LD      L, C
        LD      DE, DATA_3
        CALL    WRITE_HEX

        CALL    PRINT_TXT
        
        defb    INK, COL_WHITE, '$'
DATA_1:
        defs     4
        defb    ' / $'
DATA_2:
        defs     4
        defb    ' = $'
DATA_3:
        defs     4
        defb    ' ? ', INK
DATA_COL:
        defb    COL_WHITE, '$'
DATA_4:        
        defs     4
        defb    NEW_LINE      
        defb    STOP_MARK
