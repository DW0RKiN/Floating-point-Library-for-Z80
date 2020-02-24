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

        LD      A, H                ;           save

        LD      H, high LN_M        ;           LN_M[]        
        LD      E, (HL)             ;
        INC     H                   ;           hi LN_M[]
        LD      D, (HL)             ;

        ADD     A, A                ;           sign out, HL = abs(HL)
        LD      L, A                ;
        CP      2*BIAS              ;
        JR      z, FLN_NO_ADD       ;

        INC     H                   ;           LN2_EXP[]

        LD      A, (HL)             ; 
        INC     L                   ;
        LD      H, (HL)             ;
        LD      L, A                ;
        
        LD      A, D                ;
        OR      E                   ;
        JP      nz, FADD            ;           HL = HL + DE = +-LN2_EXP[] + LN_M[]
        RET                         ;
        
    if fix_ln 
FLN_FIX:
        LD      H, high LN_FIX      ;
        LD      E, (HL)             ;
        INC     H                   ;
        LD      D, (HL)             ;
    endif

FLN_NO_ADD:
        EX      DE, HL              ;
        RET                         ;
endif
