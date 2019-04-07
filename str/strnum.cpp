#include "../cfg.h"
#include "../include/lib.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "strnum.h"
#include "strnum.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////

unsigned char apilib::strtouch_a(const char* string, char** endptr, unsigned int radix)
{
  return apilib::__strton_x<char, unsigned char, 0>(string, endptr, radix);
}

unsigned char apilib::strtouch_w(const wchar_t* string, wchar_t** endptr, unsigned int radix)
{
  return apilib::__strton_x<wchar_t, unsigned char, 0>(string, endptr, radix);
}


unsigned char apilib::strtouch_na(const char* string, size_t length, char** endptr, unsigned int radix)
{
  return apilib::__strton_nx<char, unsigned char, 0>(string, length, endptr, radix);
}

unsigned char apilib::strtouch_nw(const wchar_t* string, size_t length, wchar_t** endptr, unsigned int radix)
{
  return apilib::__strton_nx<wchar_t, unsigned char, 0>(string, length, endptr, radix);
}


unsigned short apilib::strtoush_a(const char* string, char** endptr, unsigned int radix)
{
  return apilib::__strton_x<char, unsigned short, 0>(string, endptr, radix);
}

unsigned short apilib::strtoush_w(const wchar_t* string, wchar_t** endptr, unsigned int radix)
{
  return apilib::__strton_x<wchar_t, unsigned short, 0>(string, endptr, radix);
}


unsigned short apilib::strtoush_na(const char* string, size_t length, char** endptr, unsigned int radix)
{
  return apilib::__strton_nx<char, unsigned short, 0>(string, length, endptr, radix);
}

unsigned short apilib::strtoush_nw(const wchar_t* string, size_t length, wchar_t** endptr, unsigned int radix)
{
  return apilib::__strton_nx<wchar_t, unsigned short, 0>(string, length, endptr, radix);
}


unsigned long int apilib::strtoul_a(const char* string, char** endptr, unsigned int radix)
{
  return apilib::__strton_x<char, unsigned long int, 0>(string, endptr, radix);
}

unsigned long int apilib::strtoul_w(const wchar_t* string, wchar_t** endptr, unsigned int radix)
{
  return apilib::__strton_x<wchar_t, unsigned long int, 0>(string, endptr, radix);
}


unsigned long int apilib::strtoul_na(const char* string, size_t length, char** endptr, unsigned int radix)
{
  return apilib::__strton_nx<char, unsigned long int, 0>(string, length, endptr, radix);
}

unsigned long int apilib::strtoul_nw(const wchar_t* string, size_t length, wchar_t** endptr, unsigned int radix)
{
  return apilib::__strton_nx<wchar_t, unsigned long int, 0>(string, length, endptr, radix);
}


unsigned long long int apilib::strtoull_a(const char* string, char** endptr, unsigned int radix)
{
  return apilib::__strton_x<char, unsigned long long int, 0>(string, endptr, radix);
}

unsigned long long int apilib::strtoull_w(const wchar_t* string, wchar_t** endptr, unsigned int radix)
{
  return apilib::__strton_x<wchar_t, unsigned long long int, 0>(string, endptr, radix);
}


unsigned long long int apilib::strtoull_na(const char* string, size_t length, char** endptr, unsigned int radix)
{
  return apilib::__strton_nx<char, unsigned long long int, 0>(string, length, endptr, radix);
}

unsigned long long int apilib::strtoull_nw(const wchar_t* string, size_t length, wchar_t** endptr, unsigned int radix)
{
  return apilib::__strton_nx<wchar_t, unsigned long long int, 0>(string, length, endptr, radix);
}


char apilib::strtoch_a(const char* string, char** endptr, unsigned  int radix)
{
  return static_cast<char>(apilib::__strton_x<char, unsigned char, 1>(string, endptr, radix));
}

char apilib::strtoch_w(const wchar_t* string, wchar_t** endptr, unsigned int radix)
{
  return static_cast<char>(apilib::__strton_x<wchar_t, unsigned char, 1>(string, endptr, radix));
}


char apilib::strtoch_na(const char* string, size_t length, char** endptr, unsigned int radix)
{
  return static_cast<char>(apilib::__strton_nx<char, unsigned char, 1>(string, length, endptr, radix));
}

char apilib::strtoch_nw(const wchar_t* string, size_t length, wchar_t** endptr, unsigned int radix)
{
  return static_cast<char>(apilib::__strton_nx<wchar_t, unsigned char, 1>(string, length, endptr, radix));
}


short apilib::strtosh_a(const char* string, char** endptr, unsigned  int radix)
{
  return static_cast<short>(apilib::__strton_x<char, unsigned short, 1>(string, endptr, radix));
}

short apilib::strtosh_w(const wchar_t* string, wchar_t** endptr, unsigned int radix)
{
  return static_cast<short>(apilib::__strton_x<wchar_t, unsigned short, 1>(string, endptr, radix));
}


short apilib::strtosh_na(const char* string, size_t length, char** endptr, unsigned int radix)
{
  return static_cast<short>(apilib::__strton_nx<char, unsigned short, 1>(string, length, endptr, radix));
}

