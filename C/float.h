#ifndef _FLOAT

#define _FLOAT

#define bfloat16 uint16_t
#define binary16 uint16_t
#define danagy16 uint16_t
#define binary64 uint64_t
#define   single uint32_t

#define ibfloat16 int16_t
#define ibinary16 int16_t
#define idanagy16 int16_t
#define ibinary64 int64_t
#define   isingle int32_t


//                   6            5    4         4         3         2         1         0
//                   3    -11-    2    8         0         2  -52-   4         6         8
// double(binary64): Seee eeee eeee mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm 
#define DO_SIGN_POS         63
#define DO_EXP_POS          52
#define DO_MANT_SIZE        52
//                             8 7 6 5 4 3 2 1 0
#define DO_MASK_MANTISA     0x000fFFffFFffFFffUL
#define DO_MASK_EXP         0x7ff0000000000000L
#define DO_MASK_SIGN        0x8000000000000000UL
#define DO_BIAS             0x3ff

#define GET_DO_SIGN(a)      ( ((*(( binary64 *) &a)) >> DO_SIGN_POS     ) & 1 )
#define GET_DO_POSEXP(a)    ( ((*(( binary64 *) &a)) &  DO_MASK_EXP     ) >> DO_EXP_POS )
#define GET_DO_EXP(a)       (     (ibinary64  )     GET_DO_POSEXP(a)      -  DO_BIAS )
#define GET_DO_MANTISA(a)     ((*(( binary64 *) &a)) &  DO_MASK_MANTISA )

// float(single): Seee eeee emmm mmmm mmmm mmmm mmmm mmmm 
#define FP_SIGN_POS         31
#define FP_EXP_POS          23
#define FP_MANT_SIZE        23
#define FP_MASK_MANTISA     0x007fffff
#define FP_MASK_EXP         0x7f800000
#define FP_MASK_SIGN        0x80000000
#define FP_BIAS             0x7f

#define GET_FP_SIGN(a)      ( ((*((   single *) &a)) & FP_MASK_SIGN    ) >> FP_SIGN_POS)
#define GET_FP_POSEXP(a)    ( ((*((   single *) &a)) & FP_MASK_EXP     ) >> FP_EXP_POS )
#define GET_FP_EXP(a)       (     (  isingle  )    GET_FP_POSEXP(a)      -  FP_BIAS )
#define GET_FP_MANTISA(a)     ((*((   single *) &a)) & FP_MASK_MANTISA )

// bfloat16:                          eeee eeee Smmm mmmm
#define BF_SIGN_POS         7
#define BF_EXP_POS          8
#define BF_MANT_SIZE        7
#define BF_MASK             0xffff
#define BF_MASK_MANTISA     0x007f
#define BF_MASK_EXP         0xff00
#define BF_MASK_SIGN        0x0080
#define BF_BIAS             0x7f

#define GET_BF_SIGN(a)      ( ((*(( bfloat16 *) &a)) & BF_MASK_SIGN    ) >> BF_SIGN_POS)
#define GET_BF_POSEXP(a)    ( ((*(( bfloat16 *) &a)) & BF_MASK_EXP     ) >> BF_EXP_POS )
#define GET_BF_EXP(a)       (     (ibfloat16  )    GET_BF_POSEXP(a)      -  BF_BIAS )
#define GET_BF_MANTISA(a)     ((*(( bfloat16 *) &a)) & BF_MASK_MANTISA )

// binary16:                          Seee eemm mmmm mmmm
#define BI_SIGN_POS         15
#define BI_EXP_POS          10
#define BI_MANT_SIZE        10
#define BI_MASK             0xffff
#define BI_MASK_MANTISA     0x03ff
#define BI_MASK_EXP         0x7C00
#define BI_MASK_SIGN        0x8000
#define BI_BIAS             0x0F

#define GET_BI_SIGN(a)      ( ((*(( binary16 *) &a)) & BI_MASK_SIGN    ) >> BI_SIGN_POS)
#define GET_BI_POSEXP(a)    ( ((*(( binary16 *) &a)) & BI_MASK_EXP     ) >> BI_EXP_POS )
#define GET_BI_EXP(a)       (     (ibinary16  )    GET_BI_POSEXP(a)      -  BI_BIAS )
#define GET_BI_MANTISA(a)     ((*(( binary16 *) &a)) & BI_MASK_MANTISA )

// danagy16:                          Seee eeee mmmm mmmm
#define DA_SIGN_POS         15
#define DA_EXP_POS          8
#define DA_MANT_SIZE        8
#define DA_MASK             0xffff
#define DA_MASK_MANTISA     0x00ff
#define DA_MASK_EXP         0x7F00
#define DA_MASK_SIGN        0x8000
#define DA_BIAS             0x40

#define GET_DA_SIGN(a)      ( ((*(( danagy16 *) &a)) & DA_MASK_SIGN    ) >> DA_SIGN_POS)
#define GET_DA_POSEXP(a)    ( ((*(( danagy16 *) &a)) & DA_MASK_EXP     ) >> DA_EXP_POS )
#define GET_DA_EXP(a)       (     (idanagy16  )    GET_DA_POSEXP(a)      -  DA_BIAS )
#define GET_DA_MANTISA(a)     ((*(( danagy16 *) &a)) & DA_MASK_MANTISA )


#ifdef MAX_NUMBER

