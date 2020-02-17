ZOOM                EQU     1
TEST_PIXEL          EQU     0
DEBUG               EQU     TEST_PIXEL

carry_flow_warning  EQU 0
color_flow_warning  EQU 0

if DEBUG
    print_name_fce
    print_register
    print_variables
endif

        INCLUDE "finit.asm"
        
        ; MACROS
        INCLUDE "fmul2.asm"
        INCLUDE "ftst.asm"
        INCLUDE "forsgn.asm"

; This code is ZX Spectrum specific
        ORG     $8000               ;           Use uncontended memory (32768)
SCAN:        
        EXX
        PUSH    HL
        LD      HL, $5800
LP1:        
        DEC     HL
        LD      B, $08              ;           8 pixelu na bajt
LP2:        
        PUSH    BC
        PUSH    HL
        
        LD      A, L
        ADD     A, A
        ADD     A, A
        ADD     A, A
        ADD     A, B
        DEC     A
        LD      C, A                ;           C = 0..255 = X
        
        ; 0 1 0 y7 y6 y2 y1 y0 - y5 y4 y3 x4 x3 x2 x1 x0
    
        LD      A, L
        RRCA
        RRCA                        ;           x1 x0 y5 y4 y3 x4 x3 x2
        XOR     H
        AND     $F8                 ;           ?  ?  ?  ?  ?  0  0  0
        XOR     H                   ;           x1 x0 y5 y4 y3 y2 y1 y0
        LD      B, A
        LD      A, H
        ADD     A, A
        ADD     A, A
        ADD     A, A                ;           y7 y6 y2 y1 y0 0 0
        XOR     B
        AND     $C0                 ;           ? ? 0 0 0 0 0 0
        XOR     B
        LD      B, A
        ; B = Y = 0..191
        ; C = X = 0..255
    if (TEST_PIXEL = 1)
        LD      BC, 82*256+61
    endif
    if (TEST_PIXEL = 2)
        LD      BC, 101*256+66
    endif
    if (TEST_PIXEL = 3)
        LD      BC, 125*256+79
    endif
    if (TEST_PIXEL = 4)
        LD      BC, 97*256+64       ;           y x
    endif
    if (TEST_PIXEL = 5)
        LD      BC, 96*256+128      ;           y x
    endif
        CALL    TRACE
        POP     HL
        RR      (HL)
        POP     BC
        DJNZ    LP2
        LD      A, H
        CP      $3C
        JR      nc, LP1
        POP     HL
        EXX
        RET

SCENE:

if ZOOM = 1
;                      +2       +4       +6   +8    +10
    GROUND: DEFW    BALL1, GROUNDI, GROUNDL,  GC
    BALL1:  DEFW    BALL2, SPHEREI, SPHEREL, B1C, FP0_6, FP0_36         ; r=0.6, r*r=0.36
    BALL2:  DEFW        0, SPHEREI, SPHEREL, B2C, FP0_2, FP0_04         ; r=0.2, r*r=0.04
    ;               zero is endmark
    POS:    DEFB    3                       ; 3 positions
    GROUNDC:DEFW    FM0_3, FP0_5, FP0       ; -0.3, 0.5, 0
    BALL1C: DEFW    FM0_6, FM0_3, FP3       ; -0.6, -0.3, 3
    BALL2C: DEFW    FP0_6, FM0_6, FP2       ;  0.6, -0.6, 2
    LIGHT:  DEFW        0,   FM1,   0       ; 
endif

if ZOOM = 2
    GROUND: DEFW   BALL1, GROUNDI, GROUNDL, GC
    BALL1:  DEFW   BALL2, SPHEREI, SPHEREL, B1C, FP0_3, FP0_09          ; r=0.6, r*r=0.36
    BALL2:  DEFW       0,   SPHEREI, SPHEREL, B2C, FP0_1, FP0_01        ; r=0.2, r*r=0.04
    ;              zero is endmark
    POS:    DEFB        3                   ; 3 positions
    GROUNDC:DEFW   FM0_15, FP0_25, FP0      ; -0.3, 0.5, 0
    BALL1C: DEFW    FM0_3, FM0_15, FP1_5    ; -0.6, -0.3, 3
    BALL2C: DEFW    FP0_3,  FM0_3, FP1      ;  0.6, -0.6, 2
    LIGHT:  DEFW        0,  FM0_5,   0      ; 