short apilib::strtosh_nw(const wchar_t* string, size_t length, wchar_t** endptr, unsigned int radix)
{
  return static_cast<short>(apilib::__strton_nx<wchar_t, unsigned short, 1>(string, length, endptr, radix));
}


long int apilib::strtol_a(const char* string, char** endptr, unsigned int radix)
{
  return static_cast<long int>(apilib::__strton_x<char, unsigned long int, 1>(string, endptr, radix));
}

long int apilib::strtol_w(const wchar_t* string, wchar_t** endptr, unsigned int radix)
{
  return static_cast<long int>(apilib::__strton_x<wchar_t, unsigned long int, 1>(string, endptr, radix));
}


long int apilib::strtol_na(const char* string, size_t length, char** endptr, unsigned int radix)
{
  return static_cast<long int>(apilib::__strton_nx<char, unsigned long int, 1>(string, length, endptr, radix));
}

long int apilib::strtol_nw(const wchar_t* string, size_t length, wchar_t** endptr, unsigned int radix)
{
  return static_cast<long int>(apilib::__strton_nx<wchar_t, unsigned long int, 1>(string, length, endptr, radix));
}


long long int apilib::strtoll_a(const char* string, char** endptr, unsigned int radix)
{
  return static_cast<long long int>(apilib::__strton_x<char, unsigned long long int, 1>(string, endptr, radix));
}

long long int apilib::strtoll_w(const wchar_t* string, wchar_t** endptr, unsigned int radix)
{
  return static_cast<long long int>(apilib::__strton_x<wchar_t, unsigned long long int, 1>(string, endptr, radix));
}


long long int apilib::strtoll_na(const char* string, size_t length, char** endptr, unsigned int radix)
{
  return static_cast<long long int>(apilib::__strton_nx<char, unsigned long long int, 1>(string, length, endptr, radix));
}

long long int apilib::strtoll_nw(const wchar_t* string, size_t length, wchar_t** endptr, unsigned int radix)
{
  return static_cast<long long int>(apilib::__strton_nx<wchar_t, unsigned long long int, 1>(string, length, endptr, radix));
}


size_t apilib::uchtostr_a(unsigned char number, char* string, unsigned int radix, int prefix)
{
  return apilib::__ntostr_x<char, unsigned char, 0>(number, string, radix, prefix);
}

size_t apilib::uchtostr_w(unsigned char number, wchar_t* string, unsigned int radix, int prefix)
{
  return apilib::__ntostr_x<wchar_t, unsigned char, 0>(number, string, radix, prefix);
}


size_t apilib::uchtostr_la(unsigned char number, char* string, size_t maxlen, unsigned int radix, int prefix)
{
  return apilib::__ntostr_lx<char, unsigned char, 0>(number, string, maxlen, radix, prefix);
}

size_t apilib::uchtostr_lw(unsigned char number, wchar_t* string, size_t maxlen, unsigned int radix, int prefix)
{
  return apilib::__ntostr_lx<wchar_t, unsigned char, 0>(number, string, maxlen, radix, prefix);
}


size_t apilib::ushtostr_a(unsigned short number, char* string, unsigned int radix, int prefix)
{
  return apilib::__ntostr_x<char, unsigned short, 0>(number, string, radix, prefix);
}

size_t apilib::ushtostr_w(unsigned short number, wchar_t* string, unsigned int radix, int prefix)
{
  return apilib::__ntostr_x<wchar_t, unsigned short, 0>(number, string, radix, prefix);
}


size_t apilib::ushtostr_la(unsigned short number, char* string, size_t maxlen, unsigned int radix, int prefix)
{
  return apilib::__ntostr_lx<char, unsigned short, 0>(number, string, maxlen, radix, prefix);
}

size_t apilib::ushtostr_lw(unsigned short number, wchar_t* string, size_t maxlen, unsigned int radix, int prefix)
{
  return apilib::__ntostr_lx<wchar_t, unsigned short, 0>(number, string, maxlen, radix, prefix);
}


size_t apilib::ultostr_a(unsigned long int number, char* string, unsigned int radix, int prefix)
{
  return apilib::__ntostr_x<char, unsigned long int, 0>(number, string, radix, prefix);
}

size_t apilib::ultostr_w(unsigned long int number, wchar_t* string, unsigned int radix, int prefix)
{
  return apilib::__ntostr_x<wchar_t, unsigned long int, 0>(number, string, radix, prefix);
}


size_t apilib::ultostr_la(unsigned long int number, char* string, size_t maxlen, unsigned int radix, int prefix)
{
  return apilib::__ntostr_lx<char, unsigned long int, 0>(number, string, maxlen, radix, prefix);
}

size_t apilib::ultostr_lw(unsigned long int number, wchar_t* string, size_t maxlen, unsigned int radix, int prefix)
{
  return apilib::__ntostr_lx<wchar_t, unsigned long int, 0>(number, string, maxlen, radix, prefix);
}


