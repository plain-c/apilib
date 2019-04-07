#ifndef spliceimport_h__
#define spliceimport_h__

///////////////////////////////////////////////////////////////////////////////////////////////////

# if(APILIB_OS_WINNT != 0)
# include "../ntdll/ntdll.h"
# endif

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Тип данных идентификатора потока.
  */
# if(APILIB_OS_WINNT != 0)
  typedef unsigned long int spliceimport_tid_t;   
# endif

  /**
   * Тип данных идентификатора процесса.
  */
# if(APILIB_OS_WINNT != 0)
  typedef unsigned long int spliceimport_pid_t;   
# endif

  /**
   * Тип данных критической секции.
  */
# if(APILIB_OS_WINNT != 0)
  typedef RTL_CRITICAL_SECTION spliceimport_cs_t;
# endif

  /**
   * Некорректный TID.
  */
# if(APILIB_OS_WINNT != 0)
  static const spliceimport_tid_t THREAD_ID_INVALID = (spliceimport_tid_t)-1;
# endif

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Информация о виртуальной памяти.
  */
  struct memquery_t
  {
    void*  addr;     //Адрес.
    size_t size;     //Размер.
    bool   free;     //Память по указанному адресу свободна.
    bool   rsrv;     //Память по указанному адресу в резерве. 
  };

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Получение идентификатора текущего потока.
   *
   * @param tid идентификатор потока.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  bool spliceimport_get_current_tid(spliceimport_tid_t* tid);

  /**
   * Получение идентификатора текущего процесса.
   *
   * @param pid идентификатор процесса.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  bool spliceimport_get_current_pid(spliceimport_pid_t* pid);

  /**
   * Создание критической секции.
   *
   * @param cs критическая секция.
   * 
   * @return true в случае успеха, false в случае ошибки.
  */
  bool spliceimport_cs_create(spliceimport_cs_t* cs);

  /**
   * Разрушение критической секции.
   *
   * @param cs критическая секция.
   * 
   * @return true в случае успеха, false в случае ошибки.
  */
  bool spliceimport_cs_destroy(spliceimport_cs_t* cs);

  /**
   * Вход в критическую секцию.
   *
   * @param cs критическая секция.
   * 
   * @return true в случае успеха, false в случае ошибки.
  */
  bool spliceimport_cs_enter(spliceimport_cs_t* cs);

  /**
   * Выход из критической секции.
   *
   * @param cs критическая секция.
   * 
   * @return true в случае успеха, false в случае ошибки.
  */
  bool spliceimport_cs_leave(spliceimport_cs_t* cs);

  /**
   * Аллоцирование виртуальной памяти.
   *
   * @param size размер участка памяти.
   *
   * @return адрес участка памяти в случае успеха, NULL в случае ошибки.
  */
  void* spliceimport_memalloc(size_t size);

  /**
   * Аллоцирование виртуальной памяти по фиксированному адресу.
   *
   * @param address адрес участка памяти.
   * @param size    размер участка памяти.
   *
   * @return адрес участка памяти в случае успеха, NULL в случае ошибки.
  */
  void* spliceimport_memalloc_address(void* address, size_t size);

  /**
   * Освобождение виртуальной памяти.
   *
   * @param address адрес участка памяти.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  bool spliceimport_memfree(void* address);

  /**
   * Запрос информации о виртуальной памяти.
   *
   * @param address адрес участка памяти.
   * @param mq      информация о виртуальной памяти.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  bool spliceimport_memquery(const void* address, memquery_t* mq);

  /**
   * Чтение виртуальной памяти.
   *
   * @param address адрес участка памяти.
   * @param data    буфер.
   * @param size    размер считываемых данных в буфер.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  bool spliceimport_memread(const void* address, void* data, size_t size);

  /**
   * Запись виртуальной памяти.
   *
   * @param address адрес участка памяти.
   * @param data    данные.
   * @param size    размер данных.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  bool spliceimport_memwrite(const void* address, const void* data, size_t size);

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // spliceimport_h__