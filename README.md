# Floating-point-Library-for-Z80
Floating-Point Arithmetic Library for Z80


bfloat
======

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

binary16
========

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

    call  fadd  ; HL = HL + DE
    call  faddp ; HL = HL + DE, HL and DE have the same signs
    call  fsub  ; HL = HL - DE, HL and DE have the same signs 
    call  fsubp ; HL = HL - DE

    call  fmul  ; HL = BC * DE
    call  fdiv  ; HL = BC / HL
    call  fmod  ; HL = BC % HL

    call  fpow2 ; HL = HL * HL
    call  frac  ; HL = HL % 1
    call  fsqrt ; HL = HL ^ 0.5

    call  fwld  ; HL = unsigned word HL * 1.0
    call  fbld  ; HL = unsigned char A * 1.0

    Macros:
    
    ftst  H, L  ; if (HL >= 0) set zero;
    fmul2 H, L  ; HL = 2 * HL
