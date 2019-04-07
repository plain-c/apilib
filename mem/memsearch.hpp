#ifndef memsearch_hpp__
#define memsearch_hpp__

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * [Ўаблон] ѕоиск значени€.
   *
   * ѕоиск выполн€етс€ с шагом в 1 байт.
   * ѕоиск выполн€етс€ до первого совпадени€.
   *
   * @param data  данные.
   * @param size  размер данных.
   * @param value значение дл€ поиска.
   *
   * @return найденное значение в случае успеха,
   *         NULL если значение не найдено или произошла ошибка.
  */
  template<typename T>
  inline T* __memsearch_x(const void* data, size_t size, T value)
  {
    T* result = NULL;

    if(data != NULL && size >= sizeof(T))
    {
      const T* ptr = (const T*)((const unsigned char*)data);
      const T* end = (const T*)((const unsigned char*)data + size - sizeof(T));

      while(ptr <= end)
      {
        if(*ptr == value)
        {
          result = (T*)ptr;
          break;
        }
        ptr = (const T*)((const unsigned char*)ptr + 1);
      }
    }

    return result;
  }
  
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // memsearch_hpp__