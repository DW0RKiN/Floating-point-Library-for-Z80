
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// gcc -DTARGET=0
#ifndef TARGET
    #define TARGET 0
#endif

#if TARGET == 0
    #warning Target: bfloat
    #define MAX_NUMBER 127
#elif TARGET == 1
    #warning Target: danagy
    #define MAX_NUMBER 255
#elif TARGET == 2
    #warning Target: binary16
    #define MAX_NUMBER 1023
    
// X = $2c00..$33ff
// Y = ((X & $fff0)-$2c00) >> 3 = $00..$fe
// if ( (X & 0xff) <= SIN_LO[Y] ) { sin(X) = X + $ff00 + SIN_LO[Y+1] }
// if ( (X & 0xff) >  SIN_LO[Y] ) { sin(X) = X + $ff00 + SIN_LO[Y+3] }
unsigned char SIN_LO[] = {
//_0   _1   _2   _3   _4   _5   _6   _7   _8   _9   _A   _B   _C   _D   _E   _F
0x0f,0xff,0x1f,0xff,0x2f,0xff,0x3f,0xff,0x4f,0xff,0x5f,0xff,0x6f,0xff,0x7f,0xff,  // 0_     00f,0ff,01f,0ff,02f,0ff,03f,0ff,04f,0ff,05f,0ff,06f,0ff,07f,0ff 0_
0x8f,0xff,0x9f,0xff,0xaf,0xff,0xbf,0xff,0xcf,0xff,0xdf,0xff,0xef,0xff,0xff,0xff,  // 1_     08f,0ff,09f,0ff,0af,0ff,0bf,0ff,0cf,0ff,0df,0ff,0ef,0ff,0ff,0ff 1_
0x0f,0xff,0x1f,0xff,0x2f,0xff,0x3d,0xff,0x4f,0xfe,0x5f,0xfe,0x6f,0xfe,0x7f,0xfe,  // 2_     00f,0ff,01f,0ff,02f,0ff,03d,0ff,04f,0fe,05f,0fe,06f,0fe,07f,0fe 2_
0x8f,0xfe,0x9f,0xfe,0xaf,0xfe,0xbf,0xfe,0xcf,0xfe,0xdf,0xfe,0xef,0xfe,0xff,0xfe,  // 3_     08f,0fe,09f,0fe,0af,0fe,0bf,0fe,0cf,0fe,0df,0fe,0ef,0fe,0ff,0fe 3_
0x0f,0xfe,0x1f,0xfe,0x2f,0xfe,0x37,0xfe,0x4f,0xfd,0x5f,0xfd,0x6f,0xfd,0x7f,0xfd,  // 4_     00f,0fe,01f,0fe,02f,0fe,037,0fe,04f,0fd,05f,0fd,06f,0fd,07f,0fd 4_
0x8f,0xfd,0x9f,0xfd,0xaf,0xfd,0xbf,0xfd,0xcf,0xfd,0xdf,0xfd,0xef,0xfd,0xf4,0xfd,  // 5_     08f,0fd,09f,0fd,0af,0fd,0bf,0fd,0cf,0fd,0df,0fd,0ef,0fd,0f4,0fd 5_
0x0f,0xfc,0x1f,0xfc,0x2f,0xfc,0x3f,0xfc,0x4f,0xfc,0x5f,0xfc,0x6f,0xfc,0x7f,0xfc,  // 6_     00f,0fc,01f,0fc,02f,0fc,03f,0fc,04f,0fc,05f,0fc,06f,0fc,07f,0fc 6_
0x8f,0xfc,0x9f,0xfb,0xaf,0xfb,0xbf,0xfb,0xcf,0xfb,0xdf,0xfb,0xef,0xfb,0xff,0xfb,  // 7_     08f,0fc,09f,0fb,0af,0fb,0bf,0fb,0cf,0fb,0df,0fb,0ef,0fb,0ff,0fb 7_
0x01,0xfc,0x1f,0xfd,0x2f,0xfd,0x3f,0xfd,0x4f,0xfd,0x5f,0xfd,0x61,0xfd,0x7f,0xfc,  // 8_     001,0fc,01f,0fd,02f,0fd,03f,0fd,04f,0fd,05f,0fd,061,0fd,07f,0fc 8_
0x8f,0xfc,0x9f,0xfc,0xaf,0xfc,0xbf,0xfc,0xc3,0xfc,0xdf,0xfb,0xef,0xfb,0xff,0xfb,  // 9_     08f,0fc,09f,0fc,0af,0fc,0bf,0fc,0c3,0fc,0df,0fb,0ef,0fb,0ff,0fb 9_
0x0f,0xfb,0x18,0xfb,0x2f,0xfa,0x3f,0xfa,0x4f,0xfa,0x5f,0xfa,0x62,0xfa,0x7f,0xf9,  // A_     00f,0fb,018,0fb,02f,0fa,03f,0fa,04f,0fa,05f,0fa,062,0fa,07f,0f9 A_
0x8f,0xf9,0x9f,0xf9,0xa6,0xf9,0xbf,0xf8,0xcf,0xf8,0xdf,0xf8,0xe3,0xf8,0xff,0xf7,  // B_     08f,0f9,09f,0f9,0a6,0f9,0bf,0f8,0cf,0f8,0df,0f8,0e3,0f8,0ff,0f7 B_
0x0f,0xf7,0x1c,0xf7,0x2f,0xf6,0x3f,0xf6,0x4f,0xf6,0x52,0xf6,0x6f,0xf5,0x7f,0xf5,  // C_     00f,0f7,01c,0f7,02f,0f6,03f,0f6,04f,0f6,052,0f6,06f,0f5,07f,0f5 C_
0x83,0xf5,0x9f,0xf4,0xaf,0xf4,0xb3,0xf4,0xcf,0xf3,0xdf,0xf3,0xef,0xf2,0xff,0xf2,  // D_     083,0f5,09f,0f4,0af,0f4,0b3,0f4,0cf,0f3,0df,0f3,0ef,0f2,0ff,0f2 D_
0x0a,0xf2,0x1f,0xf1,0x2f,0xf1,0x32,0xf1,0x4f,0xf0,0x59,0xf0,0x6f,0xef,0x7e,0xef,  // E_     00a,0f2,01f,0f1,02f,0f1,032,0f1,04f,0f0,059,0f0,06f,0ef,07e,0ef E_
0x8f,0xee,0x9f,0xee,0xa2,0xee,0xbf,0xed,0xc5,0xed,0xdf,0xec,0xe7,0xec,0xff,0xeb}; // F_     08f,0ee,09f,0ee,0a2,0ee,0bf,0ed,0c5,0ed,0df,0ec,0e7,0ec,0ff,0eb F_
// X = $3400..$37ff
// Y = ((X & $fff8)-$3400) >> 2 = $00..$fe
// if ( (X & 0xff) <= SIN_LO[Y] ) { sin(X) = X + $ff00 + SIN_LO[Y+1] }
// if ( (X & 0xff) >  SIN_LO[Y] ) { sin(X) = X + $ff00 + SIN_LO[Y+3] }
unsigned char SIN_MID[] = {
//_0   _1   _2   _3   _4   _5   _6   _7   _8   _9   _A   _B   _C   _D   _E   _F
0x04,0xec,0x09,0xf1,0x17,0xf5,0x1b,0xf5,0x27,0xf4,0x2f,0xf4,0x37,0xf4,0x38,0xf4,  // 0_     004,0ec,009,0f1,017,0f5,01b,0f5,027,0f4,02f,0f4,037,0f4,038,0f4 0_
0x47,0xf3,0x4f,0xf3,0x55,0xf3,0x5f,0xf2,0x67,0xf2,0x6f,0xf2,0x77,0xf1,0x7f,0xf1,  // 1_     047,0f3,04f,0f3,055,0f3,05f,0f2,067,0f2,06f,0f2,077,0f1,07f,0f1 1_
0x87,0xf1,0x89,0xf1,0x97,0xf0,0x9f,0xf0,0xa1,0xf0,0xaf,0xef,0xb7,0xef,0xb9,0xef,  // 2_     087,0f1,089,0f1,097,0f0,09f,0f0,0a1,0f0,0af,0ef,0b7,0ef,0b9,0ef 2_
0xc7,0xee,0xcf,0xee,0xd0,0xee,0xdf,0xed,0xe6,0xed,0xef,0xec,0xf7,0xec,0xfb,0xec,  // 3_     0c7,0ee,0cf,0ee,0d0,0ee,0df,0ed,0e6,0ed,0ef,0ec,0f7,0ec,0fb,0ec 3_
0x07,0xeb,0x0f,0xeb,0x17,0xea,0x1f,0xea,0x23,0xea,0x2f,0xe9,0x36,0xe9,0x3f,0xe8,  // 4_     007,0eb,00f,0eb,017,0ea,01f,0ea,023,0ea,02f,0e9,036,0e9,03f,0e8 4_
0x47,0xe8,0x49,0xe8,0x57,0xe7,0x5b,0xe7,0x67,0xe6,0x6d,0xe6,0x77,0xe5,0x7e,0xe5,  // 5_     047,0e8,049,0e8,057,0e7,05b,0e7,067,0e6,06d,0e6,077,0e5,07e,0e5 5_
0x87,0xe4,0x8f,0xe4,0x97,0xe3,0x9f,0xe3,0xa0,0xe3,0xaf,0xe2,0xb0,0xe2,0xbf,0xe1,  // 6_     087,0e4,08f,0e4,097,0e3,09f,0e3,0a0,0e3,0af,0e2,0b0,0e2,0bf,0e1 6_
0xc0,0xe1,0xcf,0xe0,0xd7,0xdf,0xde,0xdf,0xe7,0xde,0xed,0xde,0xf7,0xdd,0xfc,0xdd,  // 7_     0c0,0e1,0cf,0e0,0d7,0df,0de,0df,0e7,0de,0ed,0de,0f7,0dd,0fc,0dd 7_
0x07,0xdc,0x0a,0xdc,0x17,0xdb,0x18,0xdb,0x26,0xda,0x2f,0xd9,0x34,0xd9,0x3f,0xd8,  // 8_     007,0dc,00a,0dc,017,0db,018,0db,026,0da,02f,0d9,034,0d9,03f,0d8 8_
0x41,0xd8,0x4e,0xd7,0x57,0xd6,0x5b,0xd6,0x67,0xd5,0x68,0xd5,0x74,0xd4,0x7f,0xd3,  // 9_     041,0d8,04e,0d7,057,0d6,05b,0d6,067,0d5,068,0d5,074,0d4,07f,0d3 9_
0x81,0xd3,0x8d,0xd2,0x97,0xd1,0x99,0xd1,0xa5,0xd0,0xaf,0xcf,0xb1,0xcf,0xbc,0xce,  // A_     081,0d3,08d,0d2,097,0d1,099,0d1,0a5,0d0,0af,0cf,0b1,0cf,0bc,0ce A_
0xc7,0xcd,0xcf,0xcc,0xd3,0xcc,0xde,0xcb,0xe7,0xca,0xe9,0xca,0xf4,0xc9,0xfe,0xc8,  // B_     0c7,0cd,0cf,0cc,0d3,0cc,0de,0cb,0e7,0ca,0e9,0ca,0f4,0c9,0fe,0c8 B_
0x07,0xc7,0x09,0xc7,0x13,0xc6,0x1e,0xc5,0x27,0xc4,0x28,0xc4,0x32,0xc3,0x3c,0xc2,  // C_     007,0c7,009,0c7,013,0c6,01e,0c5,027,0c4,028,0c4,032,0c3,03c,0c2 C_
0x46,0xc1,0x4f,0xc0,0x50,0xc0,0x59,0xbf,0x63,0xbe,0x6c,0xbd,0x76,0xbc,0x7f,0xbb,  // D_     046,0c1,04f,0c0,050,0c0,059,0bf,063,0be,06c,0bd,076,0bc,07f,0bb D_
0x87,0xba,0x88,0xba,0x91,0xb9,0x9b,0xb8,0xa4,0xb7,0xac,0xb6,0xb5,0xb5,0xbe,0xb4,  // E_     087,0ba,088,0ba,091,0b9,09b,0b8,0a4,0b7,0ac,0b6,0b5,0b5,0be,0b4 E_
0xc7,0xb3,0xcf,0xb2,0xd7,0xb1,0xd8,0xb1,0xe0,0xb0,0xe9,0xaf,0xf1,0xae,0xff,0xad}; // F_     0c7,0b3,0cf,0b2,0d7,0b1,0d8,0b1,0e0,0b0,0e9,0af,0f1,0ae,0ff,0ad F_
// X = $3800..$39ff
// Y = ((X & $fffc)-$3800) >> 1 = $00..$fe
// if ( (X & 0xff) <= SIN_LO[Y] ) { sin(X) = X + $ff00 + SIN_LO[Y+1] }
// if ( (X & 0xff) >  SIN_LO[Y] ) { sin(X) = X + $ff00 + SIN_LO[Y+3] }
unsigned char SIN_HI[] = {
//_0   _1   _2   _3   _4   _5   _6   _7   _8   _9   _A   _B   _C   _D   _E   _F
0x02,0xac,0x06,0xaf,0x0a,0xb2,0x0e,0xb5,0x12,0xb8,0x16,0xbb,0x1a,0xbe,0x1e,0xc1,  // 0_     002,0ac,006,0af,00a,0b2,00e,0b5,012,0b8,016,0bb,01a,0be,01e,0c1 0_
0x22,0xc4,0x26,0xc7,0x2a,0xca,0x2d,0xcc,0x33,0xcf,0x37,0xcf,0x38,0xcf,0x3f,0xce,  // 1_     022,0c4,026,0c7,02a,0ca,02d,0cc,033,0cf,037,0cf,038,0cf,03f,0ce 1_
0x40,0xce,0x47,0xcd,0x4b,0xcc,0x4e,0xcc,0x53,0xcb,0x55,0xcb,0x5b,0xca,0x5c,0xca,  // 2_     040,0ce,047,0cd,04b,0cc,04e,0cc,053,0cb,055,0cb,05b,0ca,05c,0ca 2_
0x63,0xc9,0x67,0xc8,0x6a,0xc8,0x6f,0xc7,0x70,0xc7,0x77,0xc6,0x7b,0xc5,0x7e,0xc5,  // 3_     063,0c9,067,0c8,06a,0c8,06f,0c7,070,0c7,077,0c6,07b,0c5,07e,0c5 3_
0x83,0xc4,0x84,0xc4,0x8a,0xc3,0x8f,0xc2,0x91,0xc2,0x97,0xc1,0x9b,0xc0,0x9d,0xc0,  // 4_     083,0c4,084,0c4,08a,0c3,08f,0c2,091,0c2,097,0c1,09b,0c0,09d,0c0 4_
0xa3,0xbf,0xa7,0xbe,0xaa,0xbe,0xaf,0xbd,0xb0,0xbd,0xb5,0xbc,0xbb,0xbb,0xbf,0xba,  // 5_     0a3,0bf,0a7,0be,0aa,0be,0af,0bd,0b0,0bd,0b5,0bc,0bb,0bb,0bf,0ba 5_
0xc1,0xba,0xc7,0xb9,0xcb,0xb8,0xcd,0xb8,0xd2,0xb7,0xd7,0xb6,0xd8,0xb6,0xde,0xb5,  // 6_     0c1,0ba,0c7,0b9,0cb,0b8,0cd,0b8,0d2,0b7,0d7,0b6,0d8,0b6,0de,0b5 6_
0xe3,0xb4,0xe7,0xb3,0xe9,0xb3,0xee,0xb2,0xf3,0xb1,0xf4,0xb1,0xf9,0xb0,0xfe,0xaf,  // 7_     0e3,0b4,0e7,0b3,0e9,0b3,0ee,0b2,0f3,0b1,0f4,0b1,0f9,0b0,0fe,0af 7_
0x03,0xae,0x04,0xae,0x09,0xad,0x0e,0xac,0x13,0xab,0x17,0xaa,0x18,0xaa,0x1d,0xa9,  // 8_     003,0ae,004,0ae,009,0ad,00e,0ac,013,0ab,017,0aa,018,0aa,01d,0a9 8_
0x22,0xa8,0x27,0xa7,0x2b,0xa6,0x2c,0xa6,0x31,0xa5,0x36,0xa4,0x3b,0xa3,0x3f,0xa2,  // 9_     022,0a8,027,0a7,02b,0a6,02c,0a6,031,0a5,036,0a4,03b,0a3,03f,0a2 9_
0x40,0xa2,0x45,0xa1,0x49,0xa0,0x4e,0x9f,0x53,0x9e,0x57,0x9d,0x58,0x9d,0x5c,0x9c,  // A_     040,0a2,045,0a1,049,0a0,04e,09f,053,09e,057,09d,058,09d,05c,09c A_
0x61,0x9b,0x65,0x9a,0x6a,0x99,0x6e,0x98,0x73,0x97,0x77,0x96,0x7b,0x95,0x7c,0x95,  // B_     061,09b,065,09a,06a,099,06e,098,073,097,077,096,07b,095,07c,095 B_
0x80,0x94,0x85,0x93,0x89,0x92,0x8d,0x91,0x92,0x90,0x96,0x8f,0x9a,0x8e,0x9e,0x8d,  // C_     080,094,085,093,089,092,08d,091,092,090,096,08f,09a,08e,09e,08d C_
0xa3,0x8c,0xa7,0x8b,0xab,0x8a,0xaf,0x89,0xb3,0x88,0xb7,0x87,0xbb,0x86,0xbf,0x85,  // D_     0a3,08c,0a7,08b,0ab,08a,0af,089,0b3,088,0b7,087,0bb,086,0bf,085 D_
0xc3,0x84,0xc4,0x84,0xc8,0x83,0xcc,0x82,0xcf,0x81,0xd3,0x80,0xd7,0x7f,0xdb,0x7e,  // E_     0c3,084,0c4,084,0c8,083,0cc,082,0cf,081,0d3,080,0d7,07f,0db,07e E_
0xdf,0x7d,0xe3,0x7c,0xe7,0x7b,0xeb,0x7a,0xf2,0x79,0xf6,0x77,0xfa,0x76,0xfe,0x75}; // F_     0df,07d,0e3,07c,0e7,07b,0eb,07a,0f2,079,0f6,077,0fa,076,0fe,075 F_




