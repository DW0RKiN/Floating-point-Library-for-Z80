if not defined @FSUBP

include "color_flow_warning.asm"

; Subtraction two floating-point numbers with the same signs
;  In: HL,DE numbers to add, no restrictions
; Out: HL = HL + DE, if ( carry_flow_warning && underflow ) set carry
; Pollutes: AF, B, DE
; -------------- HL - DE ---------------
; HL = (+HL) - (+DE) = (+HL) + (-DE)
; HL = (-HL) - (-DE) = (-HL) + (+DE)
@FSUBP:
if not defined FSUBP
; *****************************************
                   FSUBP                ; *
; *****************************************
endif
        LD      A, E                ;  1:4
        XOR     SIGN_MASK           ;  2:7
        LD      E, A                ;  1:4

; Add two floating-point numbers with the opposite signs
;  In: HL, DE numbers to add, no restrictions
; Out: HL = HL + DE
; Pollutes: AF, B, DE
; -------------- HL + DE ---------------
; HL = (+HL) + (-DE)
; HL = (-HL) + (+DE)
FSUBP_FADD_OP_SGN:
        LD      A, H                ;  1:4
        SUB     D                   ;  1:4
        JR      z, FSUBP_EQ_EXP     ;  2:12/7
        JR      nc, FSUBP_HL_GR     ;  2:12/7
        EX      DE, HL              ;  1:4
        NEG                         ;  2:8
FSUBP_HL_GR:

        CP      2 + MANT_BITS       ;  2:7      pri posunu vetsim nez o MANT_BITS + NEUKLADANY_BIT + ZAOKROUHLOVACI_BIT uz mantisy nemaji prekryt
        JR      nc, FSUBP_TOOBIG    ;  2:12/7   HL - DE = HL
        
                                    ;           Out: E = ( E | 1000 0000 ) >> (A-1)        
        LD      B, A                ;  1:4
        LD      A, E                ;  1:4
        OR      SIGN_MASK           ;  2:7
        DEC     B                   ;  1:4
        JR      z, FSUBP_NOLOOP     ;  2:12/7
FSUBP_LOOP:
        OR      A                   ;  1:4
        RRA                         ;  1:4
        DJNZ    FSUBP_LOOP          ;  2:13/8
        RL      B                   ;  2:8      B = rounding 0.25
FSUBP_NOLOOP:                       ;
        LD      E, A                ;  1:4
        LD      A, L                ;  1:4
        ADD     A, A                ;  1:4
        
        SUB     E                   ;  1:4
        JR      nc, FSUBP_SAME_EXP  ;  2:12/7
        
        LD      D, H                ;  1:4      save exp
FSUBP_NORM:                         ;           normalizace cisla
        DEC     H                   ;  1:4      exp--
        ADD     A, A                ;  1:4
        JR      nc, FSUBP_NORM      ;  2:7/12
        
        SUB     B                   ;  1:4
        
        RL      L                   ;  2:8      sign out  
        RRA                         ;  1:4      sign in
        LD      L, A                ;  1:4
        LD      A, D                ;  1:4
        SUB     H                   ;  1:4
        RET     nc                  ;  1:11/5   RET with reset carry
        JR      FSUBP_UNDERFLOW     ;  2:12
    
FSUBP_SAME_EXP:                     ;  2:8      reset carry

        RL      L                   ;  2:8      sign out  
        RRA                         ;  1:4      sign in
        LD      L, A                ;  1:4
        ADC     A, A                ;  1:4      
    if carry_flow_warning
        OR      A                   ;  1:4
    endif
        RET     nz                  ;  1:11/5
        
        DEC     B                   ;  1:4
        RET     nz                  ;  1:11/5
        
        LD      A, L                ;  1:4
        OR      MANT_MASK           ;  2:7
        LD      L, A                ;  1:4
        LD      A, H                ;  1:4
        DEC     H                   ;  1:4
        SUB     H                   ;  1:4
        RET     nc                  ;  1:11/5   RET with reset carry
        JR      FSUBP_UNDERFLOW     ;  2:12
        
;   abs(L)  >   abs(E)
; 1100 0000 - 0000 0100 = $0C0 - $04 = $BC = 1011 1100
; 0100 0000 - 1000 0100 = $140 - $84 = $BC = 1011 1100
;   abs(L)  =   abs(E)
; 1100 0000 - 0100 0000 = $0C0 - $40 = $80 = 1000 0000
; 0100 0000 - 1100 0000 = $140 - $C0 = $80 = 1000 0000
;   abs(L)  <   abs(E)
; 1000 0100 - 0100 0000 = $084 - $40 = $44 = 0100 0100
; 0000 0100 - 1100 0000 = $104 - $C0 = $44 = 0100 0100
FSUBP_EQ_EXP:
        LD      A, L                ;  1:4
        SUB     E                   ;  1:4
        ADD     A, A                ;  1:4      delete sign
        JR      z, FSUBP_UNDERFLOW  ;  2:12/7   (HL_exp = DE_exp && HL_mant = DE_mant) => HL = -DE
        JR      c, FSUBP_EQ_LOOP    ;  2:12/7
        EX      DE, HL              ;  1:4
        NEG                         ;  2:8

