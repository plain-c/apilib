#include "../cfg.h"
#include "../include/lib.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "strenc.h"
#include "strmem.h"
#include "strenc.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////

int apilib::is_utf8_bom(const void* data, size_t size)
{
  if(data != NULL && size >= 3)
  {
    const unsigned char* mark = (const unsigned char*)data;
    if(mark[0] == 0xEF && mark[1] == 0xBB && mark[2] == 0xBF)
    {
      return 1;
    }
  }
  return 0;
}

int apilib::is_utf16_bom(const void* data, size_t size)
{
  if(data != NULL && size >= 2)
  {
    const unsigned short* mark = (const unsigned short*)data;
    if(*mark == 0xFEFF)
    {
      return 1;
    }
  }
  return 0;
}

int apilib::is_utf32_bom(const void* data, size_t size)
{
  if(data != NULL && size >= 4)
  {
    const unsigned long int* mark = (const unsigned long int*)data;
    if(*mark == 0xFFFE0000)
    {
      return 1;
    }
  }
  return 0;
}


size_t apilib::acstowcs_le(wchar_t* dst, size_t maxlen, const char* src)
{
  return apilib::__xcstoxcs_le<char, wchar_t>(dst, maxlen, src);
}

size_t apilib::acstowcs_lne(wchar_t* dst, size_t maxlen, const char* src, size_t length)
{
  return apilib::__xcstoxcs_lne<char, wchar_t>(dst, maxlen, src, length);
}


size_t apilib::wcstoacs_le(char* dst, size_t maxlen, const wchar_t* src)
{
  return apilib::__xcstoxcs_le<wchar_t, char>(dst, maxlen, src);
}

size_t apilib::wcstoacs_lne(char* dst, size_t maxlen, const wchar_t* src, size_t length)
{
  return apilib::__xcstoxcs_lne<wchar_t, char>(dst, maxlen, src, length);
}


size_t apilib::acstowcs_l(wchar_t* dst, size_t maxlen, const char* src)
{
  return apilib::__mbstowcs_l(CP_ACP, 0, dst, maxlen, src);
}

size_t apilib::acstowcs_ln(wchar_t* dst, size_t maxlen, const char* src, size_t length)
{
  return apilib::__mbstowcs_ln(CP_ACP, 0, dst, maxlen, src, length);
}


size_t apilib::wcstoacs_l(char* dst, size_t maxlen, const wchar_t* src)
{
  return apilib::__wcstombs_l(CP_ACP, WC_COMPOSITECHECK, dst, maxlen, src);
}

size_t apilib::wcstoacs_ln(char* dst, size_t maxlen, const wchar_t* src, size_t length)
{
  return apilib::__wcstombs_ln(CP_ACP, WC_COMPOSITECHECK, dst, maxlen, src, length);
}


size_t apilib::ucstowcs_l(wchar_t* dst, size_t maxlen, const char* src)
{
  return apilib::__mbstowcs_l(CP_UTF8, 0, dst, maxlen, src);
}

size_t apilib::ucstowcs_ln(wchar_t* dst, size_t maxlen, const char* src, size_t length)
{
  return apilib::__mbstowcs_ln(CP_UTF8, 0, dst, maxlen, src, length);
}


size_t apilib::wcstoucs_l(char* dst, size_t maxlen, const wchar_t* src)
{
  return apilib::__wcstombs_l(CP_UTF8, 0, dst, maxlen, src);
}

size_t apilib::wcstoucs_ln(char* dst, size_t maxlen, const wchar_t* src, size_t length)
{
  return apilib::__wcstombs_ln(CP_UTF8, 0, dst, maxlen, src, length);
}


size_t apilib::acstoucs_l(char* dst, size_t maxlen, const char* src)
{
  wchar_t* tmp_string = NULL;
  size_t   tmp_length = 0;

  tmp_string = apilib::__mbstowcs_dup(CP_ACP, 0, src, &tmp_length);
  if(!tmp_string)
  {
    return (size_t)-1;
  }

  size_t converted = apilib::__wcstombs_ln(CP_UTF8, 0, dst, maxlen, tmp_string, tmp_length);
  apilib::strfree_w(tmp_string);

  return converted;
}

size_t apilib::acstoucs_ln(char* dst, size_t maxlen, const char* src, size_t length)
{
  wchar_t* tmp_string = NULL;
  size_t   tmp_length = 0;

  tmp_string = apilib::__mbstowcs_dup_n(CP_ACP, 0, src, length, &tmp_length);
  if(!tmp_string)
  {
    return (size_t)-1;
  }

  size_t converted = apilib::__wcstombs_ln(CP_UTF8, 0, dst, maxlen, tmp_string, tmp_length);
  apilib::strfree_w(tmp_string);

  return converted;
}


