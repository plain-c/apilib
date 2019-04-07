#include "../cfg.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#if(APILIB_64 != 0)

#include "../include/lib.h"
#include "../include/log.h"
#include "../include/mem.h"

#include "spliceimport.h"
#include "splicejumper64.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace apilib 
{ 
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Опкод для отладки.
  */
  static const unsigned char SPLICEJUMPER_DEBUG_OPCODE = 0x90;

  /**
   * Количество элементов добавляемых при расширении к массиву адресов возврата.
  */
  static const size_t SPLICEJUMPER_EXTEND_RETADDR_COUNT = 256;

  /**
   * Количество элементов добавляемых при расширении к массиву игнорируемых потоков.
  */
  static const size_t SPLICEJUMPER_EXTEND_SKIPPED_COUNT = 16;

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Прототип бинарного кода переключателя для 64-битного окружения.
  */
  static const splicejumper_bincode_t SPLICEJUMPER_BINCODE_PROTOTYPE =
  {
    SPLICEJUMPER_DEBUG_OPCODE,           /// ...
    0x58,                                /// pop   rax
    0x51,                                /// push  rcx
    0x52,                                /// push  rdx
    {0x41,0x50},                         /// push  r8
    {0x41,0x51},                         /// push  r9
    {0x48,0x89,0xC2},                    /// mov   rdx, rax
    {0x48,0xB9,0x0000000000000000},      /// mov   rcx, splicejumper
    {0x48,0xB8,0x0000000000000000},      /// mov   rax, splicejumper_enter_bincode
    {0x48,0x83,0xEC,0x20},               /// sub   rsp, 0x20
    {0xFF,0xD0},                         /// call  rax
    {0x48,0x83,0xC4,0x20},               /// add   rsp, 0x20
    {0x41,0x59},                         /// pop   r9
    {0x41,0x58},                         /// pop   r8
    0x5A,                                /// pop   rdx
    0x59,                                /// pop   rcx
    {0xFF,0xD0},                         /// call  rax
    0x50,                                /// push  rax
    0x50,                                /// push  rax
    {0x48,0xB9,0x0000000000000000},      /// mov   rcx, splicejumper
    {0x48,0xB8,0x0000000000000000},      /// mov   rax, splicejumper_leave_bincode
    {0x48,0x83,0xEC,0x20},               /// sub   rsp, 0x20
    {0xFF,0xD0},                         /// call  rax
    {0x48,0x83,0xC4,0x20},               /// add   rsp, 0x20
    {0x48,0x89,0x44,0x24,0x08},          /// mov   [rsp+1*8], rax
    0x58,                                /// pop   rax
    0xC3                                 /// retn
  };
  
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Получение эксклюзивного доступа к массиву адресов возврата.
   *
   * @param jumper контекст переключателя.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool splicejumper_retaddr_enter(splicejumper_t* jumper)
  {
    bool r = false;

    if(jumper != NULL)
    {
      r = spliceimport_cs_enter(&jumper->retaddr_cs);
    }

    return r;
  }

  /**
   * Освобождение эксклюзивного доступа к массиву адресов возврата.
   *
   * @param jumper контекст переключателя.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool splicejumper_retaddr_leave(splicejumper_t* jumper)
  {
    bool r = false;

    if(jumper != NULL)
    {
      r = spliceimport_cs_leave(&jumper->retaddr_cs);
    }

    return r;
  }

  /**
   * Расширение массива адресов возврата.
   *
   * @param jumper контекст переключателя.
   * @param count  число на которое увеличивается общее количество адресов возврата.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool splicejumper_retaddr_extend(splicejumper_t* jumper, size_t count)
  {
    bool r = false;

    if(jumper != NULL && count != 0)
    {
      size_t                  retaddr_count = jumper->retaddr_count + count;
      splicejumper_retaddr_t* retaddr_array = (splicejumper_retaddr_t*)spliceimport_memalloc(retaddr_count * sizeof(splicejumper_retaddr_t));
      if(retaddr_array != NULL && retaddr_count != 0)
      {
        for(size_t i = 0; i < retaddr_count; i++)
        {
          if(i < jumper->retaddr_count)
          {
            //WARNING: Тонкое место! Быстрый перенос данных без memcpy(), следует учитывать при изменении кода.
            retaddr_array[i].thread_id      = jumper->retaddr_array[i].thread_id;
            retaddr_array[i].return_address = jumper->retaddr_array[i].return_address;
          }
          else
          {
            retaddr_array[i].thread_id      = THREAD_ID_INVALID;
            retaddr_array[i].return_address = NULL;
          }
        }
        if(jumper->retaddr_array != NULL)
        {
          spliceimport_memfree(jumper->retaddr_array);
        }
        jumper->retaddr_array = retaddr_array;
        jumper->retaddr_count = retaddr_count;
        r = true;
      }
    }

    return r;
  }

  /**
   * Добавление адреса возврата по принципу 'First In Last Out'.
   *
   * @param jumper         контекст переключателя.
   * @param thread_id      идентификатор потока.
   * @param return_address адрес возврата.
   * @param old_count      количество предыдущих адресов возврата для этого потока.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool splicejumper_retaddr_address_push(splicejumper_t* jumper, spliceimport_tid_t thread_id, void* return_address, size_t* old_count)
  {
    bool r = false;

    if(jumper != NULL)
    {
      if(splicejumper_retaddr_enter(jumper) == true)
      {
        splicejumper_retaddr_t* jr = NULL;

        if(old_count != NULL && jumper->retaddr_array != NULL && jumper->retaddr_count != 0)
        {
          size_t count = 0;

          for(size_t i = 0; i < jumper->retaddr_count; i++)
          {
            if(jumper->retaddr_array[i].thread_id == thread_id)
            {
              count += 1;
            }
          }

          *old_count = count;
        }

        if(jr == NULL && jumper->retaddr_array != NULL && jumper->retaddr_count != 0)
        {
          for(size_t i = 0; i < jumper->retaddr_count; i++)
          {
            if(jr == NULL && jumper->retaddr_array[i].thread_id == THREAD_ID_INVALID)
            {
              jr = &jumper->retaddr_array[i];
              continue;
            }
            if(jr != NULL && jumper->retaddr_array[i].thread_id == thread_id)
            {
              jr = NULL;
              continue;
            }
          }
        }

        if(jr == NULL && splicejumper_retaddr_extend(jumper, SPLICEJUMPER_EXTEND_RETADDR_COUNT) == true)
        {
          for(size_t i = 0; i < jumper->retaddr_count; i++)
          {
            if(jr == NULL && jumper->retaddr_array[i].thread_id == THREAD_ID_INVALID)
            {
              jr = &jumper->retaddr_array[i];
              continue;
            }
            if(jr != NULL && jumper->retaddr_array[i].thread_id == thread_id)
            {
              jr = NULL;
              continue;
            }
          }
        }

        if(jr != NULL)
        {
          jr->thread_id = thread_id;
          jr->return_address = return_address;
          r = true;
        }

        splicejumper_retaddr_leave(jumper);
      }
    }

    return r;
  }

  /**
   * Получение адреса возврата по принципу 'First In Last Out'.
   *
   * @param jumper         контекст переключателя.
   * @param thread_id      идентификатор потока.
   * @param return_address адрес возврата.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static void* splicejumper_retaddr_address_pop(splicejumper_t* jumper, spliceimport_tid_t thread_id)
  {
    void* r = NULL;

    if(jumper != NULL)
    {
      if(splicejumper_retaddr_enter(jumper) == true)
      {
        if(jumper->retaddr_array != NULL && jumper->retaddr_count != 0)
        {
          size_t index = jumper->retaddr_count;
          while( index-- > 0 )
          {
            if(jumper->retaddr_array[index].thread_id == thread_id)
            {
              r = jumper->retaddr_array[index].return_address;
              jumper->retaddr_array[index].thread_id = THREAD_ID_INVALID;
              jumper->retaddr_array[index].return_address = NULL;
              break;
            }
          }
        }
        splicejumper_retaddr_leave(jumper);
      }
    }

    return r;
  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Получение эксклюзивного доступа к массиву потоков перехват для которых выключен.
   *
   * @param jumper контекст переключателя.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool splicejumper_skipped_enter(splicejumper_t* jumper)
  {
    bool r = false;

    if(jumper != NULL)
    {
      r = spliceimport_cs_enter(&jumper->skipped_cs);
    }

    return r;
  }
    
  /**
   * Освобождение эксклюзивного доступа к массиву потоков перехват для которых выключен.
   *
   * @param jumper контекст переключателя.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool splicejumper_skipped_leave(splicejumper_t* jumper)
  {
    bool r = false;

    if(jumper != NULL)
    {
      r = spliceimport_cs_leave(&jumper->skipped_cs);
    }

    return r;
  }

  /**
   * Расширение массива потоков перехват для которых выключен.
   *
   * @param jumper контекст переключателя.
   * @param count  количество добавляемых элементов в массив.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool splicejumper_skipped_extend(splicejumper_t* jumper, size_t count)
  {
    bool r = false;

    if(jumper != NULL && count != 0)
    {
      size_t                  skipped_count = jumper->skipped_count + count;
      splicejumper_skipped_t* skipped_array = (splicejumper_skipped_t*)spliceimport_memalloc(skipped_count * sizeof(splicejumper_skipped_t));
      if(skipped_array != NULL && skipped_count != 0)
      {
        for(size_t i = 0; i < skipped_count; i++)
        {
          if(i < jumper->skipped_count)
          {
            //WARNING: Тонкое место! Быстрый перенос данных без memcpy(), следует учитывать при изменении кода.
            skipped_array[i].thread_id = jumper->skipped_array[i].thread_id;
          }
          else
          {
            skipped_array[i].thread_id = THREAD_ID_INVALID;
          }
        }
        if(jumper->skipped_array != NULL)
        {
          spliceimport_memfree(jumper->skipped_array);
        }
        jumper->skipped_array = skipped_array;
        jumper->skipped_count = skipped_count;
        r = true;
      }
    }

    return r;
  }

  /**
   * Добавление идентификатора потока в массив потоков перехват для которых выключен.
   *
   * @param jumper    контекст переключателя.
   * @param thread_id идентификатор добавляемого потока.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool splicejumper_skipped_add(splicejumper_t* jumper, spliceimport_tid_t thread_id)
  {
    bool r = false;

    if(jumper != NULL)
    {
      if(splicejumper_skipped_enter(jumper) == true)
      {
        splicejumper_skipped_t* js = NULL;

        if(js == NULL && jumper->skipped_array != NULL && jumper->skipped_count != 0)
        {
          for(size_t i = 0; i < jumper->skipped_count; i++)
          {
            if(jumper->skipped_array[i].thread_id == thread_id)
            {
              js = &jumper->skipped_array[i];
              break;
            }
          }
        }

        if(js == NULL && jumper->skipped_array != NULL && jumper->skipped_count != 0)
        {
          for(size_t i = 0; i < jumper->skipped_count; i++)
          {
            if(jumper->skipped_array[i].thread_id == THREAD_ID_INVALID)
            {
              js = &jumper->skipped_array[i];
              break;
            }
          }
        }

        if(js == NULL && splicejumper_skipped_extend(jumper, SPLICEJUMPER_EXTEND_SKIPPED_COUNT) == true)
        {
          for(size_t i = 0; i < jumper->skipped_count; i++)
          {
            if(jumper->skipped_array[i].thread_id == THREAD_ID_INVALID)
            {
              js = &jumper->skipped_array[i];
              break;
            }
          }
        }

        if(js != NULL)
        {
          js->thread_id = thread_id;
          r = true;
        }

        splicejumper_skipped_leave(jumper);
      }
    }

    return r;
  }

  /**
   * Проверка присутствия идентификатора потока в массиве потоков перехват для которых выключен.
   *
   * @param jumper    контекст переключателя.
   * @param thread_id идентификатор проверяемого потока.
   *
   * @return true в случае присутствия потока в массиве, false в случае отсутствия или ошибки.
  */
  static bool splicejumper_skipped_check(splicejumper_t* jumper, spliceimport_tid_t thread_id)
  {
    bool r = false;

    if(jumper != NULL)
    {
      if(splicejumper_skipped_enter(jumper) == true)
      {
        if(jumper->skipped_array != NULL && jumper->skipped_count != 0)
        {
          for(size_t i = 0; i < jumper->skipped_count; i++)
          {
            if(jumper->skipped_array[i].thread_id == thread_id)
            {
              r = true;
              break;
            }
          }
        }
        splicejumper_skipped_leave(jumper);
      }
    }

    return r;
  }

  /**
   * Удаление идентификатора потока из массива потоков перехват для которых выключен.
   *
   * @param jumper    контекст переключателя.
   * @param thread_id идентификатор удаляемого потока.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool splicejumper_skipped_del(splicejumper_t* jumper, spliceimport_tid_t thread_id)
  {
    bool r = false;

    if(jumper != NULL)
    {
      if(splicejumper_skipped_enter(jumper) == true)
      {
        if(jumper->skipped_array != NULL && jumper->skipped_count != 0)
        {
          for(size_t i = 0; i < jumper->skipped_count; i++)
          {
            if(jumper->skipped_array[i].thread_id == thread_id)
            {
              jumper->skipped_array[i].thread_id = THREAD_ID_INVALID;
              r = true;
              break;
            }
          }
        }
        splicejumper_skipped_leave(jumper);
      }
    }

    return r;
  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Обработка входа в бинарный код переключателя.
   *
   * @param jumper         контекст переключателя.
   * @param return_address адрес возврата.
   *
   * @return адрес на который передается управление в случае успеха, NULL в случае ошибки.
  */
  static void* __fastcall splicejumper_enter_bincode(splicejumper_t* jumper, void* return_address)
  {
    void* r = NULL;

    do 
    {
      spliceimport_tid_t thread_id = 0;
      size_t             old_count = 0;

      if(jumper == NULL) 
      {
        loge("Invalid argument.");
        break;
      }

      if(spliceimport_get_current_tid(&thread_id) == false)
      {
        loge("Failed to get current thread id.");
        break;
      }

      if(splicejumper_retaddr_address_push(jumper, thread_id, return_address, &old_count) == false)
      {
        loge("Failed to push return address for thread id '%u'.", thread_id);
        break;
      }

      if(jumper->active == true && old_count == 0 && splicejumper_skipped_check(jumper, thread_id) == false)
      {
        r = jumper->hooker;
      }
      else
      {
        r = jumper->bridge;
      }
    }
    while(0);

    return r;
  }

  /**
   * Обработка выхода из бинарного кода переключателя.
   *
   * @param jumper контекст переключателя.
   *
   * @return адрес возврата в случае успеха, NULL в случае ошибки.
  */
  static void* __fastcall splicejumper_leave_bincode(splicejumper_t* jumper)
  {
    void* r = NULL;

    do 
    {
      if(jumper == NULL) 
      {
        loge("Invalid argument.");
        break;
      }

      spliceimport_tid_t thread_id = 0;
      if(spliceimport_get_current_tid(&thread_id) == false)
      {
        loge("Failed to get current thread id.");
        break;
      }

      void* address = splicejumper_retaddr_address_pop(jumper, thread_id);
      if(address == NULL)
      {
        loge("Failed to find return address for thread id '%u'.", thread_id);
        break;
      }

      r = address;
    }
    while(0);

    return r;
  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

///////////////////////////////////////////////////////////////////////////////////////////////////

bool apilib::splicejumper_create(splicejumper_t* jumper, void* bridge, void* hooker)
{
  bool r = false;

  if(jumper != NULL && bridge != NULL && hooker != NULL)
  {
    jumper->active = false;
    jumper->bridge = bridge;
    jumper->hooker = hooker; 
    
    if(spliceimport_cs_create(&jumper->retaddr_cs) == true)
    {
      jumper->retaddr_array = NULL;
      jumper->retaddr_count = 0;

      if(spliceimport_cs_create(&jumper->skipped_cs) == true)
      {
        jumper->skipped_array = NULL;
        jumper->skipped_count = 0;

        if(memcpy_s(&jumper->bincode, sizeof(jumper->bincode), &SPLICEJUMPER_BINCODE_PROTOTYPE, sizeof(SPLICEJUMPER_BINCODE_PROTOTYPE)) == true)
        {
          jumper->bincode.s1.jumper               = jumper;
          jumper->bincode.s2.jumper_enter_bincode = splicejumper_enter_bincode;
          jumper->bincode.s3.jumper               = jumper;
          jumper->bincode.s4.jumper_leave_bincode = splicejumper_leave_bincode;
          r                                       = true;
        }
        else
        {
          loge("Failed to copy jumper binary code.");
          spliceimport_cs_destroy(&jumper->retaddr_cs);
          spliceimport_cs_destroy(&jumper->skipped_cs);
        }
      }
      else
      {
        loge("Failed to create critical section.");
        spliceimport_cs_destroy(&jumper->retaddr_cs);
      }
    }
    else
    {
      loge("Failed to create critical section.");
    }
  }

  return r;
}

bool apilib::splicejumper_destroy(splicejumper_t* jumper)
{
  bool r = false;

  do
  {
    if(jumper == NULL)
    {
      loge("Invalid argument.");
      break;
    }

    jumper->active = false;
    jumper->bridge = NULL;
    jumper->hooker = NULL; 
   
    spliceimport_cs_destroy(&jumper->retaddr_cs);
    if(jumper->retaddr_array != NULL)
    {
      spliceimport_memfree(jumper->retaddr_array);
    }
    jumper->retaddr_array = NULL;
    jumper->retaddr_count = 0;

    spliceimport_cs_destroy(&jumper->skipped_cs);
    if(jumper->skipped_array != NULL)
    {
      spliceimport_memfree(jumper->skipped_array);
    }
    jumper->skipped_array = NULL;
    jumper->skipped_count = 0;

    memzero(&jumper->bincode, sizeof(jumper->bincode));
    r = true;
  }
  while(0);

  return r;
}

bool apilib::splicejumper_destroy_available(splicejumper_t* jumper)
{
  bool r = false;

  do
  {
    if(jumper == NULL)
    {
      loge("Invalid argument.");
      break;
    }

    if(splicejumper_retaddr_enter(jumper) == true)
    {
      size_t count = 0;

      for(size_t i = 0; i < jumper->retaddr_count; i++)
      {
        if(jumper->retaddr_array[i].thread_id != THREAD_ID_INVALID)
        {
          count += 1;
        }
      }

      if(count == 0)
      {
        r = true;
      }

      splicejumper_retaddr_leave(jumper);
    }
  }
  while(0);

  return r;
}

bool apilib::splicejumper_get_active(splicejumper_t* jumper)
{
  bool r = false;

  do
  {
    if(jumper == NULL)
    {
      loge("Invalid argument.");
      break;
    }

    r = jumper->active;
  }
  while(0);

  return r;
}

bool apilib::splicejumper_set_active(splicejumper_t* jumper, bool active)
{
  bool r = false;

  do
  {
    if(jumper == NULL)
    {
      loge("Invalid argument.");
      break;
    }

    jumper->active = active;
    r = true;
  }
  while(0);

  return r;
}

bool apilib::splicejumper_get_active_for_current_thread(splicejumper_t* jumper)
{
  bool r = false;

  if(jumper != NULL)
  {
    spliceimport_tid_t thread_id = 0;

    if(spliceimport_get_current_tid(&thread_id) == true)
    {
      r = splicejumper_skipped_check(jumper, thread_id);
    }
  }

  return r;
}

bool apilib::splicejumper_set_active_for_current_thread(splicejumper_t* jumper, bool active)
{
  bool r = false;

  if(jumper != NULL)
  {
    spliceimport_tid_t thread_id = 0;

    if(spliceimport_get_current_tid(&thread_id) == true)
    {
      if(active == true)
      {
        r = splicejumper_skipped_add(jumper, thread_id);
      }
      else
      {
        r = splicejumper_skipped_del(jumper, thread_id);
      }
    }
  }

  return r;
}

void* apilib::splicejumper_get_brdige(splicejumper_t* jumper)
{
  void* r = NULL;

  do
  {
    if(jumper == NULL)
    {
      loge("Invalid argument.");
      break;
    }

    r = jumper->bridge;
  }
  while(0);

  return r;
}

bool apilib::splicejumper_set_bridge(splicejumper_t* jumper, void* bridge)
{
  bool r = false;

  do
  {
    if(jumper == NULL)
    {
      loge("Invalid argument.");
      break;
    }

    jumper->bridge = bridge;
    r = true;
  }
  while(0);

  return r;
}

void* apilib::splicejumper_get_hooker(splicejumper_t* jumper)
{
  void* r = NULL;

  do 
  {
    if(jumper == NULL) 
    {
      loge("Invalid argument.");
      break;
    }

    r = jumper->hooker;
  }
  while(0);

  return r;
}

bool apilib::splicejumper_set_hooker(splicejumper_t* jumper, void* hooker)
{
  bool r = false;

  do
  {
    if(jumper == NULL)
    {
      loge("Invalid argument.");
      break;
    }

    jumper->hooker = hooker;
    r = true;
  }
  while(0);

  return r;
}

void* apilib::splicejumper_get_location(splicejumper_t* jumper)
{
  void* r = NULL;

  do
  {
    if(jumper == NULL)
    {
      loge("Invalid argument.");
      break;
    }

    r = &jumper->bincode;
  }
  while(0);

  return r;
}

#endif // #if(APILIB_64 != 0)

///////////////////////////////////////////////////////////////////////////////////////////////////