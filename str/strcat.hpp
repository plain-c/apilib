#ifndef strcat_hpp__
#define strcat_hpp__

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "strcpy.hpp"
#include "strlen.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * [Шаблон] Присоединение одной строки в конец другой строки.
   * База для реализации функционала аналогичного strcat, wcscat.
   *
   * @param dst  конечная строка.
   * @param src  исходная строка.
   *
   * @return  длина модифицированной конечной строки без учета нулевого символа,
   *          в случае ошибки 0.
  */
  template<typename X> 
  inline size_t __strcat_x(X* dst, const X* src)
  {
    if(!dst || !src)
    {
      return 0;
    }

    size_t length = __strlen_x<X>(dst);
    return length + __strcpy_x<X>(&dst[length], src);
  }

  /**
   * [Шаблон] Присоединение одной строки в конец другой строки.
   * База для реализации функционала аналогичного strncat, wcsncat.
   *
   * @param dst     конечная строка.
   * @param src     исходная строка.
   * @param maxcnt  максимальное количество присоединяемых символов исходной строки.
   *
   * @return  длина модифицированной конечной строки без учета нулевого символа,
   *          в случае ошибки 0.
  */
  template<typename X>
  inline size_t __strcat_nx(X* dst, const X* src, size_t maxcnt)
  {
    if(!dst || !src)
    {
      return 0;
    }

    size_t length = __strlen_x<X>(dst);
    return length + __strcpy_nx<X>(&dst[length], src, maxcnt);
  }

  /**
   * [Шаблон] Присоединение одной строки в конец другой строки.
   * База для реализации функционала аналогичного strlcat, wcslcat.
   *
   * @param dst     конечная строка.
   * @param src     исходная строка.
   * @param maxlen  максимальная длина конечной строки с учетом нулевого символа.
   *
   * @return  требуемая длина конечной строки без учета нулевого символа,
   *          требуемая длина равная или более maxlen означает недостаточный размер буфера,
   *          в случае ошибки 0.
  */
  template<typename X>
  inline size_t __strcat_lx(X* dst, const X* src, size_t maxlen)
  {
    if(!dst || !src)
    {
      return 0;
    }

    size_t length = __strlen_nx<X>(dst, maxlen);
    return length + __strcpy_lx<X>(&dst[length], src, maxlen - length);
  }

  /**
   * [Шаблон] Присоединение одной строки в конец другой строки.
   * База для реализации функционала аналогичного strncat_s, wcsncat_s.
   *
   * @param dst     конечная строка.
   * @param src     исходная строка.
   * @param maxlen  максимальная длина конечной строки с учетом нулевого символа.
   * @param maxcnt  максимальное количество присоединяемых символов исходной строки.
   *
   * @return  требуемая длина конечной строки без учета нулевого символа,
   *          требуемая длина равная или более maxlen означает недостаточный размер буфера,
   *          в случае ошибки 0.
  */
  template<typename X>
  inline size_t __strcat_lnx(X* dst, const X* src, size_t maxlen, size_t maxcnt)
  {
    if(!dst || !src)
    {
      return 0;
    }

    size_t length = __strlen_nx<X>(dst, maxlen);
    return length + __strcpy_lnx<X>(&dst[length], src, maxlen - length, maxcnt);
  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // strcat_hpp__