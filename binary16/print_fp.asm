if not defined PRINT_XFP

COL_BLACK       EQU     0
COL_BLUE        EQU     1
COL_RED         EQU     2
COL_PURPLE      EQU     3
COL_GREEN       EQU     4
COL_AZURE       EQU     5
COL_YELLOW      EQU     6
COL_WHITE       EQU     7

INK             EQU     $10
BRIGHT          EQU     $13
NEWLINE         EQU     $0D
COL_ADR         EQU     $5C8D

if defined _print_txt

PRINT_STOP_MARK EQU $CD ; CALL
PRINT_NEW_LINE  EQU $0D ; 13

; In: A = COLOR
PRINT_SET_COLOR:
        PUSH    HL                  ;  1:11
        LD      HL, COL_ADR         ;  3:10
        XOR     (HL)                ;  1:7
        AND     $07                 ;  2:7
        XOR     (HL)                ;  1:7
        LD      (HL), A             ;  1:7
        POP     HL                  ;  1:10
        RET                         ;  1:10


; In: HL = 'ba' => print "ab:"
PRINT_XX:
        LD      (PRINT_XX_DATA), HL ;  3:16
        CALL    PRINT_TXT           ;  3:17
PRINT_XX_DATA:
        defb    'FP:'

        
PRINT_TXT_SPACE_DOL:
        CALL    PRINT_TXT
        defb    ' $'
        

PRINT_TXT_CONTINUE:
        CALL    PRINT_TXT
        defb    INK, COL_WHITE, 'Continue '
        
        
PRINT_TXT_EXIT:
        CALL    PRINT_TXT
        defb    INK, COL_WHITE, 'Exit '


PRINT_TXT_FDOT:
        CALL    PRINT_TXT
        defb    INK, COL_WHITE, 'FDot', NEWLINE
        
defb    PRINT_STOP_MARK


; Input:
; Stack = Adress string = return adress
; CALL PRINT_TXT
; defb 'Text...'
PRINT_TXT:
        EX      (SP), HL            ;  1:19 Address of string
        PUSH    DE                  ;  1:11
        PUSH    BC                  ;  1:11
        PUSH    AF                  ;  1:11
        
        PUSH    HL                  ;  1:11
        LD      L, $1A              ;  2:7  Upper screen
        CALL    $1605               ;  3:17 Open channel
        POP     HL                  ;  1:10 Address of string
        
        LD      D, H                ;  1:4
        LD      E, L                ;  1:4
        LD      A, PRINT_STOP_MARK  ;  2:7
PRINT_LENGTH:
        INC     HL                  ;  1:6
        CP      (HL)                ;  1:7
        JR      nz, PRINT_LENGTH    ;  2:12/7
        
        SBC     HL, DE              ;  2:15
        LD      B, H                ;  1:4
        LD      C, L                ;  1:4 BC = Lenght of string to print
        CALL    $203E               ;  3:17 Print our string

        POP     AF                  ;  1:10
        POP     BC                  ;  1:10
        POP     DE                  ;  1:10
        POP     HL                  ;  1:10        
;         HALT
;         HALT
        RET                         ;  1:10

endif




if defined _print_hex


; In: A
; Out: ((A & $F0) >> 4 ) => '0'..'9','A'..'F'
; HL++
PRINT_HEX_HI:        
        RRA
        RRA
        RRA
        RRA
        ; fall

; In: A = number, DE = adr
; Out: (A & $0F) => '0'..'9','A'..'F'
; DE++
PRINT_HEX_LO: 
; '0'..'9' = $30..$39
; 'A'..'F' = $41..$46
; 'a'..'f' = $61..$66
        OR      $F0                 ;  2:7 reset H flag
        DAA                         ;  1:4 $F0..$F9 + $60 => $50..$59; $FA..$FF + $66 => $60..$65
        ADD     A, $A0              ;  2:7 $F0..$F9, $100..$105
        ADC     A, $40              ;  2:7 $30..$39, $41..$46   = '0'..'9', 'A'..'F'
        LD      (DE), A             ;  1:7
        INC     DE                  ;  1:6
        RET


