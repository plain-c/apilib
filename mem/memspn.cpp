#include "../cfg.h"
#include "../include/lib.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "memspn.h"
#include "memspn.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////

size_t apilib::memspn_char(const void* data, size_t size, char value)
{
  return apilib::__memspn_x<char>(data, size, value);
}

size_t apilib::memspn_uchar(const void* data, size_t size, unsigned char value)
{
  return apilib::__memspn_x<unsigned char>(data, size, value);
}


size_t apilib::memspn_short(const void* data, size_t size, short value)
{
  return apilib::__memspn_x<short>(data, size, value);
}

size_t apilib::memspn_ushort(const void* data, size_t size, unsigned short value)
{
  return apilib::__memspn_x<unsigned short>(data, size, value);
}


size_t apilib::memspn_long(const void* data, size_t size, long value)
{
  return apilib::__memspn_x<long>(data, size, value);
}

size_t apilib::memspn_ulong(const void* data, size_t size, unsigned long value)
{
  return apilib::__memspn_x<unsigned long>(data, size, value);
}


size_t apilib::memspn_longlong(const void* data, size_t size, long long value)
{
  return apilib::__memspn_x<long long>(data, size, value);
}

size_t apilib::memspn_ulonglong(const void* data, size_t size, unsigned long long value)
{
  return apilib::__memspn_x<unsigned long long>(data, size, value);
}

///////////////////////////////////////////////////////////////////////////////////////////////////