#if MAX_NUMBER == 1023
    #define PRINT_FP_TO_X   fprint_fp_to_binary16
    #define FLOAT_TO_X      float_to_binary16
    #define FLOAT_TO_X_OPT  float_to_binary16_opt
    #define X_TO_FLOAT      binary16_to_float
    #define MAKE_X          make_binary16
    
    #define DOUBLE_TO_X_OPT double_to_binary16_opt
    #define X_TO_DOUBLE     binary16_to_double
    
    #define MASK_MANTISA    BI_MASK_MANTISA
    #define MASK_SIGN       BI_MASK_SIGN 
    #define MANT_SIZE       BI_MANT_SIZE
    #define MASK_EXP        BI_MASK_EXP
    #define EXP_POS         BI_EXP_POS
    #define BIAS            BI_BIAS
    #define GET_SIGN        GET_BI_SIGN
    #define GET_EXP         GET_BI_EXP
    #define GET_MANTISA     GET_BI_MANTISA
    #define BYTE_TO_BINARY_PATTERN "%c%c%c%c %c%c%c%c %c%c"
    #define BYTE_TO_BINARY(byte)  \
  (byte & 0x200 ? '1' : '0'), \
  (byte & 0x100 ? '1' : '0'), \
  (byte & 0x080 ? '1' : '0'), \
  (byte & 0x040 ? '1' : '0'), \
  (byte & 0x020 ? '1' : '0'), \
  (byte & 0x010 ? '1' : '0'), \
  (byte & 0x008 ? '1' : '0'), \
  (byte & 0x004 ? '1' : '0'), \
  (byte & 0x002 ? '1' : '0'), \
  (byte & 0x001 ? '1' : '0') 
  
#elif MAX_NUMBER == 127
    #define PRINT_FP_TO_X   fprint_fp_to_bfloat16
    #define PRINT_X         fprint_bfloat16
    #define FLOAT_TO_X      float_to_bfloat16
    #define FLOAT_TO_X_OPT  float_to_bfloat16_opt
    #define X_TO_FLOAT      bfloat16_to_float
    #define MAKE_X          make_bfloat16
  
    #define DOUBLE_TO_X_OPT double_to_bfloat16_opt
    #define X_TO_DOUBLE     bfloat16_to_double

    #define MASK_MANTISA    BF_MASK_MANTISA
    #define MASK_SIGN       BF_MASK_SIGN  
    #define MANT_SIZE       BF_MANT_SIZE
    #define MASK_EXP        BF_MASK_EXP
    #define EXP_POS         BF_EXP_POS
    #define BIAS            BF_BIAS
    #define GET_SIGN        GET_BF_SIGN
    #define GET_EXP         GET_BF_EXP
    #define GET_MANTISA     GET_BF_MANTISA
    #define BYTE_TO_BINARY_PATTERN "%c%c%c%c %c%c%c"
    #define BYTE_TO_BINARY(byte)  \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

#elif MAX_NUMBER == 255
    #define PRINT_FP_TO_X   fprint_fp_to_danagy16
    #define FLOAT_TO_X      float_to_danagy16
    #define FLOAT_TO_X_OPT  float_to_danagy16_opt
    #define X_TO_FLOAT      danagy16_to_float
    #define MAKE_X          make_danagy16

    #define DOUBLE_TO_X_OPT double_to_danagy16_opt  
    #define X_TO_DOUBLE     danagy16_to_double

    #define MASK_MANTISA    DA_MASK_MANTISA
    #define MASK_SIGN       DA_MASK_SIGN  
    #define MANT_SIZE       DA_MANT_SIZE
    #define MASK_EXP        DA_MASK_EXP
    #define EXP_POS         DA_EXP_POS
    #define BIAS            DA_BIAS
    #define GET_SIGN        GET_DA_SIGN
    #define GET_EXP         GET_DA_EXP
    #define GET_MANTISA     GET_DA_MANTISA
    #define BYTE_TO_BINARY_PATTERN "%c%c%c%c %c%c%c%c"
    #define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

#else
    #error "Neocekavana hodnota MAX_NUMBER!"
#endif

#endif

//                   6            5    4         4         3         2         1         0
//                   3    -11-    2    8         0         2  -52-   4         6         8
// double(binary64): Seee eeee eeee mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm  
double make_double( binary64 sign, ibinary64 exponent, binary64 mantisa )
{
    sign    = ( sign & 1 ) << DO_SIGN_POS; 

    if ( exponent < -DO_BIAS ) {
        fprintf(stderr, "Warning! %s() underflow!\n", __FUNCTION__);
        return *((double *)&sign);            
    }
    exponent += DO_BIAS;
    if ( exponent > 0x7FF ) {
        fprintf(stderr, "Warning! %s() overflow!\n", __FUNCTION__);
        sign = sign + DO_MASK_EXP + DO_MASK_MANTISA;
        return *((double *)&sign);            
    }
    
    exponent <<= DO_EXP_POS;
    mantisa = mantisa & DO_MASK_MANTISA;
    
    binary64 rlt = sign + exponent + mantisa;
    return *((double *)&rlt);    
}


//                   6            5    4         4         3         2         1         0
//                   3    -11-    2    8         0         2  -52-   4         6         8
// double(binary64): Seee eeee eeee mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm
//                                  MMMM MMM? 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111
// bfloat16:                                                                     eeee eeee SMMM MMMM
double bfloat16_to_double(bfloat16 num16)
{
    binary64 temp = num16;
    binary64 res;
    res  =   ( temp & BF_MASK_SIGN )                                          << ( DO_SIGN_POS  - BF_SIGN_POS );
    res +=  (( temp & BF_MASK_EXP  ) +  ((DO_BIAS - BF_BIAS) << BF_EXP_POS )) << ( DO_EXP_POS   - BF_EXP_POS  );        // 
    res +=   ( temp & BF_MASK_MANTISA )                                       << ( DO_MANT_SIZE - BF_MANT_SIZE );       // ruzne posuny
    return *((double *) &res);
}