; In: HL
; Out: Print Hex HL
PRINT_HEX:
        PUSH    AF                  ;  1:11
        PUSH    BC                  ;  1:11
        PUSH    DE                  ;  1:11
        PUSH    HL                  ;  1:11

        LD      DE, PRINT_HEX_STR   ;  3:10
        LD      A, H                ;  1:4
        CALL    PRINT_HEX_HI        ;  3:17

        LD      A, H                ;  1:4
        CALL    PRINT_HEX_LO        ;  3:17

        LD      A, L                ;  1:4
        CALL    PRINT_HEX_HI        ;  3:17
        
        LD      A, L                ;  1:4
        CALL    PRINT_HEX_LO        ;  3:17

        
        LD      L, $1A              ;  2:7  Upper screen
        CALL    $1605               ;  3:17 Open channel
        LD      DE, PRINT_HEX_STR   ;  3:10 Address of string
        LD      BC, $03             ;  3:10 Length-1 of string to print
        CALL    $2040               ;  3:17 Print our string

        POP     HL                  ;  1:10
        POP     DE                  ;  1:10
        POP     BC                  ;  1:10
        POP     AF                  ;  1:10
;         HALT
;         HALT
        RET                         ;  1:10
        
PRINT_HEX_STR:
        defb    'DEAD'      
 

 
PRINT_HEX_STACK:
        LD      (PRINT_HEX_STACK_RESTORE), HL   ;  3:16     Save
        POP     HL                  ;  1:10     Ret
        EX      (SP), HL            ;  1:19     Hex number
        CALL    PRINT_HEX           ;  3:17
PRINT_HEX_STACK_RESTORE    EQU  $+1
        LD      HL, $0000           ;  3:10     Load
        RET                         ;  1:10

 
 
PRINT_HEX_DE:
        EX      DE, HL              ;  1:4
        CALL    PRINT_HEX           ;  3:17
        EX      DE, HL              ;  1:4
        RET                         ;  1:10

 
 
PRINT_HEX_BC:
        PUSH    HL                  ;  1:11
        LD      H, B                ;  1:4
        LD      L, C                ;  1:4
        CALL    PRINT_HEX           ;  3:17
        POP     HL                  ;  1:10
        RET                         ;  1:10


endif



if defined _print_bin



;  input: HL
; output: "+(2^+exp)*1.mant issa"
PRINT_BIN:
        PUSH    AF                  ;  1:11
        PUSH    BC                  ;  1:11
        PUSH    DE                  ;  1:11
        PUSH    HL                  ;  1:11
                
        ADD     HL, HL              ;  1:11     Sign        
        LD      A, $0B              ;  2:7      11
        ADC     A, A                ;  1:4      22 ($16), 23 ($17)
        ADD     A, A                ;  1:4      44 ($2C), 46 ($2E)
        DEC     A                   ;  1:4      43 ($2B = '+'), 45 ($2D = '-')
        LD      (PRINT_BIN_STR), A  ;  3:13

        XOR     A                   ;  1:4
        LD      B, EXP_BITS         ;  2:7
PRINT_BIN_READ_EXP:
        ADD     HL, HL              ;  1:11
        ADC     A, A                ;  1:4
        DJNZ    PRINT_BIN_READ_EXP  ;  2:13/8
        
        EX      DE, HL              ;  1:4      Save HL
        LD      HL, PRINT_BIN_EXP   ;  3:10
        LD      (HL), '+'           ;  2:10     $2B
        SUB     BIAS / $04          ;  2:7
        JR      nc, PRINT_BIN_PEXP  ;  2:12/7
        LD      (HL), '-'           ;  2:10     $2D
        NEG                         ;  2:8
PRINT_BIN_PEXP:
        INC     HL                  ;  1:6

        LD      B, $0A              ;  2:7
        CALL    A_DIV_B             ;  3:17
        ADD     A, '0'              ;  2:7
        LD      (HL), A             ;  1:7
        
        EX      DE, HL              ;  1:4
        
        LD      DE, PRINT_BIN_MAN_0 ;  3:10
        LD      B, $04              ;  2:7
        CALL    PRINT_BIN_MANTISSA  ;  3:17

        LD      DE, PRINT_BIN_MAN_4 ;  3:10
        LD      B, $04              ;  2:7
        CALL    PRINT_BIN_MANTISSA  ;  3:17

        LD      DE, PRINT_BIN_MAN_8 ;  3:10
        LD      B, MANT_BITS-$08    ;  2:7
        CALL    PRINT_BIN_MANTISSA  ;  3:17

        LD      L, $1A              ;  2:7  Upper screen
        CALL    $1605               ;  3:17 Open channel
        LD      DE, PRINT_BIN_STR   ;  3:10 Address of string
        LD      BC, PRINT_BIN_LEN-1 ;  3:10 Length-1 of string to print
        CALL    $2040               ;  3:17 Print our string
        
        POP     HL                  ;  1:10
        POP     DE                  ;  1:10
        POP     BC                  ;  1:10
        POP     AF                  ;  1:10
