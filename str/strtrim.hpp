#ifndef strtrim_hpp__
#define strtrim_hpp__

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "strlen.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * [Шаблон] Экранирование слешами символов ", ', \  в строке.
   *
   * @param string  строка.
   *
   * @return  в случае успеха - длина модифицированной строки без учета нулевого символа,
   *          в случае ошибки - 0.
  */
  template<typename X>
  inline size_t __addslashes_x(X* string)
  {
    if(!string)
    {
      return 0;
    }

    size_t length = __strlen_x<X>(string);
    size_t offset = 0;

    while( offset < length )
    {
      X chr = string[offset];
      if(chr == '"'  || chr == '\'' || chr == '\\')
      {
        X* src = &string[length + 0];
        X* dst = &string[length + 1];
        X* end = &string[offset    ];
        
        while(dst != end)
        {
          *dst-- = *src--;
        }
        *dst = '\\';

        length += 1;
        offset += 1;
      }

      offset += 1;
    }

    return length;
  }

  /**
   * [Шаблон] Экранирование слешами символов ", ', \  в строке.
   *
   * @param string  строка.
   * @param maxlen  максимальная длина строки с учетом нулевого символа.
   *
   * @return  требуемая длина строки в символах без учета нулевого символа,
   *          требуемая длина >= maxlen означает недостаточный размер буфера,
   *          в случае ошибки 0.
  */
  template<typename X>
  inline size_t __addslashes_lx(X* string, size_t maxlen)
  {
    if(!string || !maxlen)
    {
      return 0;
    }

    size_t length = __strlen_nx<X>(string, maxlen);
    size_t needed = length;
    size_t zindex = maxlen - 1;
    size_t offset = 0;

    while( offset < length )
    {
      X chr = string[offset];
      if(chr == '"' || chr == '\'' || chr == '\\')
      {
        if(length < zindex)
        {
          X* src = &string[length + 0];
          X* dst = &string[length + 1];
          X* end = &string[offset    ];

          while(dst != end)
          {
            *dst-- = *src--;
          }
          *dst = '\\';

          length += 1;
          offset += 1;
        }
        needed += 1;
      }
      offset += 1;
    }

    return needed;
  }

  /**
   * [Шаблон] * Удаление символов '\r','\n','\t',' ' из начала и конца строки.
   *
   * @param string  строка.
   *
   * @return  в случае успеха - длина модифицированной строки без учета нулевого символа,
   *          в случае ошибки - 0.
  */
  template<typename X>
  inline size_t __strtrim_x(X* string)
  {
    if(!string)
    {
      return 0;
    }

    size_t length = __strlen_x<X>(string);
    if(length != 0)
    {
      size_t offset = 0;

      while(length != 0)
      {
        X chr = string[length - 1];
        if(chr != '\r' && chr != '\n' && chr != ' ' && chr != '\t')
        {
          break;
        }
        length -= 1;
      }

      while(offset < length)
      {
        X chr = string[offset];
        if(chr != '\r' && chr != '\n' && chr != ' ' && chr != '\t')
        {
          break;
        }
        offset += 1;
      }

      if(offset != 0)
      {
        for(size_t i = 0; i < (length - offset); i++)
        {
          string[i] = string[i + offset];
        }
        length -= offset;
      }

      string[length] = '\0';
    }

    return length;
  }

  /**
   * [Шаблон] Удаление символов '\r','\n','\t',' ' из начала и конца строки.
   *
   * @param string  строка.
   * @param maxcnt  максимальная длина строки без учета нулевого символа.
   *
   * @return  в случае успеха - длина модифицированной строки без учета нулевого символа,
   *          в случае ошибки - 0.
   */
  template<typename X>
  inline size_t __strtrim_nx(X* string, size_t maxcnt)
  {
    if(!string)
    {
      return 0;
    }

    size_t length = __strlen_nx<X>(string, maxcnt);
    if(length != 0)
    {
      size_t offset = 0;

      while(length != 0)
      {
        X chr = string[length - 1];
        if(chr != '\r' && chr != '\n' && chr != ' ' && chr != '\t')
        {
          break;
        }
        length -= 1;
      }

      while(offset < length)
      {
        X chr = string[offset];
        if(chr != '\r' && chr != '\n' && chr != ' ' && chr != '\t')
        {
          break;
        }
        offset += 1;
      }

      if(offset != 0)
      {
        for(size_t i = 0; i < (length - offset); i++)
        {
          string[i] = string[i + offset];
        }
        length -= offset;
      }

      string[length] = '\0';
    }

    return length;
  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // strtrim_hpp__