#include "../cfg.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#if(APILIB_64 != 0)

#include "../include/lib.h"
#include "../include/log.h"
#include "../include/mem.h"

#include "spliceimport.h"
#include "splicerouter64.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Размер страницы памяти.
  */
  static const size_t SPLICEROUTER_MEMPAGE_SIZE = 0x1000;

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Статус элемента ноды.
  */
  enum splicerouter_item_status_t
  {
    splicerouter_item_status_free = 0, //Элемент свободен.
    splicerouter_item_status_busy = 1, //Элемент занят.
  };

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Контекст элемента ноды.
  */
  struct splicerouter_item_t
  {
    unsigned char status;      //Статус элемента splicerouter_item_status_t.
    unsigned char bincode[16]; //Бинарный код маршрута к splicejumper_t.
  };

  /**
   * Контекст ноды.
  */
  struct splicerouter_node_t
  {
    spliceimport_cs_t    cs;         //Критическая секция.
    splicerouter_node_t* next;       //Следующая нода.
    splicerouter_item_t  items[256]; //Массив элементов.
  };

  /**
   * Контекст списка нод.
  */
  struct splicerouter_list_t
  {
    spliceimport_cs_t    cs;   //Критическая секция.
    splicerouter_node_t* node; //Первая нода.
  };

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Глобальный контекст подсистемы splicerouter.
  */
  struct 
  {
    spliceimport_pid_t  pid;
    splicerouter_list_t list;
  } GlobalSpliceRouter;

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Установка стандартного состояния.
   *
   * @param item элемент.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool splicerouter_item_set_default(splicerouter_item_t* item)
  {
    bool r = false;

    if(item != NULL)
    {
      memset(&item->bincode, 0xCC, sizeof(item->bincode));
      item->status = splicerouter_item_status_free;
      r = true;
    }

    return r;
  }

  /**
   * Установка прыжка на 64-битный адрес через инструкцию JMP absoulute.
   *
   * @param item элемент.
   * @param to   адрес на который происходит прыжок.
   *
   * @return адрес инструкций в случае успеха, NULL в случае ошибки.
  */
  static void* splicerouter_item_set_jumpabs(splicerouter_item_t* item, const void* to)
  {
    void* r = NULL;

    if(item != NULL) 
    {
      uint64_t address = (uint64_t)to;

      *(uint08_t*)&item->bincode[0] = 0xFF;        /** jmp [%offset%] **/
      *(uint08_t*)&item->bincode[1] = 0x25;        /** .............. **/
      *(uint32_t*)&item->bincode[2] = 0x00000000;  /** ... [%offset%] **/
      *(uint64_t*)&item->bincode[6] = address;     /** %address data% **/

      item->status = splicerouter_item_status_busy;
      r = item->bincode;
    }

    return r;
  }

  /**
   * Установка прыжка на 64-битный адрес через инструкции PUSH/RET.
   *
   * @param item элемент.
   * @param to   адрес на который происходит прыжок.
   *
   * @return адрес инструкций в случае успеха, NULL в случае ошибки.
  */
  static void* splicerouter_item_set_pushret(splicerouter_item_t* item, const void* to)
  {
    void* r = NULL;

    if(item != NULL) 
    {
      uint32_t lo_address = (uint32_t)((((uint64_t)(to))      ) & 0x00000000FFFFFFFF);
      uint32_t hi_address = (uint32_t)((((uint64_t)(to)) >> 32) & 0x00000000FFFFFFFF);

      *(uint08_t*)&item->bincode[ 0] = 0x68;        /** push %lo address%       **/
      *(uint32_t*)&item->bincode[ 1] = lo_address;  /** .... %lo address%       **/
      *(uint08_t*)&item->bincode[ 5] = 0xC7;        /** mov [rsp], %hi address% **/
      *(uint08_t*)&item->bincode[ 6] = 0x44;        /** ....................... **/
      *(uint08_t*)&item->bincode[ 7] = 0x24;        /** ....................... **/
      *(uint08_t*)&item->bincode[ 8] = 0x04;        /** ....................... **/
      *(uint32_t*)&item->bincode[ 9] = hi_address;  /** .........  %hi address% **/
      *(uint08_t*)&item->bincode[13] = 0xC3;        /** retn                    **/

      item->status = splicerouter_item_status_busy;
      r = item->bincode;
    }

    return r;
  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Аллоцирование ноды по указанному адресу в пределах приемлемого диапазона.
   *
   * Используется в цикле автоматического перечисления неиспользуемых страниц памяти.
   * Таким образом передаваемый адрес неиспользуемой страницы памяти требует проверки.
   *
   * Принцип работы (каждый шаг допускает выход с отрицательным результатом):
   * a) Проверка находится ли адрес в пределах приемлемого диапазона.
   * b) Аллоцирование памяти для ноды по указанному адресу.
   * c) Проверка находится ли аллоцированная память в пределах приемлемого диапазона.
   *
   * @oaram accept_a крайний адрес диапазона приемлемых адресов слева.
   * @param accept_z крайний адрес диапазона приемлемых адресов справа.
   * @param address  адрес из диапазона неиспользуемых страниц памяти.
   *
   * @return аллоцированная нода в случае успеха, NULL в случае ошибки.
  */
  static splicerouter_node_t* splicerouter_node_alloc_address(void* accept_a, void* accept_z, void* address)
  {
    splicerouter_node_t* r = NULL;

    if(accept_a <= address && address <= accept_z)
    {
      splicerouter_node_t* allocated = (splicerouter_node_t*)spliceimport_memalloc_address(address, sizeof(splicerouter_node_t));
      if(allocated != NULL)
      {
        if(accept_a <= allocated && allocated <= accept_z)
        {
          memzero(allocated, sizeof(splicerouter_node_t));
          r = allocated;
        }
        else
        {
          spliceimport_memfree(allocated);
        }
      }
    }

    return r;
  }
  
  /**
   * Аллоцирование ноды из диапазона неиспользуемых страниц памяти.
   *
   * @oaram accept_a крайний адрес диапазона приемлемых адресов слева.
   * @param accept_z крайний адрес диапазона приемлемых адресов справа.
   * @param unused_a крайний адрес диапазона неиспользуемых страниц памяти слева.
   * @param unused_z крайний адрес диапазона неиспользуемых страниц памяти справа.
   * 
   * @return аллоцированная нода в случае успеха, NULL в случае ошибки.
  */
  static splicerouter_node_t* splicerouter_node_alloc_unused_range(void* accept_a, void* accept_z, void* unused_a, void* unused_z)
  {
    splicerouter_node_t* r = NULL;

    if(accept_a <= accept_z && unused_a <= unused_z)
    {
      void* ptr = unused_a;

      while(unused_a <= ptr && ptr <= unused_z)
      {
        splicerouter_node_t* node = splicerouter_node_alloc_address(accept_a, accept_z, ptr);
        if(node != NULL)
        {
          r = node;
          break;
        }
        ptr = (unsigned char*)ptr + SPLICEROUTER_MEMPAGE_SIZE;
      }
    }

    return r;
  }

  /**
   * Аллоцирование ноды из диапазона приемлемых адресов.
   *
   * @oaram accept_a крайний адрес диапазона приемлемых адресов слева.
   * @param accept_z крайний адрес диапазона приемлемых адресов справа.
   * 
   * @return аллоцированная нода в случае успеха, NULL в случае ошибки.
  */
  static splicerouter_node_t* splicerouter_node_alloc_accept_range(void* accept_a, void* accept_z)
  {
    splicerouter_node_t* r = NULL;

    if(accept_a <= accept_z)
    {
      void* ptr = accept_a;

      while(accept_a <= ptr && ptr <= accept_z)
      {
        memquery_t mq;
        memzero(&mq, sizeof(mq));

        if(spliceimport_memquery(ptr, &mq) == false)
        {
          break;
        }

        if((mq.free == true || mq.rsrv == true) && (mq.size >= sizeof(splicerouter_node_t)))
        {
          void* unused_a = (unsigned char*)mq.addr;
          void* unused_z = (unsigned char*)mq.addr + mq.size - sizeof(splicerouter_node_t);

          splicerouter_node_t* node = splicerouter_node_alloc_unused_range(accept_a, accept_z, unused_a, unused_z);
          if(node != NULL)
          {
            r = node;
            break;
          }
        }

        ptr = (unsigned char*)mq.addr + mq.size;
      }
    }

    return r;
  }

  /**
   * Аллоцирование ноды в пределах доступности для указанного адреса.
   *
   * @param address адрес.
   *
   * @return аллоцированная нода в случае успеха, NULL в случае ошибки. 
  */
  static splicerouter_node_t* splicerouter_node_alloc(const void* address)
  {
    splicerouter_node_t* r = NULL;

    if(address != NULL)
    {
      void* accept_a = (unsigned char*)address - 0x000000007FFFFFF0;
      void* accept_z = (unsigned char*)address + 0x000000007FFFFFF0 - sizeof(splicerouter_node_t);

      if(accept_a > address)accept_a = (void*)0x0000000000000000;
      if(accept_z < address)accept_z = (void*)0xFFFFFFFFFFFFFFFF;

      if(accept_a <= address && address <= accept_z)
      {
        r = splicerouter_node_alloc_accept_range(accept_a, accept_z);
      }
    }

    return r;
  }

  /**
   * Освобождение ноды из виртуальной памяти.
   *
   * @param node нода.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool splicerouter_node_free(splicerouter_node_t* node)
  {
    bool r = false;

    if(node != NULL)
    {
      r = spliceimport_memfree(node);
    }

    return r;
  }

  /**
   * Создание контекста ноды.
   *
   * @param node нода.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool splicerouter_node_create(splicerouter_node_t* node)
  {
    bool r = false;

    if(node != NULL)
    {
      if(spliceimport_cs_create(&node->cs) == true)
      {
        size_t items_total_count = sizeof(node->items) / sizeof(node->items[0]);
        size_t items_ready_count = 0;

        node->next = NULL;

        for(size_t i = 0; i < items_total_count; i++)
        {
          if(splicerouter_item_set_default(&node->items[i]) == true)
          {
            items_ready_count += 1;
          }
        }

        if(items_total_count == items_ready_count)
        {
          r = true;
        }
        else
        {
          spliceimport_cs_destroy(&node->cs);
        }
      }
    }

    return r;
  }

  /**
   * Разрушение контекста ноды.
   *
   * @param node нода.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool splicerouter_node_destroy(splicerouter_node_t* node)
  {
    bool r = false;

    if(node != NULL)
    {
      if(spliceimport_cs_destroy(&node->cs) == true)
      {
        size_t items_total_count = sizeof(node->items) / sizeof(node->items[0]);
        size_t items_ready_count = 0;

        node->next = NULL;

        for(size_t i = 0; i < items_total_count; i++)
        {
          if(splicerouter_item_set_default(&node->items[i]) == true)
          {
            items_ready_count += 1;
          }
        }

        if(items_total_count == items_ready_count)
        {
          r = true;
        }
      }
    }

    return r;
  }

  /**
   * Вход в критическую секцию ноды.
   *
   * @param node нода.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool splicerouter_node_enter(splicerouter_node_t* node)
  {
    bool r = false;

    if(node != NULL)
    {
      r = spliceimport_cs_enter(&node->cs);
    }

    return r;
  }

  /**
   * Выход из критической секции ноды.
   *
   * @param node нода.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool splicerouter_node_leave(splicerouter_node_t* node)
  {
    bool r = false;

    if(node != NULL)
    {
      r = spliceimport_cs_leave(&node->cs);
    }

    return r;
  }

  /**
   * Количество занятых элементов.
   *
   * @param node нода.
   *
   * @return количество занятых элементов в случае успеха, 0 в случае ошибки.
  */
  static size_t splicerouter_node_get_busy_items_count(splicerouter_node_t* node)
  {
    size_t r = 0;

    if(node != NULL)
    {
      size_t total = sizeof(node->items) / sizeof(node->items[0]);
      size_t count = 0;

      for(size_t i = 0; i < total; i++)
      {
        if(node->items[i].status == splicerouter_item_status_busy)
        {
          count += 1;
        }
      }

      r = count;
    }

    return r;
  }

  /**
   * Количество свободных элементов.
   *
   * @param node нода.
   *
   * @return количество свободных элементов в случае успеха, 0 в случае ошибки.
  */
  static size_t splicerouter_node_get_free_items_count(splicerouter_node_t* node)
  {
    size_t r = 0;

    if(node != NULL)
    {
      size_t total = sizeof(node->items) / sizeof(node->items[0]);
      size_t count = 0;

      for(size_t i = 0; i < total; i++)
      {
        if(node->items[i].status == splicerouter_item_status_free)
        {
          count += 1;
        }
      }

      r = count;
    }

    return r;
  }

  /**
   * Поиск свободного элемента ноды.
   *
   * @param node контекст ноды.
   * @param from адрес для которого требуется элемент ноды.
   *
   * @return контекст элемента в случае успеха, NULL в случае ошибки.
  */
  static splicerouter_item_t* splicerouter_node_find_item_free(splicerouter_node_t* node, const void* from)
  {
    splicerouter_item_t* r = NULL;

    if(node != NULL)
    {
      void* accept_a = (unsigned char*)from - 0x000000007FFFFFF0;
      void* accept_z = (unsigned char*)from + 0x000000007FFFFFF0 - sizeof(splicerouter_item_t);

      if(accept_a > from)accept_a = (void*)0x0000000000000000;
      if(accept_z < from)accept_z = (void*)0xFFFFFFFFFFFFFFFF;

      size_t total = sizeof(node->items) / sizeof(node->items[0]);

      for(size_t i = 0; i < total; i++)
      {
        splicerouter_item_t* item = &node->items[i];
        if(item->status != splicerouter_item_status_free)
        {
          continue;
        }
        if(accept_a <= item && item <= accept_z)
        {
          r = item;
          break;
        }
      }
    }

    return r;
  }

  /**
   * Поиск элемента ноды по адресу любой части этого элемента.
   *
   * @param node    контекст ноды.
   * @param address адрес любой части элемента.
   *
   * @return контекст элемента в случае успеха, NULL в случае ошибки.
  */
  static splicerouter_item_t* splicerouter_node_find_item_by_address(splicerouter_node_t* node, const void* address)
  {
    splicerouter_item_t* r = NULL;

    if(node != NULL)
    {
      void* node_a = (unsigned char*)node;
      void* node_z = (unsigned char*)node + sizeof(splicerouter_node_t);

      if(node_a <= address && address <= node_z)
      {
        size_t total = sizeof(node->items) / sizeof(node->items[0]);

        for(size_t i = 0; i < total; i++)
        {
          splicerouter_item_t* item = &node->items[i];

          void* item_a = (unsigned char*)item;
          void* item_z = (unsigned char*)item + sizeof(splicerouter_item_t);

          if(item_a <= address && address <= item_z)
          {
            r = item;
            break;
          }
        }
      }
    }

    return r;
  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Создание списка нод.
   *
   * @param list список нод.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool splicerouter_list_create(splicerouter_list_t* list)
  {
    bool r = false;

    if(list != NULL)
    {
      if(spliceimport_cs_create(&list->cs) == true)
      {
        list->node = NULL;
        r = true;
      }
    }

    return r;
  }

  /**
   * Разрушение списка нод.
   *
   * @param list список нод.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool splicerouter_list_destroy(splicerouter_list_t* list)
  {
    bool r = false;

    if(list != NULL)
    {
      if(spliceimport_cs_destroy(&list->cs) == true)
      {
        list->node = NULL;
        r = true;
      }
    }

    return r;
  }

  /**
   * Вход в критическую секцию списка нод.
   *
   * @param list список нод.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool splicerouter_list_enter(splicerouter_list_t* list)
  {
    bool r = false;

    if(list != NULL)
    {
      r = spliceimport_cs_enter(&list->cs);
    }

    return r;
  }

  /**
   * Выход из критической секции списка нод.
   *
   * @param list список нод.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool splicerouter_list_leave(splicerouter_list_t* list)
  {
    bool r = false;

    if(list != NULL)
    {
      r = spliceimport_cs_leave(&list->cs);
    }

    return r;
  }

  /**
   * Присоединение ноды к списку нод.
   *
   * @param list список нод.
   * @param node контекст ноды.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool splicerouter_list_attach_node(splicerouter_list_t* list, splicerouter_node_t* node)
  {
    bool r = false;

    if(list != NULL && node != NULL)
    {
      if(list->node == NULL)
      {
        list->node = node;
        node->next = NULL;
        r = true;
      }
      else
      {
        splicerouter_node_t* next = list->node;
        splicerouter_node_t* prev = NULL;

        while(next != NULL)
        {
          if(next == node)
          {
            break;
          }
          prev = next;
          next = next->next;
        }

        if(next == NULL && prev != NULL)
        {
          prev->next = node;
          node->next = NULL;
          r = true;
        }
      }
    }

    return r;
  }

  /**
   * Отсоединение ноды от списка нод.
   *
   * @param list список нод.
   * @param node контекст ноды.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool splicerouter_list_detach_node(splicerouter_list_t* list, splicerouter_node_t* node)
  {
    bool r = false;

    if(list != NULL && node != NULL)
    {
      if(list->node == node)
      {
        list->node = node->next;
        node->next = NULL;
        r = true;
      }
      else
      {
        splicerouter_node_t* next = list->node;
        splicerouter_node_t* prev = NULL;

        while(next != NULL)
        {
          if(next == node)
          {
            break;
          }
          prev = next;
          next = next->next;
        }

        if(next != NULL && prev != NULL)
        {
          prev->next = next->next;
          node->next = NULL;
          r = true;
        }
      }
    }

    return r;
  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Комплексное создание нового элемента с добавлением ноды в общий список.
   *
   * @param list список нод.
   * @param from адрес из которого происходит входящий прыжок.
   * @param to   адрес на который происходит исходящий прыжок.
   *
   * @return адрес плацдарма в случае успеха, NULL в случае ошибки.
  */
  static void* splicerouter_item_create_new(splicerouter_list_t* list, const void* from, const void* to)
  {
    void* r = NULL;

    if(list != NULL && from != NULL && to != NULL)
    {
      splicerouter_node_t* node = splicerouter_node_alloc(from);
      if(node != NULL)
      {
        if(splicerouter_node_create(node) == true)
        {
          bool attached = false;

          if(splicerouter_list_enter(list) == true)
          {
            attached = splicerouter_list_attach_node(list, node);
            splicerouter_list_leave(list);
          }

          if(attached == true)
          {
            splicerouter_item_t* item = splicerouter_node_find_item_free(node, from);
            if(item != NULL)
            {
              r = splicerouter_item_set_jumpabs(item, to);
              if(r != NULL)
              {
                //OK!
              }
              else
              {
                splicerouter_item_set_default(item);
              }
            }
          }
          else
          {
            splicerouter_node_destroy(node);
            splicerouter_node_free(node);
          }
        }
        else
        {
          splicerouter_node_free(node);
        }
      }
    }

    return r;
  }

  /**
   * Комплексный поиск свободного элемента по всем нодам общего списка.
   *
   * @param list список нод.
   * @param from адрес из которого происходит входящий прыжок.
   * @param to   адрес на который происходит исходящий прыжок.
   *
   * @return адрес плацдарма в случае успеха, NULL в случае ошибки.
  */
  static void* splicerouter_item_find_exist(splicerouter_list_t* list, const void* from, const void* to)
  {
    void* r = NULL;

    if(list != NULL && from != NULL && to != NULL)
    {
      if(splicerouter_list_enter(list) == true)
      {
        splicerouter_node_t* node = list->node;

        while(node != NULL)
        {
          if(splicerouter_node_enter(node) == true)
          {
            splicerouter_item_t* item = splicerouter_node_find_item_free(node, from);
            if(item != NULL)
            {
              r = splicerouter_item_set_jumpabs(item, to);
              if(r != NULL)
              {
                //OK!
              }
              else
              {
                splicerouter_item_set_default(item);  
              }
            }
            splicerouter_node_leave(node);
          }

          if(r != NULL)
          {
            break;
          }

          node = node->next;
        }

        splicerouter_list_leave(list);
      }
    }

    return r;
  }

  /**
   * Комплексное разрушение элемента ноды.
   *
   * @param list    список нод.
   * @param address адрес любой части элемента ноды.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool splicerouter_item_destroy(splicerouter_list_t* list, const void* address)
  {
    bool r = false;

    if(list != NULL && address != NULL)
    {
      if(splicerouter_list_enter(list) == true)
      {
        splicerouter_node_t* node = list->node;

        while(node != NULL)
        {
          splicerouter_item_t* item = NULL;

          if(splicerouter_node_enter(node) == true)
          {
            if((item = splicerouter_node_find_item_by_address(node, address)) != NULL)
            {
              r = splicerouter_item_set_default(item);
            }
            splicerouter_node_leave(node);
          }

          if(item != NULL)
          {
            break;
          }

          node = node->next;
        }

        splicerouter_list_leave(list);
      }
    }

    return r;
  }  

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Инициализация контекста splicerouter.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool __splicerouter_init()
  {
    bool r = false;

    spliceimport_pid_t pid;
    if(spliceimport_get_current_pid(&pid) == true)
    {
      if(GlobalSpliceRouter.pid == pid)
      {
        r = true;
      }
      else
      {
        if(splicerouter_list_create(&GlobalSpliceRouter.list) == true)
        {
          GlobalSpliceRouter.pid = pid;
          r = true;
        }
      }
    }

    return r;
  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

///////////////////////////////////////////////////////////////////////////////////////////////////

void* apilib::splicerouter_get_personal(const void* from, const void* to)
{
  void* r = NULL;

  if(from != NULL && to != NULL)
  {
    void* address = NULL;

    if(__splicerouter_init() == true)
    {
      if((address = splicerouter_item_find_exist(&GlobalSpliceRouter.list, from, to)) != NULL ||
         (address = splicerouter_item_create_new(&GlobalSpliceRouter.list, from, to)) != NULL)
      {
        r = address;
      }
    }
  }

  return r;
}

bool apilib::splicerouter_free_personal(const void* address)
{
  bool r = false;

  if(address != NULL)
  {
    if(__splicerouter_init() == true)
    {
      r = splicerouter_item_destroy(&GlobalSpliceRouter.list, address);
    }
  }

  return r;
}

#endif // #if(APILIB_64 != 0)

///////////////////////////////////////////////////////////////////////////////////////////////////

//TODO: CLEAR TRASH!
#if defined _X128_

static void* search_page(const void* address)
{
  /**
   * Метод №2 - Поиск подходящего участка слева от адреса прыжка.
  */
  if(address == NULL)
  {
    ::MEMORY_BASIC_INFORMATION mbi;
    memzero(&mbi, sizeof(mbi));

    void* lpSpaceAddr = (void*)address;
    while(::VirtualQuery(lpSpaceAddr, &mbi, sizeof(mbi)) != 0)
    {
      if((mbi.RegionSize > Hook::SPLICE64_JUMP_SIZE) && 
        ((mbi.State   & (MEM_COMMIT)) != 0) && 
        ((mbi.Protect & (PAGE_GUARD)) == 0) &&
        ((mbi.Protect & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)) != 0))
      {
          LOG_STR(WDDT_INFO, "Method #2 - Found relay in space on the left of jump address (jump address is '%#p', relay address is '%#p').", lpJumpAddr, lpRelayAddr);
      }
      lpSpaceAddr = (LPVOID)((BYTE*)mbi.BaseAddress + mbi.RegionSize);
    }
  }

  /**
   * Метод №3 - Поиск подходящего участка справа от адреса прыжка.
  */
  if(lpRelayAddr == NULL)
  {
    ::MEMORY_BASIC_INFORMATION mbi;
    Mem::_zero(&mbi, sizeof(mbi));

    LPVOID lpSpaceAddr = lpJumpAddr;
    while((lpRelayAddr == NULL) && (DELTA(lpSpaceAddr, lpJumpAddr) <= 0x7FFFFF00) && (::VirtualQuery(lpSpaceAddr, &mbi, sizeof(mbi)) != 0))
    {
      if((mbi.RegionSize > Hook::SPLICE64_JUMP_SIZE) && 
        ((mbi.State   & (MEM_COMMIT)) != 0) && 
        ((mbi.Protect & (PAGE_GUARD)) == 0) &&
        ((mbi.Protect & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)) != 0))
      {
        lpRelayAddr = Hook::Splice64CheckRelayPlace_Int(mbi.BaseAddress, mbi.RegionSize, Hook::SPLICE64_JUMP_SIZE);
        if(lpRelayAddr != NULL)
        {
#             if(1)
          LOG_STR(WDDT_INFO, "Method #3 - Found relay in space on the right of jump address (jump address is '%#p', relay address is '%#p').", lpJumpAddr, lpRelayAddr);
#             endif
        }
      }
      lpSpaceAddr = (LPVOID)((BYTE*)mbi.BaseAddress + mbi.RegionSize);
    }
  }
}