FSUBP_EQ_LOOP:                      ;           normalizace cisla
        DEC     H                   ;  1:4      exp--
        ADD     A, A                ;  1:4      musime posouvat minimalne jednou, protoze NEUKLADANY_BIT byl vynulovan
        JR      nc, FSUBP_EQ_LOOP   ;  2:7/12
        
        RL      L                   ;  2:8      sign out
        RRA                         ;  1:4      sign in
        LD      L, A                ;  1:4
        
        LD      A, D                ;  1:4      old exp
        SUB     H                   ;  1:4      -new exp
        RET     nc                  ;  1:11/5

FSUBP_UNDERFLOW:
        LD      H, $00              ;  2:7      sign only
        LD      A, L                ;  1:4
        AND     SIGN_MASK           ;  2:7
        LD      L, A                ;  1:4
    if color_flow_warning
        CALL    UNDER_COL_WARNING   ;  3:17
    endif
    if carry_flow_warning
        SCF                         ;  1:4      carry = error
    endif
        RET                         ;  1:10


FSUBP_TOOBIG:
        RET     nz                  ;  1:11/5   HL_exp - DE_exp > 7+1+1 => HL - DE = HL

; $3200 - $297F
; HL_exp - DE_exp = 9
;              L = 1 0000 000
;         L << 9 = 1 0000 0000 0000 0000
;             DE = 0 0000 0000 1111 1111
;(HL << 9 ) - DE = 0 1111 1111.0000 0001, HL_exp--
;                    7654 3210
; zaokrouhleni   = 0 1111 1111           !!!

; $3200 - $2924
; HL_exp - DE_exp = 9
;              L = 1 0000 000
;         L << 9 = 1 0000 0000 0000 0000
;             DE = 0 0000 0000 1010 0100
;(HL << 9 ) - DE = 0 1111 1111.0101 1100, HL_exp--
;                    7654 3210
; zaokrouhleni   = 0 1111 1111           !!!

; $3200 - $2900
; HL_exp - DE_exp = 9
;              L = 1 0000 000
;         L << 9 = 1 0000 0000 0000 0000
;             DE = 0 0000 0000 1000 0000
;(HL << 9 ) - DE = 0 1111 1111.1000 0000, HL_exp--
;                    7654 3210
; zaokrouhleni   = 0 1111 1111           !!!

; $3201 - $297F
; HL_exp - DE_exp = 9
;              L = 1 0000 001
;         L << 9 = 1 0000 0010 0000 0000
;             DE = 0 0000 0000 1111 1111
;(HL << 9 ) - DE = 1 0000 0001 0000 0001
;                  7 6543 210
; zaokrouhleni   = 1 0000 001               

; $3201 - $2924
; HL_exp - DE_exp = 9
;              L = 1 0000 001
;         L << 9 = 1 0000 0010 0000 0000
;             DE = 0 0000 0000 1010 0100
;(HL << 9 ) - DE = 1 0000 0001 0101 1100
;                  7 6543 210
; zaokrouhleni   = 1 0000 001               

; $3201 - $2900
; HL_exp - DE_exp = 9
;              L = 1 0000 001
;         L << 9 = 1 0000 0010 0000 0000
;             DE = 0 0000 0000 1000 0000
;(HL << 9 ) - DE = 1 0000 0001 1000 0000
;                  7 6543 210
; zaokrouhleni   = 1 0000 001      


        LD      A, L                ;  1:4
    if carry_flow_warning
        AND     MANT_MASK           ;  2:7
        RET     nz                  ;  1:11/5   HL_mant > 1.0           => HL - DE = HL

        LD      A, L                ;  1:4
        OR      MANT_MASK           ;  2:7
        LD      L, A                ;  1:4
    else
        ADD     A, A                ;  1:4      sign out
        RET     nz                  ;  1:11/5   HL_mant > 1.0           => HL - DE = HL

        DEC     A                   ;  1:4      $00 => $ff
        RRA                         ;  1:4      sign in
        LD      L, A                ;  1:4
    endif
        DEC     H                   ;  1:4      HL_exp = 7 + 1 + DE_exp  => HL_exp >= 8 => not underflow
        RET                         ;  1:10

endif
