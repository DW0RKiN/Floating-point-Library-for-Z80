; pasmo -I ../danagy -d test_fln.asm 24576.bin > test.asm ; grep "BREAKPOINT" test.asm
; randomize usr 57344

    INCLUDE "finit.asm"

    color_flow_warning  EQU     1
    carry_flow_warning  EQU     1
    DATA_ADR            EQU     $6000       ; 24576
    TEXT_ADR            EQU     $E000       ; 57344

    ORG     DATA_ADR

; red        
    dw $3fb3, $bd4e		; ln(+8.496e-01) = -1.630e-01
    dw $3fdc, $bc2b		; ln(+9.297e-01) = -7.291e-02
    dw $3fd8, $bc4d		; ln(+9.219e-01) = -8.135e-02
    dw $3fdd, $bc22		; ln(+9.316e-01) = -7.081e-02
    dw $3fbf, $bd16		; ln(+8.730e-01) = -1.358e-01
    dw $3feb, $bb57		; ln(+9.590e-01) = -4.188e-02
    dw $3ff0, $bb04		; ln(+9.688e-01) = -3.175e-02
    dw $3fa8, $bd82		; ln(+8.281e-01) = -1.886e-01

; azure
    dw $3f5d, $be88		; ln(+6.816e-01) = -3.833e-01
    dw $3f88, $be11		; ln(+7.656e-01) = -2.671e-01
    dw $3f4e, $beb5		; ln(+6.523e-01) = -4.272e-01
    dw $3f8f, $bdff		; ln(+7.793e-01) = -2.494e-01
    dw $3f9c, $bdbd		; ln(+8.047e-01) = -2.173e-01
    dw $3fb7, $bd3b		; ln(+8.574e-01) = -1.538e-01
    dw $3f4f, $beb2		; ln(+6.543e-01) = -4.242e-01
    dw $3f9a, $bdc7		; ln(+8.008e-01) = -2.222e-01
    dw $3fcc, $bcb7		; ln(+8.984e-01) = -1.071e-01
    dw $3fb7, $bd3b		; ln(+8.574e-01) = -1.538e-01
    dw $3f98, $bdd1		; ln(+7.969e-01) = -2.271e-01
    dw $3f98, $bdd1		; ln(+7.969e-01) = -2.271e-01

; yellow
    dw $3fae, $bd65		; ln(+8.398e-01) = -1.745e-01
    dw $3fa3, $bd9b		; ln(+8.184e-01) = -2.005e-01
    dw $3fba, $bd2d		; ln(+8.633e-01) = -1.470e-01
    dw $3faa, $bd79		; ln(+8.320e-01) = -1.839e-01    

; green
    dw $3f12, $bf40		; ln(+5.352e-01) = -6.252e-01
    dw $3f63, $be77		; ln(+6.934e-01) = -3.662e-01
    dw $3f38, $befb		; ln(+6.094e-01) = -4.953e-01
    dw $3f04, $bf5b		; ln(+5.078e-01) = -6.776e-01
    dw $3f72, $be4d		; ln(+7.227e-01) = -3.248e-01
    dw $3f53, $bea6		; ln(+6.621e-01) = -4.123e-01
    dw $3f47, $becb		; ln(+6.387e-01) = -4.484e-01
    dw $3f9f, $bdae		; ln(+8.105e-01) = -2.100e-01
    dw $3f7d, $be2f		; ln(+7.441e-01) = -2.955e-01
    dw $3f0f, $bf46		; ln(+5.293e-01) = -6.362e-01
    dw $3f9f, $bdae		; ln(+8.105e-01) = -2.100e-01
    dw $3f2b, $bf13		; ln(+5.840e-01) = -5.379e-01
    dw $3f14, $bf3c		; ln(+5.391e-01) = -6.179e-01
    dw $3f2e, $bf0e		; ln(+5.898e-01) = -5.279e-01
    dw $3f27, $bf1a		; ln(+5.762e-01) = -5.513e-01
    dw $3f8b, $be0a		; ln(+7.715e-01) = -2.594e-01
    dw $3f9f, $bdae		; ln(+8.105e-01) = -2.100e-01
    dw $3f12, $bf40		; ln(+5.352e-01) = -6.252e-01
    dw $3f99, $bdcc		; ln(+7.988e-01) = -2.246e-01
    dw $3f06, $bf57		; ln(+5.117e-01) = -6.700e-01
    dw $3f99, $bdcc		; ln(+7.988e-01) = -2.246e-01
    dw $3f17, $bf37		; ln(+5.449e-01) = -6.071e-01
    dw $3f8a, $be0c		; ln(+7.695e-01) = -2.620e-01
    dw $3f97, $bdd6		; ln(+7.949e-01) = -2.295e-01
    dw $3f28, $bf19		; ln(+5.781e-01) = -5.480e-01
    dw $3f02, $bf5f		; ln(+5.039e-01) = -6.854e-01
    dw $3f55, $bea0		; ln(+6.660e-01) = -4.064e-01
    dw $3f68, $be69		; ln(+7.031e-01) = -3.522e-01
    dw $3f03, $bf5d		; ln(+5.059e-01) = -6.815e-01
    dw $3f20, $bf27		; ln(+5.625e-01) = -5.754e-01
    dw $3fff, $b700		; ln(+9.980e-01) = -1.955e-03
    dw $3f80, $be27		; ln(+7.500e-01) = -2.877e-01
    dw $3f1e, $bf2a		; ln(+5.586e-01) = -5.823e-01
    dw $3f0e, $bf48		; ln(+5.273e-01) = -6.399e-01
    dw $3f0e, $bf48		; ln(+5.273e-01) = -6.399e-01
    dw $3f2f, $bf0d		; ln(+5.918e-01) = -5.246e-01
    dw $3f2f, $bf0d		; ln(+5.918e-01) = -5.246e-01
    dw $3fd4, $bc70		; ln(+9.141e-01) = -8.986e-02
    dw $3f1b, $bf30		; ln(+5.527e-01) = -5.929e-01
    dw $3f54, $bea3		; ln(+6.641e-01) = -4.094e-01
    dw $3f1d, $bf2c		; ln(+5.566e-01) = -5.858e-01
    dw $3f09, $bf51		; ln(+5.176e-01) = -6.586e-01
    dw $3f74, $be47		; ln(+7.266e-01) = -3.194e-01
    dw $3f40, $bee1		; ln(+6.250e-01) = -4.700e-01
    dw $3f2f, $bf0d		; ln(+5.918e-01) = -5.246e-01
    dw $3f44, $bed5		; ln(+6.328e-01) = -4.576e-01
    dw $3f2d, $bf10		; ln(+5.879e-01) = -5.312e-01
    dw $3f72, $be4d		; ln(+7.227e-01) = -3.248e-01
    dw $3f7d, $be2f		; ln(+7.441e-01) = -2.955e-01
    dw $3f5f, $be83		; ln(+6.855e-01) = -3.775e-01
    dw $3f0e, $bf48		; ln(+5.273e-01) = -6.399e-01
    dw $3f12, $bf40		; ln(+5.352e-01) = -6.252e-01
    dw $3f8b, $be0a		; ln(+7.715e-01) = -2.594e-01

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
