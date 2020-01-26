if not defined @FBLD

; Load Byte. Convert unsigned 8-bit integer into floating-point number
;  In:  A = Byte to convert
; Out: DE = floating point representation, zero flag if DE = 0
; Pollutes: AF
@FBLD:
if not defined FBLD
; *****************************************
                    FBLD                ; *
; *****************************************
endif
if 1
        LD      D, BIAS+7           ;  2:7
FBLD_X:
        ADD     A, A                ;  1:4
        JR      c, FBLD_ALIGNED     ;  2:11/7
else
        LD      D, BIAS+8           ;  2:7
FBLD_X:
        OR      A, A                ;  1:4
endif
        JR      z, FBLD_OUT_ZERO    ;  2:11/7       zero converted to positive epsilon
FBLD_LOOP:
        DEC     D                   ;  1:4
        ADD     A, A                ;  1:4
        JR      nc, FBLD_LOOP       ;  2:11/7
FBLD_ALIGNED:
        RRCA                        ;  1:4
        LD      E, A                ;  1:4
        RET                         ;  1:10
FBLD_OUT_ZERO:
        LD      D, A                ;  1:4
        LD      E, A                ;  1:4
        RET                         ;  1:10

endif
