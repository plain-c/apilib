#include "../cfg.h"
#include "../include/lib.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "strhex.h"
#include "strhex.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////

size_t apilib::strtohex_la(const char* string, void* buffer, size_t buffer_size)
{
  return apilib::__strtohex_lx<char>(string, buffer, buffer_size);
}

size_t apilib::strtohex_lw(const wchar_t* string, void* buffer, size_t buffer_size)
{
  return apilib::__strtohex_lx<wchar_t>(string, buffer, buffer_size);
}

size_t apilib::strtohex_lna(const char* string, size_t maxlen, void* buffer, size_t buffer_size)
{
  return apilib::__strtohex_lnx<char>(string, maxlen, buffer, buffer_size);
}

size_t apilib::strtohex_lnw(const wchar_t* string, size_t maxlen, void* buffer, size_t buffer_size)
{
  return apilib::__strtohex_lnx<wchar_t>(string, maxlen, buffer, buffer_size);
}

size_t apilib::strgenhexseq_la(const void* binary, size_t length, char* buffer, int separator, int multiline)
{
  return apilib::__strgenhexseq_lx<char>(binary, length, buffer, separator, multiline);
}

size_t apilib::strgenhexseq_lw(const void* binary, size_t length, wchar_t* buffer, int separator, int multiline)
{
  return apilib::__strgenhexseq_lx<wchar_t>(binary, length, buffer, separator, multiline);
}

size_t apilib::strgenhexseq_lna(const void* binary, size_t length, char* buffer, size_t maxlen, int separator, int multiline)
{
  return apilib::__strgenhexseq_lnx<char>(binary, length, buffer, maxlen, separator, multiline);
}

size_t apilib::strgenhexseq_lnw(const void* binary, size_t length, wchar_t* buffer, size_t maxlen, int separator, int multiline)
{
  return apilib::__strgenhexseq_lnx<wchar_t>(binary, length, buffer, maxlen, separator, multiline);
}

size_t apilib::strgenhextbl_lna(const void* binary, size_t length, char* buffer, size_t maxlen)
{
  return apilib::__strgenhextbl_lnx<char>(binary, length, buffer, maxlen);
}

size_t apilib::strgenhextbl_lnw(const void* binary, size_t length, wchar_t* buffer, size_t maxlen)
{
  return apilib::__strgenhextbl_lnx<wchar_t>(binary, length, buffer, maxlen);
}

///////////////////////////////////////////////////////////////////////////////////////////////////