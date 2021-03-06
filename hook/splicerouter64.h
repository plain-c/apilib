#ifndef splicerouter_h__
#define splicerouter_h__

///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * ������������� �������� �� �������� �������� ����� �������� ������ � ��������������� ���������.
 *
 * # ��������:
 *   ���������� ������������ ������ �������� 14 ����, � ������� ��� ������ ����� ������� 5 ����.
 *   ���������� 14 ���� ����� ����� � ��������� �� �������� ���������� � ���������� �������� � 
 *   ���������� �������� ������, �������� ������ ��� �������� �������.
 * 
 * # �������:
 *   ������� ������ �� ������� ������� �� �������� ������ � ��������������� ���������, ����� 
 *   ������� ���������� ���������� ������� ������ �� �������������. ����� ������� ����������
 *   ���������� ����� ��������� ������� �� ���������� ����� �� ���� �� ������ "�" � ������ "�".
*/

///////////////////////////////////////////////////////////////////////////////////////////////////

#if(APILIB_64 != 0)

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//


  /**
   * ��������� ������ ������������� ���������.
   *
   * @param from ����� �� �������� ���������� ������� ������.
   * @param to   ����� �� ������� ���������� ������� ������.
   *
   * @return ����� ��������� � ������ ������, NULL � ������ ������.
  */
  void* splicerouter_get_personal(const void* from, const void* to);

  /**
   * ������������ ������������� ���������.
   *
   * @param address ����� ��������� ��� ����� � �����.
   *
   * @return true � ������ ������, false � ������ ������.
  */
  bool splicerouter_free_personal(const void* address);

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

#endif // #if(APILIB_64 != 0)

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // splicerouter_h__