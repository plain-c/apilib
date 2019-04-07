#include "../cfg.h"
#include "../include/lib.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "memcpy.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
#pragma optimize( "", off )
#endif

bool apilib::memcpy(void* dst, const void* src, size_t size)
{
  if(!dst || !src)
  {
    return false;
  }

  if(dst != src)
  {
    for(size_t i = 0; i < size; i++)
    {
      ((unsigned char*)dst)[i] = ((const unsigned char*)src)[i];
    }
  }

  return true;
}

bool apilib::memcpy_s(void* dst, size_t dst_size, const void* src, size_t src_size)
{
  if(!dst || !src || src_size > dst_size)
  {
    return false;
  }

  if(dst != src)
  {
    for(size_t i = 0; i < src_size /* && i < dst_size */; i++)
    {
      ((unsigned char*)dst)[i] = ((const unsigned char*)src)[i];
    }
  }

  return true;
}

bool apilib::memmove(void* dst, const void* src, size_t size)
{
  if(!dst || !src)
  {
    return false;
  }

  if(src > dst)
  {
    size_t i = 0;
    while( i < size )
    {
      ((unsigned char*)dst)[i] = ((const unsigned char*)src)[i];
      i += 1;
    }
  }
  else if(dst < src)
  {
    size_t i = size;
    while( i > 0 )
    {
      i -= 1;
      ((unsigned char*)dst)[i] = ((const unsigned char*)src)[i];
    }
  }

  return true;
}

bool apilib::memmove_s(void* dst, size_t dst_size, const void* src, size_t src_size)
{
  if(!dst || !src || src_size > dst_size)
  {
    return false;
  }

  if(src > dst)
  {
    size_t i = 0;
    while( i < src_size )
    {
      ((unsigned char*)dst)[i] = ((const unsigned char*)src)[i];
      i += 1;
    }
  }
  else if(dst < src)
  {
    size_t i = src_size;
    while( i > 0 )
    {
      i -= 1;
      ((unsigned char*)dst)[i] = ((const unsigned char*)src)[i];
    }
  }

  return true;
}

bool apilib::memswap(void* mem1, void* mem2, size_t size)
{
  if(!mem1 || !mem2)
  {
    return false;
  }

  if(mem1 != mem2)
  {
    unsigned char* p1 = (unsigned char*)mem1;
    unsigned char* p2 = (unsigned char*)mem2;
    unsigned char value = 0;

    for(size_t i = 0; i < size; i++)
    {
      value = p1[i];
      p1[i] = p2[i];
      p2[i] = value;
    }
  }

  return true;
}

#ifdef _MSC_VER
#pragma optimize( "", on )
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////