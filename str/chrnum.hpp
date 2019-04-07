#ifndef chrnum_hpp__
#define chrnum_hpp__

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * [Шаблон] Конвертация цифры в символ.
   *
   * @param digit  цифра от 0 до 35.
   *
   * @param  в случае успеха - символ '0'-'9' или 'A'-'Z',
   *         в случае ошибки - символ ' '.
  */
  template<typename X, typename Y>
  inline X __ntochr_x(Y digit)
  {
    if(digit >=  0 && digit <=  9) return('0' + digit);
    if(digit >= 10 && digit <= 35) return('A' + digit - 10);
    return ' ';
  }

  /**
   * [Шаблон] Конвертация символа в цифру.
   *
   * @param chr    символ '0'-'9' или 'A'-'Z'.
   * @param radix  числовая основа.
   *
   * @return  в случае успеха - цифра от 0 до 35,
   *          в случае ошибки - 0.
  */
  template<typename X, typename Y>
  inline Y __chrton_x(X chr, int radix)
  {
    if(chr >= '0' && chr <= '9')
    {
      unsigned char t = (chr - '0');
      if(radix <= t)return 0;
      return t;
    }
    if(chr >= 'A' && chr <= 'Z')
    {
      unsigned char t = (chr - 'A' + 10);
      if(radix <= t)return 0;
      return t;
    }
    if(chr >= 'a' && chr <= 'z')
    {
      unsigned char t = (chr - 'a' + 10);
      if(radix <= t)return 0;
      return t;
    }
    return 0;
  }

  /**
   * [Шаблон] Конвертация цифры в символ.
   *
   * @param digit  цифра от 0 до 35.
   * @param chr    символ '0'-'9' или 'A'-'Z'.
   *
   * @param  1 - положительный результат,
   *         0 - отрицательный результат.
  */
  template<typename X, typename Y>
  inline int __ntochr_x(Y digit, X* chr)
  {
    if(digit >= 0 && digit <= 9)
    {
      if(chr)*chr = (X)('0' + digit);
      return 1;
    }
    if(digit >= 10 && digit <= 35)
    {
      if(chr)*chr = (X)('A' + digit - 10);
      return 1;
    }
    return 0;
  }

  /**
   * [Шаблон] Конвертация символа в цифру.
   *
   * @param chr    символ '0'-'9' или 'A'-'Z'.
   * @param digit  цифра от 0 до 35.
   * @param radix  числовая основа.
   *
   * @return  1 - положительный результат,
   *          0 - отрицательный результат.
  */
  template<typename X, typename Y>
  inline int __chrton_x(X chr, Y* digit, int radix)
  {
    if(chr >= '0' && chr <= '9')
    {
      unsigned char t = (chr - '0');
      if(radix <= t)return 0;
      if(digit)*digit = t;
      return 1;
    }
    if(chr >= 'A' && chr <= 'Z')
    {
      unsigned char t = (chr - 'A' + 10);
      if(radix <= t)return 0;
      if(digit)*digit = t;
      return 1;
    }
    if(chr >= 'a' && chr <= 'z')
    {
      unsigned char t = (chr - 'a' + 10);
      if(radix <= t)return 0;
      if(digit)*digit = t;
      return 1;
    }
    return 0;
  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // chrnum_hpp__