#include "../cfg.h"
#include "../include/lib.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "memset.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
#pragma optimize( "", off )
#endif

bool apilib::memset(void* dst, unsigned char val, size_t size)
{
  if(!dst)
  {
    return false;
  }

  for(size_t i = 0; i < size; i++)
  {
    ((unsigned char*)dst)[i] = val;
  }

  return true;
}

bool apilib::memzero(void* dst, size_t size)
{
  if(!dst)
  {
    return false;
  }

  for(size_t i = 0; i < size; i++)
  {
    ((unsigned char*)dst)[i] = 0;
  }

  return true;
}

#ifdef _MSC_VER
#pragma optimize( "", on )
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////