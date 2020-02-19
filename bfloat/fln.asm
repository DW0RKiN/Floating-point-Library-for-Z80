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
        LD      A, L                ;           save
        
        LD      L, H                ;
        LD      H, 1+(high LN2_EXP) ;           hi LN2_EXP[]
        LD      D, (HL)             ;
        DEC     H                   ;           lo LN2_EXP[]
        LD      E, (HL)             ;
        
        ADD     A, A                ;
        JR      z, FLN_NO_ADD       ;
        
        DEC     H                   ;           LN_M[]        
        LD      L, A                ;
        LD      A, (HL)             ; 
        INC     L                   ;
        LD      H, (HL)             ;
        LD      L, A                ;
        
        LD      A, D                ;
        OR      E                   ;
        JP      nz, FADD            ;           HL = HL + DE = LN_M[] + (+-LN2_EXP[])
        RET                         ;
FLN_NO_ADD:
        EX      DE, HL              ;
        RET                         ;
endif
