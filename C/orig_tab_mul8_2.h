#define MAX_NUMBER 255
#define TOP_BIT 0x8000
#define PRICTI 0x3F
#define POSUN_VPRAVO 7
#define POCET_BITU 8

; // 1000 0000  0... ....  
; //              11 1111  
; //  765 4321  0... ....  

; // neni presne: 0 (0.000000%), preteceni: 0, sum(tab_dif): -134, sum(abs(tab_diff)): 196
; // (( 256 + a ) * ( 256 + b )) >> 6 = (tab_plus[a+b] - tab_minus[a-b]) >> 6 = (1m mmmm mmm.) OR (01 mmmm mmmm)
; // 0 <= a <= 255, 0 <= b <= 255

; // tab_minus[i] = (i*i)/4 +- 0x2
int tab_minus[MAX_NUMBER+1] = {  // [0..255]
0x0000,0x0000,0x0001,0x0000,0x0002,0x0001,0x0003,0x0003,0x0004,0x0005,0x0006,0x0008,0x0009,0x000B,0x000C,0x000F,
0x000F,0x0012,0x0014,0x0015,0x0019,0x001C,0x001E,0x0020,0x0024,0x0027,0x002A,0x002C,0x0031,0x0035,0x0038,0x003C,
0x0040,0x0044,0x0048,0x004E,0x0051,0x0054,0x005A,0x005E,0x0064,0x0069,0x006E,0x0074,0x0079,0x007F,0x0085,0x008B,
0x008F,0x0095,0x009C,0x00A2,0x00A9,0x00AF,0x00B6,0x00BD,0x00C4,0x00CC,0x00D2,0x00DA,0x00E1,0x00E9,0x00F0,0x00F8,
0x0100,0x0109,0x0110,0x0118,0x0121,0x012A,0x0132,0x013B,0x0144,0x014F,0x0156,0x015F,0x0169,0x0173,0x017C,0x0187,
0x018F,0x019B,0x01A5,0x01AD,0x01B9,0x01C4,0x01CE,0x01D9,0x01E4,0x01EE,0x01FA,0x0206,0x0211,0x021D,0x0228,0x0235,
0x0240,0x024E,0x0258,0x0264,0x0271,0x027E,0x028A,0x0296,0x02A4,0x02B1,0x02BE,0x02CD,0x02D9,0x02E6,0x02F4,0x0302,
0x030F,0x031E,0x032C,0x033A,0x0349,0x0357,0x0366,0x0376,0x0384,0x0393,0x03A2,0x03B2,0x03C2,0x03D1,0x03E0,0x03F0,
0x0400,0x0410,0x0420,0x0430,0x0442,0x0452,0x0462,0x0473,0x0484,0x0494,0x04A6,0x04B6,0x04C9,0x04DC,0x04EC,0x04FE,
0x050F,0x0521,0x0534,0x0547,0x0559,0x056B,0x057E,0x0591,0x05A4,0x05B7,0x05CA,0x05DD,0x05F1,0x0604,0x0618,0x062B,
0x0640,0x0653,0x0668,0x067C,0x0691,0x06A5,0x06BA,0x06CF,0x06E4,0x06F9,0x070E,0x0723,0x0739,0x074F,0x0765,0x077A,
0x078F,0x07A6,0x07BC,0x07D2,0x07E9,0x07FF,0x0816,0x082C,0x0844,0x085B,0x0872,0x088A,0x08A1,0x08B8,0x08D0,0x08E8,
0x0900,0x0918,0x0930,0x0948,0x0961,0x0979,0x0992,0x09AB,0x09C4,0x09DD,0x09F6,0x0A0F,0x0A29,0x0A42,0x0A5C,0x0A77,
0x0A90,0x0AAA,0x0AC4,0x0ADE,0x0AF9,0x0B13,0x0B2E,0x0B49,0x0B64,0x0B7F,0x0B9A,0x0BB5,0x0BD1,0x0BEC,0x0C08,0x0C24,
0x0C40,0x0C5C,0x0C78,0x0C94,0x0CB1,0x0CCC,0x0CEA,0x0D07,0x0D24,0x0D41,0x0D5E,0x0D7B,0x0D99,0x0DB6,0x0DD4,0x0DF2,
0x0E10,0x0E2E,0x0E4C,0x0E6A,0x0E89,0x0EA7,0x0EC6,0x0EE5,0x0F04,0x0F23,0x0F42,0x0F61,0x0F81,0x0FA0,0x0FC0,0x0FE0};

