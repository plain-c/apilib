#include "../cfg.h"
#include "../include/lib.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "chrcmp.h"
#include "chrcmp.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////

int apilib::chrcmp_a(char c1, char c2)
{
  return apilib::__chrcmp_x<char>(c1, c2);
}

int apilib::chrcmp_w(wchar_t c1, wchar_t c2)
{
  return apilib::__chrcmp_x<wchar_t>(c1, c2);
}


int apilib::chrcmp_ea(char c1, char c2)
{
  return apilib::__chrcmp_ex<char>(c1, c2);
}

int apilib::chrcmp_ew(wchar_t c1, wchar_t c2)
{
  return apilib::__chrcmp_ex<wchar_t>(c1, c2);
}


bool apilib::chrequ_a(char c1, char c2)
{
  return apilib::__chrequ_x<char>(c1, c2);
}

bool apilib::chrequ_w(wchar_t c1, wchar_t c2)
{
  return apilib::__chrequ_x<wchar_t>(c1, c2);
}


bool apilib::chrequ_ea(char c1, char c2)
{
  return apilib::__chrequ_ex<char>(c1, c2);
}

bool apilib::chrequ_ew(wchar_t c1, wchar_t c2)
{
  return apilib::__chrequ_ex<wchar_t>(c1, c2);
}

///////////////////////////////////////////////////////////////////////////////////////////////////