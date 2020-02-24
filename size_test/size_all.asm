    INCLUDE "finit.asm"

    color_flow_warning  EQU     0
    carry_flow_warning  EQU     1
    DATA_ADR    EQU     $6000       ; 24576
    TEXT_ADR    EQU     $E000       ; 57344

    ORG     DATA_ADR

    
; Lookup tables
    INCLUDE "fdiv.tab"
    INCLUDE "fmul.tab"
    INCLUDE "fsqrt.tab"
    INCLUDE "fpow2.tab"
    INCLUDE "fln.tab"
    
; Subroutines
    INCLUDE "fdiv.asm"
    INCLUDE "fmul.asm"
    INCLUDE "fsub.asm"
    INCLUDE "fadd.asm"
    INCLUDE "fmod.asm"
    INCLUDE "fpow2.asm"
    INCLUDE "fsqrt.asm"
    INCLUDE "frac.asm"
    INCLUDE "fint.asm"
    INCLUDE "fwld.asm"
    INCLUDE "fwst.asm"
    INCLUDE "fbld.asm"
    INCLUDE "fln.asm"

