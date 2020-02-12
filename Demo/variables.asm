DX:     EQU     $                   ;           dx = x - 128 (trace.asm)
DY:     EQU     DX+2                ;           dy = y - 96 (trace.asm)
DZ:     EQU     DY+2                ;           dz = 300 (trace.asm)
DD:     EQU     DZ+2                ;           dd=dx*dx+dy*dy+dz*dz (trace.asm -> dot.asm )
DL:     EQU     DD+2                ;           dl = sqrt( dd ) (trace.asm -> fsquare.asm )
PX:     EQU     DL+2
PY:     EQU     PX+2
PZ:     EQU     PY+2
PP:     EQU     PZ+2                ;           pp=px*px+py*py+pz*pz
PL:     EQU     PP+2
TL:     EQU     PL+2
PD:     EQU     TL+2
CC:     EQU     PD+2
CL:     EQU     CC+2
BL:     EQU     CL+2
SC:     EQU     BL+2                ;           sc=px*dx+py*dy+pz*dz
DIST:   EQU     SC+2
NEAREST:EQU     DIST+2
BOUNCE: EQU     NEAREST+2           ;           stacil by 1 bajt...
GC:     EQU     BOUNCE+2            ;           -0.3,  0.5, 0
B1C:    EQU     GC+6                ;           -0.6, -0.3, 3
B2C:    EQU     B1C+6               ;            0.6, -0.6, 2
