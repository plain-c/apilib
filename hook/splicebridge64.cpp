#include "../cfg.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#if(APILIB_64 != 0)

#include "../include/lib.h"
#include "../include/log.h"
#include "../include/mem.h"
#include "../include/hde.h"

#include "spliceimport.h"
#include "splicebridge64.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace apilib 
{ 
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Присоединение инструкции 'call' (absolute 64-bit).
   *
   * @param bridge  контекст моста.
   * @param address адрес.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool splicebridge_append_call_absolute_64bit(splicebridge_t* bridge, void* address)
  {
    bool r = false;

    if(bridge != NULL && address != NULL)
    {
      if((splicebridge_addresstable_size > bridge->addresstable_size) && (splicebridge_addresstable_size - bridge->addresstable_size) >= 1 &&
         (splicebridge_instructions_size > bridge->instructions_size) && (splicebridge_instructions_size - bridge->instructions_size) >= 6)
      {
        static const uint64_t himask = 0xFFFFFFFF00000000;
        static const uint64_t lomask = 0x00000000FFFFFFFF;
        void** addresstable_entry = &bridge->addresstable[bridge->addresstable_size];
        void*  instructions_entry = &bridge->instructions[bridge->instructions_size];

        uint64_t offset = (uint64_t)addresstable_entry - ((uint64_t)instructions_entry + 6);
        uint64_t hibits = (offset & himask);
        if(hibits == 0 || hibits == himask)
        {      
          *addresstable_entry = address;

          *(uint08_t*)((unsigned char*)instructions_entry + 0) = 0xFF;                        /** call [%offset%] **/
          *(uint08_t*)((unsigned char*)instructions_entry + 1) = 0x15;                        /** ............... **/
          *(uint32_t*)((unsigned char*)instructions_entry + 2) = (uint32_t)(offset & lomask); /** ....  %offset%  **/

          bridge->addresstable_size += 1;
          bridge->instructions_size += 6;

          r = true;
        }
      }
    }

    return r;
  }

  /**
   * Присоединение инструкции 'jmp' (absolute 64-bit).
   *
   * @param bridge  контекст моста.
   * @param address адрес.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool splicebridge_append_jmp_absolute_64bit(splicebridge_t* bridge, void* address)
  {
    bool r = false;

    if(bridge != NULL && address != NULL)
    {
      if((splicebridge_addresstable_size > bridge->addresstable_size) && (splicebridge_addresstable_size - bridge->addresstable_size) >= 1 &&
         (splicebridge_instructions_size > bridge->instructions_size) && (splicebridge_instructions_size - bridge->instructions_size) >= 6)
      {
        static const uint64_t himask = 0xFFFFFFFF00000000;
        static const uint64_t lomask = 0x00000000FFFFFFFF;
        void** addresstable_entry = &bridge->addresstable[bridge->addresstable_size];
        void*  instructions_entry = &bridge->instructions[bridge->instructions_size];

        uint64_t offset = (uint64_t)addresstable_entry - ((uint64_t)instructions_entry + 6);
        uint64_t hibits = (offset & himask);
        if(hibits == 0 || hibits == himask)
        {      
          *addresstable_entry = address;

          *(uint08_t*)((unsigned char*)instructions_entry + 0) = 0xFF;                        /** jmp [%offset%] **/
          *(uint08_t*)((unsigned char*)instructions_entry + 1) = 0x25;                        /** .............. **/
          *(uint32_t*)((unsigned char*)instructions_entry + 2) = (uint32_t)(offset & lomask); /** ...  %offset%  **/

          bridge->addresstable_size += 1;
          bridge->instructions_size += 6;

          r = true;
        }
      }
    }

    return r;
  }

  /**
   * Присоединение инструкции 'jcc' (absolute 64-bit).
   *
   * @param bridge    контекст моста.
   * @param address   адрес для вызова.
   * @param condition часть опкода формирующая условие.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool splicebridge_append_jcc_absolute_64bit(splicebridge_t* bridge, void* address, uint08_t condition)
  {
    bool r = false;

    if(bridge != NULL && address != NULL)
    {
      if((splicebridge_addresstable_size > bridge->addresstable_size) && (splicebridge_addresstable_size - bridge->addresstable_size) >= 1 &&
         (splicebridge_instructions_size > bridge->instructions_size) && (splicebridge_instructions_size - bridge->instructions_size) >= 10)
      {
        static const uint64_t himask = 0xFFFFFFFF00000000;
        static const uint64_t lomask = 0x00000000FFFFFFFF;
        void** addresstable_entry = &bridge->addresstable[bridge->addresstable_size];
        void*  instructions_entry = &bridge->instructions[bridge->instructions_size];

        uint64_t offset = (uint64_t)addresstable_entry - ((uint64_t)instructions_entry + 10);
        uint64_t hibits = (offset & himask);
        if(hibits == 0 || hibits == himask)
        {
          *addresstable_entry = address;

          *(uint08_t*)((unsigned char*)instructions_entry + 0) = 0x70 | condition;            /** jcc  %goto_64bit_jump% **/
          *(uint08_t*)((unsigned char*)instructions_entry + 1) = 0x02;                        /** ...  %goto_64bit_jump% **/
          *(uint08_t*)((unsigned char*)instructions_entry + 2) = 0xEB;                        /** jmp  %skip_64bit_jump% **/
          *(uint08_t*)((unsigned char*)instructions_entry + 3) = 0x06;                        /** ...  %skip_64bit_jump% **/
          *(uint08_t*)((unsigned char*)instructions_entry + 4) = 0xFF;                        /** jmp [%offset%] ....... **/
          *(uint08_t*)((unsigned char*)instructions_entry + 5) = 0x25;                        /** ...................... **/
          *(uint32_t*)((unsigned char*)instructions_entry + 6) = (uint32_t)(offset & lomask); /** .... %offset% ........ **/

          bridge->addresstable_size += 1;
          bridge->instructions_size += 10;

          r = true;
        }
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
      HDE64_CONTEXT hde;
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
      size_t buffer_size = hde64_disasm(buffer, &hde);
      if((hde.flags & HDE64_F_ERROR) != 0)
      {
        loge("Failed to disassemble instruction at address '0x%p'.", instruction);
        break;
      }

      // Проверка достаточности размера перенесенных инструкций.
      if((unsigned char*)instruction - (unsigned char*)target >= needed)
      {
        if(splicebridge_append_jmp_absolute_64bit(bridge, instruction) == true)
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
        if(splicebridge_append_call_absolute_64bit(bridge, ((unsigned char*)instruction + hde.len + (int64_t)(int32_t)hde.imm.imm32)) == false)
        {
          loge("Failed to append 'call' (absolute 64-bit) instruction to the bridge at address '0x%p'.", instruction);
          break;
        }
      }
      // Обработка инструкции 'jmp' (relative 32-bit).
      else if(hde.opcode == 0xE9)
      {
        if(splicebridge_append_jmp_absolute_64bit(bridge, ((unsigned char*)instruction + hde.len + (int64_t)(int32_t)hde.imm.imm32)) == false)
        {
          loge("Failed to append 'jmp' (absolute 64-bit) instruction to the bridge at address '0x%p'.", instruction);
          break;
        }
      }
      // Обработка инструкции 'jmp' (relative 08-bit).
      else if(hde.opcode == 0xEB)
      {
        if(splicebridge_append_jmp_absolute_64bit(bridge, ((unsigned char*)instruction + hde.len + (int64_t)(int08_t)hde.imm.imm8)) == false)
        {
          loge("Failed to append 'jmp' (absolute 64-bit) instruction to the bridge at address '0x%p'.", instruction);
          break;
        }
      }
      // Обработка инструкции 'jcc' (relative 08-bit).
      else if((hde.opcode & 0xF0) == 0x70)
      {
        if(splicebridge_append_jcc_absolute_64bit(bridge, ((unsigned char*)instruction + hde.len + (int64_t)(int08_t)hde.imm.imm8), (hde.opcode & 0x0F)) == false)
        {
          loge("Failed to append 'jcc' (absolute 64-bit) instruction to the bridge at address '0x%p'.", instruction);
          break;
        }
      }
      // Обработка инструкции 'jcc' (relative 32-bit).
      else if((hde.opcode == 0x0F) && ((hde.opcode2 & 0xF0) == 0x80))
      {
        if(splicebridge_append_jcc_absolute_64bit(bridge, ((unsigned char*)instruction + hde.len + (int64_t)(int32_t)hde.imm.imm32), (hde.opcode2 & 0x0F)) == false)
        {
          loge("Failed to append 'jcc' (absolute 64-bit) instruction to the bridg at address '0x%p'.", instruction);
          break;
        }
      }
      // Обработка инструкции 'call' (displacement 32-bit), обработать инструкцию довольно сложно без использования регистров.
      else if((hde.opcode == 0xFF) && (hde.opcode2 == 0x15))
      {
        logw("Detected instruction 'call' (displacement 32-bit) at address '0x%p'.", instruction);
        break;
      }
      // Обработка инструкции 'jmp' (displacement 32-bit), обработать инструкцию довольно сложно без использования регистров.
      else if((hde.opcode == 0xFF) && (hde.opcode2 == 0x25))
      {
        logw("Detected instruction 'jmp' (displacement 32-bit) at address '0x%p'.", instruction);
        break;
      }
      // Обработка инструкции 'jecxz' (relative 08-bit), TODO: добавить обработку инструкции по принципу 'jcc' (relative 08-bit).
      else if(hde.opcode == 0xE3)
      {
        logw("Detected instruction 'jecxz' (relative 8-bit) at address '0x%p'.", instruction);
        break;
      }
      // Обработка неизвестной RIP-based инструкции.
      else if((hde.modrm & 0xC7) == 0x05)
      {
        logw("Detected instruction 'unrecognized RIP-based' at address '0x%p'.", instruction);
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
    memzero(bridge->addresstable, sizeof(bridge->addresstable));
    bridge->instructions_size = 0;
    bridge->addresstable_size = 0;

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

#endif // #if(APILIB_64 != 0)

///////////////////////////////////////////////////////////////////////////////////////////////////