// float(single): Seee eeee emmm mmmm mmmm mmmm mmmm mmmm 
//                   6            5    4         4         3         2         1         0
//                   3    -11-    2    8         0         2  -52-   4         6         8
// double(binary64): Seee eeee eeee mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm
//                                  MMMM MMM? 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111
// binary16:                                                                     Seee eeMM MMMM MMMM
// bfloat16:                                                                     

double binary16_to_double(binary16 num16)
{
    binary64 temp = num16;
    binary64 res;
    res  =   ( temp &   BI_MASK_SIGN )                                                            << ( DO_SIGN_POS  - BI_SIGN_POS );
#if ( DO_MANT_SIZE - BI_MANT_SIZE ) != ( DO_EXP_POS - BI_EXP_POS  )
    #error Rozdil mantis se nerovna rozdilu pozic exponentu!
#endif
    res +=  (( temp & ( BI_MASK_EXP + BI_MASK_MANTISA )) +  ((DO_BIAS - BI_BIAS) << BI_EXP_POS )) << ( DO_EXP_POS   - BI_EXP_POS  );        // shodne posuny
    return *((double *) &res);
}


// float(single): Seee eeee emmm mmmm mmmm mmmm mmmm mmmm 
//                   6            5    4         4         3         2         1         0
//                   3    -11-    2    8         0         2  -52-   4         6         8
// double(binary64): Seee eeee eeee mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm
//                                  MMMM MMM? 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111
// binary16:                                                                     Seee eeMM MMMM MMMM
// bfloat16:                                                                     

double danagy16_to_double(danagy16 num16)
{
    binary64 temp = num16;
    binary64 res;
    res  =   ( temp &   DA_MASK_SIGN )                                                             << ( DO_SIGN_POS  - DA_SIGN_POS );
#if ( DO_MANT_SIZE - DA_MANT_SIZE ) != ( DO_EXP_POS - DA_EXP_POS  )
    #error Rozdil mantis se nerovna rozdilu pozic exponentu!
#endif
    res +=  (( temp & ( DA_MASK_EXP + DA_MASK_MANTISA ) ) +  ((DO_BIAS - DA_BIAS) << DA_EXP_POS )) << ( DO_EXP_POS   - DA_EXP_POS  );        // shodne posuny
    return *((double *) &res);
}


//                   6            5    4         4         3         2         1         0
//                   3    -11-    2    8         0         2  -52-   4         6         8
// double(binary64): Seee eeee eeee mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm
//                                  MMMM MMM? 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111
// bfloat16:                                                                     eeee eeee SMMM MMMM
bfloat16 double_to_bfloat16_opt(double d)
{
    const int sign_shift = DO_SIGN_POS - BF_SIGN_POS;
    const int sign_mask  =               BF_MASK_SIGN;
    const int exp_pos    =               BF_EXP_POS;
    const int mant_size  =               BF_MANT_SIZE;
    const int bias       =               BF_BIAS;
    const int mask_m     =               BF_MASK_MANTISA;
    const int mask_e     =               BF_MASK_EXP >> exp_pos;

    bfloat16  s = ((*( binary64 *) &d) >> sign_shift) & sign_mask;                                                              //  1 bit  -> 1 bit
    ibfloat16 e = (ibinary64) GET_DO_POSEXP(d) - (DO_BIAS -  bias);                                                             // 11 bitu -> 8 bitu
    binary64  M = (((*(binary64 *) &d) & DO_MASK_MANTISA) + (DO_MASK_MANTISA >> (mant_size+1))) >> (DO_MANT_SIZE - mant_size);  // 52 bitu -> 7 bitu 
    bfloat16  m = M;
    
    if ( m > mask_m ) {
        e++;
        m = (m & mask_m ) >> 1;  // pretekli jsme na pozici neukladaneho nejvyssiho vzdy 1 bitu, kdyby tam byl bude to nula, takze bit umazeme pres masku
    }

    if ( e > mask_e ) {
        fprintf(stderr, "Warning! %s() overflow!\n", __FUNCTION__);
        s = s | ( 0xFFFF - sign_mask ); 
        return s;
    }
    
    if ( e < 0 ) {
        fprintf(stderr, "Warning! %s() underflow!\n", __FUNCTION__);
        return s;
    }
    s = s + m + ( e << exp_pos );
    return s;
 }


