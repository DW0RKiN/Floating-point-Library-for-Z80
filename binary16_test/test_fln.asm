; pasmo -I ../danagy -d test_fln.asm 24576.bin > test.asm ; grep "BREAKPOINT" test.asm
; randomize usr 57344

    INCLUDE "finit.asm"

    color_flow_warning  EQU     1
    carry_flow_warning  EQU     1
    DATA_ADR            EQU     $6000       ; 24576
    TEXT_ADR            EQU     $E000       ; 57344

    ORG     DATA_ADR

; red        
    dw $3b35, $aeae		; ln(+9.009e-01) = -1.044e-01
    dw $3b59, $ad72		; ln(+9.185e-01) = -8.506e-02
    dw $3b0e, $b006		; ln(+8.818e-01) = -1.257e-01
    dw $3bbd, $a842		; ln(+9.673e-01) = -3.326e-02
    dw $3bb8, $a895		; ln(+9.648e-01) = -3.579e-02
    dw $3b11, $aff1		; ln(+8.833e-01) = -1.241e-01
    dw $3b21, $af61		; ln(+8.911e-01) = -1.153e-01
    dw $3b9b, $aa79		; ln(+9.507e-01) = -5.057e-02
    dw $3bc9, $a6f8		; ln(+9.731e-01) = -2.722e-02
    dw $3be7, $a24a		; ln(+9.878e-01) = -1.228e-02
    dw $3b24, $af46		; ln(+8.926e-01) = -1.136e-01
    dw $3ba1, $aa14		; ln(+9.536e-01) = -4.750e-02
    dw $3bef, $a045		; ln(+9.917e-01) = -8.335e-03
    dw $3b8f, $ab44		; ln(+9.448e-01) = -5.676e-02
    dw $3be7, $a24a		; ln(+9.878e-01) = -1.228e-02
    dw $3bdf, $a429		; ln(+9.839e-01) = -1.624e-02
    dw $3b6d, $acc4		; ln(+9.282e-01) = -7.448e-02
    dw $3b43, $ae32		; ln(+9.077e-01) = -9.682e-02
    dw $3b87, $abcc		; ln(+9.409e-01) = -6.090e-02
    dw $3bf8, $9c02		; ln(+9.961e-01) = -3.914e-03
    dw $3b98, $aaac		; ln(+9.492e-01) = -5.212e-02
    dw $3be9, $a1c8		; ln(+9.888e-01) = -1.129e-02
    dw $3b62, $ad23		; ln(+9.229e-01) = -8.029e-02
    dw $3ba9, $a98e		; ln(+9.575e-01) = -4.341e-02
    dw $3b27, $af2b		; ln(+8.940e-01) = -1.120e-01
    dw $3b27, $af2b		; ln(+8.940e-01) = -1.120e-01
    dw $3bd6, $a54e		; ln(+9.795e-01) = -2.072e-02
; yellow
;     none
; azure
    dw $3ba8, $a99f		; ln(+9.570e-01) = -4.392e-02
    dw $3b0c, $b00f		; ln(+8.809e-01) = -1.269e-01
    dw $3b68, $acef		; ln(+9.258e-01) = -7.712e-02
    dw $3b34, $aeb7		; ln(+9.004e-01) = -1.049e-01
; green
    dw $3b82, $ac10		; ln(+9.385e-01) = -6.350e-02
    dw $3b75, $ac80		; ln(+9.321e-01) = -7.028e-02
    dw $3bff, $9000		; ln(+9.995e-01) = -4.884e-04
    dw $3b83, $ac08		; ln(+9.390e-01) = -6.298e-02
    dw $3b3c, $ae70		; ln(+9.043e-01) = -1.006e-01
    dw $3bc1, $a800		; ln(+9.692e-01) = -3.124e-02
    dw $3b22, $af58		; ln(+8.916e-01) = -1.147e-01