// hi = 0xff
unsigned char SIN_3a[] = {
//  _0  _1  _2  _3  _4  _5  _6  _7  _8  _9  _A  _B  _C  _D  _E  _F
0x74,0x74,0x73,0x73,0x73,0x73,0x72,0x72,0x72,0x72,0x71,0x71,0x71,0x70,0x70,0x70,  // 0_     ff74,ff74,ff73,ff73,ff73,ff73,ff72,ff72,ff72,ff72,ff71,ff71,ff71,ff70,ff70,ff70 0_
0x70,0x6f,0x6f,0x6f,0x6f,0x6e,0x6e,0x6e,0x6d,0x6d,0x6d,0x6d,0x6c,0x6c,0x6c,0x6c,  // 1_     ff70,ff6f,ff6f,ff6f,ff6f,ff6e,ff6e,ff6e,ff6d,ff6d,ff6d,ff6d,ff6c,ff6c,ff6c,ff6c 1_
0x6b,0x6b,0x6b,0x6a,0x6a,0x6a,0x6a,0x69,0x69,0x69,0x68,0x68,0x68,0x68,0x67,0x67,  // 2_     ff6b,ff6b,ff6b,ff6a,ff6a,ff6a,ff6a,ff69,ff69,ff69,ff68,ff68,ff68,ff68,ff67,ff67 2_
0x67,0x66,0x66,0x66,0x66,0x65,0x65,0x65,0x64,0x64,0x64,0x64,0x63,0x63,0x63,0x62,  // 3_     ff67,ff66,ff66,ff66,ff66,ff65,ff65,ff65,ff64,ff64,ff64,ff64,ff63,ff63,ff63,ff62 3_
0x62,0x62,0x62,0x61,0x61,0x61,0x60,0x60,0x60,0x60,0x5f,0x5f,0x5f,0x5e,0x5e,0x5e,  // 4_     ff62,ff62,ff62,ff61,ff61,ff61,ff60,ff60,ff60,ff60,ff5f,ff5f,ff5f,ff5e,ff5e,ff5e 4_
0x5d,0x5d,0x5d,0x5d,0x5c,0x5c,0x5c,0x5b,0x5b,0x5b,0x5a,0x5a,0x5a,0x5a,0x59,0x59,  // 5_     ff5d,ff5d,ff5d,ff5d,ff5c,ff5c,ff5c,ff5b,ff5b,ff5b,ff5a,ff5a,ff5a,ff5a,ff59,ff59 5_
0x59,0x58,0x58,0x58,0x57,0x57,0x57,0x57,0x56,0x56,0x56,0x55,0x55,0x55,0x54,0x54,  // 6_     ff59,ff58,ff58,ff58,ff57,ff57,ff57,ff57,ff56,ff56,ff56,ff55,ff55,ff55,ff54,ff54 6_
0x54,0x54,0x53,0x53,0x53,0x52,0x52,0x52,0x51,0x51,0x51,0x50,0x50,0x50,0x4f,0x4f,  // 7_     ff54,ff54,ff53,ff53,ff53,ff52,ff52,ff52,ff51,ff51,ff51,ff50,ff50,ff50,ff4f,ff4f 7_
0x4f,0x4f,0x4e,0x4e,0x4e,0x4d,0x4d,0x4d,0x4c,0x4c,0x4c,0x4b,0x4b,0x4b,0x4a,0x4a,  // 8_     ff4f,ff4f,ff4e,ff4e,ff4e,ff4d,ff4d,ff4d,ff4c,ff4c,ff4c,ff4b,ff4b,ff4b,ff4a,ff4a 8_
0x4a,0x4a,0x49,0x49,0x49,0x48,0x48,0x48,0x47,0x47,0x47,0x46,0x46,0x46,0x45,0x45,  // 9_     ff4a,ff4a,ff49,ff49,ff49,ff48,ff48,ff48,ff47,ff47,ff47,ff46,ff46,ff46,ff45,ff45 9_
0x45,0x44,0x44,0x44,0x43,0x43,0x43,0x42,0x42,0x42,0x41,0x41,0x41,0x40,0x40,0x40,  // A_     ff45,ff44,ff44,ff44,ff43,ff43,ff43,ff42,ff42,ff42,ff41,ff41,ff41,ff40,ff40,ff40 A_
0x3f,0x3f,0x3f,0x3e,0x3e,0x3e,0x3d,0x3d,0x3d,0x3c,0x3c,0x3c,0x3b,0x3b,0x3b,0x3a,  // B_     ff3f,ff3f,ff3f,ff3e,ff3e,ff3e,ff3d,ff3d,ff3d,ff3c,ff3c,ff3c,ff3b,ff3b,ff3b,ff3a B_
0x3a,0x3a,0x39,0x39,0x39,0x38,0x38,0x38,0x37,0x37,0x37,0x36,0x36,0x36,0x35,0x35,  // C_     ff3a,ff3a,ff39,ff39,ff39,ff38,ff38,ff38,ff37,ff37,ff37,ff36,ff36,ff36,ff35,ff35 C_
0x35,0x34,0x34,0x34,0x33,0x33,0x33,0x32,0x32,0x32,0x31,0x31,0x31,0x30,0x30,0x30,  // D_     ff35,ff34,ff34,ff34,ff33,ff33,ff33,ff32,ff32,ff32,ff31,ff31,ff31,ff30,ff30,ff30 D_
0x2f,0x2f,0x2f,0x2e,0x2e,0x2d,0x2d,0x2d,0x2c,0x2c,0x2c,0x2b,0x2b,0x2b,0x2a,0x2a,  // E_     ff2f,ff2f,ff2f,ff2e,ff2e,ff2d,ff2d,ff2d,ff2c,ff2c,ff2c,ff2b,ff2b,ff2b,ff2a,ff2a E_
0x2a,0x29,0x29,0x29,0x28,0x28,0x28,0x27,0x27,0x26,0x26,0x26,0x25,0x25,0x25,0x24}; // F_     ff2a,ff29,ff29,ff29,ff28,ff28,ff28,ff27,ff27,ff26,ff26,ff26,ff25,ff25,ff25,ff24 F_

