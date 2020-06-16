if not defined @FEXP

    
    include "fmul.asm"
    include "color_flow_warning.asm"
    
; Natural exponential function
; Input: HL 
; Output: HL = exp(HL)) +- lowest 2 bit
; e^((2^e)*m) = 
; e^((2^e)*(m1+m0.5+m0.25+m0.125+m0.0.0625)) 
; m1 => b1 = 1, m0.5 => b0.5 = 0 or 1, m0.25 => b0.25 = 0 or 1, ...
; e^( b1*  (2^e) + b0.5*  (2^e-1) + b0.25*  (2^e-2) + b0.125*  (2^e-3) + b0.0625*  (2^e-4) + ... ) = 
;     b1*e^(2^e) * b0.5*e^(2^e-1) * b0.25*e^(2^e-2) * b0.125*e^(2^e-3) * b0.0625*e^(2^e-4) * ... 
; Pollutes: AF, BC, DE

@FEXP:
if not defined FEXP
; *****************************************
                    FEXP                ; *
; *****************************************
endif
        LD      D, high EXP_TAB     ;  2:7
        LD      A, H                ;  1:4
        ADD     A, A                ;  1:4
        LD      E, A                ;  1:4
        JR      nc, $+3             ;  2:7/11
        INC     D                   ;  1:4

        CP      2*$37               ;  2:7
        JR      c, FEXP_ONE         ;  2:7/11
        CP      2*$46               ;  2:7
        JR      nc, FEXP_FLOW       ;  2:7/11

        LD      A, L                ;  1:4      fraction

        EX      DE, HL              ;  1:4
        INC     L                   ;  1:4
        LD      D, (HL)             ;  1:7
        DEC     L                   ;  1:4
        LD      E, (HL)             ;  1:7

FEXP_ZEROBIT:
        JR      z, FEXP_EXIT        ;  2:7/11
FEXP_LOOP:
        DEC     L                   ;  1:4      exp--
        LD      B, (HL)             ;  1:7
        DEC     L                   ;  1:4
        ADD     A, A                ;  1:4
        JR      nc, FEXP_ZEROBIT    ;  2:7/11
        LD      C, (HL)             ;  1:7
        
        PUSH    HL                  ;  1:11
        PUSH    AF                  ;  1:11
        CALL    FMUL                ;  3:17     HL = BC * DE
        POP     AF                  ;  1:10
        EX      DE, HL              ;  1:4
        POP     HL                  ;  1:10
        
        JP      FEXP_LOOP           ;  3:10
        
FEXP_ONE:
        LD      DE, BIAS*256        ;  3:10     $4000
FEXP_EXIT:
        EX      DE, HL              ;  1:4
        RET                         ;  1:10
        
FEXP_FLOW:
        LD      A, H                ;  1:4
        ADD     A, A                ;  1:4      sign out
        JR      c, FEXP_UNDER       ;  2:7/11
FEXP_OVER:                          ;
    if color_flow_warning
        CALL    OVER_COL_WARNING    ;  3:17
    endif
    if carry_flow_warning
        SCF                         ;  1:4
    endif
        LD      HL, FPMAX           ;  3:10
        RET                         ;  1:10

FEXP_UNDER:
    if color_flow_warning
        CALL    UNDER_COL_WARNING   ;  3:17
    endif    
        LD      HL, FPMIN           ;  3:10
        RET                         ;  1:10

endif