;         HALT
;         HALT
        RET                         ;  1:10



; In: DE Address, B number bits, HL source
; Out: DE += B, B = 0
PRINT_BIN_MANTISSA:
        XOR     A                   ;  1:4
        ADD     HL, HL              ;  1:11
        ADC     A, '0'              ;  2:7
        LD      (DE), A             ;  3:10
        INC     DE                  ;  1:6
        DJNZ    PRINT_BIN_MANTISSA  ;  2:13/8
        RET                         ;  1:10


; In: A, B, HL = Adress
; Out: C = '0' + A / B, A = A mod B, HL++
A_DIV_B:
        LD      C, '0'-1            ;  2:7
A_DIV_B_LOOP:
        INC     C                   ;  1:4
        SUB     B                   ;  1:4
        JR      nc, A_DIV_B_LOOP    ;  2:12/7
        LD      (HL), C             ;  1:7
        INC     HL                  ;  1:6
        ADD     A, B                ;  1:4
        RET                         ;  1:10
        
        
PRINT_BIN_STR: 
    DEFB        '+(2', $5E
PRINT_BIN_EXP:
    DEFB        '+00)*1.'
PRINT_BIN_MAN_0:
    DEFS        4
    DEFB        BRIGHT, 1, ' '
PRINT_BIN_MAN_4:
    DEFS        4
    DEFB        BRIGHT, 0, ' '
PRINT_BIN_MAN_8:    
    DEFS        MANT_BITS-8

PRINT_BIN_LEN   EQU $-PRINT_BIN_STR


endif


if (defined _print_bin) && (defined _print_txt) && (defined _print_hex)


; In: FP in stack
PRINT_FP_STACK:
        LD      A, COL_AZURE
PRINT_FP_STACK_COL:
        CALL    PRINT_SET_COLOR
        LD      (PRINT_FP_STACK_HL), HL ;  3:16 
        POP     HL                  ;  1:10 HL = ret
        EX      (SP), HL            ;  1:19
        CALL    PRINT_FP
PRINT_FP_STACK_HL  EQU $+1
        LD      HL, $0000
        RET

        
        
        ;  0123456789ABCDEF0123456789ABCDEF
        ; "-(2^+05)*1.1111 0000 00 $D3C0"
PRINT_FP:
        CALL    PRINT_BIN           ;  3:17
        CALL    PRINT_TXT_SPACE_DOL ;  3:17
        CALL    PRINT_HEX           ;  3:17
        RET                         ;  1:10


; ------------- Registry ---------------
                
PRINT_DE:
        LD      A, COL_AZURE        ;  2:7
PRINT_DE_COL:
        CALL    PRINT_SET_COLOR     ;  3:17
        PUSH    HL                  ;  1:11
        LD      HL, 'D'+'E' * 256   ;  3:10     "DE"
        CALL    PRINT_XX            ;  3:17
        LD      H, D                ;  1:4
        LD      L, E                ;  1:4
        CALL    PRINT_FP            ;  3:17
        POP     HL                  ;  1:10
        RET                         ;  1:10
        
        
PRINT_BC:
        LD      A, COL_AZURE        ;  2:7
PRINT_BC_COL:
        CALL    PRINT_SET_COLOR     ;  3:17
        PUSH    HL                  ;  1:11
        LD      HL, 'B'+'C' * 256   ;  3:10     "BC"
        CALL    PRINT_XX            ;  3:17
        LD      H, B                ;  1:4
        LD      L, C                ;  1:4
        CALL    PRINT_FP            ;  3:17
        POP     HL                  ;  1:10
        RET                         ;  1:10


PRINT_HL:
        LD      A, COL_AZURE        ;  2:7
PRINT_HL_COL:
        PUSH    HL                  ;  1:11
        LD      HL, 'H'+'L' * 256   ;  3:10     "HL"
        ; Fall..
        
        
; Nevolat pomoci CALL! Only JP/JR PRINT_XFP
; In: original HL on stack, HL = 'ba' => print "ab:"
PRINT_XFP:
        CALL    PRINT_SET_COLOR     ;  3:17
PRINT_XFP_NO_COL:
        CALL    PRINT_XX            ;  3:17
        POP     HL                  ;  1:10
        JP      PRINT_FP            ;  3:10

endif


endif
