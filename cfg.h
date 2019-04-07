#ifndef __rootfolder_cfg_h__
#define __rootfolder_cfg_h__

///////////////////////////////////////////////////////////////////////////////////////////////////

# if defined _X32_
# define APILIB_32 1
# define APILIB_64 0
# elif defined _X64_
# define APILIB_32 0
# define APILIB_64 1
# endif

# define APILIB_BIG_ENDIAN    0
# define APILIB_LITTLE_ENDIAN 1

# define APILIB_OS_LINUX 0
# define APILIB_OS_WINNT 1

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // __rootfolder_cfg_h__
