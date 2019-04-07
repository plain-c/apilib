#include "../cfg.h"
#include "../include/lib.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "strspn.h"
#include "strspn.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////

size_t apilib::strspn_a(const char* string, const char* accept)
{
  return apilib::__strspn_x<char>(string, accept);
}

size_t apilib::strspn_w(const wchar_t* string, const wchar_t* accept)
{
  return apilib::__strspn_x<wchar_t>(string, accept);
}


size_t apilib::strspn_na(const char* string, const char* accept, size_t maxcnt)
{
  return apilib::__strspn_nx<char>(string, accept, maxcnt);
}

size_t apilib::strspn_nw(const wchar_t* string, const wchar_t* accept, size_t maxcnt)
{
  return apilib::__strspn_nx<wchar_t>(string, accept, maxcnt);
}


size_t apilib::strcspn_a(const char* string, const char* reject)
{
  return apilib::__strcspn_x<char>(string, reject);
}

size_t apilib::strcspn_w(const wchar_t* string, const wchar_t* reject)
{
  return apilib::__strcspn_x<wchar_t>(string, reject);
}


size_t apilib::strcspn_na(const char* string, const char* reject, size_t maxcnt)
{
  return apilib::__strcspn_nx<char>(string, reject, maxcnt);
}

size_t apilib::strcspn_nw(const wchar_t* string, const wchar_t* reject, size_t maxcnt)
{
  return apilib::__strcspn_nx(string, reject, maxcnt);
}


size_t apilib::strrspn_a(const char* string, const char* accept)
{
  return apilib::__strrspn_x<char>(string, accept);
}

size_t apilib::strrspn_w(const wchar_t* string, const wchar_t* accept)
{
  return apilib::__strrspn_x<wchar_t>(string, accept);
}


size_t apilib::strrspn_na(const char* string, const char* accept, size_t maxcnt)
{
  return apilib::__strrspn_nx<char>(string, accept, maxcnt);
}

size_t apilib::strrspn_nw(const wchar_t* string, const wchar_t* accept, size_t maxcnt)
{
  return apilib::__strrspn_nx<wchar_t>(string, accept, maxcnt);
}


size_t apilib::strrcspn_a(const char* string, const char* reject)
{
  return apilib::__strrcspn_x<char>(string, reject);
}

size_t apilib::strrcspn_w(const wchar_t* string, const wchar_t* reject)
{
  return apilib::__strrcspn_x<wchar_t>(string, reject);
}


size_t apilib::strrcspn_na(const char* string, const char* reject, size_t maxcnt)
{
  return apilib::__strrcspn_nx<char>(string, reject, maxcnt);
}

size_t apilib::strrcspn_nw(const wchar_t* string, const wchar_t* reject, size_t maxcnt)
{
  return apilib::__strrcspn_nx<wchar_t>(string, reject, maxcnt);
}

///////////////////////////////////////////////////////////////////////////////////////////////////