; red
    dw $3a5e, $b34e		; ln(+7.959e-01) = -2.283e-01
    dw $3a83, $b296		; ln(+8.140e-01) = -2.058e-01
    dw $3a70, $b2f4		; ln(+8.047e-01) = -2.173e-01
    dw $3aa4, $b1f6		; ln(+8.301e-01) = -1.862e-01
    dw $3af9, $b066		; ln(+8.716e-01) = -1.374e-01
    dw $3a81, $b2a0		; ln(+8.130e-01) = -2.070e-01
    dw $3aec, $b0a2		; ln(+8.652e-01) = -1.448e-01
    dw $3aa9, $b1de		; ln(+8.325e-01) = -1.833e-01
    dw $3aaa, $b1d9		; ln(+8.330e-01) = -1.827e-01
    dw $3a4f, $b39a		; ln(+7.886e-01) = -2.375e-01
    dw $3aa5, $b1f1		; ln(+8.306e-01) = -1.856e-01
    dw $3ae1, $b0d5		; ln(+8.599e-01) = -1.510e-01
    dw $3a79, $b2c8		; ln(+8.091e-01) = -2.119e-01
    dw $3a8c, $b26a		; ln(+8.184e-01) = -2.005e-01
; azure
    dw $3ac4, $b15d		; ln(+8.457e-01) = -1.676e-01
    dw $3a25, $b439		; ln(+7.681e-01) = -2.639e-01
    dw $3a67, $b321		; ln(+8.003e-01) = -2.228e-01
    dw $3a36, $b40d		; ln(+7.764e-01) = -2.531e-01
    dw $3a0b, $b47d		; ln(+7.554e-01) = -2.805e-01
    dw $3acc, $b137		; ln(+8.496e-01) = -1.630e-01
    dw $3a04, $b490		; ln(+7.520e-01) = -2.851e-01
    dw $3a21, $b443		; ln(+7.661e-01) = -2.664e-01
    dw $3a25, $b439		; ln(+7.681e-01) = -2.639e-01
    dw $3a03, $b492		; ln(+7.515e-01) = -2.857e-01
    dw $3ad8, $b0ff		; ln(+8.555e-01) = -1.561e-01
    dw $3a8d, $b265		; ln(+8.188e-01) = -1.999e-01
    dw $3ad8, $b0ff		; ln(+8.555e-01) = -1.561e-01
    dw $3a47, $b3c3		; ln(+7.847e-01) = -2.425e-01
    dw $3a26, $b436		; ln(+7.686e-01) = -2.632e-01
    dw $3a47, $b3c3		; ln(+7.847e-01) = -2.425e-01
    dw $3a38, $b408		; ln(+7.773e-01) = -2.519e-01
    dw $3a52, $b38b		; ln(+7.900e-01) = -2.357e-01
    dw $3a23, $b43e		; ln(+7.671e-01) = -2.652e-01
    dw $3a41, $b3e1		; ln(+7.817e-01) = -2.462e-01
    dw $3a82, $b29b		; ln(+8.135e-01) = -2.064e-01
    dw $3abc, $b183		; ln(+8.418e-01) = -1.722e-01
    dw $3a03, $b492		; ln(+7.515e-01) = -2.857e-01
    dw $3af1, $b08b		; ln(+8.677e-01) = -1.419e-01
    dw $3a41, $b3e1		; ln(+7.817e-01) = -2.462e-01
    dw $3a23, $b43e		; ln(+7.671e-01) = -2.652e-01
    dw $3a5b, $b35d		; ln(+7.944e-01) = -2.301e-01
    dw $3a38, $b408		; ln(+7.773e-01) = -2.519e-01
    dw $3a50, $b395		; ln(+7.891e-01) = -2.369e-01
    dw $3a32, $b417		; ln(+7.744e-01) = -2.556e-01
    dw $3ab7, $b19b		; ln(+8.394e-01) = -1.751e-01
    dw $3a34, $b412		; ln(+7.754e-01) = -2.544e-01
    dw $3a00, $b49a		; ln(+7.500e-01) = -2.877e-01
