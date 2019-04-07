#ifndef strchr_h__
#define strchr_h__

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Поиск символа в строке.
   *
   * @param string  строка.
   * @param chr     символ.
   *
   * @return  указатель на найденный символ в строке,
   *          NULL если символ не найден или произошла ошибка.
  */
  char* strchr_a(const char* string, char chr);
  wchar_t* strchr_w(const wchar_t* string, wchar_t chr);

  /**
   * Поиск символа в строке.
   *
   * @param string  строка.
   * @param maxlen  максимальная длина строки с учетом нулевого символа.
   * @param chr     символ.
   *
   * @return  указатель на найденный символ в строке,
   *          NULL если символ не найден или произошла ошибка.
  */
  char* strchr_na(const char* string, size_t maxlen, char chr);
  wchar_t* strchr_nw(const wchar_t* string, size_t maxlen, wchar_t chr);

  /**
   * Поиск символа c конца строки.
   *
   * @param string  строка.
   * @param chr     символ.
   *
   * @return  указатель на найденный символ в строке,
   *          NULL если символ не найден или произошла ошибка.
  */
  char* strrchr_a(const char* string, char chr);
  wchar_t* strrchr_w(const wchar_t* string, wchar_t chr);

  /**
   * Поиск символа c конца строки.
   *
   * @param string  строка.
   * @param maxlen  максимальная длина строки с учетом нулевого символа.
   * @param chr     символ.
   *
   * @return  указатель на найденный символ в строке,
   *          NULL если символ не найден или произошла ошибка.
  */
  char* strrchr_na(const char* string, size_t maxlen, char chr);
  wchar_t* strrchr_nw(const wchar_t* string, size_t maxlen, wchar_t chr);
  
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // strchr_h__