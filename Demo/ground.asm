; Look at the ground
; Input: HL = scale factor of D to surface
; Output: CF color
; Pollutes: anything except IX
; IF (x-INT x>.5)<>(z-INT z>.5) THEN PLOT j,i
GROUNDL:
    if defined print_name_fce
        CALL    PRINT_GROUNDL
    endif
        EX      DE, HL
        CALL    SCALED
        CALL    HITRAY
        LD      IX, BALL1
SHADOWS:
        LD      L, (IX+6)
        LD      H, (IX+7)           ;           HL = B1C
        LD      E, (HL)
        INC     HL
        LD      D, (HL)             ;           DE = B1C.x || B2C.x
        INC     HL
        INC     HL
        INC     HL
        EX      DE, HL
    if defined print_register
        CALL    PRINT_HL
    endif
        CALL    FPOW2
        PUSH    HL                  ;           B1C.x^2 || B2C.x^2 => stack 
        EX      DE, HL
        LD      E, (HL)
        INC     HL
        LD      D, (HL)             ;           DE = B1C.z || B2C.z
        EX      DE, HL
    if defined print_register
        CALL    PRINT_HL
    endif
        CALL    FPOW2
        POP     DE
    if defined print_register
        CALL    PRINT_DE
    endif
        CALL    FADDP               ;           HL = B1C.x^2 + B1C.z^2 || B2C.x^2 + B2C.z^2 
        LD      E, (IX+10)
        LD      D, (IX+11)          ;           DE = B1r*B1r || B2r*B2r
    if defined print_register
        LD      A, COL_RED
        CALL    PRINT_HL_COL
        CALL    PRINT_DE_COL
    endif
        OR      A                   ;  1:4
        SBC     HL, DE              ;  2:15     HL>=0, DE>=0    
        CCF
    if defined print_name_fce
        CALL    c, PRINT_TXT_CONTINUE
        CALL    nc, PRINT_TXT_EXIT
        CALL    PRINT_GROUNDL
    endif
        RET     nc                  ;           jsme ve stinu koule
        LD      E, (IX+0)
        LD      D, (IX+1)
        PUSH    DE
        POP     IX                  ;           IX[0] = IX+12 = IX[1], BALL1 => BALL2 => exit SHADOWS 
        LD      A, E
        OR      D
        JR      nz, SHADOWS         ;           smycka je ukoncena nulovou zarazkou
; GROUNDX:                            
        LD      HL, (GC)
    if defined print_variables
        LD      A, COL_RED
        CALL    PRINT_GX_COL
    endif
        CALL    GRL0
        PUSH    AF
        LD      HL, (GC+4)
    if defined print_variables
        LD      A, COL_RED
        CALL    PRINT_GZ_COL
    endif
        CALL    GRL0
        POP     BC
        XOR     B
        RRCA                        ;           set carry
        RET

    
GRL0:
    if (MANT_BITS = 8)              ;           'seee eeee mmmm mmmm'
        LD      A, H                ;  1:4
        ADD     A, A                ;  1:4
        JR      c, GRL_NEG          ;  2:11/7

        CP      2*(BIAS-1)          ;
        SBC     A, A
        RET
GRL_NEG:
        CP      2*(BIAS-1)          ;
        CCF
        SBC     A, A
        RET
    endif
    
    if (MANT_BITS = 7)              ;           'eeee eeee smmm mmmm'
        BIT     7, L
        JR      nz, GRL_NEG
        LD      A, H
        CP      BIAS - 1            ;
        SBC     A, A
        RET                         ;
GRL_NEG:
        LD      A, BIAS - 2         ;
        CP      H
        SBC     A, A
        RET
    endif
    
    if (MANT_BITS = 10)             ;           'seee eemm mmmm mmmm'    
        LD      A, H
        AND     SIGN_MASK + EXP_MASK
        CP      BIAS - EXP_PLUS_ONE ;

        JR      c, GRL_NEG
        ADD     A, A
        JR      nc, GRL_NEG
        CP      2*BIAS - 2*EXP_PLUS_ONE - 1 ;
        CCF
GRL_NEG:        
        SBC     A, A
        RET
    endif
    
; Ground intersection
; Output: CF = true if ray intersects ground, HL = scale factor of D vector
; Pollutes: anything except IX
; LET n=-(y>=0 OR dy<=0): IF NOT n THEN LET s=-y/dy      if ( y < 0 && dy > 0 ) s = -y/dy;
GROUNDI:
    if defined print_name_fce
        CALL    PRINT_GROUNDI
    endif
        LD      BC, (GC+2)
        LD      HL, (DY)
        MSOR    B,C,H,L         ;           sign or sign
        ADD     A, A
        CCF                     ;           carry = not (sign or sign)
        RET     nc              ;           ani jeden neni zaporny
        CALL    FDIV            ;           HL = BC / HL = GC.y / D.y
        SCF
        RET                    
