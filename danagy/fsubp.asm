if not defined @FSUBP

include "color_flow_warning.asm"

; Subtraction two floating-point numbers with the same signs
;  In: HL,DE numbers to add, no restrictions
; Out: HL = HL + DE, if ( carry_flow_warning && underflow ) set carry
; Pollutes: AF, BC, DE
; -------------- HL - DE ---------------
; HL = (+HL) - (+DE) = (+HL) + (-DE)
; HL = (-HL) - (-DE) = (-HL) + (+DE)
@FSUBP:
if not defined FSUBP
; *****************************************
                   FSUBP                ; *
; *****************************************
endif
        LD      A, D                ;  1:4
        XOR     SIGN_MASK           ;  2:7
        LD      D, A                ;  1:4

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
        JP      m, FSUBP_HL_GR      ;  3:10
        EX      DE, HL              ;  1:4
        LD      A, H                ;  1:4
        SUB     D                   ;  1:4
FSUBP_HL_GR:
        AND     EXP_MASK            ;  2:7
        JR      z, FSUBP_EQ_EXP     ;  2:12/7

        CP      2 + MANT_BITS       ;  2:7      pri posunu vetsim nez o MANT_BITS + NEUKLADANY_BIT + ZAOKROUHLOVACI_BIT uz mantisy nemaji prekryt
        JR      nc, FSUBP_TOOBIG    ;  2:12/7   HL - DE = HL
        
                                    ;           Out: E = ( E | 1 0000 0000 ) >> A        
        LD      B, A                ;  1:4
        LD      A, E                ;  1:4
        RRA                         ;  1:4      1mmm mmmm m
        DEC     B                   ;  1:4
        JR      z, FSUBP_NOLOOP     ;  2:12/7
        DEC     B                   ;  1:4
        JR      z, FSUBP_LAST       ;  2:12/7
FSUBP_LOOP:
        OR      A                   ;  1:4
        RRA                         ;  1:4
        DJNZ    FSUBP_LOOP          ;  2:13/8
FSUBP_LAST:
        RL      B                   ;  2:8      B = rounding 0.25
        RRA                         ;  1:4        
FSUBP_NOLOOP:                       ;           carry = rounding 0.5
        LD      E, A                ;  1:4
        LD      A, L                ;  1:4              
        
        JR      c, FSUBP1           ;  2:12/7
        
        SUB     E                   ;  1:4
        JR      nc, FSUBP0_SAME_EXP ;  2:12/7
        
FSUBP_NORM_RESET:
        OR      A                   ;  1:4
FSUBP_NORM:                         ;           normalizace cisla
        DEC     H                   ;  1:4      exp--
        ADC     A, A                ;  1:4
        JR      nc, FSUBP_NORM      ;  2:7/12
        
        SUB     B                   ;  1:4
        
        LD      L, A                ;  1:4        
        LD      A, D                ;  1:4
        XOR     H                   ;  1:4
        RET     m                   ;  1:11/5   RET with reset carry
        JR      FSUBP_UNDERFLOW     ;  2:12

FSUBP0_SAME_EXP:                    ;           reset carry
        LD      L, A                ;  1:4      
        RET     nz                  ;  1:11/5

        SUB     B                   ;  1:4      exp--? => rounding 0.25 => 0.5 
        RET     z                   ;  1:11/5

        DEC     HL                  ;  1:6
        LD      A, D                ;  1:4
        XOR     H                   ;  1:4
        RET     m                   ;  1:11/5   RET with reset carry
        JR      FSUBP_UNDERFLOW     ;  2:12

FSUBP1:
        SBC     A, E                ;  1:4      rounding half down
        JR      c, FSUBP_NORM       ;  2:12/7   carry => need half up
        LD      L, A                ;  1:4 
        RET                         ;  1:10
   
FSUBP_EQ_EXP:
        LD      A, L                ;  1:4
        SUB     E                   ;  1:4
        JR      z, FSUBP_UNDERFLOW  ;  2:12/7   (HL_exp = DE_exp && HL_mant = DE_mant) => HL = -DE
        JR      nc, FSUBP_EQ_NORM   ;  2:12/7
        EX      DE, HL              ;  1:4
        NEG                         ;  2:8

FSUBP_EQ_NORM:                      ;           normalizace cisla
        DEC     H                   ;  1:4      exp--
        ADD     A, A                ;  1:4      musime posouvat minimalne jednou, protoze NEUKLADANY_BIT byl vynulovan
        JR      nc, FSUBP_EQ_NORM   ;  2:7/12
        
        LD      L, A                ;  1:4        
        LD      A, D                ;  1:4
        XOR     H                   ;  1:4 
        RET     m                   ;  1:11/5

FSUBP_UNDERFLOW:
        LD      L, $00              ;  2:7      
        LD      A, D                ;  1:4
        CPL                         ;  1:4
        AND     SIGN_MASK           ;  2:7
        LD      H, A                ;  1:4
if color_flow_warning
        CALL    UNDER_COL_WARNING   ;  3:17
endif
if carry_flow_warning
        SCF                         ;  1:4      carry = error
endif
        RET                         ;  1:10


FSUBP_TOOBIG:
        RET     nz                  ;  1:11/5   HL_exp - DE_exp > 7+1+1 => HL - DE = HL

        LD      A, L                ;  1:4
        OR      A                   ;  1:4
        RET     nz                  ;  1:11/5   HL_mant > 1.0           => HL - DE = HL

        DEC     L                   ;  1:4
        DEC     H                   ;  1:4      HL_exp = 8 + 1 + DE_exp  => HL_exp >= 9 => not underflow
        RET                         ;  1:10

endif
