#include "../cfg.h"
#include "../include/lib.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "strcpy.h"
#include "strcpy.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////

size_t apilib::strcpy_a(char* dst, const char* src)
{
  return apilib::__strcpy_x<char>(dst, src);
}

size_t apilib::strcpy_w(wchar_t* dst, const wchar_t* src)
{
  return apilib::__strcpy_x<wchar_t>(dst, src);
}


size_t apilib::strcpy_na(char* dst, const char* src, size_t maxcnt)
{
  return apilib::__strcpy_nx<char>(dst, src, maxcnt);
}

size_t apilib::strcpy_nw(wchar_t* dst, const wchar_t* src, size_t maxcnt)
{
  return apilib::__strcpy_nx<wchar_t>(dst, src, maxcnt);
}


size_t apilib::strcpy_la(char* dst, const char* src, size_t maxlen)
{
  return apilib::__strcpy_lx<char>(dst, src, maxlen);
}

size_t apilib::strcpy_lw(wchar_t* dst, const wchar_t* src, size_t maxlen)
{
  return apilib::__strcpy_lx<wchar_t>(dst, src, maxlen);
}


size_t apilib::strcpy_lna(char* dst, const char* src, size_t maxlen, size_t maxcnt)
{
  return apilib::__strcpy_lnx<char>(dst, src, maxlen, maxcnt);
}

size_t apilib::strcpy_lnw(wchar_t* dst, const wchar_t* src, size_t maxlen, size_t maxcnt)
{
  return apilib::__strcpy_lnx<wchar_t>(dst, src, maxlen, maxcnt);
}

///////////////////////////////////////////////////////////////////////////////////////////////////