// hi = 0x3a
unsigned char SIN_3b[] = {
//  _0  _1  _2  _3  _4  _5  _6  _7  _8  _9  _A  _B  _C  _D  _E  _F
0x24,0x25,0x25,0x26,0x26,0x27,0x28,0x28,0x29,0x2a,0x2a,0x2b,0x2c,0x2c,0x2d,0x2e,  // 0_     3a24,3a25,3a25,3a26,3a26,3a27,3a28,3a28,3a29,3a2a,3a2a,3a2b,3a2c,3a2c,3a2d,3a2e 0_
0x2e,0x2f,0x2f,0x30,0x31,0x31,0x32,0x33,0x33,0x34,0x34,0x35,0x36,0x36,0x37,0x38,  // 1_     3a2e,3a2f,3a2f,3a30,3a31,3a31,3a32,3a33,3a33,3a34,3a34,3a35,3a36,3a36,3a37,3a38 1_
0x38,0x39,0x3a,0x3a,0x3b,0x3b,0x3c,0x3d,0x3d,0x3e,0x3f,0x3f,0x40,0x40,0x41,0x42,  // 2_     3a38,3a39,3a3a,3a3a,3a3b,3a3b,3a3c,3a3d,3a3d,3a3e,3a3f,3a3f,3a40,3a40,3a41,3a42 2_
0x42,0x43,0x44,0x44,0x45,0x45,0x46,0x47,0x47,0x48,0x48,0x49,0x4a,0x4a,0x4b,0x4c,  // 3_     3a42,3a43,3a44,3a44,3a45,3a45,3a46,3a47,3a47,3a48,3a48,3a49,3a4a,3a4a,3a4b,3a4c 3_
0x4c,0x4d,0x4d,0x4e,0x4f,0x4f,0x50,0x50,0x51,0x52,0x52,0x53,0x54,0x54,0x55,0x55,  // 4_     3a4c,3a4d,3a4d,3a4e,3a4f,3a4f,3a50,3a50,3a51,3a52,3a52,3a53,3a54,3a54,3a55,3a55 4_
0x56,0x57,0x57,0x58,0x58,0x59,0x5a,0x5a,0x5b,0x5b,0x5c,0x5d,0x5d,0x5e,0x5f,0x5f,  // 5_     3a56,3a57,3a57,3a58,3a58,3a59,3a5a,3a5a,3a5b,3a5b,3a5c,3a5d,3a5d,3a5e,3a5f,3a5f 5_
0x60,0x60,0x61,0x62,0x62,0x63,0x63,0x64,0x65,0x65,0x66,0x66,0x67,0x68,0x68,0x69,  // 6_     3a60,3a60,3a61,3a62,3a62,3a63,3a63,3a64,3a65,3a65,3a66,3a66,3a67,3a68,3a68,3a69 6_
0x69,0x6a,0x6b,0x6b,0x6c,0x6c,0x6d,0x6e,0x6e,0x6f,0x6f,0x70,0x70,0x71,0x72,0x72,  // 7_     3a69,3a6a,3a6b,3a6b,3a6c,3a6c,3a6d,3a6e,3a6e,3a6f,3a6f,3a70,3a70,3a71,3a72,3a72 7_
0x73,0x73,0x74,0x75,0x75,0x76,0x76,0x77,0x78,0x78,0x79,0x79,0x7a,0x7b,0x7b,0x7c,  // 8_     3a73,3a73,3a74,3a75,3a75,3a76,3a76,3a77,3a78,3a78,3a79,3a79,3a7a,3a7b,3a7b,3a7c 8_
0x7c,0x7d,0x7d,0x7e,0x7f,0x7f,0x80,0x80,0x81,0x82,0x82,0x83,0x83,0x84,0x84,0x85,  // 9_     3a7c,3a7d,3a7d,3a7e,3a7f,3a7f,3a80,3a80,3a81,3a82,3a82,3a83,3a83,3a84,3a84,3a85 9_
0x86,0x86,0x87,0x87,0x88,0x88,0x89,0x8a,0x8a,0x8b,0x8b,0x8c,0x8d,0x8d,0x8e,0x8e,  // A_     3a86,3a86,3a87,3a87,3a88,3a88,3a89,3a8a,3a8a,3a8b,3a8b,3a8c,3a8d,3a8d,3a8e,3a8e A_
0x8f,0x8f,0x90,0x91,0x91,0x92,0x92,0x93,0x93,0x94,0x95,0x95,0x96,0x96,0x97,0x97,  // B_     3a8f,3a8f,3a90,3a91,3a91,3a92,3a92,3a93,3a93,3a94,3a95,3a95,3a96,3a96,3a97,3a97 B_
0x98,0x98,0x99,0x9a,0x9a,0x9b,0x9b,0x9c,0x9c,0x9d,0x9e,0x9e,0x9f,0x9f,0xa0,0xa0,  // C_     3a98,3a98,3a99,3a9a,3a9a,3a9b,3a9b,3a9c,3a9c,3a9d,3a9e,3a9e,3a9f,3a9f,3aa0,3aa0 C_
0xa1,0xa1,0xa2,0xa3,0xa3,0xa4,0xa4,0xa5,0xa5,0xa6,0xa7,0xa7,0xa8,0xa8,0xa9,0xa9,  // D_     3aa1,3aa1,3aa2,3aa3,3aa3,3aa4,3aa4,3aa5,3aa5,3aa6,3aa7,3aa7,3aa8,3aa8,3aa9,3aa9 D_
0xaa,0xaa,0xab,0xab,0xac,0xad,0xad,0xae,0xae,0xaf,0xaf,0xb0,0xb0,0xb1,0xb2,0xb2,  // E_     3aaa,3aaa,3aab,3aab,3aac,3aad,3aad,3aae,3aae,3aaf,3aaf,3ab0,3ab0,3ab1,3ab2,3ab2 E_
0xb3,0xb3,0xb4,0xb4,0xb5,0xb5,0xb6,0xb6,0xb7,0xb8,0xb8,0xb9,0xb9,0xba,0xba,0xbb}; // F_     3ab3,3ab3,3ab4,3ab4,3ab5,3ab5,3ab6,3ab6,3ab7,3ab8,3ab8,3ab9,3ab9,3aba,3aba,3abb F_

