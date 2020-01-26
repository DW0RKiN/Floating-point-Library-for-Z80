if not defined FEQUALS

    INCLUDE "print_init.asm"

;  Comparing of two floating point numbers with minimum tolerance. Does not solve overflow and underflow HL
;    Input: HL, BC
;   Output: if      ( HL            == BC ) {   set zero; reset carry; A = COL_GREEN;  }
;           else if ( HL + rounding == BC ) {   set zero;   set carry; A = COL_YELLOW; }
;           else if ( HL - rounding == BC ) {   set zero;   set carry; A = COL_AZURE;  }
;           else                            { reset zero;   set carry; A = COL_RED;    }
; Pollutes: AF
FEQUALS:                            ; HL == BC?
        LD      A, L
        SUB     C
        JR      nz, FEQUALS_PLUS
        LD      A, H
        SUB     B
        LD      A, COL_GREEN
        RET     z

; HL = eeee eeee smmm mmmm
FEQUALS_PLUS:                       ;           HL + rounding == BC?
        LD      A, L                ;  1:4       
        ADD     A, A                ;  1:4      sign out
        ADD     A, $02              ;  2:7
        LD      A, B                ;  1:4
        SBC     A, H                ;  1:4
        JR      nz, FEQUALS_MINUS   ;  2:12/7
        
        LD      A, L                ;  1:4
        ADD     A, A                ;  1:4      sign out
        INC     A                   ;  1:4
        INC     A                   ;  1:4
        RRA                         ;  1:4      sign in
        SUB     C                   ;  1:4
        LD      A, COL_YELLOW       ;  2:7
        SCF                         ;  1:4
        RET     z                   ;  1:11/5

FEQUALS_MINUS:                      ;           HL - rounding == BC?
        LD      A, L                ;  1:4
        ADD     A, A                ;  1:4      sign out
        SUB     $02                 ;  2:7
        LD      A, H                ;  1:4
        SBC     A, B                ;  1:4
        JR      nz, FEQUALS_FAIL    ;  2:12/7
        
        LD      A, L                ;  1:4
        ADD     A, A                ;  1:4      sign in
        DEC     A                   ;  1:4
        RRA                         ;  1:4      sign out
        SUB     C                   ;  1:4
        LD      A, COL_AZURE        ;  2:7
        SCF                         ;  1:4
        RET     z                   ;  1:11/5

FEQUALS_FAIL:
        LD      A, COL_RED          ;  2:7
        SCF                         ;  1:4
        RET                         ;  1:10
 
 endif
 
