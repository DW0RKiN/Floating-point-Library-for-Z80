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
        LD      A, H                ;
        CP      $77                 ;
        JR      c, FEXP_ONE         ;
        CP      $86                 ;
        JR      nc, FEXP_FLOW       ;
        
        SUB     $70                 ;
        LD      H, A                ;        
        
        LD      A, L                ;           fraction        
        ADD     A, A                ;           sign out
        LD      L, H                ;           exp
        LD      H, high EXP_TAB     ;
        JR      nc, $+4             ;
        SET     7, L                ;
        
        LD      E, (HL)             ;
        PUSH    HL                  ;
        INC     H                   ;
        LD      D, (HL)             ;

FEXP_LOOP:
        POP     HL                  ;
FEXP_ZEROBIT:
        JR      z, FEXP_EXIT        ;
        DEC     L                   ;           exp--
        ADD     A, A                ;
        JR      nc, FEXP_ZEROBIT    ;
        LD      C, (HL)             ;
        PUSH    HL                  ;
        INC     H                   ;
        LD      B, (HL)             ;
        
        PUSH    AF                  ;
        CALL    FMUL                ;           HL = BC * DE
        POP     AF                  ;
        EX      DE, HL              ;
        
        JP      FEXP_LOOP           ;
        
FEXP_ONE:
        LD      DE, BIAS*256        ;           $7f00
FEXP_EXIT:
        EX      DE, HL              ;
        RET                         ;
        
FEXP_FLOW:
        LD      A, L                ;           fraction        
        ADD     A, A                ;           sign out
        JR      c, FEXP_UNDER       ;
FEXP_OVER:                          ;
    if color_flow_warning
        CALL    OVER_COL_WARNING    ;
    endif
    if carry_flow_warning
        SCF                         ;
    endif
        LD      HL, FPMAX           ;
        RET                         ;

FEXP_UNDER:
    if color_flow_warning
        CALL    UNDER_COL_WARNING   ;
    endif    
        LD      HL, FPMIN           ;
        RET                         ;

endif
