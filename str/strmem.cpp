#include "../cfg.h"
#include "../include/lib.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "strmem.h"
#include "strmem.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////

char* apilib::stralloc_a(size_t length)
{
  return apilib::__stralloc_x<char>(length);
}

wchar_t* apilib::stralloc_w(size_t length)
{
  return apilib::__stralloc_x<wchar_t>(length);
}


void apilib::strfree_a(char* string)
{
  apilib::__strfree_x<char>(string);
}

void apilib::strfree_w(wchar_t* string)
{
  apilib::__strfree_x<wchar_t>(string);
}


char* apilib::strdup_a(const char* string)
{
  return apilib::__strdup_x<char>(string);
}

wchar_t* apilib::strdup_w(const wchar_t* string)
{
  return apilib::__strdup_x<wchar_t>(string);
}


char* apilib::strdup_na(const char* string, size_t length)
{
  return apilib::__strdup_nx<char>(string, length);
}

wchar_t* apilib::strdup_nw(const wchar_t* string, size_t length)
{
  return apilib::__strdup_nx<wchar_t>(string, length);
}


bool apilib::stradd_a(char** dst, const char* src)
{
  return apilib::__stradd_x<char>(dst, src);
}

bool apilib::stradd_w(wchar_t** dst, const wchar_t* src)
{
  return apilib::__stradd_x<wchar_t>(dst, src);
}


bool apilib::stradd_na(char** dst, const char* src, size_t maxcnt)
{
  return apilib::__stradd_nx<char>(dst, src, maxcnt);
}

bool apilib::stradd_nw(wchar_t** dst, const wchar_t* src, size_t maxcnt)
{
  return apilib::__stradd_nx<wchar_t>(dst, src, maxcnt);
}

///////////////////////////////////////////////////////////////////////////////////////////////////