; yellow
;     none
; green
    dw $3aab, $b1d4		; ln(+8.335e-01) = -1.821e-01
    dw $3a33, $b415		; ln(+7.749e-01) = -2.550e-01
    dw $3a0f, $b473		; ln(+7.573e-01) = -2.780e-01
    dw $3aef, $b094		; ln(+8.667e-01) = -1.431e-01
    dw $3ac5, $b158		; ln(+8.462e-01) = -1.670e-01
    

; azure    
    dw $399c, $b5ae		; ln(+7.012e-01) = -3.550e-01
    dw $397a, $b610		; ln(+6.846e-01) = -3.790e-01
    dw $39ef, $b4c8		; ln(+7.417e-01) = -2.988e-01
    dw $3906, $b772		; ln(+6.279e-01) = -4.653e-01
    dw $390a, $b765		; ln(+6.299e-01) = -4.622e-01
    dw $3963, $b654		; ln(+6.733e-01) = -3.955e-01
    dw $3942, $b6b7		; ln(+6.572e-01) = -4.197e-01
    dw $39b5, $b567		; ln(+7.134e-01) = -3.377e-01
    dw $3980, $b5ff		; ln(+6.875e-01) = -3.747e-01
    dw $39c5, $b53b		; ln(+7.212e-01) = -3.269e-01
    dw $397b, $b60d		; ln(+6.851e-01) = -3.783e-01
    dw $39a8, $b58c		; ln(+7.070e-01) = -3.467e-01
    dw $39f0, $b4c5		; ln(+7.422e-01) = -2.982e-01
    dw $3935, $b6df		; ln(+6.509e-01) = -4.294e-01
    dw $3985, $b5f0		; ln(+6.899e-01) = -3.711e-01
    dw $39e5, $b4e3		; ln(+7.368e-01) = -3.054e-01
    dw $39b5, $b567		; ln(+7.134e-01) = -3.377e-01
    dw $3973, $b625		; ln(+6.812e-01) = -3.840e-01
    dw $39b0, $b575		; ln(+7.109e-01) = -3.412e-01
    dw $3940, $b6bd		; ln(+6.562e-01) = -4.212e-01
    dw $398b, $b5df		; ln(+6.929e-01) = -3.669e-01
    dw $3934, $b6e2		; ln(+6.504e-01) = -4.302e-01
    dw $3973, $b625		; ln(+6.812e-01) = -3.840e-01
    dw $39df, $b4f3		; ln(+7.339e-01) = -3.094e-01
    dw $3980, $b5ff		; ln(+6.875e-01) = -3.747e-01
    dw $3986, $b5ed		; ln(+6.904e-01) = -3.704e-01
    dw $39a9, $b589		; ln(+7.075e-01) = -3.460e-01
    dw $3989, $b5e5		; ln(+6.919e-01) = -3.683e-01
    dw $39b9, $b55c		; ln(+7.153e-01) = -3.350e-01
    dw $3961, $b65a		; ln(+6.724e-01) = -3.970e-01
    dw $39d6, $b50c		; ln(+7.295e-01) = -3.154e-01
    dw $390f, $b755		; ln(+6.323e-01) = -4.584e-01
    dw $3963, $b654		; ln(+6.733e-01) = -3.955e-01
    dw $39f8, $b4b0		; ln(+7.461e-01) = -2.929e-01
    dw $398c, $b5dc		; ln(+6.934e-01) = -3.662e-01
    dw $39df, $b4f3		; ln(+7.339e-01) = -3.094e-01
    dw $396c, $b639		; ln(+6.777e-01) = -3.890e-01
    dw $391e, $b726		; ln(+6.396e-01) = -4.468e-01
    dw $39a6, $b592		; ln(+7.061e-01) = -3.481e-01
    dw $3946, $b6ab		; ln(+6.592e-01) = -4.168e-01
    dw $39d3, $b514		; ln(+7.280e-01) = -3.174e-01
    dw $3948, $b6a5		; ln(+6.602e-01) = -4.153e-01
    dw $3947, $b6a8		; ln(+6.597e-01) = -4.160e-01
    dw $39f4, $b4ba		; ln(+7.441e-01) = -2.955e-01
    dw $39f4, $b4ba		; ln(+7.441e-01) = -2.955e-01
    dw $3947, $b6a8		; ln(+6.597e-01) = -4.160e-01
    dw $39f5, $b4b8		; ln(+7.446e-01) = -2.949e-01
    dw $3927, $b70a		; ln(+6.440e-01) = -4.400e-01
    dw $3955, $b67e		; ln(+6.665e-01) = -4.057e-01
    dw $39d5, $b50f		; ln(+7.290e-01) = -3.161e-01
    dw $3981, $b5fc		; ln(+6.880e-01) = -3.740e-01
    dw $3989, $b5e5		; ln(+6.919e-01) = -3.683e-01
    dw $39ff, $b49d		; ln(+7.495e-01) = -2.883e-01
