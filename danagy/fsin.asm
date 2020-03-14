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
        JR      z, FSIN_3D          ;  2:12/7
        INC     L                   ;  1:4
        DEC     L                   ;  1:4
        JP      nz, FSIN_3D         ;  3:10
        DEC     H                   ;  1:4          HL = $3E00 => $3DFB
        LD      L, $FB              ;  2:7
        RET                         ;  1:10        
FSIN_3D:
        XOR     L                   ;  1:4
        AND     $03                 ;  2:7
        XOR     L                   ;  1:4
        RRCA                        ;  1:4          elll lll0
        LD      D, SIN_TAB_3D3E/256 ;  2:7
        LD      E, A                ;  1:4
        EX      DE, HL              ;  1:4
        LD      A, (HL)             ;  1:7
        CP      E                   ;  1:4        
        INC     L                   ;  1:4
        JR      nc, $+4             ;  2:11/7
        INC     L                   ;  1:6
        INC     L                   ;  1:4
        LD      L, (HL)             ;  1:7
        LD      H, $FF              ;  2:7
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