//                   6            5    4         4         3         2         1         0
//                   3    -11-    2    8         0         2  -52-   4         6         8
// double(binary64): Seee eeee eeee mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm
//                                  MMMM MMMM MM?1 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111
// binary16:                                                                     Seee eeMM MMMM MMMM
binary16 double_to_binary16_opt(double d)
{
    const int sign_shift = DO_SIGN_POS - BI_SIGN_POS;
    const int sign_mask  =               BI_MASK_SIGN;
    const int exp_pos    =               BI_EXP_POS;
    const int mant_size  =               BI_MANT_SIZE;
    const int bias       =               BI_BIAS;
    const int mask_m     =               BI_MASK_MANTISA;
    const int mask_e     =               BI_MASK_EXP >> exp_pos;

    bfloat16  s = ((*( binary64 *) &d) >> sign_shift) & sign_mask;                                                              //  1 bit  ->  1 bit
    ibfloat16 e = (ibinary64) GET_DO_POSEXP(d) - (DO_BIAS -  bias);                                                             // 11 bitu ->  5 bitu
    binary64  M = (((*(binary64 *) &d) & DO_MASK_MANTISA) + (DO_MASK_MANTISA >> (mant_size+1))) >> (DO_MANT_SIZE - mant_size);  // 52 bitu -> 10 bitu 
    bfloat16  m = M;
    
    if ( m > mask_m ) {
        e++;
        m = (m & mask_m ) >> 1;  // pretekli jsme na pozici neukladaneho nejvyssiho vzdy 1 bitu, kdyby tam byl bude to nula, takze bit umazeme pres masku
    }

    if ( e > mask_e ) {
        fprintf(stderr, "Warning! %s() overflow!\n", __FUNCTION__);
        s = s | ( 0xFFFF - sign_mask ); 
        return s;
    }
    
    if ( e < 0 ) {
        fprintf(stderr, "Warning! %s() underflow!\n", __FUNCTION__);
        return s;
    }
    s = s + m + ( e << exp_pos );
    return s;
 }

 
 
//                   6            5    4         4         3         2         1         0
//                   3    -11-    2    8         0         2  -52-   4         6         8
// double(binary64): Seee eeee eeee mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm
//                                  MMMM MMMM ?111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111
// danagy:                                                                       Seee eeee MMMM MMMM
danagy16 double_to_danagy16_opt(double d)
{
    const int sign_shift = DO_SIGN_POS - DA_SIGN_POS;
    const int sign_mask  =               DA_MASK_SIGN;
    const int exp_pos    =               DA_EXP_POS;
    const int mant_size  =               DA_MANT_SIZE;
    const int bias       =               DA_BIAS;
    const int mask_m     =               DA_MASK_MANTISA;
    const int mask_e     =               DA_MASK_EXP >> exp_pos;

    bfloat16  s = ((*( binary64 *) &d) >> sign_shift) & sign_mask;                                                              //  1 bit  -> 1 bit
    ibfloat16 e = (ibinary64) GET_DO_POSEXP(d) - (DO_BIAS -  bias);                                                             // 11 bitu -> 7 bitu
    binary64  M = (((*(binary64 *) &d) & DO_MASK_MANTISA) + (DO_MASK_MANTISA >> (mant_size+1))) >> (DO_MANT_SIZE - mant_size);  // 52 bitu -> 8 bitu 
    bfloat16  m = M;
    
    if ( m > mask_m ) {
        e++;
        m = (m & mask_m ) >> 1;  // pretekli jsme na pozici neukladaneho nejvyssiho vzdy 1 bitu, kdyby tam byl bude to nula, takze bit umazeme pres masku
    }

    if ( e > mask_e ) {
        fprintf(stderr, "Warning! %s() overflow!\n", __FUNCTION__);
        s = s | ( 0xFFFF - sign_mask ); 
        return s;
    }
    
    if ( e < 0 ) {
        fprintf(stderr, "Warning! %s() underflow!\n", __FUNCTION__);
        return s;
    }
    s = s + m + ( e << exp_pos );
    return s;
 }

 
float make_float( int sign, int exponent, int mantisa )
{
    const int sign_pos = FP_SIGN_POS;
    const int exp_pos  = FP_EXP_POS;  
    const int bias     = FP_BIAS;
    const int mask_m   = FP_MASK_MANTISA;
    const int mask_e   = FP_MASK_EXP;
    const int max_e    = ( mask_e >> exp_pos ) - bias;
    
    sign    = ( sign & 1 ) << sign_pos; 
    
    if ( exponent > max_e ) {
        exponent = mask_e;
        mantisa  = mask_m;
        fprintf(stderr, "Warning! %s() overflow!\n", __FUNCTION__);
    }
    else if ( exponent < -bias ) {
        exponent = 0;
        mantisa  = 0;
        fprintf(stderr, "Warning! %s() underflow!\n", __FUNCTION__);
    }
    else
    {
        exponent = (( exponent + bias ) << exp_pos ) /* & mask_e */;
        mantisa = mantisa & mask_m ;
    }
    
    uint32_t rlt = sign + exponent + mantisa;
    return *((float *)&rlt);    
}



bfloat16 make_bfloat16( int sign, int exponent, int mantisa )
{
    const int sign_pos = BF_SIGN_POS;
    const int exp_pos  = BF_EXP_POS;  
    const int bias     = BF_BIAS;
    const int mask_m   = BF_MASK_MANTISA;
    const int mask_e   = BF_MASK_EXP;
    const int max_e    = ( mask_e >> exp_pos ) - bias;
    
    sign    = ( sign & 1 ) << sign_pos; 
    
    if ( exponent > max_e ) {
        exponent = mask_e;
        mantisa  = mask_m;
        fprintf(stderr, "Warning! %s() overflow!\n", __FUNCTION__);
    }
    else if ( exponent < -bias ) {
        exponent = 0;
        mantisa  = 0;
        fprintf(stderr, "Warning! %s() underflow!\n", __FUNCTION__);
    }
    else
    {
        exponent = (( exponent + bias ) << exp_pos ) /* & mask_e */;
        mantisa = mantisa & mask_m ;
    }
    
    unsigned short rlt = sign + exponent + mantisa;
    return *((bfloat16 *)&rlt);    
}