HookInt_PublicRelay proc
  pop   rax
  push  rcx
  push  rdx
  push  r8
  push  r9
  lea   rcx, [rax-5]
  sub   rsp, 20h
  call  HookInt_PublicRelayFactory
  add   rsp, 20h
  pop   r9
  pop   r8
  pop   rdx
  pop   rcx
  test  rax, rax
  jz    __exit
  jmp   rax
__exit:
ret
  HookInt_PublicRelay endp

/*
  Экспорт процедур.
*/
extern "C"
{
  LPVOID HookInt_PublicRelay();
  LPVOID HookInt_PublicRelayFactory(LPVOID lpJumpProc);
};

/*
  Фабрика перехватов.
  
  Определяет дальнейшую логику перехваченной функции.
*/
LPVOID HookInt_PublicRelayFactory(LPVOID lpJumpProc)
{
  LPVOID lpRetVal = NULL;

  if(lpJumpProc != NULL)
  {
    for(UINT8 i = 0; i < 0xFF; i++)
    {
      ::EnterCriticalSection(&Global.csSpliceHooks);
      {
        SPLICE_HOOK* lpSplice = Global.lpSpliceHooks;
        while((lpSplice != NULL) && (lpRetVal == NULL))
        {
          if(lpSplice->lpJumpAddr == (BYTE*)lpJumpProc ||
             lpSplice->lpHotpatch == (BYTE*)lpJumpProc)
          {
            lpRetVal = lpSplice->lpSwitch;
          }
          lpSplice = lpSplice->lpNext;
        }
      }
      ::LeaveCriticalSection(&Global.csSpliceHooks);

      if(lpRetVal != NULL)
      {
        break;
      }

      ::Sleep(10);
    }
  }

  return lpRetVal;
}


