#include "../cfg.h"
#include "../include/lib.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "strtrim.h"
#include "strtrim.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////

size_t apilib::addslashes_a(char* string)
{
  return apilib::__addslashes_x<char>(string);
}

size_t apilib::addslashes_w(wchar_t* string)
{
  return apilib::__addslashes_x<wchar_t>(string);
}


size_t apilib::addslashes_la(char* string, size_t maxlen)
{
  return apilib::__addslashes_lx<char>(string, maxlen);
}

size_t apilib::addslashes_lw(wchar_t* string, size_t maxlen)
{
  return apilib::__addslashes_lx<wchar_t>(string, maxlen);
}


size_t apilib::strtrim_a(char* string)
{
  return apilib::__strtrim_x<char>(string);
}

size_t apilib::strtrim_w(wchar_t* string)
{
  return apilib::__strtrim_x<wchar_t>(string);
}


size_t apilib::strtrim_na(char* string, size_t maxcnt)
{
  return apilib::__strtrim_nx<char>(string, maxcnt);
}

size_t apilib::strtrim_nw(wchar_t* string, size_t maxcnt)
{
  return apilib::__strtrim_nx<wchar_t>(string, maxcnt);
}

///////////////////////////////////////////////////////////////////////////////////////////////////