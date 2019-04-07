#include "../cfg.h"
#include "../include/lib.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "memreplace.h"
#include "memreplace.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////

size_t apilib::memreplace_char(void* data, size_t size, char old_value, char new_value)
{
  return apilib::__memreplace_x<char>(data, size, old_value, new_value);
}

size_t apilib::memreplace_uchar(void* data, size_t size, unsigned char old_value, unsigned char new_value)
{
  return apilib::__memreplace_x<unsigned char>(data, size, old_value, new_value);
}


size_t apilib::memreplace_short(void* data, size_t size, short old_value, short new_value)
{
  return apilib::__memreplace_x<short>(data, size, old_value, new_value);
}

size_t apilib::memreplace_ushort(void* data, size_t size, unsigned short old_value, unsigned short new_value)
{
  return apilib::__memreplace_x<unsigned short>(data, size, old_value, new_value);
}


size_t apilib::memreplace_long(void* data, size_t size, long old_value, long new_value)
{
  return apilib::__memreplace_x<long>(data, size, old_value, new_value);
}

size_t apilib::memreplace_ulong(void* data, size_t size, unsigned long old_value, unsigned long new_value)
{
  return apilib::__memreplace_x<unsigned long>(data, size, old_value, new_value);
}


size_t apilib::memreplace_longlong(void* data, size_t size, long long old_value, long long new_value)
{
  return apilib::__memreplace_x<long long>(data, size, old_value, new_value);
}

size_t apilib::memreplace_ulonglong(void* data, size_t size, unsigned long long old_value, unsigned long long new_value)
{
  return apilib::__memreplace_x<unsigned long long>(data, size, old_value, new_value);
}

///////////////////////////////////////////////////////////////////////////////////////////////////