// hi = 0xfe
unsigned char SIN_3c[] = {
//  _0  _1  _2  _3  _4  _5  _6  _7  _8  _9  _A  _B  _C  _D  _E  _F
0xbb,0xbb,0xbb,0xbc,0xbc,0xbc,0xbc,0xbc,0xbc,0xbc,0xbc,0xbc,0xbc,0xbc,0xbc,0xbc,  // 0_     febb,febb,febb,febc,febc,febc,febc,febc,febc,febc,febc,febc,febc,febc,febc,febc 0_
0xbc,0xbc,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,  // 1_     febc,febc,febd,febd,febd,febd,febd,febd,febd,febd,febd,febd,febd,febd,febd,febd 1_
0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,  // 2_     febd,febd,febd,febd,febd,febd,febd,febd,febd,febd,febd,febd,febd,febd,febd,febd 2_
0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,  // 3_     febd,febd,febd,febd,febd,febd,febd,febd,febd,febd,febd,febd,febd,febd,febd,febd 3_
0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbd,0xbc,  // 4_     febd,febd,febd,febd,febd,febd,febd,febd,febd,febd,febd,febd,febd,febd,febd,febc 4_
0xbc,0xbc,0xbc,0xbc,0xbc,0xbc,0xbc,0xbc,0xbc,0xbc,0xbc,0xbc,0xbc,0xbc,0xbc,0xbb,  // 5_     febc,febc,febc,febc,febc,febc,febc,febc,febc,febc,febc,febc,febc,febc,febc,febb 5_
0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,0xba,0xba,0xba,0xba,0xba,0xba,  // 6_     febb,febb,febb,febb,febb,febb,febb,febb,febb,febb,feba,feba,feba,feba,feba,feba 6_
0xba,0xba,0xba,0xb9,0xb9,0xb9,0xb9,0xb9,0xb9,0xb9,0xb9,0xb9,0xb8,0xb8,0xb8,0xb8,  // 7_     feba,feba,feba,feb9,feb9,feb9,feb9,feb9,feb9,feb9,feb9,feb9,feb8,feb8,feb8,feb8 7_
0xb8,0xb8,0xb8,0xb7,0xb7,0xb7,0xb7,0xb7,0xb7,0xb7,0xb6,0xb6,0xb6,0xb6,0xb6,0xb6,  // 8_     feb8,feb8,feb8,feb7,feb7,feb7,feb7,feb7,feb7,feb7,feb6,feb6,feb6,feb6,feb6,feb6 8_
0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb4,0xb4,0xb4,0xb4,0xb4,0xb3,0xb3,0xb3,0xb3,0xb3,  // 9_     feb5,feb5,feb5,feb5,feb5,feb5,feb4,feb4,feb4,feb4,feb4,feb3,feb3,feb3,feb3,feb3 9_
0xb3,0xb2,0xb2,0xb2,0xb2,0xb2,0xb1,0xb1,0xb1,0xb1,0xb0,0xb0,0xb0,0xb0,0xb0,0xaf,  // A_     feb3,feb2,feb2,feb2,feb2,feb2,feb1,feb1,feb1,feb1,feb0,feb0,feb0,feb0,feb0,feaf A_
0xaf,0xaf,0xaf,0xaf,0xae,0xae,0xae,0xae,0xad,0xad,0xad,0xad,0xac,0xac,0xac,0xac,  // B_     feaf,feaf,feaf,feaf,feae,feae,feae,feae,fead,fead,fead,fead,feac,feac,feac,feac B_
0xab,0xab,0xab,0xab,0xaa,0xaa,0xaa,0xaa,0xa9,0xa9,0xa9,0xa9,0xa8,0xa8,0xa8,0xa7,  // C_     feab,feab,feab,feab,feaa,feaa,feaa,feaa,fea9,fea9,fea9,fea9,fea8,fea8,fea8,fea7 C_
0xa7,0xa7,0xa7,0xa6,0xa6,0xa6,0xa5,0xa5,0xa5,0xa5,0xa4,0xa4,0xa4,0xa3,0xa3,0xa3,  // D_     fea7,fea7,fea7,fea6,fea6,fea6,fea5,fea5,fea5,fea5,fea4,fea4,fea4,fea3,fea3,fea3 D_
0xa2,0xa2,0xa2,0xa1,0xa1,0xa1,0xa0,0xa0,0xa0,0xa0,0x9f,0x9f,0x9f,0x9e,0x9e,0x9e,  // E_     fea2,fea2,fea2,fea1,fea1,fea1,fea0,fea0,fea0,fea0,fe9f,fe9f,fe9f,fe9e,fe9e,fe9e E_
0x9d,0x9d,0x9d,0x9c,0x9c,0x9b,0x9b,0x9b,0x9a,0x9a,0x9a,0x99,0x99,0x99,0x98,0x98}; // F_     fe9d,fe9d,fe9d,fe9c,fe9c,fe9b,fe9b,fe9b,fe9a,fe9a,fe9a,fe99,fe99,fe99,fe98,fe98 F_

// hi = 0x3b
unsigned char SIN_3d[] = {
//  _0  _1  _2  _3  _4  _5  _6  _7  _8  _9  _A  _B  _C  _D  _E  _F
0x98,0x98,0x99,0x99,0x9a,0x9b,0x9b,0x9c,0x9d,0x9d,0x9e,0x9e,0x9f,0xa0,0xa0,0xa1,  // 0_     3b98,3b98,3b99,3b99,3b9a,3b9b,3b9b,3b9c,3b9d,3b9d,3b9e,3b9e,3b9f,3ba0,3ba0,3ba1 0_
0xa1,0xa2,0xa3,0xa3,0xa4,0xa4,0xa5,0xa6,0xa6,0xa7,0xa7,0xa8,0xa8,0xa9,0xaa,0xaa,  // 1_     3ba1,3ba2,3ba3,3ba3,3ba4,3ba4,3ba5,3ba6,3ba6,3ba7,3ba7,3ba8,3ba8,3ba9,3baa,3baa 1_
0xab,0xab,0xac,0xac,0xad,0xae,0xae,0xaf,0xaf,0xb0,0xb0,0xb1,0xb1,0xb2,0xb3,0xb3,  // 2_     3bab,3bab,3bac,3bac,3bad,3bae,3bae,3baf,3baf,3bb0,3bb0,3bb1,3bb1,3bb2,3bb3,3bb3 2_
0xb4,0xb4,0xb5,0xb5,0xb6,0xb6,0xb7,0xb7,0xb8,0xb8,0xb9,0xb9,0xba,0xbb,0xbb,0xbc,  // 3_     3bb4,3bb4,3bb5,3bb5,3bb6,3bb6,3bb7,3bb7,3bb8,3bb8,3bb9,3bb9,3bba,3bbb,3bbb,3bbc 3_
0xbc,0xbd,0xbd,0xbe,0xbe,0xbf,0xbf,0xc0,0xc0,0xc1,0xc1,0xc2,0xc2,0xc3,0xc3,0xc4,  // 4_     3bbc,3bbd,3bbd,3bbe,3bbe,3bbf,3bbf,3bc0,3bc0,3bc1,3bc1,3bc2,3bc2,3bc3,3bc3,3bc4 4_
0xc4,0xc4,0xc5,0xc5,0xc6,0xc6,0xc7,0xc7,0xc8,0xc8,0xc9,0xc9,0xca,0xca,0xcb,0xcb,  // 5_     3bc4,3bc4,3bc5,3bc5,3bc6,3bc6,3bc7,3bc7,3bc8,3bc8,3bc9,3bc9,3bca,3bca,3bcb,3bcb 5_
0xcb,0xcc,0xcc,0xcd,0xcd,0xce,0xce,0xcf,0xcf,0xcf,0xd0,0xd0,0xd1,0xd1,0xd2,0xd2,  // 6_     3bcb,3bcc,3bcc,3bcd,3bcd,3bce,3bce,3bcf,3bcf,3bcf,3bd0,3bd0,3bd1,3bd1,3bd2,3bd2 6_
0xd2,0xd3,0xd3,0xd4,0xd4,0xd4,0xd5,0xd5,0xd6,0xd6,0xd6,0xd7,0xd7,0xd8,0xd8,0xd8,  // 7_     3bd2,3bd3,3bd3,3bd4,3bd4,3bd4,3bd5,3bd5,3bd6,3bd6,3bd6,3bd7,3bd7,3bd8,3bd8,3bd8 7_
0xd9,0xd9,0xda,0xda,0xda,0xdb,0xdb,0xdc,0xdc,0xdc,0xdd,0xdd,0xdd,0xde,0xde,0xde,  // 8_     3bd9,3bd9,3bda,3bda,3bda,3bdb,3bdb,3bdc,3bdc,3bdc,3bdd,3bdd,3bdd,3bde,3bde,3bde 8_
0xdf,0xdf,0xe0,0xe0,0xe0,0xe1,0xe1,0xe1,0xe2,0xe2,0xe2,0xe3,0xe3,0xe3,0xe4,0xe4,  // 9_     3bdf,3bdf,3be0,3be0,3be0,3be1,3be1,3be1,3be2,3be2,3be2,3be3,3be3,3be3,3be4,3be4 9_
0xe4,0xe5,0xe5,0xe5,0xe6,0xe6,0xe6,0xe7,0xe7,0xe7,0xe8,0xe8,0xe8,0xe8,0xe9,0xe9,  // A_     3be4,3be5,3be5,3be5,3be6,3be6,3be6,3be7,3be7,3be7,3be8,3be8,3be8,3be8,3be9,3be9 A_
0xe9,0xea,0xea,0xea,0xeb,0xeb,0xeb,0xeb,0xec,0xec,0xec,0xec,0xed,0xed,0xed,0xee,  // B_     3be9,3bea,3bea,3bea,3beb,3beb,3beb,3beb,3bec,3bec,3bec,3bec,3bed,3bed,3bed,3bee B_
0xee,0xee,0xee,0xef,0xef,0xef,0xef,0xf0,0xf0,0xf0,0xf0,0xf1,0xf1,0xf1,0xf1,0xf2,  // C_     3bee,3bee,3bee,3bef,3bef,3bef,3bef,3bf0,3bf0,3bf0,3bf0,3bf1,3bf1,3bf1,3bf1,3bf2 C_
0xf2,0xf2,0xf2,0xf3,0xf3,0xf3,0xf3,0xf3,0xf4,0xf4,0xf4,0xf4,0xf5,0xf5,0xf5,0xf5,  // D_     3bf2,3bf2,3bf2,3bf3,3bf3,3bf3,3bf3,3bf3,3bf4,3bf4,3bf4,3bf4,3bf5,3bf5,3bf5,3bf5 D_
0xf5,0xf6,0xf6,0xf6,0xf6,0xf6,0xf7,0xf7,0xf7,0xf7,0xf7,0xf7,0xf8,0xf8,0xf8,0xf8,  // E_     3bf5,3bf6,3bf6,3bf6,3bf6,3bf6,3bf7,3bf7,3bf7,3bf7,3bf7,3bf7,3bf8,3bf8,3bf8,3bf8 E_
0xf8,0xf9,0xf9,0xf9,0xf9,0xf9,0xf9,0xfa,0xfa,0xfa,0xfa,0xfa,0xfa,0xfa,0xfb,0xfb}; // F_     3bf8,3bf9,3bf9,3bf9,3bf9,3bf9,3bf9,3bfa,3bfa,3bfa,3bfa,3bfa,3bfa,3bfa,3bfb,3bfb F_

