#ifndef strcpy_hpp__
#define strcpy_hpp__

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * [Шаблон] Копирование строки.
   * База для реализации функционала аналогичного strcpy, wcscpy.
   *
   * @param dst  конечная строка.
   * @param src  исходная строка.
   *
   * @return  длина модифицированной конечной строки без учета нулевого символа,
   *          0 в случае ошибки.
  */
  template<typename X> 
  inline size_t __strcpy_x(X* dst, const X* src)
  {     
    if(!dst || !src)
    {
      return 0;
    }

    size_t length = 0;

    while(1)
    {
      X chr = dst[length] = src[length];
      if(chr == '\0')
      {
        break;
      }
      length += 1;
    }

    return length;
  }

  /**
   * [Шаблон] Копирование строки.
   * База для реализации функционала аналогичного strncpy, wcsncpy.
   *
   * @param dst     конечная строка.
   * @param src     исходная строка.
   * @param maxcnt  максимальное количество копируемых символов исходной строки.
   *
   * @return  длина модифицированной конечной строки без учета нулевого символа,
   *          0 в случае ошибки.
  */
  template<typename X>
  inline size_t __strcpy_nx(X* dst, const X* src, size_t maxcnt)
  {
    if(!dst || !src)
    {
      return 0;
    }

    size_t length = 0;

    while(1)
    {
      if(length >= maxcnt)
      {
        dst[length] = '\0';
        break;
      }

      X chr = dst[length] = src[length];
      if(chr == '\0')
      {
        break;
      }

      length += 1;
    }

    return length;
  }

  /**
   * [Шаблон] Копирование строки.
   * База для реализации функционала аналогичного strlcpy, wcslcpy.
   *
   * @param dst     конечная строка.
   * @param src     исходная строка.
   * @param maxlen  максимальная длина конечной строки с учетом нулевого символа.
   *
   * @return  требуемая длина конечной строки без учета нулевого символа,
   *          требуемая длина равная или более maxlen означает недостаточный размер буфера,
   *          0 в случае ошибки.
  */
  template<typename X>
  inline size_t __strcpy_lx(X* dst, const X* src, size_t maxlen)
  {
    if(!dst || !src)
    {
      return 0;
    }

    size_t length = 0;
    size_t zindex = maxlen - 1;

    while(1)
    {
      X chr = src[length];

      if(length < maxlen)
      {
        dst[length] = ((length != zindex) ? chr : '\0');
      }

      if(chr == '\0')
      {
        break;
      }

      length += 1;
    }

    return length;
  }

  /**
   * [Шаблон] Копирование строки.
   * База для реализации функционала аналогичного strlcpy, wcslcpy.
   *
   * @param dst     конечная строка.
   * @param src     исходная строка.
   * @param maxlen  максимальная длина конечной строки с учетом нулевого символа.
   * @param maxcnt  максимальное количество копируемых символов исходной строки.
   *
   * @return  требуемая длина конечной строки без учета нулевого символа,
   *          требуемая длина равная или более maxlen означает недостаточный размер буфера,
   *          0 в случае ошибки.
  */
  template<typename X>
  inline size_t __strcpy_lnx(X* dst, const X* src, size_t maxlen, size_t maxcnt)
  {
    if(!dst || !src)
    {
      return 0;
    }

    size_t length = 0;
    size_t zindex = maxlen - 1;

    while(1)
    {
      if(length >= maxcnt)
      {
        if(length < maxlen)
        {
          dst[length] = '\0';
        }
        break;
      }

      X chr = src[length];

      if(length < maxlen)
      {
        dst[length] = ((length != zindex) ? chr : '\0');
      }

      if(chr == '\0')
      {
        break;
      }

      length += 1;
    }

    return length;
  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // strcpy_hpp__