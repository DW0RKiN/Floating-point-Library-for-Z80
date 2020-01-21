; pasmo -I ../binary16 -d test_fwld.asm 24576.bin > test.asm ; grep "BREAKPOINT" test.asm
; randomize usr 57344

    INCLUDE "finit.asm"

    color_flow_warning  EQU     1
    carry_flow_warning  EQU     1
    DATA_ADR            EQU     $6000       ; 24576
    TEXT_ADR            EQU     $E000       ; 57344

    ORG     DATA_ADR

    INCLUDE "test_fwld0.dat"

    dw      $DEAD                           ; Stop MARK

; Subroutines
    INCLUDE "fwld.asm"
    INCLUDE "fequals.asm"
    INCLUDE "print_txt.asm"
    INCLUDE "print_hex.asm"

; Lookup tables

    
    if ( $ > TEXT_ADR ) 
        .ERROR "Prilis dlouha data!"        
    endif
        
    ORG     TEXT_ADR

        LD      HL, DATA_ADR+2  ;  3:10     index
        PUSH    HL              ;  1:11
        LD      HL, (DATA_ADR)  ;  3:16     hodnota prvniho operandu
        DEC     HL              ;  1:6
        PUSH    HL              ;  1:11

READ_DATA:
        POP     DE              ;  1:10     operand
        POP     HL              ;  1:10     index
        LD      C, (HL)         ;  1:7
        INC     HL              ;  1:6
        LD      B, (HL)         ;  1:7
        INC     HL              ;  1:6
        PUSH    HL              ;  1:11     index

; HL = HL + DE
; HL = HL - DE

; HL = BC * DE

; HL = BC % HL
; HL = BC / HL

; HL = HL * HL
; HL = HL % 1
; HL = HL^0.5

        EX      DE, HL          ;  1:4
        INC     HL              ;  1:6
        PUSH    HL              ;  1:11     operand
        
BREAKPOINT:
; FUSE Debugger
; br 0xE013

        CALL    FWLD            ;           HL = 1.0 * HL
        
;     kontrola v BC
        
IGNORE  EQU     $+1
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
        POP     HL
        RET                         ;           exit

PRINT_OK:
        CALL    PRINT_DATA
        JR      READ_DATA
        

; BC = kontrolni
; HL = spocitana
PRINT_DATA:
        LD      (DATA_COL), A

        LD      DE, DATA_4
        CALL    WRITE_HEX

        LD      H, B
        LD      L, C
        LD      DE, DATA_3
        CALL    WRITE_HEX

        POP     DE                  ;           RET
        POP     HL                  ;           operand
        PUSH    HL                  ;           operand
        PUSH    DE                  ;           RET
        LD      DE, DATA_1
        CALL    WRITE_HEX

        CALL    PRINT_TXT
        
        defb    INK, COL_WHITE, '$'
DATA_1:
        defs     4
        defb    ' '
        defb    ' * 1.0 = $'
DATA_3:
        defs     4
        defb    ' ? ', INK
DATA_COL:
        defb    COL_WHITE, '$'
DATA_4:        
        defs     4
        defb    NEW_LINE      
        defb    STOP_MARK