// hi = 0xfd
unsigned char SIN_3e[] = {
//  _0  _1  _2  _3  _4  _5  _6  _7  _8  _9  _A  _B  _C  _D  _E  _F
0xfb,0xfa,0xf9,0xf8,0xf7,0xf7,0xf6,0xf5,0xf4,0xf3,0xf2,0xf1,0xf0,0xf0,0xef,0xee,  // 0_     fdfb,fdfa,fdf9,fdf8,fdf7,fdf7,fdf6,fdf5,fdf4,fdf3,fdf2,fdf1,fdf0,fdf0,fdef,fdee 0_
0xed,0xec,0xeb,0xea,0xe9,0xe8,0xe8,0xe7,0xe6,0xe5,0xe4,0xe3,0xe2,0xe1,0xe0,0xdf,  // 1_     fded,fdec,fdeb,fdea,fde9,fde8,fde8,fde7,fde6,fde5,fde4,fde3,fde2,fde1,fde0,fddf 1_
0xde,0xdd,0xdd,0xdc,0xdb,0xda,0xd9,0xd8,0xd7,0xd6,0xd5,0xd4,0xd3,0xd2,0xd1,0xd0,  // 2_     fdde,fddd,fddd,fddc,fddb,fdda,fdd9,fdd8,fdd7,fdd6,fdd5,fdd4,fdd3,fdd2,fdd1,fdd0 2_
0xcf,0xce,0xce,0xcd,0xcc,0xcb,0xca,0xc9,0xc8,0xc7,0xc6,0xc5,0xc4,0xc3,0xc2,0xc1,  // 3_     fdcf,fdce,fdce,fdcd,fdcc,fdcb,fdca,fdc9,fdc8,fdc7,fdc6,fdc5,fdc4,fdc3,fdc2,fdc1 3_
0xc0,0xbf,0xbe,0xbd,0xbc,0xbb,0xba,0xb9,0xb8,0xb7,0xb6,0xb5,0xb4,0xb3,0xb2,0xb1,  // 4_     fdc0,fdbf,fdbe,fdbd,fdbc,fdbb,fdba,fdb9,fdb8,fdb7,fdb6,fdb5,fdb4,fdb3,fdb2,fdb1 4_
0xb0,0xaf,0xae,0xad,0xac,0xab,0xaa,0xa9,0xa8,0xa7,0xa6,0xa5,0xa4,0xa3,0xa2,0xa1,  // 5_     fdb0,fdaf,fdae,fdad,fdac,fdab,fdaa,fda9,fda8,fda7,fda6,fda5,fda4,fda3,fda2,fda1 5_
0x9f,0x9e,0x9d,0x9c,0x9b,0x9a,0x99,0x98,0x97,0x96,0x95,0x94,0x93,0x92,0x91,0x90,  // 6_     fd9f,fd9e,fd9d,fd9c,fd9b,fd9a,fd99,fd98,fd97,fd96,fd95,fd94,fd93,fd92,fd91,fd90 6_
0x8e,0x8d,0x8c,0x8b,0x8a,0x89,0x88,0x87,0x86,0x85,0x84,0x83,0x81,0x80,0x7f,0x7e,  // 7_     fd8e,fd8d,fd8c,fd8b,fd8a,fd89,fd88,fd87,fd86,fd85,fd84,fd83,fd81,fd80,fd7f,fd7e 7_
0x7d,0x7c,0x7b,0x7a,0x79,0x77,0x76,0x75,0x74,0x73,0x72,0x71,0x70,0x6e,0x6d,0x6c,  // 8_     fd7d,fd7c,fd7b,fd7a,fd79,fd77,fd76,fd75,fd74,fd73,fd72,fd71,fd70,fd6e,fd6d,fd6c 8_
0x6b,0x6a,0x69,0x68,0x66,0x65,0x64,0x63,0x62,0x61,0x60,0x5e,0x5d,0x5c,0x5b,0x5a,  // 9_     fd6b,fd6a,fd69,fd68,fd66,fd65,fd64,fd63,fd62,fd61,fd60,fd5e,fd5d,fd5c,fd5b,fd5a 9_
0x59,0x57,0x56,0x55,0x54,0x53,0x51,0x50,0x4f,0x4e,0x4d,0x4c,0x4a,0x49,0x48,0x47,  // A_     fd59,fd57,fd56,fd55,fd54,fd53,fd51,fd50,fd4f,fd4e,fd4d,fd4c,fd4a,fd49,fd48,fd47 A_
0x46,0x44,0x43,0x42,0x41,0x40,0x3e,0x3d,0x3c,0x3b,0x39,0x38,0x37,0x36,0x35,0x33,  // B_     fd46,fd44,fd43,fd42,fd41,fd40,fd3e,fd3d,fd3c,fd3b,fd39,fd38,fd37,fd36,fd35,fd33 B_
0x32,0x31,0x30,0x2e,0x2d,0x2c,0x2b,0x29,0x28,0x27,0x26,0x24,0x23,0x22,0x21,0x1f,  // C_     fd32,fd31,fd30,fd2e,fd2d,fd2c,fd2b,fd29,fd28,fd27,fd26,fd24,fd23,fd22,fd21,fd1f C_
0x1e,0x1d,0x1c,0x1a,0x19,0x18,0x16,0x15,0x14,0x13,0x11,0x10,0x0f,0x0e,0x0c,0x0b,  // D_     fd1e,fd1d,fd1c,fd1a,fd19,fd18,fd16,fd15,fd14,fd13,fd11,fd10,fd0f,fd0e,fd0c,fd0b D_
0x0a,0x08,0x07,0x06,0x04,0x03,0x02,0x01,0xff,0xfe,0xfd,0xfb,0xfa,0xf9,0xf7,0xf6,  // E_     fd0a,fd08,fd07,fd06,fd04,fd03,fd02,fd01,fcff,fcfe,fcfd,fcfb,fcfa,fcf9,fcf7,fcf6 E_
0xf5,0xf3,0xf2,0xf1,0xef,0xee,0xed,0xeb,0xea,0xe9,0xe7,0xe6,0xe5,0xe3,0xe2,0xe1}; // F_     fcf5,fcf3,fcf2,fcf1,fcef,fcee,fced,fceb,fcea,fce9,fce7,fce6,fce5,fce3,fce2,fce1 F_


#else
    #error Byla nalezena neocekavana hodnota v promenne TARGET!
#endif

#include "../C/float.h"

const double lud = 2.7182818284590452353602875;

__uint16_t inc_x(__uint16_t num) {
    __uint16_t sign = num & MASK_SIGN;
    num++;
    if ( sign != ( num & MASK_SIGN )) num += MASK_SIGN;
    return num;
}


__uint16_t dec_x(__uint16_t num){
    __uint16_t sign = num & MASK_SIGN;
    num--;
    if ( sign != ( num & MASK_SIGN )) num -= MASK_SIGN;
    return num;
}