; // tab_plus[i] = ((256 + 256 + i) * (256 + 256 + i))/4 +- 0x3
int tab_plus[MAX_NUMBER+MAX_NUMBER+1] = {  // [0..510]
0x4000,0x4040,0x4080,0x40C0,0x4101,0x4141,0x4182,0x41C3,0x4204,0x4245,0x4286,0x42C7,0x4309,0x434A,0x438C,0x43CE,
0x4410,0x4452,0x4494,0x44D6,0x4519,0x455B,0x459E,0x45E1,0x4624,0x4667,0x46AA,0x46ED,0x4731,0x4774,0x47B8,0x47FD,
0x4840,0x4884,0x48C8,0x490C,0x4951,0x4996,0x49DA,0x4A1F,0x4A64,0x4AA9,0x4AEE,0x4B33,0x4B79,0x4BBE,0x4C05,0x4C4A,
0x4C90,0x4CD6,0x4D1C,0x4D62,0x4DA9,0x4DF0,0x4E36,0x4E7D,0x4EC4,0x4F0B,0x4F52,0x4F99,0x4FE2,0x5029,0x5070,0x50B8,
0x5100,0x5148,0x5190,0x51D9,0x5222,0x526A,0x52B2,0x52FC,0x5344,0x538C,0x53D6,0x5420,0x5469,0x54B3,0x54FC,0x5546,
0x5590,0x55DA,0x5625,0x5670,0x56B9,0x5703,0x574E,0x5799,0x57E4,0x5830,0x587A,0x58C5,0x5911,0x595D,0x59A8,0x59F4,
0x5A40,0x5A8C,0x5AD8,0x5B25,0x5B71,0x5BBE,0x5C0A,0x5C57,0x5CA4,0x5CF1,0x5D3E,0x5D8B,0x5DD9,0x5E28,0x5E74,0x5EC2,
0x5F10,0x5F5F,0x5FAC,0x5FFC,0x6049,0x6097,0x60E6,0x6134,0x6184,0x61D4,0x6223,0x6271,0x62C1,0x6310,0x6361,0x63B0,
0x6400,0x6450,0x64A1,0x64F1,0x6541,0x6592,0x65E3,0x6633,0x6683,0x66D6,0x6726,0x6777,0x67C9,0x681A,0x686C,0x68BE,
0x6910,0x6962,0x69B4,0x6A06,0x6A59,0x6AAD,0x6AFE,0x6B51,0x6BA4,0x6BF6,0x6C4A,0x6C9E,0x6CF1,0x6D44,0x6D98,0x6DEE,
0x6E40,0x6E95,0x6EE8,0x6F3D,0x6F91,0x6FE6,0x703A,0x708E,0x70E4,0x7139,0x718E,0x71E4,0x7239,0x728D,0x72E5,0x733B,
0x7390,0x73E7,0x743C,0x7493,0x74E9,0x753F,0x7596,0x75EF,0x7644,0x769B,0x76F2,0x774A,0x77A2,0x77F8,0x7850,0x78A9,
0x7900,0x7958,0x79B0,0x7A09,0x7A62,0x7ABA,0x7B12,0x7B6C,0x7BC4,0x7C1D,0x7C76,0x7CCF,0x7D29,0x7D82,0x7DDC,0x7E35,
0x7E90,0x7EEB,0x7F45,0x7F9F,0x7FF9,0x8054,0x80AE,0x8109,0x8164,0x81C0,0x821A,0x8276,0x82D1,0x832C,0x8388,0x83E4,
0x8440,0x849C,0x84F8,0x8554,0x85B1,0x8610,0x866A,0x86C8,0x8724,0x8781,0x87DE,0x883C,0x8899,0x88F6,0x8954,0x89B2,
0x8A10,0x8A6D,0x8ACC,0x8B2A,0x8B89,0x8BE8,0x8C46,0x8CA5,0x8D04,0x8D63,0x8DC3,0x8E21,0x8E81,0x8EE0,0x8F41,0x8FA0,
0x9000,0x9060,0x90C1,0x9120,0x9181,0x91E1,0x9243,0x92A3,0x9304,0x9365,0x93C6,0x9427,0x9489,0x94EB,0x954C,0x95AE,
0x9610,0x9673,0x96D4,0x9738,0x9799,0x97FC,0x985E,0x98C1,0x9924,0x9988,0x99EA,0x9A4D,0x9AB1,0x9B14,0x9B78,0x9BDD,
0x9C40,0x9CA4,0x9D08,0x9D6C,0x9DD1,0x9E37,0x9E9A,0x9EFF,0x9F64,0x9FCA,0xA02E,0xA093,0xA0F9,0xA15F,0xA1C5,0xA22A,
0xA290,0xA2F6,0xA35C,0xA3C3,0xA429,0xA490,0xA4F6,0xA55E,0xA5C4,0xA62B,0xA692,0xA6F9,0xA761,0xA7CA,0xA830,0xA899,
0xA900,0xA968,0xA9D0,0xAA39,0xAAA1,0xAB09,0xAB72,0xABDC,0xAC44,0xACAD,0xAD16,0xAD7F,0xADE9,0xAE53,0xAEBC,0xAF26,
0xAF90,0xAFFA,0xB065,0xB0D0,0xB139,0xB1A3,0xB20E,0xB279,0xB2E4,0xB350,0xB3BA,0xB425,0xB491,0xB4FD,0xB568,0xB5D4,
0xB640,0xB6AC,0xB718,0xB785,0xB7F1,0xB85E,0xB8CA,0xB937,0xB9A4,0xBA12,0xBA7E,0xBAEB,0xBB59,0xBBC8,0xBC34,0xBCA2,
0xBD10,0xBD7E,0xBDEC,0xBE5C,0xBEC9,0xBF37,0xBFA6,0xC014,0xC084,0xC0F3,0xC162,0xC1D1,0xC242,0xC2B1,0xC320,0xC390,
0xC400,0xC470,0xC4E0,0xC550,0xC5C2,0xC631,0xC6A2,0xC713,0xC784,0xC7F6,0xC866,0xC8D8,0xC949,0xC9BA,0xCA2C,0xCA9E,
0xCB10,0xCB82,0xCBF4,0xCC66,0xCCD9,0xCD4C,0xCDBE,0xCE31,0xCEA4,0xCF17,0xCF8A,0xCFFE,0xD071,0xD0E4,0xD158,0xD1CC,
0xD240,0xD2B5,0xD328,0xD39C,0xD411,0xD485,0xD4FA,0xD56F,0xD5E4,0xD65A,0xD6CE,0xD744,0xD7B9,0xD82E,0xD8A5,0xD91C,
0xD990,0xDA06,0xDA7C,0xDAF2,0xDB69,0xDBDF,0xDC56,0xDCCD,0xDD44,0xDDBB,0xDE32,0xDEAA,0xDF21,0xDF98,0xE010,0xE088,
0xE100,0xE178,0xE1F0,0xE268,0xE2E1,0xE359,0xE3D2,0xE44C,0xE4C4,0xE53D,0xE5B6,0xE62F,0xE6A9,0xE722,0xE79C,0xE816,
0xE890,0xE90A,0xE984,0xE9FE,0xEA79,0xEAF3,0xEB6E,0xEBE9,0xEC64,0xECDF,0xED5A,0xEDD5,0xEE51,0xEECC,0xEF48,0xEFC4,
0xF040,0xF0BC,0xF138,0xF1B4,0xF231,0xF2AD,0xF32A,0xF3A7,0xF424,0xF4A1,0xF51E,0xF59B,0xF619,0xF696,0xF714,0xF792,
0xF810,0xF88E,0xF90C,0xF98A,0xFA09,0xFA87,0xFB06,0xFB85,0xFC04,0xFC83,0xFD02,0xFD81,0xFE01,0xFE80,0xFF00};
