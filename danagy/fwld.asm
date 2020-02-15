if not defined @FWLD

; Load Word. Convert unsigned 16-bit integer into floating-point number
;  In: HL = Word to convert
; Out: HL = floating point representation
; Pollutes: AF
@FWLD:
if not defined FWLD
; *****************************************
                    FWLD                ; *
; *****************************************
endif
        LD      A, H                ;  1:4
        OR      A                   ;  1:4
        JR      z, FWLD_BYTE        ;  2:12/7
        LD      A, BIAS+16          ;  2:7          HL = xxxx xxxx xxxx xxxx
FWLD_NORM:
        ADD     HL, HL              ;  1:11
        DEC     A                   ;  1:4
        JP      nc, FWLD_NORM       ;  3:10       
        
        SLA     L                   ;  2:8          rounding
        LD      L, H                ;  1:4
        LD      H, A                ;  1:4
        RET     nc                  ;  1:11/5
        CCF                         ;  1:4
        RET     z                   ;  1:11/5
        INC     L                   ;  1:4          rounding up
        RET     nz                  ;  1:11/5
        INC     H                   ;  1:4          exp++
        RET                         ;  1:10
        

FWLD_BYTE:                          ;               HL = 0000 0000 xxxx xxxx
        OR      L                   ;  1:4
        RET     z                   ;  1:5/11
                
        LD      H, BIAS+8           ;  2:7
FWLD_BYTE_NORM:
        DEC     H                   ;  1:4
        ADD     A, A                ;  1:4
        JR      nc, FWLD_BYTE_NORM  ;  2:12/7

        LD      L, A                ;  1:4
        RET                         ;  1:10
        
endif