// -π/4..π/4 = -0.785398..0.785398
// 2^-1 * 1.570796 = 2^-1 * 1.100 1001 
// 2^-1 * 1.000 000 .. 1.570796 = 0.5 .. 0.785398
// 2^-2 * mantissa = 0.25.. 0.4999
// sin(x) = x - x^3/3! + x^5/5! - x^7/7! + ...
__uint16_t test_sin(int ie, int im)
{
    __uint16_t n = MAKE_X( 0, ie-BIAS, im );

    double num = X_TO_DOUBLE( n );
    double red, r;
    __uint16_t n_red = n & 0xfffe;

    red = (X_TO_DOUBLE(n) + X_TO_DOUBLE(n+1))/2;
//     red = pow(red,3)/6 + pow(red,5)/120 - pow(red,7)/(7*6*120) + pow(red,9)/(9*8*7*6*120);
    
    if ( ie - BIAS < -4 ) {
        r = num;
#if MAX_NUMBER == 1023
        if ( n >= 0x29c5 ) return --n;
    }
    else {
        
        if ( n >= 0x3A00 ) {
            
            if ( (n >> 8) == 0x3a ) {
                return n + 0xff00 + SIN_3a[n & 0xff];
            }
            
            if ( (n >> 8) == 0x3c ) {
                return n + 0xfe00 + SIN_3c[n & 0xff];
            }
            
            if ( (n >> 8) == 0x3e ) {
                return n + 0xfd00 + SIN_3e[n & 0xff];
            }
  
            if ( (n >> 8) == 0x3b ) {
                return 0x3a00 + SIN_3b[n & 0xff];
            }
            
            if ( (n >> 8) == 0x3d ) {
                return 0x3b00 + SIN_3d[n & 0xff];
            }
            
        }

        int y;

        if ( n >= 0x3800 ) {
            y = (( n & 0xfffc ) - 0x3800) >> 1;
        
            if ( (n & 0xff) <= SIN_HI[y] ) return ( n + 0xff00 + SIN_HI[y+1]);
            
            if ( y + 3 > 256 ) 
                return ( n + 0xff00 + SIN_3a[1]);
            
            return ( n + 0xff00 + SIN_HI[y+3]);
        }
        
        if ( n >= 0x3400 ) {
            y = (( n & 0xfff8 ) - 0x3400) >> 2;
        
            if ( (n & 0xff) <= SIN_MID[y] ) return ( n + 0xff00 + SIN_MID[y+1]);
            
            if ( y + 3 > 256 ) 
                return ( n + 0xff00 + SIN_HI[1]);
            
            return ( n + 0xff00 + SIN_MID[y+3]);
        }

        y = (( n & 0xfff0 ) - 0x2c00) >> 3;
        
        if ( (n & 0xff) <= SIN_LO[y] ) return ( n + 0xff00 + SIN_LO[y+1]);
        return ( n + 0xff00 + SIN_LO[y+3]);
        
        if ( n >= 0x3400 ) return DOUBLE_TO_X_OPT( 0.999999986*red - 0.166666367*pow(red,3) + 0.008331584*pow(red,5) - 0.000194621*pow(red,7) );
/*
 

0x33e8	7e 8 n-21	21	13288	34	-1
0x33c6	7c 6 n-20	20	13254	35	-1
0x33a3	7a 3 n-19	19	13219	36	-1
0x337f	77 f n-18	18	13183	37	-2
0x335a	75 a n-17	17	13146	39	-1
0x3333	73 3 n-16	16	13107	40	-3
0x330b	70 b n-15	15	13067	43	-1
0x32e0	6e 0 n-14	14	13024	44	-4
0x32b4	6b 4 n-13	13	12980	48	-1
0x3284	68 4 n-12	12	12932	49	-5
0x3253	65 3 n-11	11	12883	54	-3
0x321d	61 d n-10	10	12829	57	-4
0x31e4	5e 4 n-9	9	12772	61	-7
0x31a7	5a 7 n-8	8	12711	68	-6
0x3163	56 3 n-7	7	12643	74	-11
0x3119	51 9 n-6	6	12569	85	-13
0x30c4	4c 4 n-5	5	12484	98	2
0x3062	46 2 n-4	4	12386	96	95
0x3002	40 2 n-3	3	12290	1	-112  !!!
0x3001	40 1 n-4	4	12289	113	-42
0x2f90	39 0 n-5	5	12176	155	-34
0x2ef5	2f 5 n-4	4	12021	189	-61
0x2e38	23 8 n-3	3	11832	250	-68
0x2d3e	13 e n-2	2	11582	318	
0x2c00	00 0 n-1	1	11264		
 
 */
        if ( n >= 0x33e8 ) return n-21;
        if ( n >= 0x33c6 ) return n-20;
        if ( n >= 0x33a3 ) return n-19;
        if ( n >= 0x337f ) return n-18;
        if ( n >= 0x335a ) return n-17;
        if ( n >= 0x3333 ) return n-16;
        if ( n >= 0x330b ) return n-15;
        
        if ( n >= 0x32e0 ) return n-14;
        if ( n >= 0x32b4 ) return n-13;
        if ( n >= 0x3284 ) return n-12;
        if ( n >= 0x3253 ) return n-11;
        if ( n >= 0x321d ) return n-10;
        
        if ( n >= 0x31e4 ) return n-9;
        if ( n >= 0x31a7 ) return n-8;
        if ( n >= 0x3163 ) return n-7;
        if ( n >= 0x3119 ) return n-6;
        
        if ( n >= 0x30c4 ) return n-5;
        if ( n >= 0x3062 ) return n-4;
        if ( n >= 0x3002 ) return n-3;
        if ( n >= 0x3001 ) return n-4;
        
        if ( n >= 0x2f90 ) return n-5;
        
        if ( n >= 0x2ef5 ) return n-4;
        if ( n >= 0x2e38 ) return n-3;
        
        if ( n >= 0x2d3e ) return n-2;
        
        if ( n >= 0x2c00 ) return n-1;
        return n;
#else
    } else {
#endif
        r = num - X_TO_DOUBLE(DOUBLE_TO_X_OPT( red ));
        r = 0.999999986*red - 0.166666367*pow(red,3) + 0.008331584*pow(red,5) - 0.000194621*pow(red,7);
    }
    return DOUBLE_TO_X_OPT(r);
}




void print256_tab(int * tab, int idx, int posun, int stop)
{
#if ASM
    printf(";   _0  _1  _2  _3  _4  _5  _6  _7  _8  _9  _A  _B  _C  _D  _E  _F\n");
#else
    printf("//_0   _1   _2   _3   _4   _5   _6   _7   _8   _9   _A   _B   _C   _D   _E   _F\n");
#endif
    for ( int i = 0; i < 256 && i < stop; i += 16 ) {
#if ASM
        printf("db ");
#endif
        for ( int j = i; j < i + 16 && j < stop; j++ ) {
            if (j != i ) putchar(',');
#if ASM
            printf("$%02x", (tab[idx+j] >> posun) & 0xFF);
        }
        printf("   ; %X_  ", (i >> 4) & 0xF );
#else
            printf("0x%02x", (tab[idx+j] >> posun) & 0xFF);
        }
        if ((i & 0xF0) == 0xF0) 
            printf("}; // %X_  ", (i >> 4) & 0xF );
        else
            printf(",  // %X_  ", (i >> 4) & 0xF );
#endif

        if ( ! posun ) {
            printf("   " );
            for ( int j = i; j < i + 16 && j < stop; j++ ) {
                if (j != i ) putchar(',');
                printf("%03x", (tab[idx+j]));
            }
            printf(" %X_", (i >> 4) & 0xF );
        }

        printf("\n");
    }
}



void print256word_tab(int * tab, int idx, int stop)
{
    printf(";   _0    _1    _2    _3    _4    _5    _6    _7    _8    _9    _A    _B    _C    _D    _E    _F\n");

    for ( int i = 0; i < 256 && i < stop; i += 16 ) {
        printf("dw ");
        for ( int j = i; j < i + 16 && j < stop; j++ ) {
            if (j != i ) putchar(',');
            printf("$%04x", (tab[idx+j]) & 0xFFFF);
        }
        printf("   ; %X_  ", (i >> 4) & 0xF );
        printf("\n");
    }
}


int min_value(int nas, int min) 
{
    int res, dif;

    min--;      // vcetne predchozi bunky
    res = min - DOUBLE_TO_X_OPT(sin(X_TO_DOUBLE(min)));

    for ( int i = 0; i < nas; i++ ) 
    {
        min++;
        dif = min - DOUBLE_TO_X_OPT(sin(X_TO_DOUBLE(min)));
        if ( res > dif ) res = dif;
    }
    
    return res;
}



int max_value(int nas, int min) 
{
    int res, dif;
    
    min--;      // vcetne predchozi bunky
    res = min - DOUBLE_TO_X_OPT(sin(X_TO_DOUBLE(min)));

    for ( int i = 0; i < nas; i++ ) 
    {
        min++;
        dif = min - DOUBLE_TO_X_OPT(sin(X_TO_DOUBLE(min)));
        if ( res < dif ) res = dif;
    }
    
    return res;
}



int copy_tab(int size_tab, int * best_tab, int * new_tab)
{
    for ( int i = 0; i < size_tab; i++ )
        best_tab[i] = new_tab[i];
}


void show_tab(int bunek, int * tab, char * komentar) 
{
    printf("%s: ", komentar);
    for ( int i = 0; i < bunek; i++ )
    {
        printf(" %02i", tab[i]);
    }
    putchar('\n');
}



