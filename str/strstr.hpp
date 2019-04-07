#ifndef strstr_hpp__
#define strstr_hpp__

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "strcmp.hpp"
#include "strlen.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * [Шаблон] Поиск подстроки.
   * База для реализации функционала аналогичного strstr, wcsstr.
   *
   * @param string1  строка.
   * @param string2  искомая подстрока.
   *
   * @return  указатель на найденную подстроку в строке,
   *          NULL если подстрока не найдена или произошла ошибка.
  */
  template<typename X> 
  inline X* __strstr_x(const X* string1, const X* string2)
  {
    if(!string1 || !string2)
    {
      return NULL;
    }

    const size_t length1 = __strlen_x<X>(string1);
    const size_t length2 = __strlen_x<X>(string2);
    if(length1 < length2)
    {
      return NULL;
    }

    for(size_t i = 0; i <= (length1 - length2); i += 1)
    {
      if(__strequ_nx<X>(&string1[i], string2, length2) == true)
      {
        return (X*)&string1[i];
      }
    }

    return NULL;
  }

  /**
   * [Шаблон] Поиск подстроки.
   *
   * @param string1  строка.
   * @param string2  искомая подстрока.
   * @param length1  длина строки без учета нулевого символа.
   * @param length2  длина искомой подстроки без учета нулевого символа.
   *
   * @return  указатель на найденную подстроку в строке,
   *          NULL если подстрока не найдена или произошла ошибка.
  */
  template<typename X> 
  inline X* __strstr_nx(const X* string1, const X* string2, size_t length1, size_t length2)
  {
    if(!string1 || !string2 || length1 < length2)
    {
      return NULL;
    }

    for(size_t i = 0; i <= (length1 - length2); i += 1)
    {
      if(__strequ_nx<X>(&string1[i], string2, length2) == true)
      {
        return (X*)&string1[i];
      }
    }

    return NULL;
  }

  /**
   * [Шаблон] Поиск подстроки без учета регистра английских букв.
   *
   * @param string1  строка.
   * @param string2  искомая подстрока.
   *
   * @return  указатель на найденную подстроку в строке,
   *          NULL если подстрока не найдена или произошла ошибка.
  */
  template<typename X> 
  inline X* __strstr_ex(const X* string1, const X* string2)
  {
    if(!string1 || !string2)
    {
      return NULL;
    }

    const size_t length1 = __strlen_x<X>(string1);
    const size_t length2 = __strlen_x<X>(string2);
    if(length1 < length2)
    {
      return NULL;
    }

    for(size_t i = 0; i <= (length1 - length2); i += 1)
    {
      if(__strequ_enx<X>(&string1[i], string2, length2) == true)
      {
        return (X*)&string1[i];
      }
    }

    return NULL;
  }

  /**
   * [Шаблон] Поиск подстроки без учета регистра английских букв.
   *
   * @param string1  строка.
   * @param string2  искомая подстрока.
   * @param length1  длина строки без учета нулевого символа.
   * @param length2  длина искомой подстроки без учета нулевого символа.
   *
   * @return  указатель на найденную подстроку в строке,
   *          NULL если подстрока не найдена или произошла ошибка.
  */
  template<typename X> 
  inline X* __strstr_enx(const X* string1, const X* string2, size_t length1, size_t length2)
  {
    if(!string1 || !string2 || length1 < length2)
    {
      return NULL;
    }

    for(size_t i = 0; i <= (length1 - length2); i += 1)
    {
      if(__strequ_enx<X>(&string1[i], string2, length2) == true)
      {
        return (X*)&string1[i];
      }
    }

    return NULL;
  }

  /**
   * [Шаблон] Поиск подстроки с конца строки.
   * База для реализации функционала аналогичного strrstr, wcsrstr.
   *
   * @param string1  строка.
   * @param string2  искомая подстрока.
   *
   * @return  указатель на найденную подстроку в строке,
   *          NULL если подстрока не найдена или произошла ошибка.
  */
  template<typename X> 
  inline X* __strrstr_x(const X* string1, const X* string2)
  {
    if(!string1 || !string2)
    {
      return NULL;
    }

    const size_t length1 = __strlen_x<X>(string1);
    const size_t length2 = __strlen_x<X>(string2);
    if(length1 < length2)
    {
      return NULL;
    }

    size_t offset = length1 - length2;

    do
    {
      if(__strequ_nx<X>(&string1[offset], string2, length2) == true)
      {
        return (X*)&string1[offset];
      }
    }
    while(offset-- != 0);

    return NULL;
  }

  /**
   * [Шаблон] Поиск подстроки с конца строки.
   *
   * @param string1  строка.
   * @param string2  искомая подстрока.
   * @param length1  длина строки без учета нулевого символа.
   * @param length2  длина искомой подстроки без учета нулевого символа.
   *
   * @return  указатель на найденную подстроку в строке,
   *          NULL если подстрока не найдена или произошла ошибка.
  */
  template<typename X> 
  inline X* __strrstr_nx(const X* string1, const X* string2, size_t length1, size_t length2)
  {
    if(!string1 || !string2 || length1 < length2)
    {
      return NULL;
    }
       
    size_t offset = length1 - length2;
    
    do
    {
      if(__strequ_nx<X>(&string1[offset], string2, length2) == true)
      {
        return (X*)&string1[offset];
      }
    }
    while(offset-- != 0);

    return NULL;
  }

  /**
   * [Шаблон] Поиск подстроки с конца строки.
   *
   * @param string1  строка.
   * @param string2  искомая подстрока.
   *
   * @return  указатель на найденную подстроку в строке,
   *          NULL если подстрока не найдена или произошла ошибка.
  */
  template<typename X> 
  inline X* __strrstr_ex(const X* string1, const X* string2)
  {
    if(!string1 || !string2)
    {
      return NULL;
    }

    const size_t length1 = __strlen_x<X>(string1);
    const size_t length2 = __strlen_x<X>(string2);
    if(length1 < length2)
    {
      return NULL;
    }

    size_t offset = length1 - length2;

    do
    {
      if(__strequ_enx<X>(&string1[offset], string2, length2) == true)
      {
        return (X*)&string1[offset];
      }
    }
    while(offset-- != 0);

    return NULL;
  }

  /**
   * [Шаблон] Поиск подстроки с конца строки.
   *
   * @param string1  строка.
   * @param string2  искомая подстрока.
   * @param length1  длина строки без учета нулевого символа.
   * @param length2  длина искомой подстроки без учета нулевого символа.
   *
   * @return  указатель на найденную подстроку в строке,
   *          NULL если подстрока не найдена или произошла ошибка.
  */
  template<typename X> 
  inline X* __strrstr_enx(const X* string1, const X* string2, size_t length1, size_t length2)
  {
    if(!string1 || !string2 || length1 < length2)
    {
      return NULL;
    }
       
    size_t offset = length1 - length2;
    
    do
    {
      if(__strequ_enx<X>(&string1[offset], string2, length2) == true)
      {
        return (X*)&string1[offset];
      }
    }
    while(offset-- != 0);

    return NULL;
  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // strstr_hpp__