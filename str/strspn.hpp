#ifndef strspn_hpp__
#define strspn_hpp__

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "strchr.hpp"
#include "strcmp.hpp"
#include "strlen.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * [Шаблон] Количество символов присутствующих в списке с начала строки.
   *
   * @param string  строка.
   * @param accept  список символов которые должны присутствовать.
   *
   * @return  количество символов присутствующих в списке с начала строки,
   *          0 в случае ошибки.
  */
  template<typename X>
  inline size_t __strspn_x(const X* string, const X* accept)
  {
    if(!string || !accept)
    {
      return 0;
    }

    size_t length = 0;

    while(1)
    {
      X chr = string[length];
      if(chr == '\0' || __strchr_x<X>(accept, chr) == NULL)
      {
        break;
      }
      length += 1;
    }

    return length;
  }

  /**
   * [Шаблон] Количество символов присутствующих в списке с начала строки.
   *
   * @param string  строка.
   * @param accept  список символов которые должны присутствовать.
   * @param maxcnt  максимальная длина строки без учета нулевого символа.
   *
   * @return  количество символов присутствующих в списке с начала строки,
   *          0 в случае ошибки.
  */
  template<typename X>
  inline size_t __strspn_nx(const X* string, const X* accept, size_t maxcnt)
  {
    if(!string || !accept)
    {
      return 0;
    }

    size_t length = 0;

    while( length < maxcnt )
    {
      X chr = string[length];
      if(chr == '\0' || __strchr_x<X>(accept, chr) == NULL)
      {
        break;
      }
      length += 1;
    }

    return length;
  }

  /**
   * [Шаблон] Количество символов отсутствующих в списке с начала строки.
   *
   * @param string  строка.
   * @param reject  список символов которые должны отсутствовать.
   *
   * @return  количество символов отсутствующих в списке с начала строки,
   *          0 в случае ошибки.
  */
  template<typename X>
  inline size_t __strcspn_x(const X* string, const X* reject)
  {
    if(!string || !reject)
    {
      return 0;
    }

    size_t length = 0;

    while(1)
    {
      X chr = string[length];
      if(chr == '\0' || __strchr_x<X>(reject, chr) != NULL)
      {
        break;
      }
      length += 1;
    }

    return length;
  }

  /**
   * [Шаблон] Количество символов отсутствующих в списке с начала строки.
   *
   * @param string  строка.
   * @param reject  список символов которые должны отсутствовать.
   * @param maxcnt  максимальная длина строки без учета нулевого символа.
   *
   * @return  количество символов отсутствующих в списке с начала строки,
   *          0 в случае ошибки.
  */
  template<typename X>
  inline size_t __strcspn_nx(const X* string, const X* reject, size_t maxcnt)
  {
    if(!string || !reject)
    {
      return 0;
    }

    size_t length = 0;

    while( length < maxcnt )
    {
      X chr = string[length];
      if(chr == '\0' || __strchr_x<X>(reject, chr) != NULL)
      {
        break;
      }
      length += 1;
    }

    return length;
  }

  /**
   * [Шаблон] Количество символов присутствующих в списке с конца строки.
   *
   * @param string  строка.
   * @param accept  список символов которые должны присутствовать.
   *
   * @return  количество символов присутствующих в списке с конца строки,
   *          0 в случае ошибки.
  */
  template<typename X>
  inline size_t __strrspn_x(const X* string, const X* accept)
  {
    if(!string || !accept)
    {
      return 0;
    }

    size_t offset = __strlen_x<X>(string);
    size_t length = 0;

    while( offset != 0 )
    {
      offset -= 1;

      if(__strchr_x<X>(accept, string[offset]) == NULL)
      {
        break;
      }

      length += 1;
    }

    return length;
  }

  /**
   * [Шаблон] Количество символов присутствующих в списке с конца строки.
   *
   * @param string  строка.
   * @param accept  список символов которые должны присутствовать.
   * @param maxcnt  максимальная длина строки без учета нулевого символа.
   *
   * @return  количество символов присутствующих в списке с конца строки,
   *          0 в случае ошибки.
  */
  template<typename X>
  inline size_t __strrspn_nx(const X* string, const X* accept, size_t maxcnt)
  {
    if(!string || !accept || !maxcnt)
    {
      return 0;
    }

    size_t offset = __strlen_nx<X>(string, maxcnt + 1);
    size_t length = 0;

    while( offset != 0 )
    {
      offset -= 1;

      if(__strchr_x<X>(accept, string[offset]) == NULL)
      {
        break;
      }
      
      length += 1;
    }

    return length;
  }

  /**
   * [Шаблон] Количество символов отсутствующих в списке с конца строки.
   *
   * @param string  строка.
   * @param reject  список символов которые должны отсутствовать.
   *
   * @return  количество символов отсутствующих в списке с конца строки,
   *          0 в случае ошибки.
  */
  template<typename X>
  inline size_t __strrcspn_x(const X* string, const X* reject)
  {
    if(!string || !reject)
    {
      return 0;
    }

    size_t offset = __strlen_x<X>(string);
    size_t length = 0;

    while( offset != 0 )
    {
      offset -= 1;

      if(__strchr_x<X>(reject, string[offset]) != NULL)
      {
        break;
      }

      length += 1;
    }

    return length;
  }

  /**
   * [Шаблон] Количество символов отсутствующих в списке с конца строки.
   *
   * @param string  строка.
   * @param reject  список символов которые должны отсутствовать.
   * @param maxcnt  максимальная длина строки без учета нулевого символа.
   *
   * @return  количество символов отсутствующих в списке с конца строки,
   *          0 в случае ошибки.
  */
  template<typename X>
  inline size_t __strrcspn_nx(const X* string, const X* reject, size_t maxcnt)
  {
    if(!string || !reject || !maxcnt)
    {
      return 0;
    }

    size_t offset = __strlen_nx<X>(string, maxcnt + 1);
    size_t length = 0;

    while( offset != 0 )
    {
      offset -= 1;

      if(__strchr_x<X>(reject, string[offset]) != NULL)
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

#endif // strspn_hpp__