; green
    dw $390e, $b759		; ln(+6.318e-01) = -4.591e-01
    dw $39ee, $b4cb		; ln(+7.412e-01) = -2.995e-01
    dw $3924, $b714		; ln(+6.426e-01) = -4.423e-01
; yellow
;   none
    
; azure
    dw $38e5, $b7dc		; ln(+6.118e-01) = -4.913e-01
    dw $3836, $b922		; ln(+5.264e-01) = -6.418e-01
    dw $3899, $b86e		; ln(+5.747e-01) = -5.539e-01
    dw $381b, $b956		; ln(+5.132e-01) = -6.671e-01
    dw $385c, $b8db		; ln(+5.449e-01) = -6.071e-01
    dw $38c2, $b828		; ln(+5.947e-01) = -5.197e-01
    dw $38e6, $b7d9		; ln(+6.123e-01) = -4.905e-01
    dw $38bf, $b82d		; ln(+5.933e-01) = -5.221e-01
    dw $382c, $b935		; ln(+5.215e-01) = -6.511e-01
    dw $38e6, $b7d9		; ln(+6.123e-01) = -4.905e-01
    dw $38bf, $b82d		; ln(+5.933e-01) = -5.221e-01
    dw $382c, $b935		; ln(+5.215e-01) = -6.511e-01
    dw $381b, $b956		; ln(+5.132e-01) = -6.671e-01
    dw $381b, $b956		; ln(+5.132e-01) = -6.671e-01
    dw $383f, $b911		; ln(+5.308e-01) = -6.334e-01
    dw $3837, $b920		; ln(+5.269e-01) = -6.408e-01
    dw $384e, $b8f5		; ln(+5.381e-01) = -6.197e-01
    dw $3862, $b8d0		; ln(+5.479e-01) = -6.018e-01
    dw $389d, $b867		; ln(+5.767e-01) = -5.505e-01
    dw $38f5, $b7a8		; ln(+6.196e-01) = -4.786e-01
    dw $381f, $b94e		; ln(+5.151e-01) = -6.633e-01
    dw $38a8, $b854		; ln(+5.820e-01) = -5.412e-01
