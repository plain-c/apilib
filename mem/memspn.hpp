#ifndef memspn_hpp__
#define memspn_hpp__

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * [Шаблон] Количество значений присутствующих в памяти подряд от начала данных.
   *
   * @param data  данные.
   * @param size  размер данных.
   * @param value значение.
   *
   * @return количество значений подряд в случае успеха,
   *         0 если значений не найдено или произошла ошибка.
  */
  template<typename T>
  inline size_t __memspn_x(const void* data, size_t size, T value)
  {
    size_t cnt = 0;

    if(data != NULL && size >= sizeof(T))
    {
      const T* ptr = (const T*)((const unsigned char*)data);
      const T* end = (const T*)((const unsigned char*)data + size - sizeof(T));

      while(ptr <= end)
      {
        if(*ptr != value)
        {
          break;
        }
        cnt = cnt + 1;
        ptr = (const T*)((const unsigned char*)ptr + sizeof(T));
      }
    }

    return cnt;
  }
  
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // memspn_hpp__