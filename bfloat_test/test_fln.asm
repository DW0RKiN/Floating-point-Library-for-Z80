; pasmo -I ../bfloat -d test_fln.asm 24576.bin > test.asm ; grep "BREAKPOINT" test.asm
; randomize usr 57344

    INCLUDE "finit.asm"

    color_flow_warning  EQU     1
    carry_flow_warning  EQU     1
    DATA_ADR            EQU     $6000       ; 24576
    TEXT_ADR            EQU     $E000       ; 57344

    ORG     DATA_ADR

; red    
    dw $7e7b, $79a2		; ln(+9.805e-01) = -1.972e-02
    dw $7e4b, $7cee		; ln(+7.930e-01) = -2.320e-01
    dw $7e6b, $7baf		; ln(+9.180e-01) = -8.559e-02
    dw $7e7b, $79a2		; ln(+9.805e-01) = -1.972e-02
    dw $7e6e, $7b95		; ln(+9.297e-01) = -7.291e-02
    dw $7e72, $7ae6		; ln(+9.453e-01) = -5.624e-02
    dw $7e7a, $79c2		; ln(+9.766e-01) = -2.372e-02
    dw $7e4e, $7cdf		; ln(+8.047e-01) = -2.173e-01
    dw $7e63, $7bf6		; ln(+8.867e-01) = -1.202e-01
; yellow
    dw $7e47, $7d81		; ln(+7.773e-01) = -2.519e-01
    dw $7e69, $7bc1		; ln(+9.102e-01) = -9.414e-02
    dw $7e1f, $7df4		; ln(+6.211e-01) = -4.763e-01
    dw $7e28, $7dd8		; ln(+6.562e-01) = -4.212e-01
    dw $7e13, $7e8e		; ln(+5.742e-01) = -5.547e-01
    dw $7e07, $7ea4		; ln(+5.273e-01) = -6.399e-01
    dw $7e15, $7e8b		; ln(+5.820e-01) = -5.412e-01
    dw $7e20, $7df1		; ln(+6.250e-01) = -4.700e-01
    dw $7e04, $7eaa		; ln(+5.156e-01) = -6.624e-01
    dw $7e7d, $78c1		; ln(+9.883e-01) = -1.179e-02
    dw $7e2a, $7dd2		; ln(+6.641e-01) = -4.094e-01
    dw $7e08, $7ea2		; ln(+5.312e-01) = -6.325e-01
    dw $7e38, $7da9		; ln(+7.188e-01) = -3.302e-01
    dw $7e54, $7cc1		; ln(+8.281e-01) = -1.886e-01
    dw $7e52, $7ccb		; ln(+8.203e-01) = -1.981e-01
    dw $7e1f, $7df4		; ln(+6.211e-01) = -4.763e-01
    dw $7e11, $7e92		; ln(+5.664e-01) = -5.684e-01
    dw $7e7d, $78c1		; ln(+9.883e-01) = -1.179e-02
    dw $7e15, $7e8b		; ln(+5.820e-01) = -5.412e-01
    dw $7e36, $7daf		; ln(+7.109e-01) = -3.412e-01
    dw $7e52, $7ccb		; ln(+8.203e-01) = -1.981e-01
    dw $7e54, $7cc1		; ln(+8.281e-01) = -1.886e-01
; green
    dw $7e1a, $7e82		; ln(+6.016e-01) = -5.082e-01
    dw $7e0a, $7e9e		; ln(+5.391e-01) = -6.179e-01
    
    dw FP1, FPMIN
    dw FP2, $7E31
    dw $7f33, $7d2c		; ln(+1.398e+00) = +3.354e-01
    dw $7f77, $7e28		; ln(+1.930e+00) = +6.574e-01
    dw $7f4a, $7d6a		; ln(+1.578e+00) = +4.562e-01
    dw $7f4d, $7d71		; ln(+1.602e+00) = +4.710e-01
    
    INCLUDE "test_fln.dat"

    dw $BABE, $DEAD                 ; Stop MARK

; Subroutines
    INCLUDE "fln.asm"
    INCLUDE "fequals.asm"
    INCLUDE "print_txt.asm"
    INCLUDE "print_hex.asm"

; Lookup tables
    INCLUDE "fln.tab"
    
    if ( $ > TEXT_ADR ) 
        .ERROR "Prilis dlouha data!"        
    endif
        
    ORG     TEXT_ADR

        LD      HL, DATA_ADR
        PUSH    HL
READ_DATA:
        POP     HL
        LD      BC, $0004
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
        LD      HL, (OP1)
BREAKPOINT:
; FUSE Debugger
; br 0xE011

        PUSH    HL        
        CALL    FLN                 ; HL = ln(HL)
        
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

        LD      H, B
        LD      L, C
        LD      DE, DATA_3
        CALL    WRITE_HEX

        CALL    PRINT_TXT
        
        defb    INK, COL_WHITE, 'ln($'
DATA_1:
        defs     4
        defb    ') = $'
DATA_3:
        defs     4
        defb    ' ? ', INK
DATA_COL:
        defb    COL_WHITE, '$'
DATA_4:        
        defs     4
        defb    NEW_LINE      
        defb    STOP_MARK