binary16 make_binary16( int sign, int exponent, int mantisa )
{
    const int sign_pos = BI_SIGN_POS;
    const int exp_pos  = BI_EXP_POS;  
    const int bias     = BI_BIAS;
    const int mask_m   = BI_MASK_MANTISA;
    const int mask_e   = BI_MASK_EXP;
    const int max_e    = ( mask_e >> exp_pos ) - bias;
    
    sign    = ( sign & 1 ) << sign_pos; 
    
    if ( exponent > max_e ) {
        exponent = mask_e;
        mantisa  = mask_m;
        fprintf(stderr, "Warning! %s() overflow!\n", __FUNCTION__);
    }
    else if ( exponent < -bias ) {
        exponent = 0;
        mantisa  = 0;
        fprintf(stderr, "Warning! %s() underflow!\n", __FUNCTION__);
    }
    else
    {
        exponent = (( exponent + bias ) << exp_pos ) /* & mask_e */;
        mantisa = mantisa & mask_m ;
    }
    
    unsigned short rlt = sign + exponent + mantisa;
    return *((binary16 *)&rlt);    
}



danagy16 make_danagy16( int sign, int exponent, int mantisa )
{
    const int sign_pos = DA_SIGN_POS;
    const int exp_pos  = DA_EXP_POS;  
    const int bias     = DA_BIAS;
    const int mask_m   = DA_MASK_MANTISA;
    const int mask_e   = DA_MASK_EXP;
    const int max_e    = ( mask_e >> exp_pos ) - bias;
    
    sign    = ( sign & 1 ) << sign_pos; 
    
    if ( exponent > max_e ) {
        exponent = mask_e;
        mantisa  = mask_m;
        fprintf(stderr, "Warning! %s() overflow!\n", __FUNCTION__);
    }
    else if ( exponent < -bias ) {
        exponent = 0;
        mantisa  = 0;
        fprintf(stderr, "Warning! %s() underflow!\n", __FUNCTION__);
    }
    else
    {
        exponent = (( exponent + bias ) << exp_pos ) /* & mask_e */;
        mantisa = mantisa & mask_m ;
    }
    
    unsigned short rlt = sign + exponent + mantisa;
    return *((danagy16 *)&rlt);    
}



// float(single): Seee eeee emmm mmmm mmmm mmmm mmmm mmmm 
// bfloat16:                          eeee eeee Smmm mmmm
bfloat16 float_to_bfloat16(float f)
{
    const int sign_pos = BF_SIGN_POS;
    const int exp_pos  = BF_EXP_POS;
    const int mant_size= BF_MANT_SIZE;
    
    uint32_t s = ((*(uint32_t *) &f) & FP_MASK_SIGN)    >> (FP_SIGN_POS - sign_pos);   // 1 bit   -> 1 bit
    uint32_t e = ((*(uint32_t *) &f) & FP_MASK_EXP)     >> (FP_EXP_POS  - exp_pos);    // 8 bitu  -> 8 bitu
    uint32_t m = ((*(uint32_t *) &f) & FP_MASK_MANTISA) >> (FP_MANT_SIZE- mant_size);  // 23 bitu -> 7 bitu
    unsigned short ret = s + e + m;
    return *((bfloat16 *) &ret);
}


// float(single): Seee eeee emmm mmmm mmmm mmmm mmmm mmmm 
// binary16:                          Seee eemm mmmm mmmm
binary16 float_to_binary16(float f)
{
    const int sign_pos = BI_SIGN_POS;
    const int exp_pos  = BI_EXP_POS;
    const int mant_size= BI_MANT_SIZE;
    const int bias     = BI_BIAS;
    const int mask_m   = BI_MASK_MANTISA;
    const int mask_e   = BI_MASK_EXP;
    const int max_e    = ( mask_e >> exp_pos ) - bias;

    uint32_t s = ((*(uint32_t *) &f) & FP_MASK_SIGN) >> (FP_SIGN_POS - sign_pos);   // 1 bit   ->  1 bit
     int32_t e = GET_FP_EXP( f );                                                   // 8 bitu  ->  5 bitu
    uint32_t m;
    
    if ( e > max_e ) {
        e = mask_e;
        m = mask_m;
        fprintf(stderr, "Warning! %s() overflow!\n", __FUNCTION__);
    }
    else if ( e < -bias ) {
        e = 0;
        m = 0;
        fprintf(stderr, "Warning! %s() underflow!\n", __FUNCTION__);
    }
    else
    {
        e = (( e + bias ) << exp_pos ) /* & mask_e */;
        m = GET_FP_MANTISA( f ) >> ( FP_MANT_SIZE - mant_size);  // 23 bitu -> 10 bitu        
    }

    unsigned short ret = s + e + m;
    return *((binary16 *) &ret);
}


// float(single): Seee eeee emmm mmmm mmmm mmmm mmmm mmmm 
// danagy16:                          Seee eeee mmmm mmmm
danagy16 float_to_danagy16(float f)
{
    const int sign_pos = DA_SIGN_POS;
    const int exp_pos  = DA_EXP_POS;
    const int mant_size= DA_MANT_SIZE;
    const int bias     = DA_BIAS;
    const int mask_m   = DA_MASK_MANTISA;
    const int mask_e   = DA_MASK_EXP;
    const int max_e    = ( mask_e >> exp_pos ) - bias;

    uint32_t s = ((*(uint32_t *) &f) & FP_MASK_SIGN) >> (FP_SIGN_POS - sign_pos);   // 1 bit   ->  1 bit
     int32_t e = GET_FP_EXP( f );                                                   // 8 bitu  ->  7 bitu
    uint32_t m;
    
    if ( e > max_e ) {
        e = mask_e;
        m = mask_m;
        fprintf(stderr, "Warning! %s() overflow!\n", __FUNCTION__);
    }
    else if ( e < -bias ) {
        e = 0;
        m = 0;
        fprintf(stderr, "Warning! %s() underflow!\n", __FUNCTION__);
    }
    else
    {
        e = (( e + bias ) << exp_pos ) /* & mask_e */;
        m = GET_FP_MANTISA( f ) >> ( FP_MANT_SIZE - mant_size);  // 23 bitu -> 8 bitu        
    }

    unsigned short ret = s + e + m;
    return *((danagy16 *) &ret);
}



