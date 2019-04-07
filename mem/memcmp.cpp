#include "../cfg.h"
#include "../include/lib.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "memcmp.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
#pragma optimize( "", off )
#endif

int apilib::memcmp(const void* mem1, const void* mem2, size_t size)
{
  if(!mem1)return(-2);
  if(!mem2)return( 2);

  if(mem1 != mem2)
  {
    for(size_t i = 0; i < size; i++)
    {
      unsigned char b1 = ((const unsigned char*)mem1)[i];
      unsigned char b2 = ((const unsigned char*)mem2)[i];
      if(b1 > b2)return( 1);
      if(b1 < b2)return(-1);
    }
  }

  return 0;
}

int apilib::memcmp_s(const void* mem1, size_t size1, const void* mem2, size_t size2)
{
  if(!mem1 || size2 > size1)return(-2);
  if(!mem2 || size2 < size1)return( 2);

  if(mem1 != mem2)
  {
    for(size_t i = 0; i < size1 /* && i < size2 */; i++)
    {
      unsigned char b1 = ((const unsigned char*)mem1)[i];
      unsigned char b2 = ((const unsigned char*)mem2)[i];
      if(b1 > b2)return( 1);
      if(b1 < b2)return(-1);
    }
  }

  return 0;
}

bool apilib::memequ(const void* mem1, const void* mem2, size_t size)
{
  if(!mem1 || !mem2)
  {
    return false;
  }

  if(mem1 != mem2)
  {
    for(size_t i = 0; i < size; i++)
    {
      unsigned char b1 = ((const unsigned char*)mem1)[i];
      unsigned char b2 = ((const unsigned char*)mem2)[i];
      if(b1 != b2)return false;
    }
  }

  return true;
}

bool apilib::memequ_s(const void* mem1, size_t size1, const void* mem2, size_t size2)
{
  if(!mem1 || !mem2 || size1 != size2)
  {
    return false;
  }

  if(mem1 != mem2)
  {
    for(size_t i = 0; i < size1 /* && i < size2 */; i++)
    {
      unsigned char b1 = ((const unsigned char*)mem1)[i];
      unsigned char b2 = ((const unsigned char*)mem2)[i];
      if(b1 != b2)return false;
    }
  }

  return true;
}

#ifdef _MSC_VER
#pragma optimize( "", on )
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////