void optimize_dat(int nas, int min, int dohled, int * tab, int first_value, int first_predel) {

    int bunek = 2 + dohled; 
        
    int new_tab[bunek];
    int min_tab[bunek];
    int max_tab[bunek];

    int new_predel[bunek];
    int min_predel[bunek];
    int max_predel[bunek];

    int best_tab[bunek];
    int best_predel[bunek];

    
    // init tab     
    int in = min + nas;
    min_tab[0] = first_value;
    max_tab[0] = first_value;
    min_predel[0] = first_predel;
    max_predel[0] = first_predel;
    
    for (int i = 1; i < bunek; i++)
    {
        min_tab[i] = min_value(nas, in);
        max_tab[i] = max_value(nas, in);
        
        min_predel[i] = in & 0xff;
        if (min_predel[i]) min_predel[i]--;
        max_predel[i] = (in + nas - 1) & 0xff;
        
        in += nas;
    }
    copy_tab( bunek, new_tab, min_tab );
    copy_tab( bunek, new_predel, min_predel );

#if TISK
    show_tab(bunek, min_tab, "min_tab");
    show_tab(bunek, max_tab, "max_tab");
    show_tab(bunek, min_predel, "min_predel");
    show_tab(bunek, max_predel, "max_predel");
#endif

    long best = -1, best_posun, sum, posun;
    
    while ( 1 )
    {
        // odchylky
        
        sum = 0;
        posun = 0;
        in = min;
        int j;
        
        for (int i = 0; i < bunek - 1; i++)
        {
            for (int n = 0; n < nas; n++)
            {
                int dif = in - DOUBLE_TO_X_OPT(sin(X_TO_DOUBLE(in)));
                
                if ( (in & 0xff) <= new_predel[i] ) 
                    j = dif - new_tab[i];
                else
                {
                    posun++;
                    j = dif - new_tab[i+1];
                }
                sum += 2*j*j;
                in++;
            }
        }

        if ( best < 0 ) best = sum + 1;

        if ( best >= sum ) {
            
            if ( best > sum || best_posun > posun ) {
                best = sum;
                best_posun = posun;
                
                copy_tab( bunek, best_tab, new_tab );
                copy_tab( bunek, best_predel, new_predel );
                
#if TISK2
    show_tab(bunek, best_tab, "best_tab");
    show_tab(bunek, best_predel, "best_predel");
    printf("best: %li\n", best);
    printf("best_posun: %li\n", best_posun);
#endif
            }
        }
        
        
        int x = 1;
        
        while ( ++new_tab[x] > max_tab[x] ) {
            new_tab[x] = min_tab[x];
            x++;
            if ( x >= bunek ) break;
        }
#if TISK
    show_tab(bunek, new_tab, "new_tab");
#endif
        if ( x < bunek ) continue;
        
        x = 1;
        
        while ( ++(new_predel[x]) > max_predel[x] ) {
            new_predel[x] = min_predel[x];
            x++;
            if ( x >= bunek ) break;
        }
#if TISK  
    show_tab(bunek, new_predel, "new_predel");
#endif
        if ( x >= bunek ) break;
        
    }
    
#if TISK      
    printf("best: %li\n", best);
    printf("best_posun: %li\n", best_posun);
#endif

    tab[0] = best_predel[1]; 
    tab[1] = best_tab[1];
    
}


int init_table(char * name, int nas, int min, int * tab, int first_value, int first_predel) {

    int i = 0;
    if ( nas >= 32 ) i++;
    if ( nas >= 16 ) i++;
    if ( nas >= 8  ) i++;
    if ( nas >= 4  ) i++;

#ifdef ASM
    printf("; X = $%04x..$%04x\n; Y = ((X & $%04x)-$%04x) >> %i = $00..$fe\n", min, min+128*nas-1,0x10000-nas, min, i);
    printf("; if ( (X & 0xff) <= SIN_LO[Y] ) { sin(X) = X + $ff00 + SIN_LO[Y+1] }\n");
    printf("; if ( (X & 0xff) >  SIN_LO[Y] ) { sin(X) = X + $ff00 + SIN_LO[Y+3] }\n");
    printf("%s:\n", name);
#else
    printf("// X = $%04x..$%04x\n// Y = ((X & $%04x)-$%04x) >> %i = $00..$fe\n", min, min+128*nas-1,0x10000-nas, min, i);
    printf("// if ( (X & 0xff) <= SIN_LO[Y] ) { sin(X) = X + $ff00 + SIN_LO[Y+1] }\n");
    printf("// if ( (X & 0xff) >  SIN_LO[Y] ) { sin(X) = X + $ff00 + SIN_LO[Y+3] }\n");
    printf("unsigned char %s[] = {\n", name);
#endif
//     int little[10];
    int dohled = 3;
    
    int lokal[2];
    
    int value = first_value;
    int predel = first_predel;
    
    for ( int i = 0; i < 128; i++ )
    {
        optimize_dat( nas, min + (i-1) * nas, dohled, lokal, value, predel);
        predel = lokal[0];
        value  = lokal[1];
        tab[2*i+0] = lokal[0];
        tab[2*i+1] = (0x100 - lokal[1]) & 0xff;
    }

}


/*
int init_table(int nas, int min, int * tab) {
    
//     16=>3   8-4-2-1
//     8=>2      4-2-1
//     4=>1        2-1
    
    int i = 0;
    if ( nas >= 32 ) i++;
    if ( nas >= 16 ) i++;
    if ( nas >= 8  ) i++;
    if ( nas >= 4  ) i++;
    
    printf("; X = $%04x..$%04x\n; Y = ((X & $%04x)-$%04x) >> %i = $00..$fe\n", min, min+128*nas-1,0x10000-nas, min, i);
    printf("; if ( (X & 0xff) <= SIN_LO[Y] ) { sin(X) = X + $ff00 + SIN_LO[Y+1] }\n");
    printf("; if ( (X & 0xff) >  SIN_LO[Y] ) { sin(X) = X + $ff00 + SIN_LO[Y+3] }\n");
    
    __uint16_t dif_min, dif_max, dif_next, dif_last, dif;

    dif_last = min - 1 - DOUBLE_TO_X_OPT(sin(X_TO_DOUBLE(min - 1)));    
    dif_next = min - DOUBLE_TO_X_OPT(sin(X_TO_DOUBLE(min)));

    for ( i = 0; i < 128; i++ )
    {
        __uint16_t in = min + i * nas;
                        
        dif_min  = dif_next;
        dif_max  = in + nas - 1 - DOUBLE_TO_X_OPT(sin(X_TO_DOUBLE(in + nas - 1)));
        dif_next = in + nas     - DOUBLE_TO_X_OPT(sin(X_TO_DOUBLE(in + nas)));
            
        tab[2*i+1] = -dif_min & 0xff;

        int sum = 0;
//         int now  = next_cell(nas, in);
//         int next = next_cell(nas, in + nas);
        int dif_last = dif_min;

        for ( int poc = 0; poc < nas; poc++ ) 
        {
            int j = in + poc;
            
            if ( j == 0x3400 ) {
                tab[2*i+1] = -0x13 & 0xff;
                tab[2*i] = 0x05;
                break;
            }
            if ( j == 0x3408 ) {
                tab[2*i+1] = -0x0e & 0xff;
                tab[2*i] = 0x09;
                break;
            }

            dif = j - DOUBLE_TO_X_OPT(sin(X_TO_DOUBLE(j)));
            
            if ( dif != dif_last) {
                printf("; sin($%04x) = $%04x == $%04x - %i", j, DOUBLE_TO_X_OPT(sin(X_TO_DOUBLE(j))), j, dif);
                if ( dif != now)
                    printf(" Error %i %i!!!", now, dif);
                
                printf("\n");
            }
            
            if ( abs(dif - now) < abs(dif - next) ) {
                tab[2*i+1] = (-( sum + ( poc + 1 )/2) / ( poc + 1 )) & 0xff;
                sum += dif;
                tab[2*i]   = j;
            }
            dif_last = dif;
        }
    }

}

*/
int main( int argc, const char* argv[] ) 
{

#ifdef TEST
    
    int rozdil_1 = 0, rozdil_2 = 0, rozdil_3 = 0, rozdil_4 = 0;
    int chyb = 0;
    int sum = 0;

    
    int sign = 0;
    
    for (int e = 0; e <= (MASK_EXP >> EXP_POS); e++ ) 
    for (int m = 0; m <= MASK_MANTISA; m++ ) {
        
        if ( e - BIAS > 0 ) break;
                
        __uint16_t s = MAKE_X( sign, e-BIAS, m );
        
// if ( s >= 0x3400 ) break;


        double in = X_TO_DOUBLE( s );

        if ( in > 2*0.7853981633974483096156608) break;
if ( m == 0) printf("$%02x\n", 4*e);
        
        sum++;

        double f = sin(in);
        __uint16_t r = DOUBLE_TO_X_OPT(f);
        __uint16_t x = test_sin(e,m);

        
        if ( r != x ) {
            
            __uint16_t xp1 = inc_x(x);
            __uint16_t xm1 = dec_x(x);
            __uint16_t xp2 = inc_x(xp1);
            __uint16_t xm2 = dec_x(xm1);
            __uint16_t xp3 = inc_x(xp2);
            __uint16_t xm3 = dec_x(xm2);
            __uint16_t xp4 = inc_x(xp3);
            __uint16_t xm4 = dec_x(xm3);
 
                                   
            if ( r == xp1 || r == xm1 ) {
                if ( 1 || rozdil_1 == 0 /*|| rozdil_2 || rozdil_3*/) {
                    printf("e: %i = $%02x (%i), m: %04x (%i), sin(%f)\n", e-BIAS, e, e, m, m, in);
                    printf("; sin($%04x) = $%04x, e: %i, m: $%04x, má být: $%04x, jest: $%04x, rozdíl: %i\n", s, r, e-BIAS, m, r, x, r-x);
                }
                rozdil_1++;
            }           
            else if ( r == xp2 || r == xm2) {
                if ( 1 || rozdil_2 == 0 /*|| rozdil_3*/) {
                    printf("e: %i = $%02x (%i), m: %04x (%i), sin(%f)\n", e-BIAS, e, e, m, m, in);
                    printf("; sin($%04x) = $%04x, e: %i, m: $%04x, má být: $%04x, jest: $%04x, rozdíl: %i\n", s, r, e-BIAS, m, r, x, r-x);
                }
                rozdil_2++;
            }
            else if ( r == xp3 || r == xm3 ) {
                if ( 1 ||  rozdil_3 == 0 ) {
                    printf("e: %i = $%02x (%i), m: %04x (%i), sin(%f)\n", e-BIAS, e, e, m, m, in);
                    printf("; sin($%04x) = $%04x, e: %i, m: $%04x, má být: $%04x, jest: $%04x, rozdíl: %i\n", s, r, e-BIAS, m, r, x, r-x);
                }
                rozdil_3++;
            }
            else if ( r == xp4 || r == xm4 ) {
                if ( 1 ||  rozdil_4 == 0 ) {
                    printf("e: %i = $%02x (%i), m: %04x (%i), sin(%f)\n", e-BIAS, e, e, m, m, in);
                    printf("; sin($%04x) = $%04x, e: %i, m: $%04x, má být: $%04x, jest: $%04x, rozdíl: %i\n", s, r, e-BIAS, m, r, x, r-x);
                }
                rozdil_4++;
            }
            else {  
                    printf("e: %i = $%02x (%i), m: %04x (%i), sin(%f)\n", e-BIAS, e, e, m, m, in);
                printf("; sin($%04x) = $%04x, e: %i, m: $%04x, má být: $%04x, jest: $%04x, rozdíl: %i\n", s, r, e-BIAS, m, r, x, r-x);
                chyb++;
            }           
        }
        
    }
    
    printf("\n; rozdíl: má být mínus jest, nemohu se splést...\n");
    printf("; nepřesnost o 1: %i (%.3f%%)\n", rozdil_1, (100.0 * rozdil_1 )/ sum);
    printf("; nepřesnost o 2: %i (%.3f%%)\n", rozdil_2, (100.0 * rozdil_2 )/ sum);
    printf("; nepřesnost o 3: %i (%.3f%%)\n", rozdil_3, (100.0 * rozdil_3 )/ sum);
    printf("; nepřesnost o 4: %i (%.3f%%)\n", rozdil_4, (100.0 * rozdil_4 )/ sum);
    printf(";       chyb: %i (%.3f%%)\n", chyb, (100.0 * chyb )/ sum);
#if 0

        __uint16_t x = test2_exp(sign,e-BIAS, m);

        
        if ( x != r ) {
            __uint16_t xp1 = inc_x(x);
            __uint16_t xm1 = dec_x(x);
            __uint16_t xp2 = inc_x(xp1);
            __uint16_t xm2 = dec_x(xm1);
            __uint16_t xp3 = inc_x(xp2);
            __uint16_t xm3 = dec_x(xm2);
        
            int dif = (r & (MASK_MANTISA + MASK_EXP)) - (x & (MASK_MANTISA + MASK_EXP)); 
            if (dif >  MASK_SIGN) dif -= MASK_SIGN;
            if (dif < -MASK_SIGN) dif += MASK_SIGN;
        
            if (( r & MASK_SIGN ) != ( x & MASK_SIGN)) {
                chyb++;
                printf("; sin($%04x) = $%04x, e: %i, m: $%04x, má být: $%04x, jest: $%04x, rozdíl: %i, opačná znaménka!\n", s, r, e-BIAS, m, r, x, dif);                
            }
            else if ( r == xp1 || r == xm1 ) rozdil_1++;
            else if ( r == xp2 || r == xm2 ) rozdil_2++;
            else if ( r == xp3 || r == xm3 ) {
                rozdil_3++;
                printf("; sin($%04x) = $%04x, e: %i, m: $%04x, má být: $%04x, jest: $%04x, rozdíl: %i\n", s, r, e-BIAS, m, r, x, dif);
            }
            else {
                chyb++;
                printf("; sin($%04x) = $%04x, e: %i, m: $%04x, má být: $%04x, jest: $%04x, rozdíl: %i\n", s, r, e-BIAS, m, r, x, dif);
            }
        }
    }

    printf("\n; rozdíl: má být mínus jest, nemohu se splést...\n");
    printf("; nepřesnost o 1: %i (%.3f%%)\n", rozdil_1, (100.0 * rozdil_1 )/ sum);
    printf("; nepřesnost o 2: %i (%.3f%%)\n", rozdil_2, (100.0 * rozdil_2 )/ sum);
    printf("; nepřesnost o 3: %i (%.3f%%)\n", rozdil_3, (100.0 * rozdil_3 )/ sum);
    printf(";       chyb: %i (%.3f%%)\n", chyb, (100.0 * chyb )/ sum);
    