// float(single): Seee eeee emmm mmmm mmmm mmmm mmmm mmmm
//                           MMM MMMM+?111 1111 1111 1111
// bfloat16:                          eeee eeee SMMM MMMM
bfloat16 float_to_bfloat16_opt(float f)
{
    const int sign_pos = BF_SIGN_POS;
    const int exp_pos  = BF_EXP_POS;
    const int mant_size= BF_MANT_SIZE;
    const int bias     = BF_BIAS;
    const int mask_m   = BF_MASK_MANTISA;
    unsigned 
    const int mask_e   = BF_MASK_EXP;

    uint32_t s = ((*(uint32_t *) &f) & FP_MASK_SIGN) >>   (FP_SIGN_POS - sign_pos);   // 1 bit   ->  1 bit
    uint32_t e = ((*(uint32_t *) &f) & FP_MASK_EXP) >>    (FP_EXP_POS - exp_pos);     // 8 bitu  ->  8 bitu
    uint32_t m = ((*(uint32_t *) &f) & FP_MASK_MANTISA) + (FP_MASK_MANTISA >> (mant_size+1));
    
    if ( m > FP_MASK_MANTISA ) {
        e += 0x100;
        m = (m & FP_MASK_MANTISA ) >> 1;  // pretekli jsme na pozici neukladaneho nejvyssiho vzdy 1 bitu, kdyby tam byl bude to nula, takze bit umazeme pres masku
    }
    
    if ( e > mask_e ) {
        e = mask_e;
        m = mask_m;
        fprintf(stderr, "Warning! %s() overflow!\n", __FUNCTION__);
    }
    else
    {
        m = m >> ( FP_MANT_SIZE - mant_size);  // 23 bitu -> 10 bitu        
    }
    
    unsigned short ret = s + e + m;
    return *((bfloat16 *) &ret);

 }


// float(single): Seee eeee emmm mmmm mmmm mmmm mmmm mmmm
//                           MMM MMMM MMM? 1111 1111 1111
// binary16:                          Seee eeMM MMMM MMMM
binary16 float_to_binary16_opt(float f)
{
    const int sign_pos = BI_SIGN_POS;
    const int exp_pos  = BI_EXP_POS;
    const int mant_size= BI_MANT_SIZE;
    const int bias     = BI_BIAS;
    const int mask_m   = BI_MASK_MANTISA;
    const int mask_e   = BI_MASK_EXP;
    const int max_e    = ( mask_e >> exp_pos ) - bias;

    uint32_t s = ((*(uint32_t *) &f) & FP_MASK_SIGN) >>   (FP_SIGN_POS - sign_pos);   // 1 bit   ->  1 bit
     int32_t e = GET_FP_EXP( f );                                                     // 8 bitu  ->  5 bitu
    uint32_t m = ((*(uint32_t *) &f) & FP_MASK_MANTISA) + (FP_MASK_MANTISA >> (mant_size+1));

    if ( m > FP_MASK_MANTISA ) {
        e++;
        m = (m & FP_MASK_MANTISA ) >> 1;  // pretekli jsme na pozici neukladaneho nejvyssiho vzdy 1 bitu, kdyby tam byl bude to nula, takze bit umazeme pres masku
    }
    
    if ( e > max_e ) {
        e = mask_e;
        m = mask_m;
        fprintf(stderr, "Warning! %s() overflow!\n", __FUNCTION__);
    }
    else if ( e < -bias ) {
        e = 0;
        m = 0;
        fprintf(stderr, "Warning! %s() underflow!\n", __FUNCTION__);
    }
    else
    {
        e = (( e + bias ) << exp_pos ) /* & mask_e */;
        m = m >> ( FP_MANT_SIZE - mant_size);  // 23 bitu -> 10 bitu        
    }
#if 0
if ( s + e + m != (s | e | m )) 
    fprintf(stderr, "Ehm...\n");
else
    fprintf(stderr, "[Ok: %f (s:%i e:$%X m:$%X) $%04X]\n", f, GET_FP_SIGN(f), GET_FP_EXP(f), GET_FP_MANTISA(f),s+e+m);
#endif
    unsigned short ret = s + e + m;
    return *((binary16 *) &ret);
}