LPVOID Hook::Splice64CheckRelayPlace_Int(const void* lpData, SIZE_T dwDataSize, SIZE_T dwRequiredSize)
{
  LPVOID lpRetVal = NULL;

  if(lpData != NULL && dwDataSize != 0 && dwRequiredSize != 0 && dwDataSize > dwRequiredSize)
  {
    LPBYTE lpBuffer = (LPBYTE)Mem::alloc(dwDataSize);
    if(lpBuffer != NULL)
    {
      if(::ReadProcessMemory(CURRENT_PROCESS, lpData, lpBuffer, dwDataSize, NULL) != FALSE)
      {
        for(SIZE_T i = 0; (lpRetVal == NULL) && (i < dwDataSize); i++)
        {
          for(SIZE_T j = 0; (lpRetVal == NULL) && ((i+j) < dwDataSize) && (lpBuffer[i+j] == 0x90); j++)if(j >= dwRequiredSize)lpRetVal = (LPVOID)((BYTE*)lpData + i);
          for(SIZE_T j = 0; (lpRetVal == NULL) && ((i+j) < dwDataSize) && (lpBuffer[i+j] == 0xCC); j++)if(j >= dwRequiredSize)lpRetVal = (LPVOID)((BYTE*)lpData + i);
          for(SIZE_T j = 0; (lpRetVal == NULL) && ((i+j) < dwDataSize) && (lpBuffer[i+j] == 0x00); j++)if(j >= dwRequiredSize)lpRetVal = (LPVOID)((BYTE*)lpData + i);
        }
      }
      Mem::free(lpBuffer);
    }
  }

  return lpRetVal;
}

