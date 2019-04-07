#ifndef strmem_hpp__
#define strmem_hpp__

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "strcpy.hpp"
#include "strlen.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Импортируемая процедура аллоцирования памяти.
*/
extern void* apilib_mem_alloc(size_t size);

/**
 * Импортируемая процедура освобождения памяти.
*/
extern void apilib_mem_free(void* ptr);

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * [Шаблон] Аллоцирование строки в памяти.
   *
   * @param length  длина строки в символах, без учета нулевого символа.
   *
   * @return  в случае успеха - строка заполненная нулями, 
   *          в случае ошибки - NULL.
  */
  template<typename X>
  inline X* __stralloc_x(size_t length)
  {
    X* allocated = (X*)apilib_mem_alloc((length + 1) * sizeof(X));
    if(allocated)
    {
      for(size_t i = 0; i <= length; i++)
      {
        allocated[i] = 0;
      }
    }
    return allocated;
  }

  /**
   * [Шаблон] Освобождение строки из памяти.
   *
   * @param string  строка аллоцированная в памяти.
   *
   * @return  в случае успеха - true,
   *          в случае ошибки - false.
  */
  template<typename X>
  inline bool __strfree_x(X* string)
  {
    if(!string)return false;
    apilib_mem_free(string);
    return true;
  }

  /**
   * [Шаблон] Копирование строки через аллоцирование памяти.
   *
   * @param string  строка.
   *
   * @return  в случае успеха - копия строки в памяти,
   *          в случае ошибки - NULL.
  */
  template<typename X>
  inline X* __strdup_x(const X* string)
  {
    if(!string)
    {
      return NULL;
    }
    
    size_t length = __strlen_x<X>(string);

    X* allocated = (X*)apilib_mem_alloc((length + 1) * sizeof(X));
    if(allocated)
    {
      if(length != 0)
      {
        __strcpy_nx<X>(allocated, string, length);
      }
      else
      {
        allocated[length] = 0;
      }
    }

    return allocated;
  }

  /**
   * [Шаблон] Копирование строки через аллоцирование памяти.
   *
   * @param string  строка.
   * @param length  количество копируемых символов.
   *
   * @return  в случае успеха - копия строки в памяти,
   *          в случае ошибки - NULL.
  */
  template<typename X>
  inline X* __strdup_nx(const X* string, size_t length)
  {
    if(!string)
    {
      return NULL;
    }

    X* allocated = (X*)apilib_mem_alloc((length + 1) * sizeof(X));
    if(allocated)
    {
      if(length != 0)
      {
        __strcpy_nx<X>(allocated, string, length);
      }
      else
      {
        allocated[length] = 0;
      }
    }

    return allocated;
  }

  /**
  * [Шаблон] Добавление одной строки в конец другой строки через реаллоцирование памяти.
  *
  * @param dst  конечная строка обновляемая через двойной указатель.
  * @param src  исходная строка добавляемая в конец конечной строки.
   *
   * @return  в случае успеха - true,
   *          в случае ошибки - false.
  */
  template<typename X>
  inline bool __stradd_x(X** dst, const X* src)
  {
    if(!dst || !src)
    {
      return false;
    }

    size_t dst_length = __strlen_x<X>(*dst);
    size_t src_length = __strlen_x<X>(src);  
    
    X* allocated = (X*)apilib_mem_alloc((dst_length + src_length + 1) * sizeof(X));
    if(allocated)
    {
      if(dst_length)__strcpy_nx<X>(&allocated[0],         *dst, dst_length);
      if(src_length)__strcpy_nx<X>(&allocated[dst_length], src, src_length);
      *dst = allocated;
      return true;
    }

    return false;
  }

  /**
   * [Шаблон] Добавление одной строки в конец другой строки через реаллоцирование памяти.
   *
   * @param dst     конечная строка обновляемая через двойной указатель.
   * @param src     исходная строка добавляемая в конец конечной строки.
   * @param maxcnt  максимальное количество добавляемых символов из исходной строки.
   *
   * @return  в случае успеха - true,
   *          в случае ошибки - false.
  */
  template<typename X>
  inline bool __stradd_nx(X** dst, const X* src, size_t maxcnt)
  {
    if(!dst || !src)
    {
      return false;
    }
    
    size_t dst_length = __strlen_x<X>(*dst);
    size_t src_length = __strlen_nx<X>(src, maxcnt);
    
    X* allocated = (X*)apilib_mem_alloc((dst_length + src_length + 1) * sizeof(X));
    if(allocated)
    {
      if(dst_length)__strcpy_nx<X>(&allocated[0],         *dst, dst_length);
      if(src_length)__strcpy_nx<X>(&allocated[dst_length], src, src_length);
      *dst = allocated;
      return true;
    }

    return false;
  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // strmem_hpp__