// float(single): Seee eeee emmm mmmm mmmm mmmm mmmm mmmm
//                           MMM MMMM M?11 1111 1111 1111
// danagy16:                          Seee eeee mmmm mmmm
danagy16 float_to_danagy16_opt(float f)
{
    const int sign_pos = DA_SIGN_POS;
    const int exp_pos  = DA_EXP_POS;
    const int mant_size= DA_MANT_SIZE;
    const int bias     = DA_BIAS;
    const int mask_m   = DA_MASK_MANTISA;
    const int mask_e   = DA_MASK_EXP;
    const int max_e    = ( mask_e >> exp_pos ) - bias;

    uint32_t s = ((*(uint32_t *) &f) & FP_MASK_SIGN) >>   (FP_SIGN_POS - sign_pos);   // 1 bit   ->  1 bit
     int32_t e = GET_FP_EXP( f );                                                     // 8 bitu  ->  7 bitu
    uint32_t m = ((*(uint32_t *) &f) & FP_MASK_MANTISA) + (FP_MASK_MANTISA >> (mant_size+1));
// fprintf(stderr, "%s %f s:%X e:%i m:%X\n", __FUNCTION__, f, s, e, m);

    if ( m > FP_MASK_MANTISA ) {
        e++;
        m = (m & FP_MASK_MANTISA ) >> 1;  // pretekli jsme na pozici neukladaneho nejvyssiho vzdy 1 bitu, kdyby tam byl bude to nula, takze bit umazeme pres masku
    }
    
    if ( e > max_e ) {
        e = mask_e;
        m = mask_m;
        fprintf(stderr, "Warning! %s() overflow!\n", __FUNCTION__);
    }
    else if ( e < -bias ) {
        e = 0;
        m = 0;
        fprintf(stderr, "Warning! %s() underflow!\n", __FUNCTION__);
    }
    else
    {
        e = (( e + bias ) << exp_pos ) /* & mask_e */;
        m = m >> ( FP_MANT_SIZE - mant_size);  // 23 bitu -> 8 bitu        
// fprintf(stderr, "%s %f s:%X e:%X m:%X\n", __FUNCTION__, f, s, e, m);

    }

    unsigned short ret = s + e + m;
    return *((danagy16 *) &ret);
}



// float(single): Seee eeee emmm mmmm mmmm mmmm mmmm mmmm 
// bfloat16:                          eeee eeee Smmm mmmm
float bfloat16_to_float(bfloat16 num16)
{
    const int sign_pos = BF_SIGN_POS;
    const int exp_pos  = BF_EXP_POS;
    const int mant_size= BF_MANT_SIZE;

    uint32_t s = (*((uint32_t *) &num16) << (FP_SIGN_POS - sign_pos)) & FP_MASK_SIGN;      //  1 bit  ->  1 bit
    uint32_t e = (*((uint32_t *) &num16) << (FP_EXP_POS - exp_pos)) & FP_MASK_EXP;         //  8 bitu ->  8 bitu
    uint32_t m = (*((uint32_t *) &num16) << (FP_MANT_SIZE - mant_size)) & FP_MASK_MANTISA; //  7 bitu -> 23 bitu
    uint32_t res = s + e + m;
    return *((float *) &res);
}


// float(single): Seee eeee emmm mmmm mmmm mmmm mmmm mmmm 
// binary16:                          Seee eemm mmmm mmmm
float binary16_to_float(binary16 num16)
{
#if 1
// -127 = $00  A... BCDE       a bcde
//  -15 = $70  0111 0000 = 00  0 0000 = $70 + abcde
//   -1 = $7E  0111 1110 = 0E  0 1110 = $70 + abcde
//    0 = $7F  0111 1111 = 0F  0 1111 = $70 + abcde
//    1 = $80  1000 0000 = 10  1 0000 = $70 + abcde
//   16 = $8F  1000 1111 = 1F  1 1111 = $70 + abcde
//  128 = $FF
// binary16:                      Sa. ..bc demm mmmm mmmm
    uint32_t res = *((uint32_t *) &num16);
//     ( 7F - F ) << 10 = 1C000
    res = ((( res & 0x8000 ) << 3 ) + ( res & 0x7FFF ) + 0x1C000 ) << 13;
#else
    const int sign_pos = BI_SIGN_POS;
      int32_t e = GET_BI_EXP( num16 );
    const int mant_size= BI_MANT_SIZE;

    uint32_t s = (*((uint32_t *) &num16) << (FP_SIGN_POS - sign_pos)) & FP_MASK_SIGN;      //  1 bit  ->  1 bit
             e = ( e + FP_BIAS )         << FP_EXP_POS;                                    //  5 bitu ->  8 bitu
    uint32_t m = (*((uint32_t *) &num16) << (FP_MANT_SIZE - mant_size)) & FP_MASK_MANTISA; // 10 bitu -> 23 bitu
    uint32_t res = s + e + m;
#endif
    return *((float *) &res);
}


// float(single): Seee eeee emmm mmmm mmmm mmmm mmmm mmmm 
// binary16:                          Seee eeee mmmm mmmm
float danagy16_to_float(danagy16 num16)
{
    const int sign_pos = DA_SIGN_POS;
      int32_t e = GET_DA_EXP( num16 );
    const int mant_size= DA_MANT_SIZE;

    uint32_t s = (*((uint32_t *) &num16) << (FP_SIGN_POS - sign_pos)) & FP_MASK_SIGN;      //  1 bit  ->  1 bit
             e = ( e + FP_BIAS )         << FP_EXP_POS;                                    //  7 bitu ->  8 bitu
    uint32_t m = (*((uint32_t *) &num16) << (FP_MANT_SIZE - mant_size)) & FP_MASK_MANTISA; //  8 bitu  -> 23 bitu
    uint32_t res = s + e + m;
    return *((float *) &res);
}


