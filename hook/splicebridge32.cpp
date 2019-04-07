#include "../cfg.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#if(APILIB_32 != 0)

#include "../include/lib.h"
#include "../include/log.h"
#include "../include/mem.h"
#include "../include/hde.h"

#include "spliceimport.h"
#include "splicebridge32.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Присоединение инструкции 'call' (absolute 32-bit).
   *
   * @param bridge  контекст моста.
   * @param address адрес.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool splicebridge_append_call_absolute_32bit(splicebridge_t* bridge, const void* address)
  {
    bool r = false;

    if(bridge != NULL && address != NULL)
    {
      if((splicebridge_instructions_size > bridge->instructions_size) && 
         (splicebridge_instructions_size - bridge->instructions_size) >= 6)
      {
        void* instructions = &bridge->instructions[bridge->instructions_size];
        *(uint08_t*)((unsigned char*)instructions + 0) = 0xFF;                                             /** call [%offset%] **/
        *(uint08_t*)((unsigned char*)instructions + 1) = 0x15;                                             /** ............... **/
        *(uint32_t*)((unsigned char*)instructions + 2) = (uint32_t)address - ((uint32_t)instructions + 6); /** ....  %offset%  **/
        bridge->instructions_size += 6;
        r = true;
      }
    }

    return r;
  }

  /**
   * Присоединение инструкции 'call' (relative 32-bit).
   *
   * @param bridge  контекст моста.
   * @param address адрес.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool splicebridge_append_call_relative_32bit(splicebridge_t* bridge, const void* address)
  {
    bool r = false;

    if(bridge != NULL && address != NULL)
    {
      if((splicebridge_instructions_size > bridge->instructions_size) && 
         (splicebridge_instructions_size - bridge->instructions_size) >= 5)
      {
        void* instructions = &bridge->instructions[bridge->instructions_size];
        *(uint08_t*)((unsigned char*)instructions + 0) = 0xE8;                                             /** call %offset% **/
        *(uint32_t*)((unsigned char*)instructions + 1) = (uint32_t)address - ((uint32_t)instructions + 5); /** .... %offset% **/
        bridge->instructions_size += 5;
        r = true;
      }
    }

    return r;
  }

  /**
   * Присоединение инструкции 'jmp' (absolute 32-bit).
   *
   * @param bridge  контекст моста.
   * @param address адрес.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool splicebridge_append_jmp_absolute_32bit(splicebridge_t* bridge, const void* address)
  {
    bool r = false;

    if(bridge != NULL && address != NULL)
    {
      if((splicebridge_instructions_size > bridge->instructions_size) && 
         (splicebridge_instructions_size - bridge->instructions_size) >= 6)
      {
        void* instructions = &bridge->instructions[bridge->instructions_size];
        *(uint08_t*)((unsigned char*)instructions + 0) = 0xFF;                                             /** jmp [%offset%] **/
        *(uint08_t*)((unsigned char*)instructions + 1) = 0x25;                                             /** .............. **/
        *(uint32_t*)((unsigned char*)instructions + 2) = (uint32_t)address - ((uint32_t)instructions + 6); /** .... %offset%  **/
        bridge->instructions_size += 6;
        r = true;
      }
    }

    return r;
  }

  /**
   * Присоединение инструкции 'jmp' (relative 32-bit).
   *
   * @param bridge  контекст моста.
   * @param address адрес.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool splicebridge_append_jmp_relative_32bit(splicebridge_t* bridge, const void* address)
  {
    bool r = false;

    if(bridge != NULL && address != NULL)
    {
      if((splicebridge_instructions_size > bridge->instructions_size) && 
         (splicebridge_instructions_size - bridge->instructions_size) >= 5)
      {
        void* instructions = &bridge->instructions[bridge->instructions_size];
        *(uint08_t*)((unsigned char*)instructions + 0) = 0xE9;                                             /** jmp %offset% **/
        *(uint32_t*)((unsigned char*)instructions + 1) = (uint32_t)address - ((uint32_t)instructions + 5); /** ... %offset% **/
        bridge->instructions_size += 5;
        r = true;
      }
    }

    return r;
  }

  /**
   * Присоединение инструкции 'jcc' (relative 32-bit).
   *
   * @param bridge    контекст моста.
   * @param address   адрес.
   * @param condition часть опкода формирующая условие.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool splicebridge_append_jcc_relative_32bit(splicebridge_t* bridge, void* address, uint08_t condition)
  {
    bool r = false;

    if(bridge != NULL && address != NULL)
    {
      if((splicebridge_instructions_size > bridge->instructions_size) && 
         (splicebridge_instructions_size - bridge->instructions_size) >= 6)
      {
        void* instructions = &bridge->instructions[bridge->instructions_size];
        *(uint08_t*)((unsigned char*)instructions + 0) = 0xFF;                                             /** jcc %offset% **/
        *(uint08_t*)((unsigned char*)instructions + 1) = 0x80 | condition;                                 /** ... %offset% **/
        *(uint32_t*)((unsigned char*)instructions + 2) = (uint32_t)address - ((uint32_t)instructions + 6); /** ... %offset% **/
        bridge->instructions_size += 6;
        r = true;
      }
    }

    return r;
  }

  /**
   * Присоединение инструкции не требующей корректировок.
   *
   * @param bridge           контекст моста.
   * @param instruction_data данные инструкции.
   * @param instruction_size размер инструкции.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool splicebridge_append_instruction(splicebridge_t* bridge, void* instruction_data, size_t instruction_size)
  {
    bool r = false;

    if(bridge != NULL && instruction_data != NULL && instruction_size != 0)
    {
      if((splicebridge_instructions_size > bridge->instructions_size) && 
         (splicebridge_instructions_size - bridge->instructions_size) >= instruction_size)
      {
        if(memcpy(&bridge->instructions[bridge->instructions_size], instruction_data, instruction_size) == true)
        {
          bridge->instructions_size += instruction_size;
          r = true;
        }
      }
    }

    return r;
  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

///////////////////////////////////////////////////////////////////////////////////////////////////

bool apilib::splicebridge_create(splicebridge_t* bridge, const void* target, size_t needed)
{
  bool r = false;

  do 
  {
    if(bridge == NULL || target == NULL /** || needed == 0 **/)
    {
      loge("Invalid argument.");
      break;
    }

    size_t target_offset = 0;

    while(splicebridge_instructions_size > bridge->instructions_size)
    {
      HDE32_CONTEXT hde;
      memzero(&hde, sizeof(hde));

      unsigned char buffer[16];
      memzero(buffer, sizeof(buffer));

      // Чтение инструкции во временный буфер.
      void* instruction = (void*)((unsigned char*)target + target_offset);
      if(spliceimport_memread(instruction, buffer, sizeof(buffer)) == false)
      {
        loge("Failed to read instruction at address '0x%p'.", instruction);
        break;
      }

      // Дизассембелирование инструкции.
      size_t buffer_size = hde32_disasm(buffer, &hde);
      if((hde.flags & HDE32_F_ERROR) != 0)
      {
        loge("Failed to disassemble instruction at address '0x%p'.", instruction);
        break;
      }

      // Проверка достаточности размера перенесенных инструкций.
      if((unsigned char*)instruction - (unsigned char*)target >= needed)
      {
        if(splicebridge_append_jmp_relative_32bit(bridge, instruction) == true)
        {
          r = true;
          break;
        }
        else
        {
          loge("Failed to append the latest jump to the bridge.");
          break;
        }
      }
      // Обработка инструкции 'call' (relative 32-bit).
      else if(hde.opcode == 0xE8)
      {
        if(splicebridge_append_call_relative_32bit(bridge, ((unsigned char*)instruction + hde.len + (int32_t)hde.imm.imm32)) == false)
        {
          loge("Failed to append 'call' (relative 32-bit) instruction to the bridge at address '0x%p'.", instruction);
          break;
        }
      }
      // Обработка инструкции 'jmp' (relative 32-bit).
      else if(hde.opcode == 0xE9)
      {
        if(splicebridge_append_jmp_relative_32bit(bridge, ((unsigned char*)instruction + hde.len + (int32_t)hde.imm.imm32)) == false)
        {
          loge("Failed to append 'jmp' (relative 32-bit) instruction to the bridge at address '0x%p'.", instruction);
          break;
        }
      }
      // Обработка инструкции 'jmp' (relative 08-bit).
      else if(hde.opcode == 0xEB)
      {
        if(splicebridge_append_jmp_relative_32bit(bridge, ((unsigned char*)instruction + hde.len + (int32_t)(int08_t)hde.imm.imm8)) == false)
        {
          loge("Failed to append 'jmp' (relative 32-bit) instruction to the bridge at address '0x%p'.", instruction);
          break;
        }
      }
      // Обработка инструкции 'jcc' (relative 08-bit).
      else if((hde.opcode & 0xF0) == 0x70)
      {
        if(splicebridge_append_jcc_relative_32bit(bridge, ((unsigned char*)instruction + hde.len + (int32_t)(int08_t)hde.imm.imm8), (hde.opcode & 0x0F)) == false)
        {
          loge("Failed to append 'jcc' (relative 32-bit) instruction to the bridge at address '0x%p'.", instruction);
          break;
        }
      }
      // Обработка инструкции 'jcc' (relative 32-bit).
      else if((hde.opcode == 0x0F) && ((hde.opcode2 & 0xF0) == 0x80))
      {
        if(splicebridge_append_jcc_relative_32bit(bridge, ((unsigned char*)instruction + hde.len + (int32_t)hde.imm.imm32), (hde.opcode2 & 0x0F)) == false)
        {
          loge("Failed to append 'jcc' (relative 32-bit) instruction to the bridge at address '0x%p'.", instruction);
          break;
        }
      }
      // Обработка инструкции 'call' (displacement 32-bit).
      else if((hde.opcode == 0xFF) && (hde.opcode2 == 0x15))
      {
        if(splicebridge_append_call_absolute_32bit(bridge, ((unsigned char*)instruction + hde.len + (int32_t)hde.disp.disp32)) == false)
        {
          loge("Failed to append 'call' (absolute 32-bit) instruction to the bridge at address '0x%p'.", instruction);
          break;
        }
      }
      // Обработка инструкции 'jmp' (displacement 32-bit).
      else if((hde.opcode == 0xFF) && (hde.opcode2 == 0x25))
      {
        if(splicebridge_append_jmp_absolute_32bit(bridge, ((unsigned char*)instruction + hde.len + (int32_t)hde.disp.disp32)) == false)
        {
          loge("Failed to append 'jmp' (absolute 32-bit) instruction to the bridge at address '0x%p'.", instruction);
          break;
        }
      }
      // Обработка инструкции 'jecxz' (relative 08-bit).
      else if(hde.opcode == 0xE3)
      {
        logw("Detected instruction 'jecxz' (relative 8-bit) at address '0x%p'.", instruction);
        break;
      }
      // Обработка инструкции 'ret'.
      else if((hde.opcode & 0xFE) == 0xC2)
      {
        logw("Detected instruction 'ret' at address '0x%p'.", instruction);
        break;
      }
      // Обработка инструкции не требующей корректировки.
      else 
      {
        if(splicebridge_append_instruction(bridge, buffer, buffer_size) == false)
        {
          loge("Failed to append instruction to the bridge at address '0x%p'.", instruction);
          break;
        }
      }

      target_offset += buffer_size;
    }
  }
  while(0);

  return r;
}

bool apilib::splicebridge_destroy(splicebridge_t* bridge)
{
  bool r = false;

  do 
  {
    if(bridge == NULL)
    {
      loge("Invalid argument.");
      break;
    }

    memzero(bridge->instructions, sizeof(bridge->instructions));
    bridge->instructions_size = 0;

    r = true;
  }
  while(0);

  return r;
}

bool apilib::splicebridge_destroy_available(splicebridge_t* bridge)
{
  bool r = false;

  do 
  {
    if(bridge == NULL)
    {
      loge("Invalid argument.");
      break;
    }

    r = true;
  }
  while(0);

  return r;
}

void* apilib::splicebridge_get_location(splicebridge_t* bridge)
{
  void* r = NULL;

  do 
  {
    if(bridge == NULL)
    {
      loge("Invalid argument.");
      break;
    }

    r = bridge->instructions;
  }
  while(0);

  return r;
}

#endif // #if(APILIB_32 != 0)

///////////////////////////////////////////////////////////////////////////////////////////////////