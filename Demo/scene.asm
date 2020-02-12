; fall from spectrum.asm

; Look at object hit
; Input: HL distance of hitting, IX = object hit
; Pollutes: anything
SCENEL: 
    if defined print_name_fce
        call    PRINT_SCENEL
    endif
    if defined print_variables
        PUSH    HL
        LD      HL, (DX)
        CALL    PRINT_DX
        LD      HL, (DY)
        CALL    PRINT_DY
        LD      HL, (DZ)
        CALL    PRINT_DZ
        POP     HL
    endif
        LD      E, (IX+4)
        LD      D, (IX+5)
        PUSH    DE                  ;           push GROUNDL or SPHEREL
        RET                         ;           call GROUNDL or SPHEREL

; Intersection with scenery
; Input: IX = first object in scenery
; Output: CF = true if any object is hit by ray, HL = distance of hitting, IX = object hit (GROUND, BALL1, BALL2 )
; Pollutes: anything
SCENEI
    if defined print_name_fce
        call    PRINT_SCENEI
    endif
        LD      HL, 0
        LD      (NEAREST), HL
        LD      IX, SCENE
SCLOOP:       
        LD      L, (IX+2)
        LD      H, (IX+3)           ;           HL = &GROUNDI, &SPHEREI, &SPHEREI
        CALL    JPHL
        JR      nc, NOHITS
        LD      DE, (NEAREST)
        LD      A, E
        OR      D
        JR      z, HITS
        LD      DE, (DIST)
        OR      A
        SBC     HL, DE              ;           This is okay, as scaling factor is non-negative
        JR      nc, NOHITS
        ADD     HL, DE
HITS:       
        LD      (DIST), HL
        LD      (NEAREST), IX
NOHITS:       
        LD      E, (IX+0)
        LD      D, (IX+1)           ;           DE = BALL1, BALL2, 0
        LD      A, D
        OR      E
        JR      z, HITSC
        PUSH    DE
        POP     IX                  ;           IX = SCENE->BALL1, BALL1->BALL2
        JR      SCLOOP
JPHL:       
        JP      (HL)
HITSC:       
        LD      HL, (NEAREST)
        LD      A, H
        OR      L
        RET     z
        PUSH    HL
        POP     IX
        LD      HL, (DIST)
        SCF
        RET
