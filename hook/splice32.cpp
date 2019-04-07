#include "../cfg.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#if(APILIB_32 != 0)

#include "../include/lib.h"
#include "../include/log.h"
#include "../include/mem.h"

#include "spliceimport.h"
#include "splicebridge32.h"
#include "splicejumper32.h"
#include "spliceplaces32.h"

#include "splice.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Статус контекста сплайсинга.
  */
  enum splice_status_t
  {
    splice_status_no             = 0x00, // Пустой контекст.
    splice_status_created_places = 0x01, // Создан контекст областей.
    splice_status_created_bridge = 0x02, // Создан контекст моста.
    splice_status_created_jumper = 0x04, // Создан контекст переключателя.
  };

  /**
   * Контекст сплайсинга в 32-битном окружении.
  */
  struct splice_t 
  {
    uint32_t       status; // Состояние контекста.
    spliceplaces_t places; // Контекст областей.
    splicebridge_t bridge; // Контекст моста.
    splicejumper_t jumper; // Контекст переключателя.
  };

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Получение абсолютного адреса точки назначения на которую указывает инструкция 'jmp'.
   *
   * @param address адрес по которому распологается инструкция 'jmp'.
   *
   * @return абсолютного адрес точки назначения в случае успеха или NULL в случае ошибки.
  */
  static void* splice_get_jump_address(void* address)
  {
    void* r = NULL;

    do 
    {
      if(address == NULL)
      {
        loge("Invalid argument.");
        break;
      }

      // Чтение инструкций по указанному адресу во временный буфер.
      unsigned char buffer[16];
      if(spliceimport_memread(address, buffer, sizeof(buffer)) == false)
      {
        loge("Failed to read virtual memory.");
        break;
      }

      // Проверка инструкции 'jmp' (relative 08-bit).
      if(buffer[0] == 0xEB)
      {
        r = (void*)((unsigned char*)address + 2 + (int32_t)(*(int08_t*)&buffer[1])); 
        break;
      }

      // Проверка инструкции 'jmp' (relative 32-bit).
      if(buffer[0] == 0xE9)
      {
        r = (void*)((unsigned char*)address + 5 + (int32_t)(*(int32_t*)&buffer[1]));
        break;
      }

      // Проверка инструкции 'jmp' (absolute 64-bit).
      if(buffer[0] == 0xFF && buffer[1] == 0x25) 
      {
        void* tmp = NULL;
        void* ptr = (void*)((unsigned char*)address + 6 + (int32_t)(*(int32_t*)&buffer[2]));
        if(spliceimport_memread(ptr, &tmp, sizeof(tmp)) == true)
        {
          r = tmp;
        }
        else
        {
          loge("Failed to read virtual memory.");
        }
        break;
      }
    } 
    while(0);

    return r;
  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

///////////////////////////////////////////////////////////////////////////////////////////////////

void* apilib::splice_search_redirect(void* target)
{
  void* r = NULL;

  do 
  {
    if(target == NULL)
    {
      loge("Invalid argument.");
      break;
    }

    while((target = splice_get_jump_address(target)) != NULL)
    {
      r = target;
    }
  }
  while(0);

  return r;
}

apilib::splice_t* apilib::splice_create(void* target, void* hooker)
{
  splice_t* r = NULL;

  do 
  {
    if(target == NULL || hooker == NULL)
    {
      loge("Invalid argument.");
      break;
    }

    splice_t* splice = (splice_t*)spliceimport_memalloc(sizeof(splice_t));
    if(splice == NULL)
    {
      loge("Failed to allocate virtual memory.");
      break;
    }

    if(memzero(splice, sizeof(splice_t)) == false)
    {
      loge("Failed to zero virtual memory.");
      spliceimport_memfree(splice);
      break;
    }

    void* bridge = splicebridge_get_location(&splice->bridge);
    if(bridge == NULL)
    {
      loge("Failed to get bridge address.");
      splice_destroy(splice);
      break;
    }

    void* jumper = splicejumper_get_location(&splice->jumper);
    if(jumper == NULL)
    {
      loge("Failed to get jumper address.");
      splice_destroy(splice);
      break;
    }

    void*  bridge_address = NULL;
    size_t bridge_movecnt = 0;
    if(spliceplaces_create(&splice->places, target, jumper, &bridge_address, &bridge_movecnt) == true)
    {
      splice->status |= splice_status_created_places;
    }
    else
    {
      loge("Failed to create the places context.");
      splice_destroy(splice);
      break;
    }    

    if(splicebridge_create(&splice->bridge, bridge_address, bridge_movecnt) == true)
    {
      splice->status |= splice_status_created_bridge;
    }
    else
    {
      loge("Failed to create the bridge context.");
      splice_destroy(splice);
      break;
    }
   
    if(splicejumper_create(&splice->jumper, bridge, hooker) == true)
    {
      splice->status |= splice_status_created_jumper;
    }
    else
    {
      loge("Failed to create the jumper context.");
      splice_destroy(splice);
      break;
    }

    r = splice;
  }
  while(0);

  return r;
}

bool apilib::splice_destroy(splice_t* splice)
{
  bool r = false;

  do
  {
    if(splice == NULL)
    {
      loge("Invalid argument.");
      break;
    }

    if((splice->status & splice_status_created_jumper) != 0)
    {
      if(splicejumper_destroy_available(&splice->jumper) == false)
      {
        logw("The jumper context is not ready to destroy.");
        break;
      }
      if(splicejumper_destroy(&splice->jumper) == false)
      {
        loge("Failed to destroy the jumper context.");
        break;
      }
      splice->status &= ~splice_status_created_jumper;
    }

    if((splice->status & splice_status_created_bridge) != 0)
    {
      if(splicebridge_destroy_available(&splice->bridge) == false)
      {
        logw("The bridge context is not ready to destroy.");
        break;
      }
      if(splicebridge_destroy(&splice->bridge) == false)
      {
        loge("Failed to destroy the bridge context.");
        break;
      }
      splice->status &= ~splice_status_created_bridge;
    }

    if((splice->status & splice_status_created_places) != 0)
    {
      if(spliceplaces_destroy_available(&splice->places) == false)
      {
        logw("The places context is not ready to destroy.");
        break;
      }
      if(spliceplaces_destroy(&splice->places) == false)
      {
        loge("Failed to destroy the places context.");
        break;
      }
      splice->status &= ~splice_status_created_places;
    }

    memzero(splice, sizeof(splice_t));
        
    if(spliceimport_memfree(splice) == false)
    {
      loge("Failed to free virtual memory.");
      break;
    }

    r = true;
  }
  while(0);

  return r;
}

bool apilib::splice_write_modified_bytes(splice_t* splice)
{
  bool r = false;

  do 
  {
    if(splice == NULL)
    {
      loge("Invalid argument.");
      break;
    }

    if((splice->status & splice_status_created_places) == 0)
    {
      logw("The places context is not ready.");
      break;
    }

    r = spliceplaces_write_modified_bytes(&splice->places);
  }
  while(0);

  return r;
}

bool apilib::splice_write_original_bytes(splice_t* splice)
{
  bool r = false;

  do 
  {
    if(splice == NULL)
    {
      loge("Invalid argument.");
      break;
    }

    if((splice->status & splice_status_created_places) == 0)
    {
      logw("The places context is not ready.");
      break;
    }

    r = spliceplaces_write_original_bytes(&splice->places);
  }
  while(0);

  return r;
}

bool apilib::splice_enable(splice_t* splice)
{
  bool r = false;

  do 
  {
    if(splice == NULL)
    {
      loge("Invalid argument.");
      break;
    }

    if((splice->status & splice_status_created_jumper) == 0)
    {
      logw("The jumper context is not ready.");
      break;
    }

    r = splicejumper_set_active(&splice->jumper, true);
  }
  while(0);

  return r;
}

bool apilib::splice_disable(splice_t* splice)
{
  bool r = false;

  do 
  {
    if(splice == NULL)
    {
      loge("Invalid argument.");
      break;
    }

    if((splice->status & splice_status_created_jumper) == 0)
    {
      logw("The jumper context is not ready.");
      break;
    }

    r = splicejumper_set_active(&splice->jumper, false);
  }
  while(0);

  return r;
}

bool apilib::splice_enable_for_current_thread(splice_t* splice)
{
  bool r = false;

  do 
  {
    if(splice == NULL)
    {
      loge("Invalid argument.");
      break;
    }

    if((splice->status & splice_status_created_jumper) == 0)
    {
      logw("The jumper context is not ready.");
      break;
    }

    r = splicejumper_set_active_for_current_thread(&splice->jumper, true);
  }
  while(0);

  return r;
}

bool apilib::splice_disable_for_current_thread(splice_t* splice)
{
  bool r = false;

  do 
  {
    if(splice == NULL)
    {
      loge("Invalid argument.");
      break;
    }

    if((splice->status & splice_status_created_jumper) == 0)
    {
      logw("The jumper context is not ready.");
      break;
    }

    r = splicejumper_set_active_for_current_thread(&splice->jumper, false);
  }
  while(0);

  return r;
}

void* apilib::splice_get_bridge_location(splice_t* splice)
{
  void* r = NULL;

  do 
  {
    if(splice == NULL)
    {
      loge("Invalid argument.");
      break;
    }

    r = splicebridge_get_location(&splice->bridge);
  }
  while(0);

  return r;
}

#endif // # if(APILIB_32 != 0)

///////////////////////////////////////////////////////////////////////////////////////////////////