#if 0
bfloat16 bfloat16_minimal_increment(bfloat16 num)
{
    short mantisa = GET_BF_MANTISA(num);
    short exponent= GET_BF_EXP(num);
    short sign = GET_BF_SIGN(num);
    mantisa++;
    if ( mantisa > BF_MASK_MANTISA ) {
        exponent++;                 // neosetrene preteceni exponentu!!!
        if  (exponent + BF_BIAS > 255 ) fprintf(stderr, "Error! Overflow exponent!\n" );
        mantisa &= BF_MASK_MANTISA;
    }   
    return make_bfloat16(sign, exponent, mantisa);
}


// float(single): Seee eeee emmm mmmm mmmm mmmm mmmm mmmm 
// bfloat:                            eeee eeee Smmm mmmm
bfloat16 float_to_bfloat16_optimize(float f)
{
    bfloat16 h_orez = float_to_bfloat16(f);
    bfloat16 h_plus = bfloat16_minimal_increment(h_orez);
    
    float f_orez, f_plus;
    f_orez = bfloat16_to_float(h_orez);
    f_plus = bfloat16_to_float(h_plus);
    
    float dif1, dif2;
    
    dif1 = f - f_orez;
    dif2 = f - f_plus;
    if ( dif1 < 0 ) dif1 = - dif1;
    if ( dif2 < 0 ) dif2 = - dif2;
    
    if ( dif1 > dif2 ) h_orez = h_plus;
    
    h_plus = float_to_bfloat16_opt(f);
    if ( h_orez != h_plus )
        fprintf(stderr, "??? %04X %04X\n", *((short *) &h_orez), *((short *) &h_plus));
    
    return h_orez;
}
#endif


//assumes little endian
void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i=size-1;i>=0;i--)
    {
        for (j=7;j>=0;j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
        putchar(' ');
    }
}


// bfloat:                            eeee eeee Smmm mmmm
void fprint_bfloat16(FILE *file, bfloat16 num16)
{
    const int    mask = BF_MASK;
    const int    bias = BF_BIAS;
    const int exp_pos = BF_EXP_POS;
    const int sign_pos= BF_SIGN_POS;
    int s = GET_BF_SIGN(num16);
    int e = GET_BF_EXP(num16);
    int m = GET_BF_MANTISA(num16);
    double d = bfloat16_to_double(num16);
    
    fprintf(file, "$%04x",*((int *) &num16) & mask);
    fprintf(file, "   ; %e", d);
    fprintf(file, " sign(%i=$%02X) exp(%3i=$%04X) mantisa(%3i=$%02X)", s, s << sign_pos, e, (e+bias) << exp_pos, m, m);
}


// binary16:                          Seee eemm mmmm mmmm
void fprint_binary16(FILE *file, binary16 num16)
{
    const int    mask = BI_MASK;
    const int    bias = BI_BIAS;
    const int exp_pos = BI_EXP_POS;
    const int sign_pos= BI_SIGN_POS;
    int s = GET_BI_SIGN(num16);
    int e = GET_BI_EXP(num16);
    int m = GET_BI_MANTISA(num16);
    double d = binary16_to_double(num16);
    
    fprintf(file, "$%04x",*((int *) &num16) & mask);
    fprintf(file, "   ; %e", d);
    fprintf(file, " sign(%i=$%04X) exp(%3i=$%04X) mantisa(%4i=$%03X)", s, s << sign_pos, e, (e+bias) << exp_pos, m, m);
}


// binary16:                          Seee eeee mmmm mmmm
void fprint_danagy16(FILE *file, danagy16 num16)
{
    const int    mask = DA_MASK;
    const int    bias = DA_BIAS;
    const int exp_pos = DA_EXP_POS;
    const int sign_pos= DA_SIGN_POS;
    int s = GET_DA_SIGN(num16);
    int e = GET_DA_EXP(num16);
    int m = GET_DA_MANTISA(num16);
    double d = danagy16_to_double(num16);
    
    fprintf(file, "$%04x",*((int *) &num16) & mask);
    fprintf(file, "   ; %e", d);
    fprintf(file, " sign(%i=$%04X) exp(%3i=$%04X) mantisa(%3i=$%02X)", s, s << sign_pos, e, (e+bias) << exp_pos, m, m);
}


void fprint_double(FILE *file, double f)
{
//     fprintf(file, "%f\t", f);
    
    char buf[256];
    sprintf(buf,"%f", f);
    int i = 0;
    while ( buf[i] ) i++; 
    i--;
    while ( buf[i] == '0' ) buf[i--] = 0;
    if ( buf[i] == '.' ) buf[i] = 0;
    i = 0;
    while ( buf[i] ) {
        if ( buf[i] == '-') buf[i] = 'M';        
        if ( buf[i] == '.') buf[i] = '_';
        i++;
    }
    
    if (f<0) fprintf(file, "F%s", buf);
    else fprintf(file, "FP%s", buf);

    i = 14 - i;
    if ( i < 1 ) i = 1;
    while (i--) putc(' ', file);
    if ( f < 0 ) putc(' ', file);
    
    fprintf(file, "EQU ");
}


void fprint_fp_to_bfloat16(FILE *file, double d)
{
    fprint_double( file, d);
    fprint_bfloat16(file, double_to_bfloat16_opt(d));
    putc('\n', file);
}


void fprint_fp_to_binary16(FILE *file, double d)
{
    fprint_double( file, d);
    fprint_binary16(file, double_to_binary16_opt(d));
    putc('\n', file);
}


void fprint_fp_to_danagy16(FILE *file, double d)
{
    fprint_double( file, d);
    fprint_danagy16(file, double_to_danagy16_opt(d));
    putc('\n', file);
}


#endif
