# Floating-point-Library-for-Z80
Floating-Point Arithmetic Library for Z80


bfloat
======
https://en.wikipedia.org/wiki/Bfloat16_floating-point_format

![Ray tracing using bfloat](https://github.com/DW0RKiN/Floating-point-Library-for-Z80/blob/master/bfloat.png)

    FEDC BA98 7654 3210                  
    eeee eeee Smmm mmmm

    S (bit[7]):     Sign bit
    e (bits[F:8]):  Biased exponent
    m (bits[6:0]):  Fraction

    BIAS = $7F = 127
    MIN = 2^-127 * 1 = 5.8774717541114375×10⁻³⁹
    MAX = 2^+128 * 1.9921875 = 6.779062778503071×10³⁸

floating-point number = (-1)^S * 2^(eeee eeee - BIAS) * ( 128 + mmm mmmm) / 128

Sign position is moved from the original.

danagy 
======
https://github.com/nagydani/lpfp

![Ray tracing using danagy](https://github.com/DW0RKiN/Floating-point-Library-for-Z80/blob/master/danagy.png)

    FEDC BA98 7654 3210                  
    Seee eeee mmmm mmmm

    S (bit[F]):     Sign bit
    e (bits[E:8]):  Biased exponent
    m (bits[7:0]):  Fraction

    BIAS = $40 = 64
    MIN = 2^-64 * 1 = 5.421010862427522170037264×10⁻²⁰
    MAX = 2^+63 * 1.99609375 = 1.8410715276690587648×10¹⁹

floating-point number = (-1)^S * 2^(eee eeee - BIAS) * ( 256 + mmmm mmmm) / 256


binary16 (half)
========
https://en.wikipedia.org/wiki/Half-precision_floating-point_format

![Ray tracing using binary16](https://github.com/DW0RKiN/Floating-point-Library-for-Z80/blob/master/binary16.png)

    FEDC BA98 7654 3210                  
    Seee eemm mmmm mmmm

    S (bit[F]):     Sign bit
    e (bits[E:A]):  Biased exponent
    m (bits[9:0]):  Fraction

    BIAS = $F = 15
    MIN = 2^-15 * 1 = 0.000030517578125
    MAX = 2^+16 * 1.9990234375 = 131008

floating-point number = (-1)^S * 2^(ee eee - BIAS) * ( 1024 + mm mmmm mmmm) / 1024 

applies to everything
=====================

Without support for infinity, zero, and NaN.

    if ( a + b > MAX ) { res = MAX; set carry }
    if ( a - b < MIN ) { res = MIN; set carry }

Rounding of lost bits is (no matter what the sign):

    mmmm 0000 .. mmmm 1000 => mmmm + 0
    mmmm 1001 .. mmmm 1111 => mmmm + 1
    
finit.asm should be included as the first file.

If a math operation needs to include another operation, it will do it itself.
Data files ( *.tab ) must be included manually. 
They must be aligned to the address divisible by 256.

The fdiv operation has lower precision, where the lowest bit of the mantissa may not be valid.

    call  fadd          ; HL = HL + DE
    call  faddp         ; HL = HL + DE, HL and DE have the same signs
    call  fsub          ; HL = HL - DE
    call  fsubp         ; HL = HL - DE, HL and DE have the same signs

    call  fmul          ; HL = BC * DE
    call  fdiv          ; HL = BC / HL
    call  fmod          ; HL = BC % HL

    call  fpow2         ; HL = HL * HL
    call  fsqrt         ; HL = HL ^ 0.5

    call  frac          ; HL = HL % 1
    call  fint          ; HL = truncate(HL) * 1.0 = HL - ( HL % 1 )

    call  fwld          ; HL = unsigned word HL * 1.0
    call  fbld          ; DE = unsigned char A * 1.0

    Macros (must be included before first use):
    
    ftst  H, L          ; if (HL >= 0) set zero;
    fmul2 H, L          ; HL = 2 * HL
    forsgn H, L, D, E   ; (BIT 7, A) = HL_sign or DE_sign


Size in bytes

     color_flow_warning  EQU     0
     carry_flow_warning  EQU     1

                    binary16    danagy      bfloat
                    --------    ------      ------
     fadd+fsub:     369         177         188 
     fadd:          365         173         184
     fsub:          369         177         188
     
     fmul+fdiv:     10496       2048        1536
     fmul:          8448        1792        1280
     fdiv:          10496       2048        1536
     
     fmod:          118         68          77
     
     fpow2:         8448        512         384
     fsqrt:         4352        768         512
     
     frac:          55          31          33
     fint:          38          23          23
     
     fwld:          58          32          37
     fbld:          18          16          17
     
     all:           15240       3091        2231
