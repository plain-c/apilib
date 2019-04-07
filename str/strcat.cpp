#include "../cfg.h"
#include "../include/lib.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "strcat.h"
#include "strcat.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////

size_t apilib::strcat_a(char* dst, const char* src)
{
  return apilib::__strcat_x<char>(dst, src);
}

size_t apilib::strcat_w(wchar_t* dst, const wchar_t* src)
{
  return apilib::__strcat_x<wchar_t>(dst, src);
}


size_t apilib::strcat_na(char* dst, const char* src, size_t maxcnt)
{
  return apilib::__strcat_nx<char>(dst, src, maxcnt);
}

size_t apilib::strcat_nw(wchar_t* dst, const wchar_t* src, size_t maxcnt)
{
  return apilib::__strcat_nx<wchar_t>(dst, src, maxcnt);
}


size_t apilib::strcat_la(char* dst, const char* src, size_t maxlen)
{
  return apilib::__strcat_lx<char>(dst, src, maxlen);
}

size_t apilib::strcat_lw(wchar_t* dst, const wchar_t* src, size_t maxlen)
{
  return apilib::__strcat_lx<wchar_t>(dst, src, maxlen);
}


size_t apilib::strcat_lna(char* dst, const char* src, size_t maxlen, size_t maxcnt)
{
  return apilib::__strcat_lnx<char>(dst, src, maxlen, maxcnt);
}

size_t apilib::strcat_lnw(wchar_t* dst, const wchar_t* src, size_t maxlen, size_t maxcnt)
{
  return apilib::__strcat_lnx<wchar_t>(dst, src, maxlen, maxcnt);
}

///////////////////////////////////////////////////////////////////////////////////////////////////