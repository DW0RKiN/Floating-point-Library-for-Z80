if not defined PRINT_DX

; --------- Promenne ---------

PRINT_DX:
        LD      A, COL_GREEN            ;  2:7
PRINT_DX_COL:
        PUSH    HL                      ;  1:11
        LD      HL, 'D'+'X' * 256       ;  3:10
        JR      PRINT_XFP               ;  2:12


PRINT_DY:
        LD      A, COL_GREEN            ;  2:7
PRINT_DY_COL:
        PUSH    HL                      ;  1:11
        LD      HL, 'D'+'Y' * 256       ;  3:10
        JR      PRINT_XFP               ;  2:12


PRINT_DZ:
        LD      A, COL_GREEN            ;  2:7        
PRINT_DZ_COL:
        PUSH    HL                      ;  1:11
        LD      HL, 'D'+'Z' * 256       ;  3:10
        JR      PRINT_XFP               ;  2:12


PRINT_GX:
        LD      A, COL_PURPLE           ;  2:7
PRINT_GX_COL:
        PUSH    HL                      ;  1:11
        LD      HL, 'G'+'X' * 256       ;  3:10
        JR      PRINT_XFP               ;  2:12

        
PRINT_GY:
        LD      A, COL_PURPLE           ;  2:7
PRINT_GY_COL:
        PUSH    HL                      ;  1:11
        LD      HL, 'G'+'Y' * 256       ;  3:10
        JR      PRINT_XFP               ;  2:12

        
PRINT_GZ:
        LD      A, COL_PURPLE           ;  2:7
PRINT_GZ_COL:
        PUSH    HL                      ;  1:11
        LD      HL, 'G'+'Z' * 256       ;  3:10
        JR      PRINT_XFP               ;  2:12

        
PRINT_DD:
        LD      A, COL_YELLOW           ;  2:7
PRINT_DD_COL:
        PUSH    HL                      ;  1:11
        LD      HL, 'D'+'D' * 256       ;  3:10
        JR      PRINT_XFP               ;  2:12


PRINT_DL:
        LD      A, COL_YELLOW           ;  2:7
PRINT_DL_COL:
        PUSH    HL                      ;  1:11
        LD      HL, 'D'+'L' * 256       ;  3:10
        JR      PRINT_XFP               ;  2:12
        

; -----------  Jmena FCI -------------

PRINT_GROUNDI:
        CALL    PRINT_TXT
        defb    INK, COL_WHITE, 'Groundi', NEW_LINE

        
PRINT_GROUNDL:
        CALL    PRINT_TXT
        defb    INK, COL_WHITE, 'Groundl', NEW_LINE


PRINT_SCENEL:
        CALL    PRINT_TXT
        defb    INK, COL_WHITE, 'Scenel', NEW_LINE


PRINT_SCENEI:
        CALL    PRINT_TXT
        defb    INK, COL_WHITE, 'Scenei', NEW_LINE


PRINT_HITRAY:
        CALL    PRINT_TXT
        defb    INK, COL_WHITE, 'Hitray', NEW_LINE


PRINT_SCALED:
        CALL    PRINT_TXT
        defb    INK, COL_WHITE, 'Scaled', NEW_LINE
        

PRINT_SPHEREL:
        CALL    PRINT_TXT
        defb    INK, COL_WHITE, 'Spherel', NEW_LINE


PRINT_SPHEREI:
        CALL    PRINT_TXT
        defb    INK, COL_WHITE, 'Spherei', NEW_LINE

        
defb    STOP_MARK

endif
