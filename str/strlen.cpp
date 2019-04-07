#include "../cfg.h"
#include "../include/lib.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "strlen.h"
#include "strlen.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////

size_t apilib::strlen_a(const char* string)
{
  return apilib::__strlen_x<char>(string);
}

size_t apilib::strlen_w(const wchar_t* string)
{
  return apilib::__strlen_x<wchar_t>(string);
}


size_t apilib::strlen_na(const char* string, size_t maxlen)
{
  return apilib::__strlen_nx<char>(string, maxlen);
}

size_t apilib::strlen_nw(const wchar_t* string, size_t maxlen)
{
  return apilib::__strlen_nx<wchar_t>(string, maxlen);
}

///////////////////////////////////////////////////////////////////////////////////////////////////