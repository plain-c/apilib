#include "../cfg.h"
#include "../include/lib.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "memsearch.h"
#include "memsearch.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////

char* apilib::memsearch_char(const void* data, size_t size, char value)
{
  return apilib::__memsearch_x<char>(data, size, value);
}

unsigned char* apilib::memsearch_uchar(const void* data, size_t size, unsigned char value)
{
  return apilib::__memsearch_x<unsigned char>(data, size, value);
}


short* apilib::memsearch_short(const void* data, size_t size, short value)
{
  return apilib::__memsearch_x<short>(data, size, value);
}

unsigned short* apilib::memsearch_ushort(const void* data, size_t size, unsigned short value)
{
  return apilib::__memsearch_x<unsigned short>(data, size, value);
}


long* apilib::memsearch_long(const void* data, size_t size, long value)
{
  return apilib::__memsearch_x<long>(data, size, value);
}

unsigned long* apilib::memsearch_ulong(const void* data, size_t size, unsigned long value)
{
  return apilib::__memsearch_x<unsigned long>(data, size, value);
}


long long* apilib::memsearch_longlong(const void* data, size_t size, long long value)
{
  return apilib::__memsearch_x<long long>(data, size, value);
}

unsigned long long* apilib::memsearch_ulonglong(const void* data, size_t size, unsigned long long value)
{
  return apilib::__memsearch_x<unsigned long long>(data, size, value);
}

///////////////////////////////////////////////////////////////////////////////////////////////////