#include "../cfg.h"
#include "../include/lib.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "strcmp.h"
#include "strcmp.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////

int apilib::strcmp_a(const char* s1, const char* s2)
{
  return apilib::__strcmp_x<char>(s1, s2);
}

int apilib::strcmp_w(const wchar_t* s1, const wchar_t* s2)
{
  return apilib::__strcmp_x<wchar_t>(s1, s2);
}


int apilib::strcmp_ea(const char* s1, const char* s2)
{
  return apilib::__strcmp_ex<char>(s1, s2);
}

int apilib::strcmp_ew(const wchar_t* s1, const wchar_t* s2)
{
  return apilib::__strcmp_ex<wchar_t>(s1, s2);
}


int apilib::strcmp_na(const char* s1, const char* s2, size_t maxcnt)
{
  return apilib::__strcmp_nx<char>(s1, s2, maxcnt);
}

int apilib::strcmp_nw(const wchar_t* s1, const wchar_t* s2, size_t maxcnt)
{
  return apilib::__strcmp_nx<wchar_t>(s1, s2, maxcnt);
}


int apilib::strcmp_ena(const char* s1, const char* s2, size_t maxcnt)
{
  return apilib::__strcmp_enx<char>(s1, s2, maxcnt);
}

int apilib::strcmp_enw(const wchar_t* s1, const wchar_t* s2, size_t maxcnt)
{
  return apilib::__strcmp_enx<wchar_t>(s1, s2, maxcnt);
}


bool apilib::strequ_a(const char* s1, const char* s2)
{
  return apilib::__strequ_x<char>(s1, s2);
}

bool apilib::strequ_w(const wchar_t* s1, const wchar_t* s2)
{
  return apilib::__strequ_x<wchar_t>(s1, s2);
}


bool apilib::strequ_ea(const char* s1, const char* s2)
{
  return apilib::__strequ_ex<char>(s1, s2);
}

bool apilib::strequ_ew(const wchar_t* s1, const wchar_t* s2)
{
  return apilib::__strequ_ex<wchar_t>(s1, s2);
}


bool apilib::strequ_na(const char* s1, const char* s2, size_t maxcnt)
{
  return apilib::__strequ_nx<char>(s1, s2, maxcnt);
}

bool apilib::strequ_nw(const wchar_t* s1, const wchar_t* s2, size_t maxcnt)
{
  return apilib::__strequ_nx<wchar_t>(s1, s2, maxcnt);
}


bool apilib::strequ_ena(const char* s1, const char* s2, size_t maxcnt)
{
  return apilib::__strequ_enx<char>(s1, s2, maxcnt);
}

bool apilib::strequ_enw(const wchar_t* s1, const wchar_t* s2, size_t maxcnt)
{
  return apilib::__strequ_enx<wchar_t>(s1, s2, maxcnt);
}

///////////////////////////////////////////////////////////////////////////////////////////////////