#ifndef strnum_h__
#define strnum_h__

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  //TODO: добавить atol, wtol...

  /**
   * Конвертация строки в число.
   *
   * @param string  строка.
   * @param endptr  первый символ прервавший конвертацию строки в число.
   * @param radix   числовая основа от 2 до 35, для автоматического определения 0.
   *
   * @return  в случае успеха - число,
   *          в случае ошибки - 0.
  */
  unsigned char strtouch_a(const char* string, char** endptr, unsigned int radix);
  unsigned char strtouch_w(const wchar_t* string, wchar_t** endptr, unsigned int radix);

  /**
   * Конвертация строки в число.
   *
   * @param string  строка.
   * @param length  длина строки без учета нулевого символа.
   * @param endptr  первый символ прервавший конвертацию строки в число.
   * @param radix   числовая основа от 2 до 35, для автоматического определения 0.
   *
   * @return  в случае успеха - число,
   *          в случае ошибки - 0.
  */
  unsigned char strtouch_na(const char* string, size_t length, char** endptr, unsigned int radix);
  unsigned char strtouch_nw(const wchar_t* string, size_t length, wchar_t** endptr, unsigned int radix);

  /**
   * Конвертация строки в число.
   *
   * @param string  строка.
   * @param endptr  первый символ прервавший конвертацию строки в число.
   * @param radix   числовая основа от 2 до 35, для автоматического определения 0.
   *
   * @return  в случае успеха - число,
   *          в случае ошибки - 0.
  */
  char strtoch_a(const char* string, char** endptr, unsigned int radix);
  char strtoch_w(const wchar_t* string, wchar_t** endptr, unsigned int radix);

  /**
   * Конвертация строки в число.
   *
   * @param string  строка.
   * @param length  длина строки без учета нулевого символа.
   * @param endptr  первый символ прервавший конвертацию строки в число.
   * @param radix   числовая основа от 2 до 35, для автоматического определения 0.
   *
   * @return  в случае успеха - число,
   *          в случае ошибки - 0.
  */
  char strtoch_na(const char* string, size_t length, char** endptr, unsigned int radix);
  char strtoch_nw(const wchar_t* string, size_t length, wchar_t** endptr, unsigned int radix);

  /**
   * Конвертация строки в число.
   *
   * @param string  строка.
   * @param endptr  первый символ прервавший конвертацию строки в число.
   * @param radix   числовая основа от 2 до 35, для автоматического определения 0.
   *
   * @return  в случае успеха - число,
   *          в случае ошибки - 0.
  */
  unsigned short strtoush_a(const char* string, char** endptr, unsigned int radix);
  unsigned short strtoush_w(const wchar_t* string, wchar_t** endptr, unsigned int radix);

  /**
   * Конвертация строки в число.
   *
   * @param string  строка.
   * @param length  длина строки без учета нулевого символа.
   * @param endptr  первый символ прервавший конвертацию строки в число.
   * @param radix   числовая основа от 2 до 35, для автоматического определения 0.
   *
   * @return  в случае успеха - число,
   *          в случае ошибки - 0.
  */
  unsigned short strtoush_na(const char* string, size_t length, char** endptr, unsigned int radix);
  unsigned short strtoush_nw(const wchar_t* string, size_t length, wchar_t** endptr, unsigned int radix);

  /**
   * Конвертация строки в число.
   *
   * @param string  строка.
   * @param endptr  первый символ прервавший конвертацию строки в число.
   * @param radix   числовая основа от 2 до 35, для автоматического определения 0.
   *
   * @return  в случае успеха - число,
   *          в случае ошибки - 0.
  */
  short strtosh_a(const char* string, char** endptr, unsigned int radix);
  short strtosh_w(const wchar_t* string, wchar_t** endptr, unsigned int radix);

  /**
   * Конвертация строки в число.
   *
   * @param string  строка.
   * @param length  длина строки без учета нулевого символа.
   * @param endptr  первый символ прервавший конвертацию строки в число.
   * @param radix   числовая основа от 2 до 35, для автоматического определения 0.
   *
   * @return  в случае успеха - число,
   *          в случае ошибки - 0.
  */
  short strtosh_na(const char* string, size_t length, char** endptr, unsigned int radix);
  short strtosh_nw(const wchar_t* string, size_t length, wchar_t** endptr, unsigned int radix);

  /**
   * Конвертация строки в число.
   *
   * @param string  строка.
   * @param endptr  первый символ прервавший конвертацию строки в число.
   * @param radix   числовая основа от 2 до 35, для автоматического определения 0.
   *
   * @return  в случае успеха - число,
   *          в случае ошибки - 0.
  */
  unsigned long int strtoul_a(const char* string, char** endptr, unsigned int radix);
  unsigned long int strtoul_w(const wchar_t* string, wchar_t** endptr, unsigned int radix);

  /**
   * Конвертация строки в число.
   *
   * @param string  строка.
   * @param length  длина строки без учета нулевого символа.
   * @param endptr  первый символ прервавший конвертацию строки в число.
   * @param radix   числовая основа от 2 до 35, для автоматического определения 0.
   *
   * @return  в случае успеха - число,
   *          в случае ошибки - 0.
  */
  unsigned long int strtoul_na(const char* string, size_t length, char** endptr, unsigned int radix);
  unsigned long int strtoul_nw(const wchar_t* string, size_t length, wchar_t** endptr, unsigned int radix);

  /**
   * Конвертация строки в число.
   *
   * @param string  строка.
   * @param endptr  первый символ прервавший конвертацию строки в число.
   * @param radix   числовая основа от 2 до 35, для автоматического определения 0.
   *
   * @return  в случае успеха - число,
   *          в случае ошибки - 0.
  */
  long int strtol_a(const char* string, char** endptr, unsigned int radix);
  long int strtol_w(const wchar_t* string, wchar_t** endptr, unsigned int radix);

  /**
   * Конвертация строки в число.
   *
   * @param string  строка.
   * @param length  длина строки без учета нулевого символа.
   * @param endptr  первый символ прервавший конвертацию строки в число.
   * @param radix   числовая основа от 2 до 35, для автоматического определения 0.
   *
   * @return  в случае успеха - число,
   *          в случае ошибки - 0.
  */
  long int strtol_na(const char* string, size_t length, char** endptr, unsigned int radix);
  long int strtol_nw(const wchar_t* string, size_t length, wchar_t** endptr, unsigned int radix);

  /**
   * Конвертация строки в число.
   *
   * @param string  строка.
   * @param endptr  первый символ прервавший конвертацию строки в число.
   * @param radix   числовая основа от 2 до 35, для автоматического определения 0.
   *
   * @return  в случае успеха - число,
   *          в случае ошибки - 0.
  */
  unsigned long long int strtoull_a(const char* string, char** endptr, unsigned int radix);
  unsigned long long int strtoull_w(const wchar_t* string, wchar_t** endptr, unsigned int radix);

  /**
   * Конвертация строки в число.
   *
   * @param string  строка.
   * @param length  длина строки без учета нулевого символа.
   * @param endptr  первый символ прервавший конвертацию строки в число.
   * @param radix   числовая основа от 2 до 35, для автоматического определения 0.
   *
   * @return  в случае успеха - число,
   *          в случае ошибки - 0.
  */
  unsigned long long int strtoull_na(const char* string, size_t length, char** endptr, unsigned int radix);
  unsigned long long int strtoull_nw(const wchar_t* string, size_t length, wchar_t** endptr, unsigned int radix);

  /**
   * Конвертация строки в число.
   *
   * @param string  строка.
   * @param endptr  первый символ прервавший конвертацию строки в число.
   * @param radix   числовая основа от 2 до 35, для автоматического определения 0.
   *
   * @return  в случае успеха - число,
   *          в случае ошибки - 0.
  */
  long long int strtoll_a(const char* string, char** endptr, unsigned int radix);
  long long int strtoll_w(const wchar_t* string, wchar_t** endptr, unsigned int radix);

  /**
   * Конвертация строки в число.
   *
   * @param string  строка.
   * @param length  длина строки без учета нулевого символа.
   * @param endptr  первый символ прервавший конвертацию строки в число.
   * @param radix   числовая основа от 2 до 35, для автоматического определения 0.
   *
   * @return  в случае успеха - число,
   *          в случае ошибки - 0.
  */
  long long int strtoll_na(const char* string, size_t length, char** endptr, unsigned int radix);
  long long int strtoll_nw(const wchar_t* string, size_t length, wchar_t** endptr, unsigned int radix);

  /**
   * Конвертация числа в строку.
   *
   * @param number  число.
   * @param string  строка.
   * @param radix   числовая основа от 2 до 35, по умолчанию при 0 как десятичная основа.
   *
   * @return  требуемая длина строки без учета нулевого символа,
   *          в случае ошибки 0.
  */
  size_t uchtostr_a(unsigned char number, char* string, unsigned int radix, int prefix);
  size_t uchtostr_w(unsigned char number, wchar_t* string, unsigned int radix, int prefix);

  /**
   * Конвертация числа в строку.
   *
   * @param number  число.
   * @param string  строка.
   * @param maxlen  максимальная длина строки с учетом нулевого символа.
   * @param radix   числовая основа от 2 до 35, по умолчанию при 0 как десятичная основа.
   *
   * @return  требуемая длина строки без учета нулевого символа,
   *          требуемая длина равная или больше maxlen означает недостаточный размер буфера,
   *          в случае ошибки 0.
  */
  size_t uchtostr_la(unsigned char number, char* string, size_t maxlen, unsigned int radix, int prefix);
  size_t uchtostr_lw(unsigned char number, wchar_t* string, size_t maxlen, unsigned int radix, int prefix);

  /**
   * Конвертация числа в строку.
   *
   * @param number  число.
   * @param string  строка.
   * @param radix   числовая основа от 2 до 35, по умолчанию при 0 как десятичная основа.
   *
   * @return  требуемая длина строки без учета нулевого символа,
   *          в случае ошибки 0.
  */
  size_t chtostr_a(char number, char* string, unsigned int radix, int prefix);
  size_t chtostr_w(char number, wchar_t* string, unsigned int radix, int prefix);

  /**
   * Конвертация числа в строку.
   *
   * @param number  число.
   * @param string  строка.
   * @param maxlen  максимальная длина строки с учетом нулевого символа.
   * @param radix   числовая основа от 2 до 35, по умолчанию при 0 как десятичная основа.
   *
   * @return  требуемая длина строки без учета нулевого символа,
   *          требуемая длина равная или больше maxlen означает недостаточный размер буфера,
   *          в случае ошибки 0.
  */
  size_t chtostr_la(char number, char* string, size_t maxlen, unsigned int radix, int prefix);
  size_t chtostr_lw(char number, wchar_t* string, size_t maxlen, unsigned int radix, int prefix);

  /**
   * Конвертация числа в строку.
   *
   * @param number  число.
   * @param string  строка.
   * @param radix   числовая основа от 2 до 35, по умолчанию при 0 как десятичная основа.
   *
   * @return  требуемая длина строки без учета нулевого символа,
   *          в случае ошибки 0.
  */
  size_t ushtostr_a(unsigned short number, char* string, unsigned int radix, int prefix);
  size_t ushtostr_w(unsigned short number, wchar_t* string, unsigned int radix, int prefix);

  /**
   * Конвертация числа в строку.
   *
   * @param number  число.
   * @param string  строка.
   * @param maxlen  максимальная длина строки с учетом нулевого символа.
   * @param radix   числовая основа от 2 до 35, по умолчанию при 0 как десятичная основа.
   *
   * @return  требуемая длина строки без учета нулевого символа,
   *          требуемая длина равная или больше maxlen означает недостаточный размер буфера,
   *          в случае ошибки 0.
  */
  size_t ushtostr_la(unsigned short number, char* string, size_t maxlen, unsigned int radix, int prefix);
  size_t ushtostr_lw(unsigned short number, wchar_t* string, size_t maxlen, unsigned int radix, int prefix);

  /**
   * Конвертация числа в строку.
   *
   * @param number  число.
   * @param string  строка.
   * @param radix   числовая основа от 2 до 35, по умолчанию при 0 как десятичная основа.
   *
   * @return  требуемая длина строки без учета нулевого символа,
   *          в случае ошибки 0.
  */
  size_t shtostr_a(short number, char* string, unsigned int radix, int prefix);
  size_t shtostr_w(short number, wchar_t* string, unsigned int radix, int prefix);

  /**
   * Конвертация числа в строку.
   *
   * @param number  число.
   * @param string  строка.
   * @param maxlen  максимальная длина строки с учетом нулевого символа.
   * @param radix   числовая основа от 2 до 35, по умолчанию при 0 как десятичная основа.
   *
   * @return  требуемая длина строки без учета нулевого символа,
   *          требуемая длина равная или больше maxlen означает недостаточный размер буфера,
   *          в случае ошибки 0.
  */
  size_t shtostr_la(short number, char* string, size_t maxlen, unsigned int radix, int prefix);
  size_t shtostr_lw(short number, wchar_t* string, size_t maxlen, unsigned int radix, int prefix);

  /**
   * Конвертация числа в строку.
   *
   * @param number  число.
   * @param string  строка.
   * @param radix   числовая основа от 2 до 35, по умолчанию при 0 как десятичная основа.
   *
   * @return  требуемая длина строки без учета нулевого символа,
   *          в случае ошибки 0.
  */
  size_t ultostr_a(unsigned long int number, char* string, unsigned int radix, int prefix);
  size_t ultostr_w(unsigned long int number, wchar_t* string, unsigned int radix, int prefix);

  /**
   * Конвертация числа в строку.
   *
   * @param number  число.
   * @param string  строка.
   * @param maxlen  максимальная длина строки с учетом нулевого символа.
   * @param radix   числовая основа от 2 до 35, по умолчанию при 0 как десятичная основа.
   *
   * @return  требуемая длина строки без учета нулевого символа,
   *          требуемая длина равная или больше maxlen означает недостаточный размер буфера,
   *          в случае ошибки 0.
  */
  size_t ultostr_la(unsigned long int number, char* string, size_t maxlen, unsigned int radix, int prefix);
  size_t ultostr_lw(unsigned long int number, wchar_t* string, size_t maxlen, unsigned int radix, int prefix);

  /**
   * Конвертация числа в строку.
   *
   * @param number  число.
   * @param string  строка.
   * @param radix   числовая основа от 2 до 35, по умолчанию при 0 как десятичная основа.
   *
   * @return  требуемая длина строки без учета нулевого символа,
   *          в случае ошибки 0.
  */
  size_t ltostr_a(long int number, char* string, unsigned int radix, int prefix);
  size_t ltostr_w(long int number, wchar_t* string, unsigned int radix, int prefix);

  /**
   * Конвертация числа в строку.
   *
   * @param number  число.
   * @param string  строка.
   * @param maxlen  максимальная длина строки с учетом нулевого символа.
   * @param radix   числовая основа от 2 до 35, по умолчанию при 0 как десятичная основа.
   *
   * @return  требуемая длина строки без учета нулевого символа,
   *          требуемая длина равная или больше maxlen означает недостаточный размер буфера,
   *          в случае ошибки 0.
  */
  size_t ltostr_la(long int number, char* string, size_t maxlen, unsigned int radix, int prefix);
  size_t ltostr_lw(long int number, wchar_t* string, size_t maxlen, unsigned int radix, int prefix);

  /**
   * Конвертация числа в строку.
   *
   * @param number  число.
   * @param string  строка.
   * @param radix   числовая основа от 2 до 35, по умолчанию при 0 как десятичная основа.
   *
   * @return  требуемая длина строки без учета нулевого символа,
   *          в случае ошибки 0.
  */
  size_t ulltostr_a(unsigned long long int number, char* string, unsigned int radix, int prefix);
  size_t ulltostr_w(unsigned long long int number, wchar_t* string, unsigned int radix, int prefix);

  /**
   * Конвертация числа в строку.
   *
   * @param number  число.
   * @param string  строка.
   * @param maxlen  максимальная длина строки с учетом нулевого символа.
   * @param radix   числовая основа от 2 до 35, по умолчанию при 0 как десятичная основа.
   *
   * @return  требуемая длина строки без учета нулевого символа,
   *          требуемая длина равная или больше maxlen означает недостаточный размер буфера,
   *          в случае ошибки 0.
  */
  size_t ulltostr_la(unsigned long long int number, char* string, size_t maxlen, unsigned int radix, int prefix);
  size_t ulltostr_lw(unsigned long long int number, wchar_t* string, size_t maxlen, unsigned int radix, int prefix);

  /**
   * Конвертация числа в строку.
   *
   * @param number  число.
   * @param string  строка.
   * @param radix   числовая основа от 2 до 35, по умолчанию при 0 как десятичная основа.
   *
   * @return  требуемая длина строки без учета нулевого символа,
   *          в случае ошибки 0.
  */
  size_t lltostr_a(long long int number, char* string, unsigned int radix, int prefix);
  size_t lltostr_w(long long int number, wchar_t* string, unsigned int radix, int prefix);

  /**
   * Конвертация числа в строку.
   *
   * @param number  число.
   * @param string  строка.
   * @param maxlen  максимальная длина строки с учетом нулевого символа.
   * @param radix   числовая основа от 2 до 35, по умолчанию при 0 как десятичная основа.
   *
   * @return  требуемая длина строки без учета нулевого символа,
   *          требуемая длина равная или больше maxlen означает недостаточный размер буфера,
   *          в случае ошибки 0.
  */
  size_t lltostr_la(long long int number, char* string, size_t maxlen, unsigned int radix, int prefix);
  size_t lltostr_lw(long long int number, wchar_t* string, size_t maxlen, unsigned int radix, int prefix);

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // strnum_h__