endif

if ZOOM = 4 
    GROUND: DEFW      BALL1, GROUNDI, GROUNDL, GC
    BALL1:  DEFW      BALL2, SPHEREI, SPHEREL, B1C, FP0_15, FP0_0225    ; r=0.6, r*r=0.36
    BALL2:  DEFW          0,   SPHEREI, SPHEREL, B2C, FP0_05, FP0_0025  ; r=0.2, r*r=0.04
    ;               zero is endmark
    POS:    DEFB        3                       ; 3 positions
    GROUNDC:DEFW    FM0_075, FP0_125, FP0       ; -0.3, 0.5, 0
    BALL1C: DEFW     FM0_15, FM0_075, FP0_75    ; -0.6, -0.3, 3
    BALL2C: DEFW     FP0_15,  FM0_15, FP0_5     ;  0.6, -0.6, 2
    LIGHT:  DEFW          0,  FM0_25,   0       ; 
endif

if ZOOM = 16 
    GROUND: DEFW    BALL1, GROUNDI, GROUNDL, GC
    BALL1:  DEFW    BALL2, SPHEREI, SPHEREL, B1C, FP0_0375, FP0_00140625    ; r=0.6, r*r=0.36
    BALL2:  DEFW        0, SPHEREI, SPHEREL, B2C, FP0_0125, FP0_00015625    ; r=0.2, r*r=0.04
    ;               zero is endmark
    POS:    DEFB        3                           ; 3 positions
    GROUNDC:DEFW    FM0_01875, FP0_03125, FP0       ; -0.3, 0.5, 0
    BALL1C: DEFW     FM0_0375, FM0_01875, FP0_1875  ; -0.6, -0.3, 3
    BALL2C: DEFW     FP0_0375,  FM0_0375, FP0_125   ;  0.6, -0.6, 2
    LIGHT:  DEFW            0,  FM0_0625,   0       ; 
endif

        INCLUDE "trace.asm"
        CALL    SCENEI
        RET     nc
        INCLUDE "scene.asm"
        
        INCLUDE "ground.asm"
        INCLUDE "sphere.asm"
        INCLUDE "hitray.asm"        
; Subroutines
    if DEBUG
        INCLUDE "print_fp.asm"
        INCLUDE "print.asm"

        INCLUDE "debug_fsub.asm"
        INCLUDE "debug_fadd.asm"
        INCLUDE "debug_fdiv.asm"
        INCLUDE "debug_fmul.asm"
        INCLUDE "debug_frac.asm"
        INCLUDE "debug_fsqrt.asm"        
    endif
        INCLUDE "fsub.asm"
        INCLUDE "fadd.asm"
        INCLUDE "fdiv.asm"
        INCLUDE "flen.asm"
        INCLUDE "frac.asm"
        INCLUDE "fsqrt.asm"
        INCLUDE "fpow2.asm"
    if ZOOM = 1
        INCLUDE "fbld.asm"
        FCE         EQU     FBLD
        STRED_X     EQU     FM128
        STRED_Y     EQU     FM96
        STRED_Z     EQU     FP300
    endif
    if ZOOM = 2
        INCLUDE "fbld_div2.asm"
        FCE         EQU     FBLD_DIV2
        STRED_X     EQU     FM64
        STRED_Y     EQU     FM48
        STRED_Z     EQU     FP150
    endif
    if ZOOM = 4
        INCLUDE "fbld_div4.asm"
        FCE         EQU     FBLD_DIV4
        STRED_X     EQU     FM32
        STRED_Y     EQU     FM24
        STRED_Z     EQU     FP75
    endif
    if ZOOM = 16
        INCLUDE "fbld_div16.asm"
        FCE         EQU     FBLD_DIV16
        STRED_X     EQU     FM8
        STRED_Y     EQU     FM6
        STRED_Z     EQU     FP18_75
    endif
        INCLUDE "fdot.asm"

    ; Lookup tables
        INCLUDE "fdiv.tab"
        INCLUDE "fmul.tab"
        INCLUDE "fsqrt.tab"
        INCLUDE "fpow2.tab"
        
    ; Variables
        INCLUDE "variables.asm"
