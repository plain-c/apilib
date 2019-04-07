#ifndef strnum_hpp__
#define strnum_hpp__

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "chr.hpp"
#include "chrnum.hpp"
#include "strlen.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Завершение конвертации строки с ошибкой.
   * Вспомогательная процедура для __strton_x() и __strton_nx().
   *
   * @param string  строка.
   * @param endptr  символ прервавший конвертацию строки в число.
   *
   * @return  0 во всех случаях как индикатор ошибки.
  */
  template<typename X, typename Y>
  inline Y __strton_exit_x(const X* string, X** endptr)
  {
    if(endptr)
    {
      *endptr = (X*)string;
    }
    return 0;
  }

  /**
   * Конвертация строки в число через битовый сдвиг для числовых основ 2, 4, 8, 16, 32.
   * Вспомогательная процедура для __strton_x().
   *
   * Параметр SHIFT определяет длину сдвига, который указывается вручную:
   *   1) Числовая основа  2 -> сдвиг на 1.
   *   2) Числовая основа  4 -> сдвиг на 2.
   *   3) Числовая основа  8 -> сдвиг на 3.
   *   4) Числовая основа 16 -> сдвиг на 4.
   *   5) Числовая основа 32 -> сдвиг на 5.
   *
   * @param string  строка.
   * @param endptr  первый символ прервавший конвертацию символов в число.
   * @param radix   числовая основа.
   * @param limit   максимально допустимое значение числа со знаком, 0 для беззнаковых чисел.
   *
   * @return  в случае успеха - число,
   *          в случае ошибки - 0.
  */
  template<typename X, typename Y, int SHIFT>
  inline Y __strton_lsh_x(const X* string, X** endptr, unsigned int radix, Y limit)
  {
    Y value = 0;

    if(string)
    {
      while(1)
      {
        Y digit;

        if(__chrton_x<X, Y>(*string, &digit, radix) == 0)
        {
          break;
        }

        Y temp = ((value << SHIFT) + digit);
        if(limit != 0 && temp > limit)break;
        if(value != 0 && temp < value)break;
        if(value != 0 && temp < value + radix)break;
        value = temp;

        string += 1;
      }
    }

    if(endptr)
    {
      *endptr = (X*)string;
    }

    return value;
  }

  /**
   * Конвертация строки в число через умножение для любых числовых основ.
   * Вспомогательная процедура для __strton_x().
   *
   * @param string  строка.
   * @param endptr  первый символ прервавший конвертацию символов в число.
   * @param radix   числовая основа.
   * @param limit   максимально допустимое значение числа со знаком, 0 для беззнаковых чисел.
   *
   * @return  в случае успеха - число,
   *          в случае ошибки - 0.
  */
  template<typename X, typename Y>
  inline Y __strton_mul_x(const X* string, X** endptr, unsigned int radix, Y limit)
  {
    Y value = 0;

    if(string)
    {
      while(1)
      {
        Y digit;

        if(__chrton_x<X, Y>(*string, &digit, radix) == 0)
        {
          break;
        }

        Y temp = ((value * radix) + digit);
        if(limit != 0 && temp > limit)break;
        if(value != 0 && temp < value)break;
        if(value != 0 && temp < value + radix)break;
        value = temp;

        string += 1;
      }
    }

    if(endptr)
    {
      *endptr = (X*)string;
    }

    return value;
  }

  /**
   * [Шаблон] Конвертация строки в число.
   *
   * @param string  строка.
   * @param endptr  первый символ прервавший конвертацию строки в число.
   * @param radix   числовая основа от 2 до 36, для автоматического определения 0.
   *
   * @return  в случае успеха - число,
   *          в случае ошибки - 0.
  */
  template<typename X, typename Y, int IS_SIGNED>
  inline Y __strton_x(const X* string, X** endptr, unsigned int radix)
  {
    /** 
     * Проверка корректности аргументов.
    */
    if(!string || ((radix != 0) && (radix < 2 || radix > 36)))
    {
      return __strton_exit_x<X, Y>(string, endptr);
    }

    /**
     * Игнорирование пробельных символов.
    */
    while(__isblank_ex<X>(*string) != 0)
    {
      string += 1;
    }

    int negative = 0;

    /**
     * Чтение знака (наличие знака минуса считается ошибкой для беззнаковых типов).
    */
    if(IS_SIGNED != 0)
    {
      switch(*string)
      {
      case '+': negative = 0; string += 1; break;
      case '-': negative = 1; string += 1; break;
      }
    }
    else
    {
      switch(*string)
      {
      case '+': negative = 0; string += 1; break;
      case '-': return __strton_exit_x<X, Y>(string, endptr);
      }
    }

    /** 
     * Игнорирование пробельных символов.
    */
    while(__isblank_ex<X>(*string) != 0)
    {
      string += 1;
    }

    /**
     * Чтение префикса, где возможны три варианта:
     *
     * 1) Префикс шестнадцатеричного числа '0xN...N'.
     * 2) Префикс восьмиричного числа '0N...N'
     * 3) Префикс отсутсвует и число является самостоятельным нулем.
    */
    if(string[0] == '0')
    {
      X chr = string[1];

      if(chr == 'x' || chr == 'X')
      {
        if     (radix ==  0)radix = 16;
        else if(radix != 16)return __strton_exit_x<X, Y>(string, endptr);
        string += 2;
      }
      else if(chr >= '0' && chr <= '7')
      {
        if     (radix == 0)radix = 8;
        else if(radix != 8)return __strton_exit_x<X, Y>(string, endptr);
        string += 1;
      }
    }

    /**
     * По умолчанию числовая основа считается десятичной.
    */
    if(radix == 0)
    {
      radix = 10;
    }

    Y limit = 0;

    /**
     * Определение лимита требуемого для корректной конвертации знаковых типов.
    */
    if(IS_SIGNED != 0)
    {
      if(negative == 0)
      {
        limit = (((Y)-1) >> 1);
      }
      else
      {
        limit = (((Y)-1) >> 1) + 1;
      }
    }

    Y value = 0;

    /**
     * Чтение числа.
    */
    switch(radix)
    {
    case  2: value = __strton_lsh_x<X, Y, 1>(string, endptr, radix, limit); break;
    case  4: value = __strton_lsh_x<X, Y, 2>(string, endptr, radix, limit); break;
    case  8: value = __strton_lsh_x<X, Y, 3>(string, endptr, radix, limit); break;
    case 16: value = __strton_lsh_x<X, Y, 4>(string, endptr, radix, limit); break;
    case 32: value = __strton_lsh_x<X, Y, 5>(string, endptr, radix, limit); break;
    default: value = __strton_mul_x<X, Y   >(string, endptr, radix, limit); break;
    }

    return ((negative != 0) ? (~value + 1) : (value));
  }

  /**
   * Конвертация строки в число через битовый сдвиг для числовых основ 2, 4, 8, 16, 32.
   * Вспомогательная процедура для __strton_nx().
   *
   * Параметр SHIFT определяет длину сдвига, который указывается вручную:
   *   1) Числовая основа  2 -> сдвиг на 1.
   *   2) Числовая основа  4 -> сдвиг на 2.
   *   3) Числовая основа  8 -> сдвиг на 3.
   *   4) Числовая основа 16 -> сдвиг на 4.
   *   5) Числовая основа 32 -> сдвиг на 5.
   *
   * @param string  строка.
   * @param length  длина строки.
   * @param endptr  первый символ прервавший конвертацию символов в число.
   * @param radix   числовая основа.
   * @param limit   максимально допустимое значение числа со знаком, 0 для беззнаковых чисел.
   *
   * @return  в случае успеха - число,
   *          в случае ошибки - 0.
  */
  template<typename X, typename Y, int SHIFT>
  inline Y __strton_lsh_nx(const X* string, size_t length, X** endptr, unsigned int radix, Y limit)
  {
    Y value = 0;

    if(string)
    {
      while(length != 0)
      {
        Y digit;

        if(__chrton_x<X, Y>(*string, &digit, radix) == 0)
        {
          break;
        }

        Y temp = ((value << SHIFT) + digit);
        if(limit != 0 && temp > limit)break;
        if(value != 0 && temp < value)break;
        if(value != 0 && temp < value + radix)break;
        value = temp;

        string += 1;
        length -= 1;
      }
    }

    if(endptr)
    {
      *endptr = (X*)string;
    }

    return value;
  }

  /**
   * Конвертация строки в число с применением умножения для любых числовых основ.
   * Вспомогательная процедура для __strton_nx().
   *
   * @param string  строка.
   * @param length  длина строки.
   * @param endptr  первый символ прервавший конвертацию символов в число.
   * @param radix   числовая основа.
   * @param limit   максимально допустимое значение числа со знаком, 0 для беззнаковых чисел.
   *
   * @return  в случае успеха - число,
   *          в случае ошибки - 0.
  */
  template<typename X, typename Y>
  inline Y __strton_mul_nx(const X* string, size_t length, X** endptr, unsigned int radix, Y limit)
  {
    Y value = 0;

    if(string)
    {
      while(length != 0)
      {
        Y digit;

        if(__chrton_x<X, Y>(*string, &digit, radix) == 0)
        {
          break;
        }

        Y temp = ((value * radix) + digit);
        if(limit != 0 && temp > limit)break;
        if(value != 0 && temp < value)break;
        if(value != 0 && temp < value + radix)break;
        value = temp;

        string += 1;
        length -= 1;
      }
    }

    if(endptr)
    {
      *endptr = (X*)string;
    }

    return value;
  }

  /**
   * [Шаблон] Конвертация строки в число.
   *
   * @param string  строка.
   * @param length  длина строки без учета нулевого символа.
   * @param endptr  первый символ прервавший конвертацию строки в число.
   * @param radix   числовая основа от 2 до 36, для автоматического определения 0.
   *
   * @return  в случае успеха - число,
   *          в случае ошибки - 0.
  */
  template<typename X, typename Y, int IS_SIGNED>
  inline Y __strton_nx(const X* string, size_t length, X** endptr, unsigned int radix)
  {
    /** 
     * Проверка корректности аргументов.
    */
    if(!string || !length || ((radix != 0) && (radix < 2 || radix > 36)))
    {
      return __strton_exit_x<X, Y>(string, endptr);
    }

    int negative = 0;

    /**
     * Игнорирование пробельных символов.
    */
    while(length != 0 && __isblank_ex<X>(*string) != 0)
    {
      string += 1;
      length -= 1;
    }

    /**
     * Проверка нулевой длины после модификаций.
    */
    if(length == 0)
    {
      return __strton_exit_x<X, Y>(string, endptr);
    }

    /**
     * Чтение знака (наличие знака минуса считается ошибкой для беззнаковых типов).
    */
    if(IS_SIGNED != 0)
    {
      switch(*string)
      {
      case '+': negative = 0; string += 1; length -= 1; break;
      case '-': negative = 1; string += 1; length -= 1; break;
      }
    }
    else
    {
      switch(*string)
      {
      case '+': negative = 0; string += 1; length -= 1; break;
      case '-': return __strton_exit_x<X, Y>(string, endptr);
      }
    }

    /**
     * Проверка нулевой длины после модификаций.
    */
    if(length == 0)
    {
      return __strton_exit_x<X, Y>(string, endptr);
    }

    /**
     * Игнорирование пробельных символов.
    */
    while(length != 0 && __isblank_ex<X>(*string) != 0)
    {
      string += 1;
      length -= 1;
    }

    /**
     * Проверка нулевой длины после модификаций.
    */
    if(length == 0)
    {
      return __strton_exit_x<X, Y>(string, endptr);
    }

    /**
     * Чтение префикса, где возможны три варианта:
     *
     * 1) Префикс шестнадцатеричного числа '0xN...N'.
     * 2) Префикс восьмиричного числа '0N...N'
     * 3) Префикс отсутсвует и число является самостоятельным нулем.
    */
    if(string[0] == '0')
    {
      /**
       * [!] Длина строки должна быть более 1 символа на данном этапе.
      */
      if(length > 1)
      {
        X chr = string[1];

        if(chr == 'x' || chr == 'X')
        {
          if     (radix ==  0)radix = 16;
          else if(radix != 16)return __strton_exit_x<X, Y>(string, endptr);
          string += 2;
          length -= 2;
        }
        else if(chr >= '0' && chr <= '7')
        {
          if     (radix == 0)radix = 8;
          else if(radix != 8)return __strton_exit_x<X, Y>(string, endptr);
          string += 1;
          length -= 1;
        }
      }
    }

    /**
     * Проверка нулевой длины после модификаций.
    */
    if(length == 0)
    {
      return __strton_exit_x<X, Y>(string, endptr);
    }

    /**
     * По умолчанию числовая основа считается десятичной.
    */
    if(radix == 0)
    {
      radix = 10;
    }

    Y limit = 0;

    /**
     * Определение лимита требуемого для корректной конвертации знаковых типов.
    */
    if(IS_SIGNED != 0)
    {
      if(negative == 0)
      {
        limit = (((Y)-1) >> 1);
      }
      else
      {
        limit = (((Y)-1) >> 1) + 1;
      }
    }

    Y value = 0;

    /**
     * Чтение числа.
    */
    switch(radix)
    {
    case  2: value = __strton_lsh_nx<X, Y, 1>(string, length, endptr, radix, limit); break;
    case  4: value = __strton_lsh_nx<X, Y, 2>(string, length, endptr, radix, limit); break;
    case  8: value = __strton_lsh_nx<X, Y, 3>(string, length, endptr, radix, limit); break;
    case 16: value = __strton_lsh_nx<X, Y, 4>(string, length, endptr, radix, limit); break;
    case 32: value = __strton_lsh_nx<X, Y, 5>(string, length, endptr, radix, limit); break;
    default: value = __strton_mul_nx<X, Y   >(string, length, endptr, radix, limit); break;
    }

    return ((negative != 0) ? (~value + 1) : (value));
  }

  /**
   * Конвертация числа в строку через битовый сдвиг для числовых основ 2, 4, 8, 16, 32.
   * Вспомогательная процедура для __ntostr_x().
   *
   * Параметр SHIFT определяет длину сдвига, который указывается вручную:
   *   1) Числовая основа  2 -> сдвиг на 1.
   *   2) Числовая основа  4 -> сдвиг на 2.
   *   3) Числовая основа  8 -> сдвиг на 3.
   *   4) Числовая основа 16 -> сдвиг на 4.
   *   5) Числовая основа 32 -> сдвиг на 5.
   *
   * @param number  число.
   * @param string  строка.
   * @param radix   числовая основа от 2 до 36.
   *
   * @return  требуемая длина строки без учета нулевого символа,
   *          в случае ошибки 0.
  */
  template<typename X, typename Y, int SHIFT>
  inline size_t __ntostr_lsh_x(Y number, X* string, unsigned int radix)
  {
    size_t length = 0;

    do
    {
      if(string)
      {
        Y digit = number % radix;

        if(__ntochr_x<X, Y>(digit, string) == 0)
        {
          break;
        }

        string -= 1;
      }

      length = length + 1;
      number = number >> SHIFT;
    }
    while(number != 0);

    return length;
  }

  /**
   * Конвертация числа в строку через деление для любых числовых основ.
   * Вспомогательная процедура для __ntostr_x().
   *
   * @param number  число.
   * @param string  строка.
   * @param radix   числовая основа от 2 до 36.
   *
   * @return  требуемая длина строки без учета нулевого символа,
   *          в случае ошибки 0.
  */
  template<typename X, typename Y>
  inline size_t __ntostr_div_x(Y number, X* string, unsigned int radix)
  {
    size_t length = 0;

    do
    {
      if(string)
      {
        Y digit = number % radix;

        if(__ntochr_x<X, Y>(digit, string) == 0)
        {
          break;
        }

        string -= 1;
      }

      length = length + 1;
      number = number / radix;
    }
    while(number != 0);

    return length;
  }

  /**
   * [Шаблон] Конвертация числа в строку.
   *
   * @param number  число.
   * @param string  строка.
   * @param radix   числовая основа от 2 до 36, по умолчанию при 0 как десятичная основа.
   *
   * @return  требуемая длина строки без учета нулевого символа,
   *          в случае ошибки 0.
  */
  template<typename X, typename Y, int IS_SIGNED> 
  inline size_t __ntostr_x(Y number, X* string, unsigned int radix, int prefix)
  {
    size_t length = 0;

    /** 
     * Проверка корректности аргументов.
    */
    if((radix != 0) && (radix < 2 || radix > 36))
    {
      return 0;
    }

    /**
     * Запись знака при необходимости.
    */
    if(IS_SIGNED != 0)
    {
      const Y transition = (((Y)-1) >> 1) + 1;
      if(number >= transition)
      {
        if(string != NULL)
        {
          *string++ = '-';
        }
        length = (length + 1);
        number = ~number + 1;
      }
    }

    /**
     * Запись префикса при необходимости.
    */
    if(prefix != 0)
    {
      switch(radix)
      {
      case 8:
        if(string != NULL)
        {
          *string++ = '0';
        }
        length += 1;
        break;
      case 16:
        if(string != NULL)
        {
          *string++ = '0';
          *string++ = 'x';
        }
        length += 2;
        break;
      }
    }

    /**
     * По умолчанию числовая основа считается десятичной.
    */
    if(radix == 0)
    {
      radix = 10;
    }

    size_t offset = 0;

    /**
     * Определение количества символов требуемых для записи числа.
    */
    switch(radix)
    {
    case  2: offset = __ntostr_lsh_x<X, Y, 1>(number, NULL, radix); break;
    case  4: offset = __ntostr_lsh_x<X, Y, 2>(number, NULL, radix); break;
    case  8: offset = __ntostr_lsh_x<X, Y, 3>(number, NULL, radix); break;
    case 16: offset = __ntostr_lsh_x<X, Y, 4>(number, NULL, radix); break;
    case 32: offset = __ntostr_lsh_x<X, Y, 5>(number, NULL, radix); break;
    default: offset = __ntostr_div_x<X, Y   >(number, NULL, radix); break;
    }

    /**
     * Запись числа от конца к началу.
    */
    if(string != NULL && offset != 0)
    {
      switch(radix)
      {
      case  2: offset = __ntostr_lsh_x<X, Y, 1>(number, &string[offset - 1], radix); break;
      case  4: offset = __ntostr_lsh_x<X, Y, 2>(number, &string[offset - 1], radix); break;
      case  8: offset = __ntostr_lsh_x<X, Y, 3>(number, &string[offset - 1], radix); break;
      case 16: offset = __ntostr_lsh_x<X, Y, 4>(number, &string[offset - 1], radix); break;
      case 32: offset = __ntostr_lsh_x<X, Y, 5>(number, &string[offset - 1], radix); break;
      default: offset = __ntostr_div_x<X, Y   >(number, &string[offset - 1], radix); break;
      }
      string += offset;
    }

    /**
     * Обновление длины строки.
    */
    length += offset;

    /**
     * Запись нулевого символа.
    */
    if(string != NULL)
    {
      *string++ = '\0';
    }
    
    return length;
  }

  /**
   * Конвертация числа в строку через битовый сдвиг для числовых основ 2, 4, 8, 16, 32.
   * Вспомогательная процедура для __ntostr_lx().
   *
   * @param number  число.
   * @param string  строка.
   * @param maxcnt  максимальная длина строки без учета нулевого символа.
   * @param radix   числовая основа от 2 до 36.
   *
   * @return  требуемая длина строки без учета нулевого символа,
   *          требуемая длина больше maxcnt означает недостаточный размер буфера,
   *          в случае ошибки 0.
  */
  template<typename X, typename Y, int SHIFT>
  inline size_t __ntostr_lsh_lx(Y number, X* string, size_t maxcnt, unsigned int radix)
  {
    size_t length = 0;

    do
    {
      if(string != NULL && length < maxcnt)
      {
        Y digit = number % radix;

        if(__ntochr_x<X, Y>(digit, string) == 0)
        {
          break;
        }

        string -= 1;
      }

      length = length + 1;
      number = number >> SHIFT;
    }
    while(number != 0);

    return length;
  }
  
  /**
   * Конвертация числа в строку через деление для любых числовых основ.
   * Вспомогательная процедура для __ntostr_lx().
   *
   * @param number  число.
   * @param string  строка.
   * @param maxcnt  максимальная длина строки без учета нулевого символа.
   * @param radix   числовая основа от 2 до 36.
   *
   * @return  требуемая длина строки без учета нулевого символа,
   *          требуемая длина больше maxcnt означает недостаточный размер буфера,
   *          в случае ошибки 0.
  */
  template<typename X, typename Y>
  inline size_t __ntostr_div_lx(Y number, X* string, size_t maxcnt, unsigned int radix)
  {
    size_t length = 0;

    do
    {
      if(string != NULL && length < maxcnt)
      {
        Y digit = number % radix;

        if(__ntochr_x<X, Y>(digit, string) == 0)
        {
          break;
        }

        string -= 1;
      }

      length = length + 1;
      number = number / radix;
    }
    while(number != 0);

    return length;
  }

  /**
   * [Шаблон] Конвертация числа в строку.
   *
   * @param number  число.
   * @param string  строка.
   * @param maxlen  максимальная длина строки с учетом нулевого символа.
   * @param radix   числовая основа от 2 до 36, по умолчанию при 0 как десятичная основа.
   *
   * @return  требуемая длина строки без учета нулевого символа,
   *          требуемая длина равная или больше maxlen означает недостаточный размер буфера,
   *          в случае ошибки 0.
  */
  template<typename X, typename Y, int IS_SIGNED> 
  inline size_t __ntostr_lx(Y number, X* string, size_t maxlen, unsigned int radix, int prefix)
  {
    size_t length = 0;
    size_t maxcnt = 0;

    /** 
     * Проверка корректности аргументов.
    */
    if((radix != 0) && (radix < 2 || radix > 36))
    {
      return 0;
    }

    /**
     * Определение максимальной длины строки без учета нулевого символа.
    */
    if(string != NULL && maxlen != 0)
    {
      maxcnt = maxlen - 1;
    }

    /**
     * Запись знака при необходимости.
    */
    if(IS_SIGNED != 0)
    {
      const Y transition = (((Y)-1) >> 1) + 1;
      if(number >= transition)
      {
        if(string != NULL && length < maxcnt)
        {
          *string++ = '-';
        }
        length = (length + 1);
        number = ~number + 1;
      }
    }

    /**
     * Запись префикса при необходимости.
    */
    if(prefix != 0)
    {
      switch(radix)
      {
      case 8:
        if(string != NULL && length < maxcnt)
        {
          *string++ = '0';
        }
        length += 1;
        break;
      case 16:
        if(string != NULL && length < maxcnt)
        {
          *string++ = '0';
        }
        length += 1;
        if(string != NULL && length < maxcnt)
        {
          *string++ = 'x';
        }
        length += 1;
        break;
      }
    }

    /**
     * По умолчанию числовая основа считается десятичной.
    */
    if(radix == 0)
    {
      radix = 10;
    }

    size_t offset = 0;

    /**
     * Определение количества символов требуемых для записи числа.
    */
    switch(radix)
    {
    case  2: offset = __ntostr_lsh_lx<X, Y, 1>(number, NULL, 0, radix); break;
    case  4: offset = __ntostr_lsh_lx<X, Y, 2>(number, NULL, 0, radix); break;
    case  8: offset = __ntostr_lsh_lx<X, Y, 3>(number, NULL, 0, radix); break;
    case 16: offset = __ntostr_lsh_lx<X, Y, 4>(number, NULL, 0, radix); break;
    case 32: offset = __ntostr_lsh_lx<X, Y, 5>(number, NULL, 0, radix); break;
    default: offset = __ntostr_div_lx<X, Y   >(number, NULL, 0, radix); break;
    }

    /**
     * Запись числа от конца к началу при достаточном размере строки.
    */
    if(string != NULL && offset != 0 && maxcnt >= length + offset)
    {
      switch(radix)
      {
      case  2: offset = __ntostr_lsh_lx<X, Y, 1>(number, &string[offset - 1], maxcnt - length, radix); break;
      case  4: offset = __ntostr_lsh_lx<X, Y, 2>(number, &string[offset - 1], maxcnt - length, radix); break;
      case  8: offset = __ntostr_lsh_lx<X, Y, 3>(number, &string[offset - 1], maxcnt - length, radix); break;
      case 16: offset = __ntostr_lsh_lx<X, Y, 4>(number, &string[offset - 1], maxcnt - length, radix); break;
      case 32: offset = __ntostr_lsh_lx<X, Y, 5>(number, &string[offset - 1], maxcnt - length, radix); break;
      default: offset = __ntostr_div_lx<X, Y   >(number, &string[offset - 1], maxcnt - length, radix); break;
      }
      string += offset;
    }

    /**
     * Обновление длины строки.
    */
    length += offset;
 
    /**
     * Запись нулевого символа.
    */
    if(string != NULL && length < maxlen)
    {
      *string++ = '\0';
    }

    return length;
  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // strnum_hpp__