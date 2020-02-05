if not defined @FADDP
        ; continue from @FADD (if it was included)
     
; Add two floating point numbers with the same sign
;  In: HL, DE numbers to add, no restrictions
; Out: HL = HL + DE,  if ( carry_flow_warning && overflow ) set carry
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
        JR      z, FADDP_EQ_EXP     ;  2:7/12
        JR      nc, FADDP_HL_GR     ;  2:7/12   
        EX      DE, HL              ;  1:4      
        NEG                         ;  2:8
FADDP_HL_GR:
        CP      2 + MANT_BITS       ;  2:7      pri posunu o NEUKLADANY_BIT+BITS_MANTIS uz mantisy nemaji prekryt, ale jeste se muze zaokrouhlovat 
        RET     nc                  ;  1:5/11   HL + DE = HL

                                    ;           Out: E = --( E | 1000 0000 ) >> (A-1)        
        SET     7, E                ;  2:8
        DEC     E                   ;  1:4
        DEC     A                   ;  1:4
        JR      z, FADDP_STOP       ;  2:12/7
        LD      B, A                ;  1:4
FADDP_LOOP:
        SRL     E                   ;  2:8
        DJNZ    FADDP_LOOP          ;  2:13/8
FADDP_STOP:

        LD      A, L                ;  1:4
        ADD     A, A                ;  1:4      A = 01 mmmm mmm0, kvuli zaokrouhleni potrebujeme znat hodnotu prvniho bitu za desetinou carkou 
        ADD     A, E                ;  1:4      soucet mantis
        JR      nc, FADDP_SAME_EXP  ;  2:12/7
                                    ;           A = 10 mmmm mmri, r = rounding bit, i = ignored bit, carry = 1 :(
        ADD     A, $02              ;  2:7      rounding
        RRA                         ;  1:4      A = 01 cmmm mmmr
FADDP_EXP_PLUS:
        RL      L                   ;  2:8      sign out
        RRA                         ;  1:4      sign in && shift
        LD      L, A                ;  1:4
    if carry_flow_warning
        OR      A                   ;  1:4      RET with reset carry
    endif
        INC     H                   ;  1:4      exp++
        RET     nz                  ;  1:11/5
        JR      FADDP_OVERFLOW      ;  2:12

FADDP_SAME_EXP:                     ;           A = 01 mmmm mmmr, r = rounding bit
    if 1
        RL      L                   ;  2:8      sign out
        RRA                         ;  1:4      sign in && shift       
        LD      L, A                ;  1:4
        RET     nc                  ;  1:11/5   50%
        
        INC     L                   ;  1:4      rounding
        XOR     L                   ;  1:4      clear carry
        RET     p                   ;  1.11/5   49% same sign
                                    ;           A = 1111 1111, L = z000 0000, z = 1 - s
        LD      A, L                ;  1:4      
        XOR     SIGN_MASK           ;  2:7
        LD      L, A                ;  1:4
        
        INC     H                   ;  1:4      exp++
        RET     nz                  ;  1:11/5
        JR      FADDP_OVERFLOW      ;  2:12        
    else
        INC     A                   ;  1:4      rounding
        JR      z, FADDP_EXP_PLUS   ;  2:12/7   A = 10 0000 0000 && carry = 0
        
        RL      L                   ;  2:8      sign out
        RRA                         ;  1:4      sign in && shift       
        LD      L, A                ;  1:4
    if carry_flow_warning
        OR      A                   ;  1:4      RET with reset carry
    endif
        RET                         ;  1:10
    endif

FADDP_EQ_EXP:                       ;           HL exp = DE exp
        LD      A, L                ;  1:4        1mmm mmmm    0mmm mmmm
        ADD     A, E                ;  1:4       +1mmm mmmm    0mmm mmmm
                                    ;           1 mmmm mmmm  0 mmmm mmmm shodna znamenka se promeni v carry
        RRA                         ;  1:4      sign in && shift       
        LD      L, A                ;  1:4

    if carry_flow_warning
        OR      A                   ;  1:4      RET with reset carry
    endif
        INC     H                   ;  1:4      exp++
        RET     nz                  ;  1:11/5
        ; fall

; In: A = L = smmm mmmm, H = 0
; Out: HL = +- MAX
FADDP_OVERFLOW:
        DEC     H                   ;  1:4      $00 => $FF
        OR      SIGN_XOR            ;  2:7
        LD      L, A                ;  1:4
    if color_flow_warning
        CALL    OVER_COL_WARNING    ;  3:17
    endif
    if carry_flow_warning
        SCF                         ;  1:4      carry = error
    endif
        RET                         ;  1:10

    include "color_flow_warning.asm"
    
endif
