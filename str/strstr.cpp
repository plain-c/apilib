#include "../cfg.h"
#include "../include/lib.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "strstr.h"
#include "strstr.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////

char* apilib::strstr_a(const char* string1, const char* string2)
{
  return apilib::__strstr_x<char>(string1, string2);
}

wchar_t* apilib::strstr_w(const wchar_t* string1, const wchar_t* string2)
{
  return apilib::__strstr_x<wchar_t>(string1, string2);
}


char* apilib::strstr_na(const char* string1, const char* string2, size_t length1, size_t length2)
{
  return apilib::__strstr_nx<char>(string1, string2, length1, length2);
}

wchar_t* apilib::strstr_nw(const wchar_t* string1, const wchar_t* string2, size_t length1, size_t length2)
{
  return apilib::__strstr_nx<wchar_t>(string1, string2, length1, length2);
}


char* apilib::strstr_ea(const char* string1, const char* string2)
{
  return apilib::__strstr_ex<char>(string1, string2);
}

wchar_t* apilib::strstr_ew(const wchar_t* string1, const wchar_t* string2)
{
  return apilib::__strstr_ex<wchar_t>(string1, string2);
}


char* apilib::strstr_ena(const char* string1, const char* string2, size_t length1, size_t length2)
{
  return apilib::__strstr_enx<char>(string1, string2, length1, length2);
}

wchar_t* apilib::strstr_enw(const wchar_t* string1, const wchar_t* string2, size_t length1, size_t length2)
{
  return apilib::__strstr_enx<wchar_t>(string1, string2, length1, length2);
}


char* apilib::strrstr_a(const char* string1, const char* string2)
{
  return apilib::__strrstr_x<char>(string1, string2);
}

wchar_t* apilib::strrstr_w(const wchar_t* string1, const wchar_t* string2)
{
  return apilib::__strrstr_x<wchar_t>(string1, string2);
}


char* apilib::strrstr_na(const char* string1, const char* string2, size_t length1, size_t length2)
{
  return apilib::__strrstr_nx<char>(string1, string2, length1, length2);
}

wchar_t* apilib::strrstr_nw(const wchar_t* string1, const wchar_t* string2, size_t length1, size_t length2)
{
  return apilib::__strrstr_nx<wchar_t>(string1, string2, length1, length2);
}


char* apilib::strrstr_ea(const char* string1, const char* string2)
{
  return apilib::__strrstr_ex<char>(string1, string2);
}

wchar_t* apilib::strrstr_ew(const wchar_t* string1, const wchar_t* string2)
{
  return apilib::__strrstr_ex<wchar_t>(string1, string2);
}


char* apilib::strrstr_ena(const char* string1, const char* string2, size_t length1, size_t length2)
{
  return apilib::__strrstr_enx<char>(string1, string2, length1, length2);
}

wchar_t* apilib::strrstr_enw(const wchar_t* string1, const wchar_t* string2, size_t length1, size_t length2)
{
  return apilib::__strrstr_enx<wchar_t>(string1, string2, length1, length2);
}

///////////////////////////////////////////////////////////////////////////////////////////////////