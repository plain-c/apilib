#include "../cfg.h"
#include "../include/lib.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "chr.h"
#include "chr.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////

int apilib::isalnum_ea(char c)
{
  return apilib::__isalnum_ex<char>(c);
}

int apilib::isalnum_ew(wchar_t c)
{
  return apilib::__isalnum_ex<wchar_t>(c);
}


int apilib::isalpha_ea(char c)
{
  return apilib::__isalpha_ex<char>(c);
}

int apilib::isalpha_ew(wchar_t c)
{
  return apilib::__isalpha_ex<wchar_t>(c);
}


int apilib::isblank_ea(char c)
{
  return apilib::__isblank_ex<char>(c);
}

int apilib::isblank_ew(wchar_t c)
{
  return apilib::__isblank_ex<wchar_t>(c);
}


int apilib::isbreak_ea(char c)
{
  return apilib::__isbreak_ex<char>(c);
}

int apilib::isbreak_ew(wchar_t c)
{
  return apilib::__isbreak_ex<wchar_t>(c);
}


int apilib::iscntrl_ea(char c)
{
  return apilib::__iscntrl_ex<char>(c);
}

int apilib::iscntrl_ew(wchar_t c)
{
  return apilib::__iscntrl_ex<wchar_t>(c);
}


int apilib::isdigit_ea(char c)
{
  return apilib::__isdigit_ex<char>(c);
}

int apilib::isdigit_ew(wchar_t c)
{
  return apilib::__isdigit_ex<wchar_t>(c);
}


int apilib::isgraph_ea(char c)
{
  return apilib::__isgraph_ex<char>(c);
}

int apilib::isgraph_ew(wchar_t c)
{
  return apilib::__isgraph_ex<wchar_t>(c);
}


int apilib::islower_ea(char c)
{
  return apilib::__islower_ex<char>(c);
}

int apilib::islower_ew(wchar_t c)
{
  return apilib::__islower_ex<wchar_t>(c);
}


int apilib::isprint_ea(char c)
{
  return apilib::__isprint_ex<char>(c);
}

int apilib::isprint_ew(wchar_t c)
{
  return apilib::__isprint_ex<wchar_t>(c);
}


int apilib::ispunct_ea(char c)
{
  return apilib::__ispunct_ex<char>(c);
}

int apilib::ispunct_ew(wchar_t c)
{
  return apilib::__ispunct_ex<wchar_t>(c);
}


int apilib::isspace_ea(char c)
{
  return apilib::__isspace_ex<char>(c);
}

int apilib::isspace_ew(wchar_t c)
{
  return apilib::__isspace_ex<wchar_t>(c);
}


int apilib::isupper_ea(char c)
{
  return apilib::__isupper_ex<char>(c);
}

int apilib::isupper_ew(wchar_t c)
{
  return apilib::__isupper_ex<wchar_t>(c);
}


int apilib::isxdigit_ea(char c)
{
  return apilib::__isxdigit_ex<char>(c);
}

int apilib::isxdigit_ew(wchar_t c)
{
  return apilib::__isxdigit_ex<wchar_t>(c);
}


char apilib::tolower_ea(char c)
{
  return apilib::__tolower_ex<char>(c);
}

wchar_t apilib::tolower_ew(wchar_t c)
{
  return apilib::__tolower_ex<wchar_t>(c);
}


char apilib::toupper_ea(char c)
{
  return apilib::__toupper_ex<char>(c);
}

wchar_t apilib::toupper_ew(wchar_t c)
{
  return apilib::__toupper_ex<wchar_t>(c);
}

///////////////////////////////////////////////////////////////////////////////////////////////////