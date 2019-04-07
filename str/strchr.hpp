#ifndef strchr_hpp__
#define strchr_hpp__

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "strlen.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * [Шаблон] Поиск символа в строке.
   * База для реализации функционала аналогичного strchr, wcschr.
   *
   * @param string  строка.
   * @param chr     символ.
   *
   * @return  указатель на найденный символ в строке,
   *          NULL если символ не найден или произошла ошибка.
  */
  template<typename X> 
  inline X* __strchr_x(const X* string, X chr)
  {
    if(!string)
    {
      return NULL;
    }

    size_t length = 0;

    while(1)
    {
      X tmp = string[length];

      if(tmp == chr)
      {
        return (X*)&string[length];
      }

      if(tmp == '\0')
      {
        break;
      }

      length += 1;
    }

    return NULL;
  }

  /**
   * [Шаблон] Поиск символа в строке.
   *
   * @param string  строка.
   * @param maxlen  максимальная длина строки с учетом нулевого символа.
   * @param chr     символ.
   *
   * @return  указатель на найденный символ в строке,
   *          NULL если символ не найден или произошла ошибка.
  */
  template<typename X> 
  inline X* __strchr_nx(const X* string, size_t maxlen, X chr)
  {
    if(!string || !maxlen)
    {
      return NULL;
    }

    size_t length = 0;

    while( length < maxlen )
    {
      X tmp = string[length];

      if(tmp == chr)
      {
        return (X*)&string[length];
      }

      if(tmp == '\0')
      {
        break;
      }

      length += 1;
    }

    return NULL;
  }

  /**
   * [Шаблон] Поиск символа c конца строки.
   * База для реализации функционала аналогичного strrchr, wcsrchr.
   *
   * @param string  строка.
   * @param chr     символ.
   *
   * @return  указатель на найденный символ в строке,
   *          NULL если символ не найден или произошла ошибка.
  */
  template<typename X> 
  inline X* __strrchr_x(const X* string, X chr)
  {
    if(!string)
    {
      return NULL;
    }

    size_t length = __strlen_x<X>(string);

    while(1)
    {
      if(string[length] == chr)
      {
        return (X*)&string[length];
      }
      if(length == 0)
      {
        break;
      }
      length -= 1;
    }

    return NULL;
  }

  /**
   * [Шаблон] Поиск символа c конца строки.
   *
   * @param string  строка.
   * @param maxlen  максимальная длина строки с учетом нулевого символа.
   * @param chr     символ.
   *
   * @return  указатель на найденный символ в строке,
   *          NULL если символ не найден или произошла ошибка.
  */
  template<typename X> 
  inline X* __strrchr_nx(const X* string, size_t maxlen, X chr)
  {
    if(!string || !maxlen)
    {
      return NULL;
    }

    size_t length = __strlen_nx<X>(string, maxlen);

    while(1)
    {
      if(string[length] == chr)
      {
        return (X*)&string[length];
      }
      if(length == 0)
      {
        break;
      }
      length -= 1;
    }
    
    return NULL;
  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // strchr_hpp__