if not defined @FMUL

        ; continue from @FDIV (if it was included)

; Floating-point multiplication
;  In: DE, BC multiplicands
; Out: HL = BC * DE, if ( carry_flow_warning && (overflow || underflow )) set carry;
; Pollutes: AF, BC, DE

; EEEE EEEE SMMM MMMM
; Sign       0 .. 1          = ???? ???? 0??? ???? .. ???? ???? 1??? ???? 
; Exp     -127 .. 128        = 0000 0000 ???? ???? .. 1111 1111 ???? ????;   (Bias 127 = $7F)
; Mantis   1.0 .. 1,9921875  = ???? ???? ?000 0000 .. ???? ???  ?111 1111 = 1.000 0000 .. 1.111 1111
; use Tab_AmB
; use Tab_ApB
@FMUL:
if not defined FMUL
; *****************************************
                   FMUL                ; *
; *****************************************
endif
        LD      A, C                ;  1:4
        XOR     E                   ;  1:4
        OR      $FF - SIGN_MASK     ;  2:7
        LD      L, A                ;  1:4          s111 1111

        ; A = (EXP-BIAS + EXP-BIAS) + BIAS = EXP-BIAS + EXP
;                                                                      c1  c2
        ; 128 + 128 =  256! => $FF + $FF - $7F = $1FE - $7F = $17F!     1   0
        ;  64 +  65 =  129! => $BF + $C0 - $7F = $17F - $7F = $100!     1   0
        ;  64 +  64 =  128  => $BF + $BF - $7F = $17E - $7F =  $FF      1   1
        ;   1 +   1 =    2  => $80 + $80 - $7F = $100 - $7F =  $81      1   1
        ;   0 +   1 =    1  => $7F + $80 - $7F =  $FF - $7F =  $80      0   0
        ; -63 + -64 = -127  => $40 + $3F - $7F =  $7F - $7F =  $00      0   0
        ; -64 + -64 = -128! => $3F + $3F - $7F =  $7E - $7F = $FFF!     0   1
        ;-127 +-127 = -254! => $00 + $00 - $7F =  $00 - $7F = $F81!     0   1
        
        ; overflow             $17F..$1FE - $7F =       $100..$17F   
        ; ok                    $7F..$17E - $7F =        $00..$FF   (u $FF a vysokych mantis to jeste pretece na $00)
        ; underflow             $00.. $7E - $7F =       $F81..$FFF  (u $FF je sance ze u vysokych mantis se to jeste zachrani a preleze na $00)

        LD      A, D                ;  1:4
        ADD     A, B                ;  1:4
        JR      c, FMUL_C1          ;  2:12/7
        SUB     BIAS                ;  2:7
        JR      nc, FMUL_NEXT       ;  2:12/7
        
        INC     A                   ;  1:4          $FF => $00?
        JR      nz, FMULP_UNDERFLOW ;  2:11/7       no chance        
        INC     D                   ;  1:4
        CALL    FMUL                ;  3:17         recursion
        DEC     H                   ;  1:4
        RET     z                   ;  1:11/5      
FMULP_UNDERFLOW:
        LD      H, $00              ;  1:4          HL = 0000 0000 s000 0000
    if color_flow_warning
        CALL    UNDER_COL_WARNING   ;  3:17
    endif
        LD      A, L                ;  1:4          s111 1111
        AND     SIGN_MASK           ;  2:7          
        LD      L, A                ;  1:4
    if carry_flow_warning
        SCF                         ;  1:4          carry = error
    endif
        RET                         ;  1:10
        
FMUL_C1:
        SUB     BIAS                ;  2:7
        JR      nc, FMULP_OVERFLOW  ;  2:12/7        
FMUL_NEXT:
        LD      H, A                ;  1:4          new exponent
        PUSH    HL                  ;  1:11
        
        RES     7, C                ;  2:8
        RES     7, E                ;  2:8
                
        LD      A, C                ;  1:4
        ADD     A, E                ;  1:4
        LD      E, A                ;  1:4          E = A + B
        LD      D, Tab_ApB_lo/256   ;  2:7          (DE) = Tab_ApB_lo

        SUB     C                   ;  1:4
        SUB     C                   ;  1:4
        LD      L, A                ;  1:4          L = A - B
        LD      H, Tab_AmB_lo/256   ;  2:7

        LD      A, (DE)             ;  1:7
        ADD     A, (HL)             ;  1:7
        LD      C, A                ;  1:4
        INC     D                   ;  1:4
        LD      A, (DE)             ;  1:7
        INC     H                   ;  1:4
        ADC     A, (HL)             ;  1:7
                                    ; 17:78
        JP      p, FMULP_SAME_EXP   ;  3:10
                                    ;               (ApB)+(AmB) >= $8000 => pricti: $40 (60.882568%)
                                    ;               AC = 1mmm mmmm mmmm mmmm
        POP     HL                  ;  1:10
        INC     H                   ;  1:4          exp++
        JR      z, FMULP_OVERFLOW   ;  2:7/12

        AND     L                   ;  1:4
        LD      L, A                ;  1:4

        LD      A, C                ;  1:4
        ADD     A, $40              ;  2:7
        RET     nc                  ;  1:11/5       RET with reset carry
                                    ; 10:51
        
        INC     L                   ;  1:4          25%
    if carry_flow_warning
        OR      A                   ;  1:4          RET with reset carry
    endif
        RET                         ;  1:10
        

FMULP_OVERFLOW:
        LD      H, $FF              ;  3:10
    if color_flow_warning
        CALL    OVER_COL_WARNING    ;  3:17
    endif
    if carry_flow_warning
        SCF                         ;  1:4          carry = error
    endif
        RET                         ;  1:10
        
        
FMULP_SAME_EXP:                     ;               (ApB)+(AmB) >= $4000 => pricti: $0 (38.882446%)
                                    ;               AC = 01mm mmmm mmmm mmmm
        RL      C                   ;  2:8          
        ADC     A, A                ;  1:4
        POP     HL                  ;  1:10         AC = 1mmm mmmm mmmm mmm0
        AND     L                   ;  1:4          RET with reset carry
        LD      L, A                ;  1:4
        RET                         ;  1:10

    include "color_flow_warning.asm"
        
endif
