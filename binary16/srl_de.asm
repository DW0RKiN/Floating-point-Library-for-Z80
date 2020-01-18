if not defined @SRL_DE

;  In: DE, A = 2*(SHIFT + 1) = 2..2*(MANT_BITS+1)
; Out: DE = (( DE & 11 1111 1111 ) | 100 0000 0000 ) >> ((A/2)-2)
@SRL_DE:
SRL_DE:
SRL_DE_POSUN    EQU     (SRLDEC_DE_SWITCH - SRL_DE_SWITCH - 2)

        ADD     A, SRL_DE_POSUN     ;
        LD      (SRL_DE_SWITCH), A  ;  3:13
        LD      A, D                ;  1:4
        AND     MANT_MASK_HI        ;  2:7
        OR      EXP_PLUS_ONE        ;  2:7      clear carry
      
SRL_DE_SWITCH   EQU     $+1
        JR      SRLDEC_DE_CASE_0    ;  2:12
        

;  In: DE, A = 2*(SHIFT + 1) = 2..2*(MANT_BITS+1)
; Out: DE = --(( DE & 11 1111 1111 ) | 100 0000 0000 ) >> ((A/2)-1)
@SRLDEC_DE:
SRLDEC_DE:
        LD     (SRLDEC_DE_SWITCH), A;  3:13        
        SET     2, D                ;  2:8      pridam NEUKLADANY_BIT
        DEC     DE                  ;  1:6      down(0 .. 0.5) => down(0 .. 0.4999), takze pri zaokrouhlovani resime jen prvni bit za desetinou carkou 
        LD      A, D                ;  1:4
        AND     2*MANT_MASK_HI+1    ;  2:7      clear carry
SRLDEC_DE_SWITCH EQU     $+1
        JR      SRLDEC_DE_CASE_0    ;  2:12
        JR      SRLDEC_DE_CASEm1    ;  2:12 
SRLDEC_DE_CASE_0:
        LD      D, A                ;  1:4
        RET                         ;  1:10
        JR      SRLDEC_DE_CASE_1    ;  1:12
        JR      SRLDEC_DE_CASE_2    ;  1:12
        JR      SRLDEC_DE_CASE_3    ;  1:12
        JR      SRLDEC_DE_CASE_4    ;  1:12
        JR      SRLDEC_DE_CASE_5    ;  1:12
        JR      SRLDEC_DE_CASE_6    ;  1:12
        JR      SRLDEC_DE_CASE_7    ;  1:12
        JR      SRLDEC_DE_CASE_8    ;  1:12
        JR      SRLDEC_DE_CASE_9    ;  1:12
SRLDEC_DE_CASE_10:                  ;           1.. .... .... =>  000 0000 0001
        LD      DE, $0001           ;  3:10
        AND     $04                 ;  2:7
        RET     nz                  ;  1:11/5   99%
        DEC     E                   ;  1:4
        RET                         ;  1:10

SRLDEC_DE_CASEm1:                   ;           1.. .... .... => 1... .... ...0 
        RL      E                   ;  2:8
        ADC     A, A                ;  1:4
        LD      D, A                ;  1:4
        RET                         ;  1:10
        
SRLDEC_DE_CASE_9:                   ;           1.. .... .... =>  000 0000 001. 
        RRA                         ;  1:4
SRLDEC_DE_CASE_8:                   ;           1.. .... .... =>  000 0000 01..
        LD      E, A                ;  1:4
        LD      D, $00              ;  2:7
        RET                         ;  1:10
                
SRLDEC_DE_CASE_5:                   ;           1.. .... .... =>  000 001. .... 
        RL      E                   ;  2:8
        ADC     A, A                ;  1:4
SRLDEC_DE_CASE_6:                   ;           1.. .... .... =>  000 0001 ....
        RL      E                   ;  2:8
        ADC     A, A                ;  1:4
SRLDEC_DE_CASE_7:                   ;           1.. .... .... =>  000 0000 1... 
        RL      E                   ;  2:8
        ADC     A, A                ;  1:4
        LD      E, A                ;  1:4
        LD      D, $00              ;  2:7
        RET                         ;  1:10
        
SRLDEC_DE_CASE_4:                   ;           1.. .... .... =>  000 01.. ....
        RRA                         ;  1:4
        RR      E                   ;  2:8        
SRLDEC_DE_CASE_3:                   ;           1.. .... .... =>  000 1... ....
        RRA                         ;  1:4
        RR      E                   ;  2:8        
        RRA                         ;  1:4
        RR      E                   ;  2:8
        RRA                         ;  1:4
        RR      E                   ;  2:8
        LD      D, $00              ;  2:7
        RET                         ;  1:10        
                
SRLDEC_DE_CASE_2:                   ;           1.. .... .... =>  001 .... ....
        RRA                         ;  1:4
        RR      E                   ;  2:8
        OR      A                   ;  1:4      clear carry
SRLDEC_DE_CASE_1:                   ;           1.. .... .... =>  01. .... ....
        RRA                         ;  1:4
        LD      D, A                ;  1:4
        RR      E                   ;  2:8
        RET                         ;  1:10        

endif
