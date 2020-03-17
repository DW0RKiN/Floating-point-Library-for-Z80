if not defined @FSIN

; Trigonometric function sine
; Input: HL -π/2..π/2
; Output: HL = sin(HL)
; Pollutes: AF
; if ( abs(HL) < 0x7b69 ) return HL;
; if ( abs(HL) > 0x7bff ) use SIN_TAB;
; else return HL-1;
@FSIN:
if not defined FSIN
; *****************************************
                    FSIN                ; *
; *****************************************
endif

        LD      A, H                ;  1:4
        ADD     A, $100-$7C         ;  2:7
        JR      c, FSIN_7C7F        ;  2:11/7
        INC     A                   ;  2:7
        RET     nz                  ;  1:11/5
        LD      A, L                ;  1:4
        ADD     A, A                ;  1:4          sign out
        ADD     A, $100-2*$68       ;  2:7
        RET     nc                  ;  1:11/5
        DEC     L                   ;  1:4
    if carry_flow_warning
        OR      A                   ;  1:4          reset carry
    endif
        RET                         ;  1:10    
        
FSIN_7C7F:
        SRL     A                   ;  2:8          A = 0,1,2,3 => 0,1 + carry
        LD      A, H                ;  1:4
        LD      H, SIN_TAB_7C7D/256 ;  2:7
        JR      z, $+3              ;  2:11/7
        INC     H                   ;  1:4 
        RL      L                   ;  2:8          carry in and sign out
        LD      L, (HL)             ;  1:7
        RR      L                   ;  2:8          sign in
        SBC     A, $00              ;  2:7
        LD      H, A                ;  1:4
        RET                         ;  1:10

endif
