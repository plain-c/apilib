#ifndef strstr_h__
#define strstr_h__

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Поиск подстроки.
   *
   * @param string1  строка.
   * @param string2  искомая подстрока.
   *
   * @return  указатель на найденную подстроку в строке,
   *          NULL если подстрока не найдена или произошла ошибка.
  */
  char* strstr_a(const char* string1, const char* string2);
  wchar_t* strstr_w(const wchar_t* string1, const wchar_t* string2);

  /**
   * Поиск подстроки.
   *
   * @param string1  строка.
   * @param string2  искомая подстрока.
   * @param length1  длина строки без учета нулевого символа.
   * @param length2  длина искомой подстроки без учета нулевого символа.
   *
   * @return  указатель на найденную подстроку в строке,
   *          NULL если подстрока не найдена или произошла ошибка.
  */
  char* strstr_na(const char* string1, const char* string2, size_t length1, size_t length2);
  wchar_t* strstr_nw(const wchar_t* string1, const wchar_t* string2, size_t length1, size_t length2);

  /**
   * Поиск подстроки без учета регистра английских букв.
   *
   * @param string1  строка.
   * @param string2  искомая подстрока.
   *
   * @return  указатель на найденную подстроку в строке,
   *          NULL если подстрока не найдена или произошла ошибка.
  */
  char* strstr_ea(const char* string1, const char* string2);
  wchar_t* strstr_ew(const wchar_t* string1, const wchar_t* string2);

  /**
   * Поиск подстроки без учета регистра английских букв.
   *
   * @param string1  строка.
   * @param string2  искомая подстрока.
   * @param length1  длина строки без учета нулевого символа.
   * @param length2  длина искомой подстроки без учета нулевого символа.
   *
   * @return  указатель на найденную подстроку в строке,
   *          NULL если подстрока не найдена или произошла ошибка.
  */
  char* strstr_ena(const char* string1, const char* string2, size_t length1, size_t length2);
  wchar_t* strstr_enw(const wchar_t* string1, const wchar_t* string2, size_t length1, size_t length2);

  /**
   * Поиск подстроки с конца строки.
   *
   * @param string1  строка.
   * @param string2  искомая подстрока.
   *
   * @return  указатель на найденную подстроку в строке,
   *          NULL если подстрока не найдена или произошла ошибка.
  */
  char* strrstr_a(const char* string1, const char* string2);
  wchar_t* strrstr_w(const wchar_t* string1, const wchar_t* string2);

  /**
   * Поиск подстроки с конца строки.
   *
   * @param string1  строка.
   * @param string2  искомая подстрока.
   * @param length1  длина строки без учета нулевого символа.
   * @param length2  длина искомой подстроки без учета нулевого символа.
   *
   * @return  указатель на найденную подстроку в строке,
   *          NULL если подстрока не найдена или произошла ошибка.
  */
  char* strrstr_na(const char* string1, const char* string2, size_t length1, size_t length2);
  wchar_t* strrstr_nw(const wchar_t* string1, const wchar_t* string2, size_t length1, size_t length2);

  /**
   * Поиск подстроки с конца строки.
   *
   * @param string1  строка.
   * @param string2  искомая подстрока.
   *
   * @return  указатель на найденную подстроку в строке,
   *          NULL если подстрока не найдена или произошла ошибка.
  */
  char* strrstr_ea(const char* string1, const char* string2);
  wchar_t* strrstr_ew(const wchar_t* string1, const wchar_t* string2);

  /**
   * Поиск подстроки с конца строки.
   *
   * @param string1  строка.
   * @param string2  искомая подстрока.
   * @param length1  длина строки без учета нулевого символа.
   * @param length2  длина искомой подстроки без учета нулевого символа.
   *
   * @return  указатель на найденную подстроку в строке,
   *          NULL если подстрока не найдена или произошла ошибка.
  */
  char* strrstr_ena(const char* string1, const char* string2, size_t length1, size_t length2);
  wchar_t* strrstr_enw(const wchar_t* string1, const wchar_t* string2, size_t length1, size_t length2);

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // strstr_h__