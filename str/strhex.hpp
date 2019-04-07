#ifndef strhex_hpp__
#define strhex_hpp__

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "chr.hpp"
#include "chrnum.hpp"
#include "strlen.hpp"

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * [Шаблон] Конвертация текстовой HEX последовательности в бинарные данные.
   *
   * Процедура содержит механизм расчета размера конечных данных на основании содержимого.
   * Позволяет определить требуемый размер буфера через вызов процедуры с указанием адреса 
   * буфера в качестве NULL (классическое применение в два последовательных вызова).
   *
   * @param string       текстовая HEX последовательности.
   * @param buffer       буфер для записи бинарных данных.
   * @param buffer_size  размер буфера в байтах для записи бинарных данных.
   *
   * @return  требуемый размер буфера в байтах для записи бинарных данных,
   *          результат успешен если возвращаемое значение не превышает размер буфера,
   *          0 в случае ошибки.
  **/
  template<typename X>
  inline size_t __strtohex_lx(const X* string, void* buffer, size_t buffer_size)
  {
    size_t result = 0;

    if(string != NULL)
    {
      size_t length = apilib::__strlen_x<X>(string);
      size_t offset = 0;

      while( length > 0 )
      {
        X ca = string[0];
        if(apilib::__isspace_ex<X>(ca) == 1)
        {
          string += 1;
          length -= 1;
          continue;
        }
        if(apilib::__isxdigit_ex<X>(ca) == 0 || length < 2)
        {
          offset = 0;
          break;
        }
        X cb = string[1];
        if(apilib::__isxdigit_ex<X>(cb) == 0)
        {
          offset = 0;
          break;
        }
        if(buffer && offset < buffer_size)
        {
          unsigned char da = 0;
          if(apilib::__chrton_x<X, unsigned char>(ca, &da, 16) == 0)
          {
            offset = 0;
            break;
          }
          unsigned char db = 0;
          if(apilib::__chrton_x<X, unsigned char>(cb, &db, 16) == 0)
          {
            offset = 0;
            break;
          }
          ((unsigned char*)buffer)[offset] = ((da << 4) & 0xf0) | (db & 0x0f);
        }
        offset += 1;
        string += 2;
        length -= 2;
      }

      result = offset;
    }

    return result;
  }

  /**
   * [Шаблон] Конвертация текстовой HEX последовательности в бинарные данные.
   *
   * Процедура содержит механизм расчета размера конечных данных на основании содержимого.
   * Позволяет определить требуемый размер буфера через вызов процедуры с указанием адреса 
   * буфера в качестве NULL (классическое применение в два последовательных вызова).
   *
   * @param string       текстовая HEX последовательности.
   * @param maxlen       максимальный длина текстовой HEX последовательности.
   * @param buffer       буфер для записи бинарных данных.
   * @param buffer_size  размер буфера в байтах для записи бинарных данных.
   *
   * @return  требуемый размер буфера в байтах для записи бинарных данных,
   *          результат успешен если возвращаемое значение не превышает размер буфера,
   *          0 в случае ошибки.
  **/
  template<typename X>
  inline size_t __strtohex_lnx(const X* string, size_t maxlen, void* buffer, size_t buffer_size)
  {
    size_t result = 0;

    if(string != NULL && maxlen != 0)
    {
      size_t length = apilib::__strlen_nx<X>(string, maxlen);
      size_t offset = 0;

      while( length > 0 )
      {
        X ca = string[0];
        if(apilib::__isspace_ex<X>(ca) == 1)
        {
          string += 1;
          length -= 1;
          continue;
        }
        if(apilib::__isxdigit_ex<X>(ca) == 0 || length < 2)
        {
          offset = 0;
          break;
        }
        X cb = string[1];
        if(apilib::__isxdigit_ex<X>(cb) == 0)
        {
          offset = 0;
          break;
        }
        if(buffer && offset < buffer_size)
        {
          unsigned char da = 0;
          if(apilib::__chrton_x<X, unsigned char>(ca, &da, 16) == 0)
          {
            offset = 0;
            break;
          }
          unsigned char db = 0;
          if(apilib::__chrton_x<X, unsigned char>(cb, &db, 16) == 0)
          {
            offset = 0;
            break;
          }
          ((unsigned char*)buffer)[offset] = ((da << 4) & 0xf0) | (db & 0x0f);
        }
        offset += 1;
        string += 2;
        length -= 2;
      }

      result = offset;
    }

    return result;
  }

  /**
   * [Шаблон] Генерация текстовой HEX-последовательности представляющей бинарные данные.
   *
   * Процедура содержит механизм быстрого предварительного расчета размера конечных данных.
   * Позволяет определить требуемый размер буфера через вызов процедуры с указанием адреса 
   * буфера в качестве NULL (классическое применение в два последовательных вызова).
   *
   * Данная процедура предназначена для записи коротких HEX последовательностей с заранее
   * предсказанным размером буфера, рекомендуется использовать безопасную версию процедуры
   * @link __strgenhexseq_lnx.
   *
   * @param binary     бинарные данные.
   * @param length     размер бинарных данных в байтах.
   * @param buffer     буфер для записи текстовых данных, допускается NULL.
   * @param separator  следует ли разделять символы пробелами, 1 - разделять, 0 - не разделять.
   * @param multiline  следует ли разделять символы на строки, 1 - разделять, 0 - не разделять.
   *
   * @return  требуемый размер буфера для записи результата без учета нулевого символа,
   *          результат успешно записан если возвращаемое значение меньше @link maxlen,
   *          0 в случае ошибки.
  */
  template<typename X>
  inline size_t __strgenhexseq_lx(const void* binary, size_t length, X* buffer, int separator, int multiline)
  {
    size_t result = 0;

    if(binary != NULL && length != 0)
    {
      static const size_t LINE_LENGTH = 16;
      size_t amount = 0;

      /**
       * Предварительное определение размера конечных данных.
       *
       * 1. Для записи каждого байта данных требуется два текстовых символа.
       * 2. При разделении текстовых символов используется один символ пробела ' '.
       * 3. При разделении текстовых строк используются два символа переноса строки '\r' и '\n'.
       *
       * Формула рассчета количества разделителей текстовых символов:
       *    count = ('binary size' - 'all lines count').
       * Формула рассчета количества разделителей текстовых строк:
       *    count = ('all lines count' - '1').
       *
       * ВАЖНО: Следует аккуратно вносить правки в код генератора конечных данных, т.к.
       * отсутствует прямая зависимость в расчете предварительного размера конечных данных.
      **/
      if(multiline)
      {
        const size_t lcount = (length / LINE_LENGTH) + ((length % LINE_LENGTH) ? 1 : 0);
        if(separator)amount = (length * 2) + ((lcount - 1) * 2) + (length - lcount);
        else         amount = (length * 2) + ((lcount - 1) * 2);
      }
      else 
      {
        if(separator)amount = (length * 2) + (length - 1);
        else         amount = (length * 2);
      }

      if(buffer != NULL)
      {
        for(size_t i = 0; i < length; i++)
        {
          unsigned char value = ((const unsigned char*)binary)[i];

          if(i != 0)
          {
            if(multiline == 1 && (i % LINE_LENGTH) == 0)
            {
              *buffer++ = '\r';
              *buffer++ = '\n';
            }
            else if(separator == 1)
            {
              *buffer++ = ' ';
            }
          }

          *buffer++ = apilib::__ntochr_x<X, unsigned char>((value >> 4) & 0x0f);
          *buffer++ = apilib::__ntochr_x<X, unsigned char>((value     ) & 0x0f);
        }

        *buffer++ = '\0';
      }

      result = amount;
    }

    return result;
  }

  /**
   * [Шаблон] Генерация текстовой HEX-последовательности представляющей бинарные данные.
   *
   * Процедура содержит механизм быстрого предварительного расчета размера конечных данных.
   * Позволяет определить требуемый размер буфера через вызов процедуры с указанием адреса 
   * буфера в качестве NULL (классическое применение в два последовательных вызова).
   *
   * Конечные данные записываются только при условии достаточного размера буфера, 
   * частичная запись данных не осуществляется.
   *
   * @param binary     бинарные данные.
   * @param length     размер бинарных данных в байтах.
   * @param buffer     буфер для записи текстовых данных, допускается NULL.
   * @param maxlen     максимальная длина текстовых данных с учетом нулевого символа.
   * @param separator  следует ли разделять символы пробелами, 1 - разделять, 0 - не разделять.
   * @param multiline  следует ли разделять символы на строки, 1 - разделять, 0 - не разделять.
   *
   * @return  требуемый размер буфера для записи результата без учета нулевого символа,
   *          результат успешно записан если возвращаемое значение меньше @link maxlen,
   *          0 в случае ошибки.
  */
  template<typename X>
  inline size_t __strgenhexseq_lnx(const void* binary, size_t length, X* buffer, size_t maxlen, int separator, int multiline)
  {
    size_t result = 0;

    if(binary != NULL && length != 0)
    {
      static const size_t LINE_LENGTH = 16;
      size_t amount = 0;

      /**
       * Предварительное определение размера конечных данных.
       *
       * 1. Для записи каждого байта данных требуется два текстовых символа.
       * 2. При разделении текстовых символов используется один символ пробела ' '.
       * 3. При разделении текстовых строк используются два символа переноса строки '\r' и '\n'.
       *
       * Формула рассчета количества разделителей текстовых символов:
       *    count = ('binary size' - 'all lines count').
       * Формула рассчета количества разделителей текстовых строк:
       *    count = ('all lines count' - '1').
       *
       * ВАЖНО: Следует аккуратно вносить правки в код генератора конечных данных, т.к.
       * отсутствует прямая зависимость в расчете предварительного размера конечных данных.
      **/
      if(multiline)
      {
        const size_t lcount = (length / LINE_LENGTH) + ((length % LINE_LENGTH) ? 1 : 0);
        if(separator)amount = (length * 2) + ((lcount - 1) * 2) + (length - lcount);
        else         amount = (length * 2) + ((lcount - 1) * 2);
      }
      else 
      {
        if(separator)amount = (length * 2) + (length - 1);
        else         amount = (length * 2);
      }

      if(buffer != NULL && amount < maxlen)
      {
        for(size_t i = 0; i < length; i++)
        {
          unsigned char value = ((const unsigned char*)binary)[i];

          if(i != 0)
          {
            if(multiline == 1 && (i % LINE_LENGTH) == 0)
            {
              *buffer++ = '\r';
              *buffer++ = '\n';
            }
            else if(separator == 1)
            {
              *buffer++ = ' ';
            }
          }

          *buffer++ = apilib::__ntochr_x<X, unsigned char>((value >> 4) & 0x0f);
          *buffer++ = apilib::__ntochr_x<X, unsigned char>((value     ) & 0x0f);
        }

        *buffer++ = '\0';
      }

      result = amount;
    }

    return result;
  }

  /**
   * [Шаблон] Генерация текстовой HEX-таблицы представляющей бинарные данные.
   *
   * Процедура содержит механизм быстрого предварительного расчета размера конечных данных.
   * Позволяет определить требуемый размер буфера через вызов процедуры с указанием адреса 
   * буфера в качестве NULL (классическое применение в два последовательных вызова).
   *
   * Конечные данные записываются только при условии достаточного размера буфера, 
   * частичная запись данных не осуществляется.
   *
   * @param binary  бинарные данные.
   * @param length  размер бинарных данных в байтах.
   * @param buffer  буфер для записи текстовых данных, допускается NULL.
   * @param maxlen  максимальная длина текстовых данных с учетом нулевого символа.
   *
   * @return  требуемый размер буфера для записи результата без учета нулевого символа,
   *          результат успешно записан если возвращаемое значение меньше @link maxlen,
   *          0 в случае ошибки.
  **/
  template<typename X>
  inline size_t __strgenhextbl_lnx(const void* binary, size_t length, X* buffer, size_t maxlen)
  {
    size_t result = 0;

    if(binary != NULL && length != 0)
    {
      static const size_t LINE_LENGTH = 86;

      /**
       * Предварительное определение размера конечных данных.
       *
       * 1. Длина линии фиксирована и завершается двумя символами переноса строки '\r' и '\n'.
       * 2. Таблица содержит две линии обрамления фиксированного размера (сверху и снизу).
       * 3. Содержимое оформлено тремя столбиками: смещение, HEX-байты, ASCII-символы.
       * 4. Содержимое формируется из блоков по 16 символов для каждой линии.
       * 5. Содержимое неполной линии заполняется пробелами для выравнивания геометрии.
       *
       * ВАЖНО: Следует аккуратно вносить правки в код генератора конечных данных, т.к.
       * отсутствует прямая зависимость в расчете предварительного размера конечных данных.
      **/
      size_t amount = 0;
      amount += ((LINE_LENGTH + 2) * 2);                     // Размер линий обрамления сверху и снизу.
      amount += ((LINE_LENGTH + 2) * (length / 16));         // Размер линий контента (полных линий).
      amount += ((LINE_LENGTH + 2) * (length % 16 ? 1 : 0)); // Размер линий контента (неполных линий).

      if(buffer != NULL && amount < maxlen)
      {
        /**
         * Отрисовка линии сверху.
        **/
        for(size_t i = 0; i < LINE_LENGTH; i++) *buffer++ = '-';
        *buffer++ = '\r';
        *buffer++ = '\n';

        /**
         * Отрисовка контента.
        **/
        for(size_t i = 0; i < length; i += 16)
        {
          *buffer++ = '|';
          *buffer++ = ' ';
          *buffer++ = '0';
          *buffer++ = 'x';
          *buffer++ = apilib::__ntochr_x<X, unsigned char>((i >> 28) & 0x0f);
          *buffer++ = apilib::__ntochr_x<X, unsigned char>((i >> 24) & 0x0f);
          *buffer++ = apilib::__ntochr_x<X, unsigned char>((i >> 20) & 0x0f);
          *buffer++ = apilib::__ntochr_x<X, unsigned char>((i >> 16) & 0x0f);
          *buffer++ = apilib::__ntochr_x<X, unsigned char>((i >> 12) & 0x0f);
          *buffer++ = apilib::__ntochr_x<X, unsigned char>((i >>  8) & 0x0f);
          *buffer++ = apilib::__ntochr_x<X, unsigned char>((i >>  4) & 0x0f);
          *buffer++ = apilib::__ntochr_x<X, unsigned char>((i      ) & 0x0f);
          *buffer++ = ' ';
          *buffer++ = '|';
          *buffer++ = ' ';

          for(size_t j = 0; j < 16; j++)
          {
            if(j != 0) *buffer++ = ' ';
            if(j == 8) *buffer++ = ' ';

            if((i + j) < length)
            {
              unsigned char value = ((const unsigned char*)binary)[i + j];
              *buffer++ = apilib::__ntochr_x<X, unsigned char>((value >> 4) & 0x0f);
              *buffer++ = apilib::__ntochr_x<X, unsigned char>((value     ) & 0x0f);
            }
            else
            {
              *buffer++ = ' ';
              *buffer++ = ' ';
            }
          }

          *buffer++ = ' ';
          *buffer++ = '|';
          *buffer++ = ' ';
          *buffer++ = '"';

          for(size_t j = 0; j < 16; j++)
          {
            if((i + j) < length)
            {
              unsigned char value = ((const unsigned char*)binary)[i + j];
              *buffer++ = ((value < 32 || value >= 127) ? '.' : value);
            }
            else
            {
              *buffer++ = '.';
            }
          }

          *buffer++ = '"';
          *buffer++ = ' ';
          *buffer++ = '|';
          *buffer++ = '\r';
          *buffer++ = '\n';
        }

        /**
         * Отрисовка линии снизу.
        **/
        for(size_t i = 0; i < LINE_LENGTH; i++) *buffer++ = '-';
        *buffer++ = '\r';
        *buffer++ = '\n';

        /**
         * Нулевой символ.
        **/
        *buffer++ = '\0';
      }

      result = amount;
    }

    return result;
  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // strhex_hpp__