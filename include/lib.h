#ifndef include_lib_h__
#define include_lib_h__

namespace apilib
{
  typedef char                    int08_t;
  typedef short                   int16_t;
  typedef long int                int32_t;
  typedef long long int           int64_t;

# define int08_min (-128i8)
# define int08_max ( 127i8)
# define int16_min (-32768i16)
# define int16_max ( 32767i16)
# define int32_min (-2147483648i32)
# define int32_max ( 2147483647i32)
# define int64_min (-9223372036854775808i64)
# define int64_max ( 9223372036854775807i64)

  typedef unsigned char           uint08_t;
  typedef unsigned short          uint16_t;
  typedef unsigned long int       uint32_t;
  typedef unsigned long long int  uint64_t;

# define uint08_min (0ui8)
# define uint08_max (255ui8)
# define uint16_min (0ui16)
# define uint16_max (65535ui16)
# define uint32_min (0ui32)
# define uint32_max (4294967295ui32)
# define uint64_min (0ui64)
# define uint64_max (18446744073709551615ui64)

# if(APILIB_32 != 0)
  typedef unsigned int size_t;
# endif

# if(APILIB_64 != 0)
  typedef unsigned long long size_t;
# endif
};

#ifndef NULL
#define NULL 0
#endif

#endif // include_lib_h__