size_t apilib::ucstoacs_l(char* dst, size_t maxlen, const char* src)
{
  wchar_t* tmp_string = NULL;
  size_t   tmp_length = 0;

  tmp_string = apilib::__mbstowcs_dup(CP_UTF8, 0, src, &tmp_length);
  if(!tmp_string)
  {
    return (size_t)-1;
  }

  size_t converted = apilib::__wcstombs_ln(CP_ACP, WC_COMPOSITECHECK, dst, maxlen, tmp_string, tmp_length);
  apilib::strfree_w(tmp_string);

  return converted;
}

size_t apilib::ucstoacs_ln(char* dst, size_t maxlen, const char* src, size_t length)
{
  wchar_t* tmp_string = NULL;
  size_t   tmp_length = 0;

  tmp_string = apilib::__mbstowcs_dup_n(CP_UTF8, 0, src, length, &tmp_length);
  if(!tmp_string)
  {
    return (size_t)-1;
  }

  size_t converted = apilib::__wcstombs_ln(CP_ACP, WC_COMPOSITECHECK, dst, maxlen, tmp_string, tmp_length);
  apilib::strfree_w(tmp_string);

  return converted;
}


wchar_t*apilib:: acstowcs_dup(const char* src, size_t* result)
{
  return apilib::__mbstowcs_dup(CP_ACP, 0, src, result);
}

wchar_t* apilib::acstowcs_dup_n(const char* src, size_t length, size_t* result)
{
  return apilib::__mbstowcs_dup_n(CP_ACP, 0, src, length, result);
}


char* apilib::wcstoacs_dup(const wchar_t* src, size_t* result)
{
  return apilib::__wcstombs_dup(CP_ACP, WC_COMPOSITECHECK, src, result);
}

char* apilib::wcstoacs_dup_n(const wchar_t* src, size_t length, size_t* result)
{
  return apilib::__wcstombs_dup_n(CP_ACP, WC_COMPOSITECHECK, src, length, result);
}


wchar_t* apilib::ucstowcs_dup(const char* src, size_t* result)
{
  return apilib::__mbstowcs_dup(CP_UTF8, 0, src, result);
}

wchar_t* apilib::ucstowcs_dup(const char* src, size_t length, size_t* result)
{
  return apilib::__mbstowcs_dup_n(CP_UTF8, 0, src, length, result);
}


char* apilib::wcstoucs_dup(const wchar_t* src, size_t* result)
{
  return apilib::__wcstombs_dup(CP_UTF8, 0, src, result);
}

char* apilib::wcstoucs_dup_n(const wchar_t* src, size_t length, size_t* result)
{
  return apilib::__wcstombs_dup_n(CP_UTF8, 0, src, length, result);
}


char* apilib::acstoucs_dup(const char* src, size_t* result)
{
  wchar_t* tmp_string = NULL;
  size_t   tmp_length = 0;

  tmp_string = apilib::__mbstowcs_dup(CP_ACP, 0, src, &tmp_length);
  if(!tmp_string)
  {
    return NULL;
  }

  char* allocated = apilib::__wcstombs_dup_n(CP_UTF8, 0, tmp_string, tmp_length, result);
  apilib::strfree_w(tmp_string);

  return allocated;
}

char* apilib::acstoucs_dup_n(const char* src, size_t length, size_t* result)
{
  wchar_t* tmp_string = NULL;
  size_t   tmp_length = 0;

  tmp_string = apilib::__mbstowcs_dup_n(CP_ACP, 0, src, length, &tmp_length);
  if(!tmp_string)
  {
    return NULL;
  }

  char* allocated = apilib::__wcstombs_dup_n(CP_UTF8, 0, tmp_string, tmp_length, result);
  apilib::strfree_w(tmp_string);

  return allocated;
}


char* apilib::ucstoacs_dup(const char* src, size_t* result)
{
  wchar_t* tmp_string = NULL;
  size_t   tmp_length = 0;

  tmp_string = apilib::__mbstowcs_dup(CP_UTF8, 0, src, &tmp_length);
  if(!tmp_string)
  {
    return NULL;
  }

  char* allocated = apilib::__wcstombs_dup_n(CP_ACP, WC_COMPOSITECHECK, tmp_string, tmp_length, result);
  apilib::strfree_w(tmp_string);

  return allocated;
}

char* apilib::ucstoacs_dup_n(const char* src, size_t length, size_t* result)
{
  wchar_t* tmp_string = NULL;
  size_t   tmp_length = 0;

  tmp_string = apilib::__mbstowcs_dup_n(CP_UTF8, 0, src, length, &tmp_length);
  if(!tmp_string)
  {
    return NULL;
  }

  char* allocated = apilib::__wcstombs_dup_n(CP_ACP, WC_COMPOSITECHECK, tmp_string, tmp_length, result);
  apilib::strfree_w(tmp_string);

  return allocated;
}

///////////////////////////////////////////////////////////////////////////////////////////////////