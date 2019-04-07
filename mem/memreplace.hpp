#ifndef memreplace_hpp__
#define memreplace_hpp__

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * [Шаблон] Запись новых значений поверх старых.
   *
   * Поиск выполняется с шагом в 1 байт.
   * Перезаписанное значение пропускается полностью, чтобы не создавать коллизий.
   *
   * @param data      данные.
   * @param size      размер данных.
   * @param old_value значение для поиска.
   * @param new_value значение для записи поверх найденного.
   *
   * @return количество перезаписанных значений,
   *         0 если не найдено значений для перезаписи или произошла ошибка.
  */
  template<typename T>
  inline size_t __memreplace_x(void* data, size_t size, T old_value, T new_value)
  {
    size_t cnt = 0;

    if(data != NULL && size >= sizeof(T))
    {
      T* ptr = (T*)((unsigned char*)data);
      T* end = (T*)((unsigned char*)data + size - sizeof(T));

      while(ptr <= end)
      {
        if(*ptr == old_value)
        {
          *ptr = new_value;
          cnt = cnt + 1;
          ptr = (T*)((unsigned char*)ptr + sizeof(T)); 
        }
        else
        {
          ptr = (T*)((unsigned char*)ptr + 1);
        }
      }
    }

    return cnt;
  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // memreplace_hpp__