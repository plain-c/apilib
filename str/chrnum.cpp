#include "../cfg.h"
#include "../include/lib.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "chrnum.h"
#include "chrnum.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////

int apilib::uchtochr_a(unsigned char digit, char* chr)
{
  return apilib::__ntochr_x<char, unsigned char>(digit, chr);
}

int apilib::uchtochr_w(unsigned char digit, wchar_t* chr)
{
  return apilib::__ntochr_x<wchar_t, unsigned char>(digit, chr);
}


int apilib::ushtochr_a(unsigned short digit, char* chr)
{
  return apilib::__ntochr_x<char, unsigned short>(digit, chr);
}

int apilib::ushtochr_w(unsigned short digit, wchar_t* chr)
{
  return apilib::__ntochr_x<wchar_t, unsigned short>(digit, chr);
}


int apilib::ultochr_a(unsigned long int digit, char* chr)
{
  return apilib::__ntochr_x<char, unsigned long int>(digit, chr);
}

int apilib::ultochr_w(unsigned long int digit, wchar_t* chr)
{
  return apilib::__ntochr_x<wchar_t, unsigned long int>(digit, chr);
}


int apilib::ulltochr_a(unsigned long long int digit, char* chr)
{
  return apilib::__ntochr_x<char, unsigned long long int>(digit, chr);
}

int apilib::ulltochr_w(unsigned long long int digit, wchar_t* chr)
{
  return apilib::__ntochr_x<wchar_t, unsigned long long int>(digit, chr);
}


int apilib::chtochr_a(char digit, char* chr)
{
  return apilib::__ntochr_x<char, char>(digit, chr);
}

int apilib::chtochr_w(char digit, wchar_t* chr)
{
  return apilib::__ntochr_x<wchar_t, char>(digit, chr);
}


int apilib::shtochr_a(short digit, char* chr)
{
  return apilib::__ntochr_x<char, short>(digit, chr);
}

int apilib::shtochr_w(short digit, wchar_t* chr)
{
  return apilib::__ntochr_x<wchar_t, short>(digit, chr);
}


int apilib::ltochr_a(long int digit, char* chr)
{
  return apilib::__ntochr_x<char, long int>(digit, chr);
}

int apilib::ltochr_w(long int digit, wchar_t* chr)
{
  return apilib::__ntochr_x<wchar_t, long int>(digit, chr);
}


int apilib::lltochr_a(long long int digit, char* chr)
{
  return apilib::__ntochr_x<char, long long int>(digit, chr);
}

int apilib::lltochr_w(long long int digit, wchar_t* chr)
{
  return apilib::__ntochr_x<wchar_t, long long int>(digit, chr);
}


int apilib::chrtouch_a(char chr, unsigned char* digit, int radix)
{
  return apilib::__chrton_x<char, unsigned char>(chr, digit, radix);
}

int apilib::chrtouch_w(wchar_t chr, unsigned char* digit, int radix)
{
  return apilib::__chrton_x<wchar_t, unsigned char>(chr, digit, radix);
}


int apilib::chrtoush_a(char chr, unsigned short* digit, int radix)
{
  return apilib::__chrton_x<char, unsigned short>(chr, digit, radix);
}

int apilib::chrtoush_w(wchar_t chr, unsigned short* digit, int radix)
{
  return apilib::__chrton_x<wchar_t, unsigned short>(chr, digit, radix);
}


int apilib::chrtoul_a(char chr, unsigned long int* digit, int radix)
{
  return apilib::__chrton_x<char, unsigned long int>(chr, digit, radix);
}

int apilib::chrtoul_w(wchar_t chr, unsigned long int* digit, int radix)
{
  return apilib::__chrton_x<wchar_t, unsigned long int>(chr, digit, radix);
}


int apilib::chrtoull_a(char chr, unsigned long long int* digit, int radix)
{
  return apilib::__chrton_x<char, unsigned long long int>(chr, digit, radix);
}

int apilib::chrtoull_w(wchar_t chr, unsigned long long int* digit, int radix)
{
  return apilib::__chrton_x<wchar_t, unsigned long long int>(chr, digit, radix);
}


int apilib::chrtoch_a(char chr, char* digit, int radix)
{
  return apilib::__chrton_x<char, char>(chr, digit, radix);
}

int apilib::chrtoch_w(wchar_t chr, char* digit, int radix)
{
  return apilib::__chrton_x<wchar_t, char>(chr, digit, radix);
}


int apilib::chrtosh_a(char chr, short* digit, int radix)
{
  return apilib::__chrton_x<char, short>(chr, digit, radix);
}

int apilib::chrtosh_w(wchar_t chr, short* digit, int radix)
{
  return apilib::__chrton_x<wchar_t, short>(chr, digit, radix);
}


int apilib::chrtol_a(char chr, long int* digit, int radix)
{
  return apilib::__chrton_x<char, long int>(chr, digit, radix);
}

int apilib::chrtol_w(wchar_t chr, long int* digit, int radix)
{
  return apilib::__chrton_x<wchar_t, long int>(chr, digit, radix);
}


int apilib::chrtoll_a(char chr, long long int* digit, int radix)
{
  return apilib::__chrton_x<char, long long int>(chr, digit, radix);
}

int apilib::chrtoll_w(wchar_t chr, long long int* digit, int radix)
{
  return apilib::__chrton_x<wchar_t, long long int>(chr, digit, radix);
}

///////////////////////////////////////////////////////////////////////////////////////////////////