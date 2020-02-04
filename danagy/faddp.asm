if not defined @FADDP
        ; continue from @FADD (if it was included)
     
; Add two floating point numbers with the same sign
;  In: HL, DE numbers to add, no restrictions
; Out: HL = HL + DE,  if ( _test_over && overflow ) set carry
; Pollutes: AF, B, DE
; -------------- HL + DE ---------------
; HL = (+HL) + (+DE)
; HL = (-HL) + (-DE)
@FADDP:
if not defined FADDP
; *****************************************
                   FADDP                ; *
; *****************************************
endif
        LD      A, H                ;  1:4
        SUB     D                   ;  1:4
        JR      nc, FADDP_HL_GR     ;  2:7/12   
        EX      DE, HL              ;  1:4      
        NEG                         ;  2:8
FADDP_HL_GR:
        AND     EXP_MASK            ;  2:7
        JR      z, FADDP_EQ_EXP     ;  2:12/7   neresime zaokrouhlovani
        CP      2 + MANT_BITS       ;  2:7      pri posunu o NEUKLADANY_BIT+BITS_MANTIS uz mantisy nemaji prekryt, ale jeste se muze zaokrouhlovat 
        RET     nc                  ;  1:11/5   HL + DE = HL, RET with reset carry

                                    ;           Out: A = --( E | 1 0000 0000 ) >> A        
        LD      B, A                ;  1:4
        LD      A, E                ;  1:4
        DEC     A                   ;  1:4
        CP      $FF                 ;  2:7
        db      $1E                 ;  2:7      LD E, $B7
FADDP_LOOP:
        OR      A                   ;  1:4
        RRA                         ;  1:4
        DJNZ    FADDP_LOOP          ;  2:13/8
        
        JR      c, FADDP1           ;  2:12/7

        ADD     A, L                ;  1:4      soucet mantis
        JR      nc, FADDP0_SAME_EXP ;  2:12/7
FADDP_EXP_PLUS:                     ;           A = 10 mmmm mmmr, r = rounding bit                                    
        ADC     A, B                ;  1:4      rounding
        RRA                         ;  1:4      A = 01 cmmm mmmm
        LD      L, A                ;  1:4
        LD      A, H                ;  1:4        
        INC     H                   ;  1:4
        XOR     H                   ;  1:4      RET with reset carry
        RET     p                   ;  1:11/5        
        JR      FADDP_OVERFLOW      ;  2:12

FADDP0_SAME_EXP:                    ;           A = 01 mmmm mmmm 0
        LD      L, A                ;  1:4
        RET                         ;  1:10   
        
FADDP1:
        ADD     A, L                ;  1:4      soucet mantis
        JR      c, FADDP_EXP_PLUS   ;  2:12/7
        
FADDP1_SAME_EXP:                    ;           A = 01 mmmm mmmm 1, reset carry
if 1
        LD      L, A                ;  1:4
        INC     L                   ;  1:4
        RET     nz                  ;  1:11/5
        
        LD      A, H                ;  1:4        
        INC     H                   ;  1:4
        XOR     H                   ;  1:4      RET with reset carry
        RET     p                   ;  1:11/5
else
        LD      L, A                ;  1:4
        LD      A, H                ;  1:4        
        INC     HL                  ;  1:6
        XOR     H                   ;  1:4      RET with reset carry
        RET     p                   ;  1:11/5
endif
        JR      FADDP_OVERFLOW      ;  2:12

FADDP_EQ_EXP:                       ;           HL exp = DE exp
        LD      A, L                ;  1:4       1 mmmm mmmm
        ADD     A, E                ;  1:4      +1 mmmm mmmm
                                    ;           1m mmmm mmmm
        RRA                         ;  1:4      sign in && shift       
        LD      L, A                ;  1:4
        
        LD      A, H                ;  1:4        
        INC     H                   ;  1:4
        XOR     H                   ;  1:4      RET with reset carry
        RET     p                   ;  1:11/5
        ; fall

; In: H = s111 1111 + 1
; Out: HL = +- MAX
FADDP_OVERFLOW:
        DEC     H                   ;  1:4      
        LD      L, $FF              ;  2:7
    if color_flow_warning
        CALL    OVER_COL_WARNING    ;  3:17
    endif
    if defined _test_over
        SCF                         ;  1:4      carry = error
    endif
        RET                         ;  1:10

    include "color_flow_warning.asm"
endif