LPVOID Hook::Splice64FindPublicRelay_Int(HMODULE hModule, LPVOID lpJumpAddr)
{
  LPVOID lpRetVal = NULL;

# define DELTA(a,b)((((UINT_PTR)(a)) > ((UINT_PTR)(b))) ? (((UINT_PTR)(a)) - ((UINT_PTR)(b))) : (((UINT_PTR)(b)) - ((UINT_PTR)(a))))
# define MINUS(a,b)((((UINT_PTR)(a)) > ((UINT_PTR)(b))) ? (((UINT_PTR)(a)) - ((UINT_PTR)(b))) : 0)
  
  if(lpJumpAddr != NULL) 
  {
    LPVOID lpRelayAddr = NULL;

    /**
     * Поиск адреса в списке существующих релеев.
    */
    ::EnterCriticalSection(&Global.csPublicRelays);
    {
      for(SIZE_T i = 0; i < Global.dwPublicRelaysCount; i++)
      {
        if(hModule == NULL || lpRelayAddr != NULL)
        {
          break;
        }
        if(hModule != Global.lpPublicRelays[i].hModule)
        {
          continue;
        }
        if(DELTA(Global.lpPublicRelays[i].lpRelayAddr, lpJumpAddr) <= 0x7FFFFF00)
        {
          lpRelayAddr = Global.lpPublicRelays[i].lpRelayAddr;
          LOG_STR(WDDT_INFO, "Found existing public relay (module is '%#p', jump address is '%#p', relay address is '%#p').", hModule, lpJumpAddr, lpRelayAddr);
          break;
        }
      }

      for(SIZE_T i = 0; i < Global.dwPublicRelaysCount; i++)
      {
        if(lpRelayAddr != NULL)
        {
          break;
        }
        if(DELTA(Global.lpPublicRelays[i].lpRelayAddr, lpJumpAddr) <= 0x7FFFFF00)
        {
          lpRelayAddr = Global.lpPublicRelays[i].lpRelayAddr;
          LOG_STR(WDDT_INFO, "Found existing public relay (module is '%#p', jump address is '%#p', relay address is '%#p').", hModule, lpJumpAddr, lpRelayAddr);
          break;
        }
      }
    }
    ::LeaveCriticalSection(&Global.csPublicRelays);

    if(lpRelayAddr != NULL)
    {
      lpRetVal = lpRelayAddr;
    }
    else
    {
      /**
       * Метод №1 - Поиск подходящего участка в пределах исполняемого модуля.
      */
      if(lpRelayAddr == NULL && hModule != NULL)
      {
        LPVOID lpModuleData = (LPVOID)hModule;
        SIZE_T dwModuleSize = Hook::GetModuleImageSize_Int(hModule);
        if(lpModuleData != NULL && dwModuleSize != 0)
        {
          ::MEMORY_BASIC_INFORMATION mbi;
          Mem::_zero(&mbi, sizeof(mbi));

          LPVOID lpAddressOffset = lpModuleData;
          LPVOID lpAddressFinish = (BYTE*)lpModuleData + dwModuleSize;

          while((lpRelayAddr == NULL) && (lpAddressOffset < lpAddressFinish) && (::VirtualQuery(lpAddressOffset, &mbi, sizeof(mbi)) != 0))
          {
            if((mbi.RegionSize > Hook::SPLICE64_JUMP_SIZE) && 
              ((mbi.State   & (MEM_COMMIT)) != 0) && 
              ((mbi.Protect & (PAGE_GUARD)) == 0) &&
              ((mbi.Protect & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)) != 0))
            {
              lpRelayAddr = Hook::Splice64CheckRelayPlace_Int(mbi.BaseAddress, mbi.RegionSize, Hook::SPLICE64_JUMP_SIZE);
              if(lpRelayAddr != NULL)
              {
#               if(1)
                LOG_STR(WDDT_INFO, "Method #1 - Found relay address in module virtual space (relay address is '%#p').", lpRelayAddr);
#               endif
              }
            }
            lpAddressOffset = (LPVOID)((BYTE*)mbi.BaseAddress + mbi.RegionSize);
          }
        }
      }

      /**
       * Метод №2 - Поиск подходящего участка слева от адреса прыжка.
      */
      if(lpRelayAddr == NULL)
      {
        ::MEMORY_BASIC_INFORMATION mbi;
        Mem::_zero(&mbi, sizeof(mbi));

        LPVOID lpSpaceAddr = (LPVOID)MINUS(lpJumpAddr, 0x7FFFFF00);
        while((lpRelayAddr == NULL) && (DELTA(lpSpaceAddr, lpJumpAddr) <= 0x7FFFFF00) && (::VirtualQuery(lpSpaceAddr, &mbi, sizeof(mbi)) != 0))
        {
          if((mbi.RegionSize > Hook::SPLICE64_JUMP_SIZE) && 
            ((mbi.State   & (MEM_COMMIT)) != 0) && 
            ((mbi.Protect & (PAGE_GUARD)) == 0) &&
            ((mbi.Protect & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)) != 0))
          {
            lpRelayAddr = Hook::Splice64CheckRelayPlace_Int(mbi.BaseAddress, mbi.RegionSize, Hook::SPLICE64_JUMP_SIZE);
            if(lpRelayAddr != NULL)
            {
#             if(1)
              LOG_STR(WDDT_INFO, "Method #2 - Found relay in space on the left of jump address (jump address is '%#p', relay address is '%#p').", lpJumpAddr, lpRelayAddr);
#             endif
            }
          }
          lpSpaceAddr = (LPVOID)((BYTE*)mbi.BaseAddress + mbi.RegionSize);
        }
      }

      /**
       * Метод №3 - Поиск подходящего участка справа от адреса прыжка.
      */
      if(lpRelayAddr == NULL)
      {
        ::MEMORY_BASIC_INFORMATION mbi;
        Mem::_zero(&mbi, sizeof(mbi));

        LPVOID lpSpaceAddr = lpJumpAddr;
        while((lpRelayAddr == NULL) && (DELTA(lpSpaceAddr, lpJumpAddr) <= 0x7FFFFF00) && (::VirtualQuery(lpSpaceAddr, &mbi, sizeof(mbi)) != 0))
        {
          if((mbi.RegionSize > Hook::SPLICE64_JUMP_SIZE) && 
            ((mbi.State   & (MEM_COMMIT)) != 0) && 
            ((mbi.Protect & (PAGE_GUARD)) == 0) &&
            ((mbi.Protect & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)) != 0))
          {
            lpRelayAddr = Hook::Splice64CheckRelayPlace_Int(mbi.BaseAddress, mbi.RegionSize, Hook::SPLICE64_JUMP_SIZE);
            if(lpRelayAddr != NULL)
            {
#             if(1)
              LOG_STR(WDDT_INFO, "Method #3 - Found relay in space on the right of jump address (jump address is '%#p', relay address is '%#p').", lpJumpAddr, lpRelayAddr);
#             endif
            }
          }
          lpSpaceAddr = (LPVOID)((BYTE*)mbi.BaseAddress + mbi.RegionSize);
        }
      }

      /**
       * Метод №4 - Поиск подходящего участка в заголовке модуля.
      */
      if(lpRelayAddr == NULL && hModule != NULL)
      {
        WORD wImageDosSignature = 0;
        
        if(::ReadProcessMemory(CURRENT_PROCESS, hModule, &wImageDosSignature, sizeof(wImageDosSignature), NULL) != FALSE && wImageDosSignature == IMAGE_DOS_SIGNATURE)
        {
          lpRelayAddr = (LPVOID)((BYTE*)hModule + 2);
          if(lpRelayAddr != NULL)
          {
#           if(1)
            LOG_STR(WDDT_INFO, "Method #4 - Found relay in module header space (relay address is '%#p').", lpRelayAddr);
#           endif
          }
        }
      }

      /**
       * Запись инструкции и сохранение релея в глобальном списке.
      */
      if(lpRelayAddr != NULL)
      {
        LPVOID lpBackupData = Mem::alloc(Hook::SPLICE64_JUMP_SIZE);
        SIZE_T dwBackupSize = Hook::SPLICE64_JUMP_SIZE;
        if(lpBackupData != NULL && ::ReadProcessMemory(CURRENT_PROCESS, lpRelayAddr, lpBackupData, dwBackupSize, NULL) == FALSE)
        {
          Mem::free(lpBackupData);
          lpBackupData = NULL;
          dwBackupSize = 0;
        }

        OP_JMP_ADDRESS jmp;
        jmp.opcode1 = 0xB848;
        jmp.operand = (UINT64)HookInt_PublicRelay;
        jmp.opcode2 = 0xE0FF;

        SIZE_T dwNumberOfBytesWritten = 0;
        DWORD  dwOldProtect = 0;
        if(::WriteProcessMemory(CURRENT_PROCESS, lpRelayAddr, &jmp, sizeof(jmp), &dwNumberOfBytesWritten) != FALSE && 
           ::VirtualProtect(lpRelayAddr, Hook::SPLICE64_JUMP_SIZE, PAGE_EXECUTE_READWRITE, &dwOldProtect) != FALSE)
        {
          ::EnterCriticalSection(&Global.csPublicRelays);
          {
            if(Mem::reallocEx(&Global.lpPublicRelays, (Global.dwPublicRelaysCount + 1) * sizeof(Hook::PUBLIC_RELAY)) == true)
            {
              PUBLIC_RELAY* lpRelay = &Global.lpPublicRelays[Global.dwPublicRelaysCount++];
              lpRelay->hModule      = hModule;
              lpRelay->lpRelayAddr  = lpRelayAddr;
              lpRelay->lpBackupData = lpBackupData;
              lpRelay->dwBackupSize = dwBackupSize;
              lpRetVal              = lpRelayAddr;
            }
          }
          ::LeaveCriticalSection(&Global.csPublicRelays);
        }
      }
    }
  }

# undef DELTA

  return lpRetVal;
}

#endif
