if not defined @FLN

    include "fadd.asm"

; logaritmus naturalis
; Input: HL 
; Output: HL = ln(abs(HL)) +- lowest bit (with exponent -1($7E) the error is bigger...)
; ln(2^e*m) = ln(2^e) + ln(m) = e*ln(2) + ln(m) = ln2_exp[e] + ln_m[m]
; Pollutes: AF, B, DE
@FLN:
if not defined FLN
; *****************************************
                     FLN                ; *
; *****************************************
endif
    if fix_ln
                                    ;           fixes input errors with exponent equal to -1 
        LD      A, H                ;  1:4       
        ADD     A, A                ;  1:4
        XOR     2*$3F               ;  2:7
        JR      z, FLN_FIX          ;  2:12/7 

    endif

        LD      A, H                ;  1:4      save

        LD      H, high LN_M        ;  2:7      LN_M[]        
        LD      E, (HL)             ;  1:7
        INC     H                   ;  1:4      hi LN_M[]
        LD      D, (HL)             ;  1:7

        ADD     A, A                ;  1:4      sign out, HL = abs(HL)
        LD      L, A                ;  1:4
        CP      2*BIAS              ;  2:7
        JR      z, FLN_NO_ADD       ;  2:7/11

        INC     H                   ;  1:4      LN2_EXP[]

        LD      A, (HL)             ;  1:7
        INC     L                   ;  1:4
        LD      H, (HL)             ;  1:7
        LD      L, A                ;  1:4
        
        LD      A, D                ;  1:4
        OR      E                   ;  1:4
        JP      nz, FADD            ;  3:10     HL = HL + DE = +-LN2_EXP[] + LN_M[]
        RET                         ;  1:10
        
    if fix_ln 
FLN_FIX:
        LD      H, high LN_FIX      ;  2:7
        LD      E, (HL)             ;  1:7
        INC     H                   ;  1:4
        LD      D, (HL)             ;  1:7
    endif

FLN_NO_ADD:
        EX      DE, HL              ;  1:4
        RET                         ;  1:10
endif
