if not defined @FPOW2

; e = 2*exp
; mantisa < 2**0.5
; (2**exp * mantisa)**2 = 2**e * mantisa**2
; mantisa > 2**0.5
; (2**exp * mantisa)**2 = 2**e * (mantisa**2)/2 * 2 = 2**(e+1) * (mantisa**2)/2

; Square of a floating-point number
; In: HL = number to square
; Out: HL = HL * HL
; Pollutes: AF
@FPOW2:
if not defined FPOW2
; *****************************************
                    FPOW2                ; *
; *****************************************
endif
        ; A = (EXP-BIAS + EXP-BIAS) + BIAS = EXP-BIAS + EXP

        ; 128 + 128 =  256! => $FF + $FF - $7F = $1FE - $7F = $17F!
        ;  65 +  65 =  130! => $C0 + $C0 - $7F = $180 - $7F = $101!
        ;  64 +  64 =  128  => $BF + $BF - $7F = $17E - $7F =  $FF
        ;   1 +   1 =    2  => $80 + $80 - $7F = $100 - $7F =  $81
        ;   0 +   0 =    0  => $7F + $7F - $7F =  $FE - $7F =  $7F
        ; -63 + -63 = -126  => $40 + $40 - $7F =  $80 - $7F =  $01
        ; -64 + -64 = -128! => $3F + $3F - $7F =  $7E - $7F = $FFF!
        ;-127 +-127 = -254! => $00 + $00 - $7F =  $00 - $7F = $F81!
        
        ; overflow             $180..$1FE - $7F =       $101..$17F   
        ; ok                    $80..$17E - $7F =        $01..$FF   (u $FF a vysokych mantis to jeste pretece na $00)
        ; underflow             $00.. $7E - $7F =       $F81..$FFF  (u $FF je sance ze u vysokych mantis se to jeste zachrani a preleze na $00)

        RES     7, L                ;  2:8      abs(HL)
        LD      A, PREDEL_POW2      ;  2:7
        CP      L                   ;  1:4
        LD      A, H                ;  1:4
        ADC     A, A                ;  1:4
        LD      H, POW2TAB/256      ;  2:7
        LD      L, (HL)             ;  1:7
        JR      c, FPOW2_HI         ;  2:12/7

        SUB     BIAS                ;  2:7
        LD      H, A                ;  1:4
        RET     nc                  ;  1:11/5

FPOW2_UNDERFLOW:
        LD      HL, FPMIN           ;  3:10
    if color_flow_warning
        CALL    UNDER_COL_WARNING   ;  3:17
    endif
        RET                         ;  1:10
                
FPOW2_HI:
        SUB     BIAS                ;  2:7
        LD      H, A                ;  1:4
    if carry_flow_warning
        CCF                         ;  1:4
        RET     nc                  ;  1:11/5
    else
        RET     c                   ;  1:11/5
    endif


FPOW2_OVERFLOW:
        LD      HL, FPMAX           ;  3:10
    if color_flow_warning
        CALL    OVER_COL_WARNING    ;  3:17
    endif
    if carry_flow_warning
        SCF                         ;  1:4
    endif
        RET                         ;  1:10


if defined FMUL
; Power 2 of a floating-point number
; In: HL = number to square
; Out: HL = HL * HL
; Pollutes: AF
FPOW2_USE_FMUL:
        PUSH    BC                  ;  1:11
        PUSH    DE                  ;  1:11
        LD      B, H                ;  1:4
        LD      C, L                ;  1:4        
        EX      DE, HL              ;  1:4
        call    FMUL                ;           HL = (DE*BC)
        POP     DE                  ;  1:10
        POP     BC                  ;  1:10
        RET
endif

endif
