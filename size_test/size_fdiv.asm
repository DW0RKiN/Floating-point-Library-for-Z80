    INCLUDE "finit.asm"

    color_flow_warning  EQU     0
    carry_flow_warning  EQU     1
    DATA_ADR    EQU     $6000       ; 24576
    TEXT_ADR    EQU     $E000       ; 57344

    ORG     DATA_ADR

; Lookup tables
    INCLUDE "fdiv.tab"

; Subroutines
    INCLUDE "fdiv.asm"

