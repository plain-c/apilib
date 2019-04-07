#ifndef strlen_hpp__
#define strlen_hpp__

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * [Шаблон] Определение длины строки.
   * База для реализации функционала аналогичного strlen, wcslen.
   *
   * @param string  строка.
   * 
   * @return  в случае успеха - длина строки,
   *          в случае ошибки - 0.
  */
  template<typename X> 
  inline size_t __strlen_x(const X* string)
  {
    if(!string)
    {
      return 0;
    }

    size_t length = 0;

    while( string[length] != '\0' )
    {
      length += 1;
    }

    return length;
  }

  /**
   * [Шаблон] Определение длины строки.
   * База для реализации функционала аналогичного strnlen, wcsnlen.
   *
   * @param string  строка.
   * @param maxlen  максимальная длина строки с учетом нулевого символа.
   * 
   * @return  в случае успеха - длина строки без учета нулевого символа,
   *          в случае ошибки - 0.
  */
  template<typename X>
  inline size_t __strlen_nx(const X* string, size_t maxlen)
  {
    if(!string || !maxlen) 
    {
      return 0;
    }

    size_t length = 0;
    size_t zindex = maxlen - 1;

    while( length < zindex && string[length] != '\0' )
    {
      length += 1;
    }

    return length;
  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // strlen_hpp__