size_t apilib::ulltostr_a(unsigned long long int number, char* string, unsigned int radix, int prefix)
{
  return apilib::__ntostr_x<char, unsigned long long int, 0>(number, string, radix, prefix);
}

size_t apilib::ulltostr_w(unsigned long long int number, wchar_t* string, unsigned int radix, int prefix)
{
  return apilib::__ntostr_x<wchar_t, unsigned long long int, 0>(number, string, radix, prefix);
}


size_t apilib::ulltostr_la(unsigned long long int number, char* string, size_t maxlen, unsigned int radix, int prefix)
{
  return apilib::__ntostr_lx<char, unsigned long long int, 0>(number, string, maxlen, radix, prefix);
}

size_t apilib::ulltostr_lw(unsigned long long int number, wchar_t* string, size_t maxlen, unsigned int radix, int prefix)
{
  return apilib::__ntostr_lx<wchar_t, unsigned long long int, 0>(number, string, maxlen, radix, prefix);
}


size_t apilib::chtostr_a(char number, char* string, unsigned int radix, int prefix)
{
  return apilib::__ntostr_x<char, unsigned char, 1>(static_cast<unsigned char>(number), string, radix, prefix);
}

size_t apilib::chtostr_w(char number, wchar_t* string, unsigned int radix, int prefix)
{
  return apilib::__ntostr_x<wchar_t, unsigned char, 1>(static_cast<unsigned char>(number), string, radix, prefix);
}


size_t apilib::chtostr_la(char number, char* string, size_t maxlen, unsigned int radix, int prefix)
{
  return apilib::__ntostr_lx<char, unsigned char, 1>(static_cast<unsigned char>(number), string, maxlen, radix, prefix);
}

size_t apilib::chtostr_lw(char number, wchar_t* string, size_t maxlen, unsigned int radix, int prefix)
{
  return apilib::__ntostr_lx<wchar_t, unsigned char, 1>(static_cast<unsigned char>(number), string, maxlen, radix, prefix);
}


size_t apilib::shtostr_a(short number, char* string, unsigned int radix, int prefix)
{
  return apilib::__ntostr_x<char, unsigned short, 1>(static_cast<unsigned short>(number), string, radix, prefix);
}

size_t apilib::shtostr_w(short number, wchar_t* string, unsigned int radix, int prefix)
{
  return apilib::__ntostr_x<wchar_t, unsigned short, 1>(static_cast<unsigned short>(number), string, radix, prefix);
}


size_t apilib::shtostr_la(short number, char* string, size_t maxlen, unsigned int radix, int prefix)
{
  return apilib::__ntostr_lx<char, unsigned short, 1>(static_cast<unsigned short>(number), string, maxlen, radix, prefix);
}

size_t apilib::shtostr_lw(short number, wchar_t* string, size_t maxlen, unsigned int radix, int prefix)
{
  return apilib::__ntostr_lx<wchar_t, unsigned short, 1>(static_cast<unsigned short>(number), string, maxlen, radix, prefix);
}


size_t apilib::ltostr_a(long int number, char* string, unsigned int radix, int prefix)
{
  return apilib::__ntostr_x<char, unsigned long int, 1>(static_cast<unsigned long int>(number), string, radix, prefix);
}

size_t apilib::ltostr_w(long int number, wchar_t* string, unsigned int radix, int prefix)
{
  return apilib::__ntostr_x<wchar_t, unsigned long int, 1>(static_cast<unsigned long int>(number), string, radix, prefix);
}


size_t apilib::ltostr_la(long int number, char* string, size_t maxlen, unsigned int radix, int prefix)
{
  return apilib::__ntostr_lx<char, unsigned long int, 1>(static_cast<unsigned long int>(number), string, maxlen, radix, prefix);
}

size_t apilib::ltostr_lw(long int number, wchar_t* string, size_t maxlen, unsigned int radix, int prefix)
{
  return apilib::__ntostr_lx<wchar_t, unsigned long int, 1>(static_cast<unsigned long int>(number), string, maxlen, radix, prefix);
}


size_t apilib::lltostr_a(long long int number, char* string, unsigned int radix, int prefix)
{
  return apilib::__ntostr_x<char, unsigned long long int, 1>(static_cast<unsigned long long int>(number), string, radix, prefix);
}

size_t apilib::lltostr_w(long long int number, wchar_t* string, unsigned int radix, int prefix)
{
  return apilib::__ntostr_x<wchar_t, unsigned long long int, 1>(static_cast<unsigned long long int>(number), string, radix, prefix);
}


size_t apilib::lltostr_la(long long int number, char* string, size_t maxlen, unsigned int radix, int prefix)
{
  return apilib::__ntostr_lx<char, unsigned long long int, 1>(static_cast<unsigned long long int>(number), string, maxlen, radix, prefix);
}

size_t apilib::lltostr_lw(long long int number, wchar_t* string, size_t maxlen, unsigned int radix, int prefix)
{
  return apilib::__ntostr_lx<wchar_t, unsigned long long int, 1>(static_cast<unsigned long long int>(number), string, maxlen, radix, prefix);
}

///////////////////////////////////////////////////////////////////////////////////////////////////