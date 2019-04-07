#ifndef strspn_h__
#define strspn_h__

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Количество символов присутствующих в списке с начала строки.
   *
   * @param string  строка.
   * @param accept  список символов которые должны присутствовать.
   *
   * @return  количество символов присутствующих в списке с начала строки,
   *          0 в случае ошибки.
  */
  size_t strspn_a(const char* string, const char* accept);
  size_t strspn_w(const wchar_t* string, const wchar_t* accept);

  /**
   * Количество символов присутствующих в списке с начала строки.
   *
   * @param string  строка.
   * @param accept  список символов которые должны присутствовать.
   * @param maxcnt  максимальная длина строки без учета нулевого символа.
   *
   * @return  количество символов присутствующих в списке с начала строки,
   *          0 в случае ошибки.
  */
  size_t strspn_na(const char* string, const char* accept, size_t maxcnt);
  size_t strspn_nw(const wchar_t* string, const wchar_t* accept, size_t maxcnt);

  /**
   * Количество символов отсутствующих в списке с начала строки.
   *
   * @param string  строка.
   * @param reject  список символов которые должны отсутствовать.
   *
   * @return  количество символов отсутствующих в списке с начала строки,
   *          0 в случае ошибки.
  */
  size_t strcspn_a(const char* string, const char* reject);
  size_t strcspn_w(const wchar_t* string, const wchar_t* reject);

  /**
   * Количество символов отсутствующих в списке с начала строки.
   *
   * @param string  строка.
   * @param reject  список символов которые должны отсутствовать.
   * @param maxcnt  максимальная длина строки без учета нулевого символа.
   *
   * @return  количество символов отсутствующих в списке с начала строки,
   *          0 в случае ошибки.
  */
  size_t strcspn_na(const char* string, const char* reject, size_t maxcnt);
  size_t strcspn_nw(const wchar_t* string, const wchar_t* reject, size_t maxcnt);

  /**
   * Количество символов присутствующих в списке с конца строки.
   *
   * @param string  строка.
   * @param accept  список символов которые должны присутствовать.
   *
   * @return  количество символов присутствующих в списке с конца строки,
   *          0 в случае ошибки.
  */
  size_t strrspn_a(const char* string, const char* accept);
  size_t strrspn_w(const wchar_t* string, const wchar_t* accept);

  /**
   * Количество символов присутствующих в списке с конца строки.
   *
   * @param string  строка.
   * @param accept  список символов которые должны присутствовать.
   * @param maxcnt  максимальная длина строки без учета нулевого символа.
   *
   * @return  количество символов присутствующих в списке с конца строки,
   *          0 в случае ошибки.
  */
  size_t strrspn_na(const char* string, const char* accept, size_t maxcnt);
  size_t strrspn_nw(const wchar_t* string, const wchar_t* accept, size_t maxcnt);

  /**
   * Количество символов отсутствующих в списке с конца строки.
   *
   * @param string  строка.
   * @param reject  список символов которые должны отсутствовать.
   *
   * @return  количество символов отсутствующих в списке с конца строки,
   *          0 в случае ошибки.
  */
  size_t strrcspn_a(const char* string, const char* reject);
  size_t strrcspn_w(const wchar_t* string, const wchar_t* reject);

  /**
   * Количество символов отсутствующих в списке с конца строки.
   *
   * @param string  строка.
   * @param reject  список символов которые должны отсутствовать.
   * @param maxcnt  максимальная длина строки без учета нулевого символа.
   *
   * @return  количество символов отсутствующих в списке с конца строки,
   *          0 в случае ошибки.
  */
  size_t strrcspn_na(const char* string, const char* reject, size_t maxcnt);
  size_t strrcspn_nw(const wchar_t* string, const wchar_t* reject, size_t maxcnt);

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // strspn_h__