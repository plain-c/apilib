#ifndef strcmp_hpp__
#define strcmp_hpp__

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "chrcmp.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * [Шаблон] Сравнение двух строк.
   * База для реализации функционала аналогичного strcmp, wcscmp.
   *
   * @param s1  первая строка.
   * @param s2  вторая строка.
   *
   * @return  ( 1) - первая строка больше второй строки,
   *          (-1) - вторая строка больше первой строки,
   *          ( 0) - строки совпадают,
   *          ( 2) - первая строка больше второй строки являющейся нулевый указателем,
   *          (-2) - вторая строка больше первой строки являющейся нулевый указателем.
  */
  template<typename X> 
  inline int __strcmp_x(const X* s1, const X* s2)
  {
    if(s1 == s2)
    {
      return 0;
    }

    if(!s1)return(-2);
    if(!s2)return( 2);

    while(1)
    {
      X c1 = *s1;
      X c2 = *s2;
      int r = __chrcmp_x<X>(c1, c2);
      if( r != 0 )
      {
        return r;
      }
      if(c1 == 0 || c2 == 0)
      {
        break;
      }
      s1 += 1;
      s2 += 1;
    }

    return 0;
  }

  /**
   * [Шаблон] Сравнение двух строк без учета регистра английских букв.
   *
   * @param s1  первая строка.
   * @param s2  вторая строка.
   *
   * @return  ( 1) - первая строка больше второй строки,
   *          (-1) - вторая строка больше первой строки,
   *          ( 0) - строки совпадают,
   *          ( 2) - первая строка больше второй строки являющейся нулевый указателем,
   *          (-2) - вторая строка больше первой строки являющейся нулевый указателем.
  */
  template<typename X> 
  inline int __strcmp_ex(const X* s1, const X* s2)
  {
    if(s1 == s2)
    {
      return 0;
    }

    if(!s1)return(-2);
    if(!s2)return( 2);

    while(1)
    {
      X c1 = *s1;
      X c2 = *s2;
      int r = __chrcmp_ex<X>(c1, c2);
      if( r != 0 )
      {
        return r;
      }
      if(c1 == 0 || c2 == 0)
      {
        break;
      }
      s1 += 1;
      s2 += 1;
    }

    return 0;
  }

  /**
   * [Шаблон] Сравнение двух строк.
   * База для реализации функционала аналогичного strncmp, wcsncmp.
   *
   * @param s1      первая строка.
   * @param s2      вторая строка.
   * @param maxcnt  максимальное количество сравниваемых символов.
   *
   * @return  ( 1) - первая строка больше второй строки,
   *          (-1) - вторая строка больше первой строки,
   *          ( 0) - строки совпадают,
   *          ( 2) - первая строка больше второй строки являющейся нулевый указателем,
   *          (-2) - вторая строка больше первой строки являющейся нулевый указателем.
  */
  template<typename X> 
  inline int __strcmp_nx(const X* s1, const X* s2, size_t maxcnt)
  {
    if(s1 == s2)
    {
      return 0;
    }

    if(!s1)return(-2);
    if(!s2)return( 2);

    for(size_t i = 0; i < maxcnt; i += 1)
    {
      X c1 = s1[i];
      X c2 = s2[i];
      int r = __chrcmp_x<X>(c1, c2);
      if( r != 0 )
      {
        return r;
      }
      if(c1 == 0 || c2 == 0)
      {
        break;
      }
    }

    return 0;
  }

  /**
   * [Шаблон] Сравнение двух строк без учета регистра английских букв.
   *
   * @param s1      первая строка.
   * @param s2      вторая строка.
   * @param maxcnt  максимальное количество сравниваемых символов.
   *
   * @return  ( 1) - первая строка больше второй строки,
   *          (-1) - вторая строка больше первой строки,
   *          ( 0) - строки совпадают,
   *          ( 2) - первая строка больше второй строки являющейся нулевый указателем,
   *          (-2) - вторая строка больше первой строки являющейся нулевый указателем.
  */
  template<typename X> 
  inline int __strcmp_enx(const X* s1, const X* s2, size_t maxcnt)
  {
    if(s1 == s2)
    {
      return 0;
    }

    if(!s1)return(-2);
    if(!s2)return( 2);

    for(size_t i = 0; i < maxcnt; i += 1)
    {
      X c1 = s1[i];
      X c2 = s2[i];
      int r = __chrcmp_ex<X>(c1, c2);
      if( r != 0 )
      {
        return r;
      }
      if(c1 == 0 || c2 == 0)
      {
        break;
      }
    }

    return 0;
  }

  /**
   * [Шаблон] Проверка идентичности строк.
   *
   * @param s1  первая строка.
   * @param s2  вторая строка.
   *
   * @return  true  - строки идентичны,
   *          false - строки отличаются.
  */
  template<typename X> 
  inline bool __strequ_x(const X* s1, const X* s2)
  {
    if(s1 == s2)
    {
      return true;
    }

    if(!s1)return false;
    if(!s2)return false;

    while(1)
    {
      X c1 = *s1;
      X c2 = *s2;
      if(__chrequ_x<X>(c1, c2) == false)
      {
        return false;
      }
      if(c1 == 0 || c2 == 0)
      {
        break;
      }
      s1 += 1;
      s2 += 1;
    }

    return true;
  }

  /**
   * [Шаблон] Проверка идентичности строк без учета регистра английских букв.
   *
   * @param s1  первая строка.
   * @param s2  вторая строка.
   *
   * @return  true  - строки идентичны,
   *          false - строки отличаются.
  */
  template<typename X> 
  inline bool __strequ_ex(const X* s1, const X* s2)
  {
    if(s1 == s2)
    {
      return true;
    }

    if(!s1)return false;
    if(!s2)return false;

    while(1)
    {
      X c1 = *s1;
      X c2 = *s2;
      if(__chrequ_ex<X>(c1, c2) == false)
      {
        return false;
      }
      if(c1 == 0 || c2 == 0)
      { 
        break;
      }
      s1 += 1;
      s2 += 1;
    }

    return true;
  }

  /**
   * [Шаблон] Проверка идентичности строк.
   *
   * @param s1      первая строка.
   * @param s2      вторая строка.
   * @param maxcnt  максимальное количество сравниваемых символов.
   *
   * @return  true  - строки идентичны,
   *          false - строки отличаются.
  */
  template<typename X> 
  inline bool __strequ_nx(const X* s1, const X* s2, size_t maxcnt)
  {
    if(s1 == s2)
    {
      return true;
    }
    if(!s1 || !s2 || !maxcnt)
    {
      return false;
    }

    for(size_t i = 0; i < maxcnt; i += 1)
    {
      X c1 = s1[i];
      X c2 = s2[i];
      if(__chrequ_x<X>(c1, c2) == false)
      {
        return false;
      }
      if(c1 == 0 || c2 == 0)
      {
        break;
      }
    }

    return true;
  }

  /**
   * [Шаблон] Проверка идентичности строк без учета регистра английских букв.
   *
   * @param s1      первая строка.
   * @param s2      вторая строка.
   * @param maxcnt  максимальное количество сравниваемых символов.
   *
   * @return  true  - строки идентичны,
   *          false - строки отличаются.
  */
  template<typename X> 
  inline bool __strequ_enx(const X* s1, const X* s2, size_t maxcnt)
  {
    if(s1 == s2)
    {
      return true;
    }
    if(!s1 || !s2 || !maxcnt)
    {
      return false;
    }

    for(size_t i = 0; i < maxcnt; i += 1)
    {
      X c1 = s1[i];
      X c2 = s2[i];
      if(__chrequ_ex<X>(c1, c2) == false)
      {
        return false;
      }
      if(c1 == 0 || c2 == 0)
      {
        break;
      }
    }

    return true;
  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // strcmp_hpp__