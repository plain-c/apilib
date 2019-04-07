#ifndef strlen_h__
#define strlen_h__

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * ����������� ����� ������.
   *
   * @param string  ������.
   * 
   * @return  � ������ ������ - ����� ������,
   *          � ������ ������ - 0.
  */
  size_t strlen_a(const char* s);
  size_t strlen_w(const wchar_t* s);

  /**
   * ����������� ����� ������.
   *
   * @param string  ������.
   * @param maxlen  ������������ ����� ������ � ������ �������� �������.
   * 
   * @return  � ������ ������ - ����� ������,
   *          � ������ ������ - 0.
  */
  size_t strlen_na(const char* s, size_t maxlen);
  size_t strlen_nw(const wchar_t* s, size_t maxlen);
  
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // strlen_h__