#endif

#else
    #if MAX_NUMBER == 127
        int tab[256];
        int i;

        for ( i = 0; i <= 255; i++ )
        {
            __uint16_t s;
            double f;
            if ( i < 0x70 ) f = pow(lud,pow(2,0x70 + i-BIAS));
            else f = pow(lud,-pow(2,-0x10 + i-BIAS));
            s = DOUBLE_TO_X_OPT( f );
            tab[i] = s;
        }

        printf("EXP_TAB:\n; lo plus,  exp-$70\n");
        print256_tab(tab, 0, 0, 0x70 );
        printf("; lo minus, exp-$70+$80\n");
        print256_tab(tab, 0x70, 0, 256 - 0x70);
        printf("; hi plus,  exp-$70\n");
        print256_tab(tab, 0, 8, 0x70 );
        printf("; hi minus, exp-$70+$80\n");        
        print256_tab(tab, 0x70, 8, 256 - 0x70);
        

    #elif MAX_NUMBER == 255

        int tab[256];
        int i;

        for ( i = 0; i <= 255; i++ )
        {
            __uint16_t s;
            double f;
            if ( i < 0x80 ) f = pow(lud,pow(2,i-BIAS));
            else f = pow(lud,-pow(2,i-0x80-BIAS));
            s = DOUBLE_TO_X_OPT( f );
            tab[i] = s;
        }

        printf("EXP_TAB:\n; plus\n");
        print256word_tab(tab, 0, 0x80 );
        printf("; minus\n");
        print256word_tab(tab, 0x80, 256 - 0x80);


    #elif MAX_NUMBER == 1023
        int tab[256];

        int min = 0x2c00;
        int nas = 16;
        init_table("SIN_LO",nas, min, tab, min, 0);
        
        print256_tab(tab, 0, 0, 256 );
           
        min = 0x3400;
        nas = 8;
        init_table("SIN_MID",nas, min, tab, tab[254], tab[255]);
        print256_tab(tab, 0, 0, 256 );

        min = 0x3800;
        nas = 4;
        init_table("SIN_HI",nas, min, tab, tab[254], tab[255]);
        print256_tab(tab, 0, 0, 256 );
        
        for ( int j = 0x3a; j <= 0x3e; j++ ) {
            
            for ( int i = 0; i <= 255; i++ )
            {
                __uint16_t m = j*256+i;
                __uint16_t n = DOUBLE_TO_X_OPT(sin(X_TO_DOUBLE( m )));
                if ( j == 0x3a || j == 0x3c || j == 0x3e ) tab[i] = 0xffff & (n - m);
                else tab[i] = n;
            }
            printf("; hi = $%02x\nSIN_%02x:\n", tab[0] >> 8, j);    
            print256_tab(tab, 0, 0, 256);
//             print256_tab(tab, 0, 8, 256);
        }
        
    #else
        #error   Neocekavana hodnota MAX_NUMBER!
    #endif

#endif



#define SKOK   16

// SKOK 16 2c00..39ff shodna: 16*147, sum: 224, original: 3584, new: 2576
// SKOK  8 2c00..39ff shodna:  8*226, sum: 448, original: 3584, new: 2256
// SKOK  4 2c00..39ff shodna:  4*319, sum: 896, original: 3584, new: 2172

    int data[4000][SKOK];
    int kolik = 0;
    int in;
    sum = 0;
    int start = 0x2c00;
    int end   = 0x39ff;
    
    for ( int i = start; i <= end; i+= SKOK ) 
    {
        sum++;
        
        int new[SKOK];
        
        for ( int j = 0; j < SKOK; j++ ) 
        {
            in = i + j;
            new[j] =  in - DOUBLE_TO_X_OPT(sin(X_TO_DOUBLE(in)));
        }
        
        int shoda = 0;

        for ( int k = 0; k < kolik; k++ )
        {
            
            for ( int j = 0; j < SKOK; j++ ) 
            {
                if ( new[j] != data[k][j] ) break;
                if ( j == SKOK - 1 ) shoda = k+1;
            }

            if ( shoda ) break;            
        }
        
        if ( shoda == 0 ) 
        {
            kolik++;
            for ( int j = 0; j < SKOK; j++ ) 
            {
                data[kolik][j] = new[j];
            }
        }
    }
    
    printf("// SKOK %i %04x..%04x shodna: %i*%i, sum: %i, original: %i, new: %i\n", SKOK, start, end, SKOK, kolik, sum, sum*SKOK, SKOK*kolik+sum);

#if 0
    for ( int i = 0x33f8; i < 0x3418; i++ ) 
    {
        __uint16_t s = DOUBLE_TO_X_OPT(sin(X_TO_DOUBLE(i)));
        __uint16_t dif = i-s;
        if ((i & 0x7) == 0  ) printf("\n"); 
        printf("sin($%04x) = $%04x ($%04x)\n", i, s, dif);
    }

    printf("\n");
    
    for ( int i = 0x3820; i < 0x3834; i++ ) 
    {
        __uint16_t s = DOUBLE_TO_X_OPT(sin(X_TO_DOUBLE(i)));
        __uint16_t dif = i-s;
        if ((i & 0x3) == 0  ) printf("\n"); 
        printf("sin($%04x) = $%04x ($%04x)\n", i, s, dif);
    }
#endif
    
/*
sin($3820) = $37e4 ($003c)   0
sin($3821) = $37e5 ($003c)   0
sin($3822) = $37e7 ($003b)  -1
sin($3823) = $37e9 ($003a)     +1

sin($3824) = $37eb ($0039)   0
sin($3825) = $37ec ($0039)   0
sin($3826) = $37ee ($0038)  -1
sin($3827) = $37f0 ($0037)     +1

sin($3828) = $37f2 ($0036)   0
sin($3829) = $37f3 ($0036)   0
sin($382a) = $37f5 ($0035)  -1
sin($382b) = $37f7 ($0034)     +1

sin($382c) = $37f8 ($0034)  +1
sin($382d) = $37fa ($0033)   0
sin($382e) = $37fc ($0032)  -1
sin($382f) = $37fe ($0031)      0

sin($3830) = $37ff ($0031)      0
sin($3831) = $3801 ($0030)     -1
sin($3832) = $3801 ($0031)      0
sin($3833) = $3802 ($0031)      0
 */

}
