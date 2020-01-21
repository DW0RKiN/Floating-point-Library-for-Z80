if color_flow_warning && not defined UNDER_COL_WARNING

BORDER   EQU     8859               ; ROM border change permanent

CBLU     EQU     1
CMNG     EQU     3
CRED     EQU     2
CYEL     EQU     6


UNDER_COL_WARNING:
        LD      A, CBLU + CMNG      ;  2:7      A = 4
FMUL_SELF_U:
        SUB     CBLU                ;  2:7      A = 3 or 1 
        LD      ($-1), A            ;  3:13
        CALL    BORDER              ;  3:17     change border colour
        RET                         ;  1:10

        
OVER_COL_WARNING:
        LD      A, CRED + CYEL      ;  2:7      A = 8
FMUL_SELF_O:
        SUB     CRED                ;  2:7      A = 6 or 2 
        LD      ($-1), A            ;  3:13
        CALL    BORDER              ;  3:17     change border colour
        RET                         ;  1:10

endif