; green
    dw $38b0, $b847		; ln(+5.859e-01) = -5.345e-01
    dw $38b1, $b845		; ln(+5.864e-01) = -5.337e-01
    dw $384b, $b8fb		; ln(+5.366e-01) = -6.225e-01
    dw $383d, $b915		; ln(+5.298e-01) = -6.353e-01
    dw $38a6, $b858		; ln(+5.811e-01) = -5.429e-01
    dw $3886, $b890		; ln(+5.654e-01) = -5.702e-01
    dw $3864, $b8cd		; ln(+5.488e-01) = -6.000e-01
    dw $38ec, $b7c6		; ln(+6.152e-01) = -4.858e-01
    dw $38f4, $b7ac		; ln(+6.191e-01) = -4.794e-01
    dw $3816, $b960		; ln(+5.107e-01) = -6.719e-01
    dw $3882, $b897		; ln(+5.635e-01) = -5.736e-01
    dw $380a, $b978		; ln(+5.049e-01) = -6.834e-01
    dw $3832, $b92a		; ln(+5.244e-01) = -6.455e-01
    dw $380a, $b978		; ln(+5.049e-01) = -6.834e-01
    dw $38d0, $b811		; ln(+6.016e-01) = -5.082e-01
    dw $38d0, $b811		; ln(+6.016e-01) = -5.082e-01
    dw $3820, $b94d		; ln(+5.156e-01) = -6.624e-01
    dw $38c7, $b820		; ln(+5.972e-01) = -5.156e-01
    dw $380a, $b978		; ln(+5.049e-01) = -6.834e-01
    dw $385d, $b8da		; ln(+5.454e-01) = -6.062e-01
    dw $3853, $b8ec		; ln(+5.405e-01) = -6.152e-01
    dw $380b, $b976		; ln(+5.054e-01) = -6.825e-01    
    dw $38be, $b82f		; ln(+5.928e-01) = -5.229e-01
    dw $3845, $b906		; ln(+5.337e-01) = -6.279e-01
    dw $3805, $b982		; ln(+5.024e-01) = -6.883e-01
    dw $38b7, $b83b		; ln(+5.894e-01) = -5.287e-01
    dw $386f, $b8b9		; ln(+5.542e-01) = -5.902e-01
    dw $383d, $b915		; ln(+5.298e-01) = -6.353e-01
    dw $382e, $b932		; ln(+5.225e-01) = -6.492e-01
    dw $3854, $b8ea		; ln(+5.410e-01) = -6.143e-01
    dw $38d9, $b802		; ln(+6.060e-01) = -5.009e-01
    dw $38e0, $b7ed		; ln(+6.094e-01) = -4.953e-01
; yellow
;   none

    INCLUDE "test_fln.dat"

    dw $BABE, $DEAD                 ; Stop MARK

; Subroutines
    INCLUDE "fln.asm"
    INCLUDE "fequals.asm"
    INCLUDE "print_txt.asm"
    INCLUDE "print_hex.asm"

; Lookup tables
    INCLUDE "fln.tab"
    
    if ( $ > TEXT_ADR ) 
        .ERROR "Prilis dlouha data!"        
    endif
        
    ORG     TEXT_ADR

        LD      HL, DATA_ADR
        PUSH    HL
READ_DATA:
        POP     HL
        LD      BC, $0004
        LD      DE, OP1
        LDIR
        PUSH    HL

; HL = HL + DE
; HL = HL - DE

; HL = BC * DE

; HL = BC % HL
; HL = BC / HL

; HL = HL * HL
; HL = HL % 1
; HL = HL^0.5
        LD      HL, (OP1)
BREAKPOINT:
; FUSE Debugger
; br 0xE011

        PUSH    HL        
        CALL    FLN                 ; HL = ln(HL)
        
        POP     BC
;     kontrola
        LD      BC, (RESULT)
        
IGNORE  EQU $+1
        LD      A, $00
        OR      A
        LD      A, $00
        LD      (IGNORE), A
        JR      nz, READ_DATA

        CALL    FEQUALS
if 0
        JR      nc, READ_DATA
        JR      z, PRINT_OK
else
        JR      z, PRINT_OK
endif
        CALL    PRINT_DATA

if 1
        OR      A
        LD      HL, $DEAD
        SBC     HL, BC
        JR      nz, READ_DATA
endif
        POP     HL
        RET                         ; exit

PRINT_OK:
        CALL    PRINT_DATA
        JR      READ_DATA
        
        
OP1:
        defs    2
RESULT:
        defs    2


; BC = kontrolni
; HL = spocitana
PRINT_DATA:
        LD      (DATA_COL), A
        
        LD      DE, DATA_4
        CALL    WRITE_HEX
        
        LD      HL, (OP1)
        LD      DE, DATA_1
        CALL    WRITE_HEX

        LD      H, B
        LD      L, C
        LD      DE, DATA_3
        CALL    WRITE_HEX

        CALL    PRINT_TXT
        
        defb    INK, COL_WHITE, 'ln($'
DATA_1:
        defs     4
        defb    ') = $'
DATA_3:
        defs     4
        defb    ' ? ', INK
DATA_COL:
        defb    COL_WHITE, '$'
DATA_4:        
        defs     4
        defb    NEW_LINE      
        defb    STOP_MARK
