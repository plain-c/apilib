#ifndef strenc_hpp__
#define strenc_hpp__

///////////////////////////////////////////////////////////////////////////////////////////////////

# include "strmem.hpp"
# if(APILIB_OS_WINNT != 0)
# include <windows.h>
# endif

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * [Шаблон] Конвертация между кодировками ANSI и Unicode только для ASCII символов.
   *
   * Этот шаблон создан для двунаправленной зеркальной конвертации символов.
   * Символы выходящие за пределы диапазона 0x00 - 0x7F считаются ошибкой.
   *
   * @param dst     конечная строка.
   * @param maxlen  максимальная длина конечной строки с учетом нулевого символа.
   * @param src     исходная строка.
   *
   * @return  требуемая длина строки без учета нулевого символа,
   *          требуемая длина равная или больше maxlen означает недостаточный размер буфера,
   *          в случае ошибки (size_t)-1.
  */
  template<typename X1, typename X2>
  inline size_t __xcstoxcs_le(X2* dst, size_t maxlen, const X1* src)
  {
    size_t result = 0;

    if(src)
    {
      size_t length = __strlen_x<X1>(src);

      if(dst && length < maxlen)
      {
        for(size_t i = 0; i < length; i++)
        {
          X1 chr = src[i];
          if(chr > 0x7F)
          {
            result = (size_t)-1;
            break;
          }
          dst[i] = chr;
          result = result + 1;
        }
      }
      else
      {
        result = length;
      }
    }

    if(dst && maxlen)
    {
      if(result < maxlen)dst[result] = 0;
      else               dst[     0] = 0;
    }

    return result;
  }

  /**
   * [Шаблон] Конвертация между кодировками ANSI и Unicode только для ASCII символов.
   *
   * Этот шаблон создан для двунаправленной зеркальной конвертации символов.
   * Символы выходящие за пределы диапазона 0x00 - 0x7F считаются ошибкой.
   *
   * @param dst     конечная строка.
   * @param maxlen  максимальная длина конечной строки с учетом нулевого символа.
   * @param src     исходная строка.
   * @oaram length  длина исходной строки без учета нулевого символа.
   *
   * @return  требуемая длина строки без учета нулевого символа,
   *          требуемая длина равная или больше maxlen означает недостаточный размер буфера,
   *          в случае ошибки (size_t)-1.
  */
  template<typename X1, typename X2>
  inline size_t __xcstoxcs_lne(X2* dst, size_t maxlen, const X1* src, size_t length)
  {
    size_t result = 0;

    if(src)
    {
      if(dst && length < maxlen)
      {
        for(size_t i = 0; i < length; i++)
        {
          X1 chr = src[i];
          if(chr > 0x7F)
          {
            result = (size_t)-1;
            break;
          }
          dst[i] = chr;
          result = result + 1;
        }
      }
      else
      {
        result = length;
      }
    }

    if(dst && maxlen)
    {
      if(result < maxlen)dst[result] = 0;
      else               dst[     0] = 0;
    }

    return result;
  }

  /**
   * [Шаблон] Конвертация из многобайтовой кодировки в широкосимвольную кодировку.
   *
   * @param codepage  кодировка многобайтовой строки.
   * @param flags     дополнительные флаги.
   * @param dst       конечная строка.
   * @param maxlen    максимальная длина конечной строки с учетом нулевого символа.
   * @param src       исходная строка.
   *
   * @return  требуемая длина строки без учета нулевого символа,
   *          требуемая длина равная или больше maxlen означает недостаточный размер буфера,
   *          в случае ошибки (size_t)-1.
  */
  inline size_t __mbstowcs_l(unsigned long int codepage, unsigned long int flags, wchar_t* dst, size_t maxlen, const char* src)
  {
#   if(APILIB_OS_WINNT != 0)
    size_t result = 0;
    size_t length = 0;

    if(src && (length = __strlen_x<char>(src)) != 0)
    {
      ::SetLastError(ERROR_SUCCESS);

      int converted = ::MultiByteToWideChar(codepage, flags, src, (int)length, dst, (int)maxlen);
      if( converted > 0 )
      {
        result = converted;
      }

      unsigned long int error = ::GetLastError();
      if(error != ERROR_SUCCESS &&
         error != ERROR_INSUFFICIENT_BUFFER)
      {
        result = (size_t)-1;
      }
    }

    if(dst && maxlen)
    {
      if(result < maxlen)dst[result] = 0;
      else               dst[     0] = 0;
    }
    return result;
#   else
    return(size_t)-1;
#   endif
  }


  /**
   * [Шаблон] Конвертация из многобайтовой кодировки в широкосимвольную кодировку.
   *
   * @param codepage  кодировка многобайтовой строки.
   * @param flags     дополнительные флаги.
   * @param dst       конечная строка.
   * @param maxlen    максимальная длина конечной строки с учетом нулевого символа.
   * @param src       исходная строка.
   * @oaram length    длина исходной строки без учета нулевого символа.
   *
   * @return  требуемая длина строки без учета нулевого символа,
   *          требуемая длина равная или больше maxlen означает недостаточный размер буфера,
   *          в случае ошибки (size_t)-1.
  */
  inline size_t __mbstowcs_ln(unsigned long int codepage, unsigned long int flags, wchar_t* dst, size_t maxlen, const char* src, size_t length)
  {
#   if(APILIB_OS_WINNT != 0)
    size_t result = 0;

    if(src && length)
    {
      ::SetLastError(ERROR_SUCCESS);

      int converted = ::MultiByteToWideChar(codepage, flags, src, (int)length, dst, (int)maxlen);
      if( converted > 0 )
      {
        result = converted;
      }

      unsigned long int error = ::GetLastError();
      if(error != ERROR_SUCCESS &&
         error != ERROR_INSUFFICIENT_BUFFER)
      {
        result = (size_t)-1;
      }
    }

    if(dst && maxlen)
    {
      if(result < maxlen)dst[result] = 0;
      else               dst[     0] = 0;
    }

    return result;
#   else
    return(size_t)-1;
#   endif
  }

  /**
   * [Шаблон] Конвертация из широкосимвольной кодировки в многобайтовую кодировку.
   *
   * @param codepage  кодировка многобайтовой строки.
   * @param flags     дополнительные флаги.
   * @param dst       конечная строка.
   * @param maxlen    максимальная длина конечной строки с учетом нулевого символа.
   * @param src       исходная строка.
   *
   * @return  требуемая длина строки без учета нулевого символа,
   *          требуемая длина равная или больше maxlen означает недостаточный размер буфера,
   *          в случае ошибки (size_t)-1.
  */
  inline size_t __wcstombs_l(unsigned long int codepage, unsigned long int flags, char* dst, size_t maxlen, const wchar_t* src)
  {
#   if(APILIB_OS_WINNT != 0)
    size_t result = 0;
    size_t length = 0;

    if(src && (length = __strlen_x<wchar_t>(src)) != 0)
    {
      ::SetLastError(ERROR_SUCCESS);

      int converted = ::WideCharToMultiByte(codepage, flags, src, (int)length, dst, (int)maxlen, NULL, NULL);
      if( converted > 0 )
      {
        result = converted;
      }

      unsigned long int error = ::GetLastError();
      if(error != ERROR_SUCCESS &&
         error != ERROR_INSUFFICIENT_BUFFER)
      {
        result = (size_t)-1;
      }
    }

    if(dst && maxlen)
    {
      if(result < maxlen)dst[result] = 0;
      else               dst[     0] = 0;
    }

    return result;
#   else
    return(size_t)-1;
#   endif
  }

  /**
   * [Шаблон] Конвертация из широкосимвольной кодировки в многобайтовую кодировку.
   *
   * @param codepage  кодировка многобайтовой строки.
   * @param flags     дополнительные флаги.
   * @param dst       конечная строка.
   * @param maxlen    максимальная длина конечной строки с учетом нулевого символа.
   * @param src       исходная строка.
   * @oaram length    длина исходной строки без учета нулевого символа.
   *
   * @return  требуемая длина строки без учета нулевого символа,
   *          требуемая длина равная или больше maxlen означает недостаточный размер буфера,
   *          в случае ошибки (size_t)-1.
  */
  inline size_t __wcstombs_ln(unsigned long int codepage, unsigned long int flags, char* dst, size_t maxlen, const wchar_t* src, size_t length)
  {
#   if(APILIB_OS_WINNT != 0)
    size_t result = 0;

    if(src && length)
    {
      ::SetLastError(ERROR_SUCCESS);

      int converted = ::WideCharToMultiByte(codepage, flags, src, (int)length, dst, (int)maxlen, NULL, NULL);
      if( converted > 0 )
      {
        result = converted;
      }

      unsigned long int error = ::GetLastError();
      if(error != ERROR_SUCCESS &&
         error != ERROR_INSUFFICIENT_BUFFER)
      {
        result = (size_t)-1;
      }
    }

    if(dst && maxlen)
    {
      if(result < maxlen)dst[result] = 0;
      else               dst[     0] = 0;
    }

    return result;
#   else
    return(size_t)-1;
#   endif
  }

  /**
   * [Шаблон] Конвертация из многобайтовой кодировки в широкосимвольную кодировку через аллоцирование памяти.
   *
   * @param codepage  кодировка многобайтовой строки.
   * @param flags     дополнительные флаги.
   * @param src       исходная строка.
   * @param result    длина конечной строки, допускается NULL.
   *
   * @return  в случае успеха - строка, которую следует освободить через strfree_w(),
   *          в случае ошибки - NULL.
  */
  inline wchar_t* __mbstowcs_dup(unsigned long int codepage, unsigned long int flags, const char* src, size_t* result)
  {
    size_t required = __mbstowcs_l(codepage, flags, NULL, 0, src);
    if(required == (size_t)-1)
    {
      return NULL;
    }

    wchar_t* allocated = __stralloc_x<wchar_t>(required);
    if(allocated == NULL)
    {
      return NULL;
    }

    size_t converted = __mbstowcs_l(codepage, flags, allocated, required + 1, src);
    if(converted == (size_t)-1)
    {
      __strfree_x<wchar_t>(allocated);
      return NULL;
    }

    if(result)
    {
      *result = (size_t)converted;
    }

    return allocated;
  }

  /**
   * [Шаблон] Конвертация из многобайтовой кодировки в широкосимвольную кодировку через аллоцирование памяти.
   *
   * @param codepage  кодировка многобайтовой строки.
   * @param flags     дополнительные флаги.
   * @param src       исходная строка.
   * @oaram length    длина исходной строки без учета нулевого символа.
   * @param result    длина конечной строки, допускается NULL.
   *
   * @return  в случае успеха - строка, которую следует освободить через strfree_w(),
   *          в случае ошибки - NULL.
  */
  inline wchar_t* __mbstowcs_dup_n(unsigned long int codepage, unsigned long int flags, const char* src, size_t length, size_t* result)
  {
    size_t required = __mbstowcs_ln(codepage, flags, NULL, 0, src, length);
    if(required == (size_t)-1)
    {
      return NULL;
    }

    wchar_t* allocated = __stralloc_x<wchar_t>(required);
    if(allocated == NULL)
    {
      return NULL;
    }

    size_t converted = __mbstowcs_ln(codepage, flags, allocated, required + 1, src, length);
    if(converted == (size_t)-1)
    {
      __strfree_x<wchar_t>(allocated);
      return NULL;
    }

    if(result)
    {
      *result = (size_t)converted;
    }

    return allocated;
  }

  /**
   * [Шаблон] Конвертация из широкосимвольной кодировки в многобайтовую кодировку через аллоцирование памяти.
   *
   * @param codepage  кодировка многобайтовой строки.
   * @param flags     дополнительные флаги.
   * @param src       исходная строка.
   * @param result    длина конечной строки, допускается NULL.
   *
   * @return  в случае успеха - строка, которую следует освободить через strfree_a(),
   *          в случае ошибки - NULL.
  */
  inline char* __wcstombs_dup(unsigned long int codepage, unsigned long int flags, const wchar_t* src, size_t* result)
  {
    size_t required = __wcstombs_l(codepage, flags, NULL, 0, src);
    if(required == (size_t)-1)
    {
      return NULL;
    }

    char* allocated = __stralloc_x<char>(required);
    if(allocated == NULL)
    {
      return NULL;
    }

    size_t converted = __wcstombs_l(codepage, flags, allocated, required + 1, src);
    if(converted == (size_t)-1)
    {
      __strfree_x<char>(allocated);
      return NULL;
    }

    if(result)
    {
      *result = (size_t)converted;
    }

    return allocated;
  }

  /**
   * [Шаблон] Конвертация из широкосимвольной кодировки в многобайтовую кодировку через аллоцирование памяти.
   *
   * @param codepage  кодировка многобайтовой строки.
   * @param flags     дополнительные флаги.
   * @param src       исходная строка.
   * @oaram length    длина исходной строки без учета нулевого символа.
   * @param result    длина конечной строки, допускается NULL.
   *
   * @return  в случае успеха - строка, которую следует освободить через strfree_a(),
   *          в случае ошибки - NULL.
  */
  inline char* __wcstombs_dup_n(unsigned long int codepage, unsigned long int flags, const wchar_t* src, size_t length, size_t* result)
  {
    size_t required = __wcstombs_ln(codepage, flags, NULL, 0, src, length);
    if(required == (size_t)-1)
    {
      return NULL;
    }

    char* allocated = __stralloc_x<char>(required);
    if(allocated == NULL)
    {
      return NULL;
    }

    size_t converted = __wcstombs_ln(codepage, flags, allocated, required + 1, src, length);
    if(converted == (size_t)-1)
    {
      __strfree_x<char>(allocated);
      return NULL;
    }

    if(result)
    {
      *result = (size_t)converted;
    }

    return allocated;
  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // strenc_hpp__