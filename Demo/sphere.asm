; Look at a sphere
; Input: HL = scale factor of D to surface
; Output: CF color
; Pollutes: anything except IX
SPHEREL:
    if defined print_name_fce
        CALL    PRINT_SPHEREL       ;
    endif
        EX      DE, HL              ;
        CALL    SCALED              ;
        CALL    SCALEDL             ;
        CALL    HITRAY              ;

        LD      L, (IX+6)           ;
        LD      H, (IX+7)           ;           HL = @BnC, n = 1,2,...
        LD      DE, DX              ;
        LD      A, $03              ;
        CALL    FDOT                ;           BC = BnC.x*DX + BnC.y*DY + BnC.z*DZ = skalarni soucin = SS
        MMUL2   B, C                ;           BC = 2 * SS

        DEC     HL                  ;
        LD      D, (HL)             ;
        DEC     HL                  ;
        LD      E, (HL)             ;           DE = BnC.z
        PUSH    DE                  ;

        DEC     HL                  ;
        LD      D, (HL)             ;
        DEC     HL                  ;
        LD      E, (HL)             ;           DE = BnC.y
        PUSH    DE                  ;

        DEC     HL                  ;
        LD      D, (HL)             ;
        DEC     HL                  ;
        LD      E, (HL)             ;           DE = BnC.x
        PUSH    DE                  ;

        LD      L, (IX+10)
        LD      H, (IX+11)          ;           HL = r*r
    if defined print_register
        LD      A, COL_RED          ;
        CALL    PRINT_BC_COL        ;
        CALL    PRINT_HL_COL        ;
    endif
        CALL    FDIV                ;           HL = BC / HL = 2*SS/rr

        POP     BC                  ;           BC = BnC.x
        PUSH    HL                  ;
        EX      DE, HL              ;
        CALL    FMUL                ;           HL = DE * BC = 2*SS/rr * BnC.x
        EX      DE, HL              ;
        LD      HL, (DX)            ;
        CALL    FSUB                ;           HL = HL - DE = (DX) - 2*SS/rr * BnC.x
    if defined print_variables
        CALL    PRINT_DX            ;
    endif
        LD      (DX), HL            ;           (DX) =  (DX) - 2*SS/rr * BnC.x

        POP     DE                  ;
        POP     BC                  ;           BC = BnC.y
        PUSH    DE                  ;
        CALL    FMUL                ;           HL = DE * BC = 2*SS/rr * BnC.y
        EX      DE, HL              ;
        LD      HL, (DY)            ;
        CALL    FSUB                ;           HL = HL - DE = (DY) - 2*SS/rr * BnC.y
    if defined print_variables
        CALL    PRINT_DY            ;
    endif
        LD      (DY), HL            ;           (DY) = (DY) - 2*SS/rr * BnC.y

        POP     DE                  ;
        POP     BC                  ;           BC = BnC.z
        CALL    FMUL                ;           HL = DE * BC = 2*SS/rr * BnC.z
        EX      DE, HL              ;
        LD      HL, (DZ)            ;
        CALL    FSUB                ;           HL = HL - DE = (DZ) - 2*SS/rr * BnC.z
    if defined print_variables
        CALL    PRINT_DZ            ;
    endif
        LD      (DZ), HL            ;           (DZ) = (DZ) - 2*SS/rr * BnC.z

        JP      TRACEL


; Sphere intersection
; Output: CF = true if ray intersects sphere, HL = scale factor of D vector
; Pollutes: anything except IX
SPHEREI:
    if defined print_name_fce
        CALL    PRINT_SPHEREI
    endif
        LD      HL, (DX)
        PUSH    HL
        LD      HL, (DY)
        PUSH    HL
        LD      HL, (DZ)
        PUSH    HL                  ;           save D
        LD      L, (IX+6)
        LD      H, (IX+7)           ;           @B1C, @B2C
        LD      DE, PX
        PUSH    DE
        LD      BC, 6
        LDIR                        ;           (DE++) = (HL++), (PX=B1C.x PY=B1C.y PZ=B1C.z)
        POP     HL                  ;           HL = PX
        LD      DE, DX
        LD      A, 3
        CALL    FDOT                ;           D * P  ; (BC=sc=px*dx+py*dy+pz*dz)
        AND     A                   ;           clear carry
        MGE0    B, C                ;           zero flag if BC is positive
    if defined print_name_fce
        CALL    z, PRINT_TXT_CONTINUE
        CALL    nz, PRINT_TXT_EXIT
        CALL    PRINT_SPHEREI
    endif
        JR      nz, RESTD           ;           sphere behind us
        LD      HL, (DD)
        CALL    FDIV                ;           HL = BC / HL, HL = sc / DD, (??? LET bb=sc*sc/dd ???)
        PUSH    HL                  ;           save P * D / DD
        EX      DE, HL
        CALL    SCALED              ;           LET dx=dx*s: LET dy=dy*s: LET dz=dz*s
        LD      HL, (DX)
        LD      DE, (PX)
        CALL    FSUB                ;           HL = HL - DE = DX - PX
        CALL    FPOW2               ;           HL = HL * HL = ( DX - PX )^2
        PUSH    HL
        LD      HL, (DY)
        LD      DE, (PY)
        CALL    FSUB
        CALL    FPOW2               ;           HL = HL * HL = ( DY - PY )^2
        POP     DE
        CALL    FADDP               ;           HL = HL + DE = ( DX - PX )^2 + ( DY - PY )^2
        PUSH    HL
        LD      HL, (DZ)
        LD      DE, (PZ)
        CALL    FSUB
        CALL    FPOW2               ;           HL = HL * HL = ( DZ - PZ )^2
        POP     DE
        CALL    FADDP               ;           HL = HL + DE = ( DX - PX )^2 + ( DY - PY )^2 + ( DZ - PZ )^2
        LD      E, (IX+10)
        LD      D, (IX+11)          ;           DE = r*r
    if defined print_variables
        LD      A, COL_RED
        CALL    PRINT_DE_COL
        CALL    PRINT_HL_COL
    endif
        OR      A
        SBC     HL, DE
    if defined print_name_fce
        CALL    c, PRINT_TXT_CONTINUE
        CALL    nc, PRINT_TXT_EXIT
        CALL    PRINT_SPHEREI
    endif
        JR      nc, RESTD2
; SPHEREH:
        ADD     HL, DE
        EX      DE, HL
        CALL    FSUB                ;           HL = HL - DE = r*r - ( DX - PX )^2 - ( DY - PY )^2 - ( DZ - PZ )^2
        CALL    FSQRT               ;           HL = HL^0.5
        LD      C, L
        LD      B, H
        LD      HL, (DL)            ;           HL = SQRT( DD ) = DL
        CALL    FDIV                ;           HL = BC / HL = (r*r - ( DX - PX )^2 - ( DY - PY )^2 - ( DZ - PZ )^2)^0.5 / DL
        POP     DE                  ;           P * D / DD
        EX      DE, HL
        CALL    FSUBP               ;           HL = HL - DE = P * D / DD - (r*r - ( DX - PX )^2 - ( DY - PY )^2 - ( DZ - PZ )^2)^0.5 / DL
        SCF
        db      $16                 ;           opcode LD  D, $D1     ; 2:7
RESTD2:
        db      $D1                 ;           opcode POP DE         ; 1:10  clean P * D / DD
RESTD:
        POP     DE
        LD      (DZ), DE
        POP     DE
        LD      (DY), DE
        POP     DE
        LD      (DX), DE
        RET
