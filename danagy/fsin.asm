if not defined @FSIN

; Trigonometric function sine
; Input: HL -π/2..π/2
; Output: HL = sin(HL)
; Pollutes: AF, DE
@FSIN:
if not defined FSIN
; *****************************************
                    FSIN                ; *
; *****************************************
endif

        LD      A, H                ;  1:4
        AND     $FF - SIGN_MASK     ;  2:7          abs(HL)
        SUB     $3F                 ;  2:7
        JR      nc, FSIN_3F40       ;  2:12/7
        ADD     A, $02              ;  2:7
        JR      c, FSIN_3D3E        ;  2:12/7
        INC     A                   ;  1:4
        RET     nz                  ;  1:5/11
        LD      A, $71              ;  2:7
        SUB     L                   ;  1:4
        RET     nc                  ;  1:5/11
    if carry_flow_warning
        OR      A                   ;  1:4          reset carry
    endif
        DEC      L                  ;  1:4
        RET                         ;  1:10
FSIN_3D3E:
        LD      D, SIN_TAB_3D3E/256 ;  2:7
        RRA                         ;  1:4
        LD      A, L                ;  1:4
        RRA                         ;  1:4
        LD      E, A                ;  1:4
        LD      A, (DE)             ;  1:7
        JR      c, $+6              ;  2:12/7        
        RRA                         ;  1:4
        RRA                         ;  1:4
        RRA                         ;  1:4
        RRA                         ;  1:4
        OR      $F0                 ;  2:7
        RL      E                   ;  2:8
        
        JR      nc, FSIN_OK         ;  2:12/7
        JP      p, FSIN_OK          ;  3:10
        SUB     $08                 ;  2:7
FSIN_OK:
        LD      E, A                ;  1:4
        LD      D, $FF              ;  2:7
        ADD     HL, DE              ;  1:11
    if carry_flow_warning
        OR      A                   ;  1:4          reset carry
    endif
        RET                         ;  1:10
FSIN_3F40:
        ADD     A, high SIN_TAB_3F  ;  2:7
        EX      DE, HL              ;  1:4
        LD      H, A                ;  1:4
        LD      L, E                ;  1:4
        LD      L, (HL)             ;  1:7
        LD      H, $FF              ;  2:7
        ADD     HL, DE              ;  1:11
    if carry_flow_warning
        OR      A                   ;  1:4          reset carry
    endif
        RET                         ;  1:10

endif
