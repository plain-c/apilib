#include "../cfg.h"
#include "../include/lib.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "memcmp.h"
#include "memfind.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
#pragma optimize( "", off )
#endif

void* apilib::memfind(const void* mem, size_t mem_size, const void* data, size_t data_size)
{
  if(!mem || !mem_size || !data || !data_size)
  {
    return NULL;
  }

  if(mem_size >= data_size)
  {
    const unsigned char* ptr = (const unsigned char*)mem;
    const unsigned char* end = (const unsigned char*)mem + mem_size - data_size;

    while(ptr <= end)
    {
      if(apilib::memequ(ptr, data, data_size) == true)
      {
        return (void*)ptr;
      }
      ptr = (const unsigned char*)ptr + 1;
    }
  }

  return NULL;
}

#ifdef _MSC_VER
#pragma optimize( "", on )
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////