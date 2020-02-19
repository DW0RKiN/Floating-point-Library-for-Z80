if not defined @FLN

    include "fadd.asm"

; logaritmus naturalis
; Input: HL 
; Output: HL = ln(abs(HL)) +- lowest bit (with exponent -1($3A..$3B) the error is bigger...)
; ln(2^e*m) = ln(2^e) + ln(m) = e*ln(2) + ln(m) = ln2_exp[e] + ln_m[m]
; Pollutes: AF, B, DE
@FLN:
if not defined FLN
; *****************************************
                     FLN                ; *
; *****************************************
endif
        LD      B, H                ;           save exp
        
        LD      A, H                ;           
        AND     MANT_MASK_HI        ;           HL = abs(HL)
        ADD     A, A                ;
        ADD     A, high LN_M        ;
        LD      H, A                ;           LN_M[]        
        LD      E, (HL)             ;
        INC     H                   ;           hi LN_M[]
        LD      D, (HL)             ;

        LD      A, B                ;           load exp
        AND     EXP_MASK            ;
        CP      BIAS                ;
        JR      z, FLN_NO_ADD       ;
        RRCA                        ;
        LD      H, high LN2_EXP     ;
        LD      L, A                ;           LN2_EXP[]

        LD      A, (HL)             ; 
        INC     L                   ;
        LD      H, (HL)             ;
        LD      L, A                ;
        
        LD      A, D                ;
        OR      E                   ;
        JP      nz, FADD            ;           HL = HL + DE = +-LN2_EXP[] + LN_M[]
        RET                         ;
FLN_NO_ADD:
        EX      DE, HL              ;
        RET                         ;
endif
