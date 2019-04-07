#include "../cfg.h"
#include "../include/lib.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "strchr.h"
#include "strchr.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////

char* apilib::strchr_a(const char* string, char chr)
{
  return apilib::__strchr_x<char>(string, chr);
}

wchar_t* apilib::strchr_w(const wchar_t* string, wchar_t chr)
{
  return apilib::__strchr_x<wchar_t>(string, chr);
}


char* apilib::strchr_na(const char* string, size_t maxlen, char chr)
{
  return apilib::__strchr_nx<char>(string, maxlen, chr);
}

wchar_t* apilib::strchr_nw(const wchar_t* string, size_t maxlen, wchar_t chr)
{
  return apilib::__strchr_nx<wchar_t>(string, maxlen, chr);
}


char* apilib::strrchr_a(const char* string, char chr)
{
  return apilib::__strrchr_x<char>(string, chr);
}

wchar_t* apilib::strrchr_w(const wchar_t* string, wchar_t chr)
{
  return apilib::__strrchr_x<wchar_t>(string, chr);
}


char* apilib::strrchr_na(const char* string, size_t maxlen, char chr)
{
  return apilib::__strrchr_nx<char>(string, maxlen, chr);
}

wchar_t* apilib::strrchr_nw(const wchar_t* string, size_t maxlen, wchar_t chr)
{
  return apilib::__strrchr_nx<wchar_t>(string, maxlen, chr);
}

///////////////////////////////////////////////////////////////////////////////////////////////////