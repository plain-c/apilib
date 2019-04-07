#include "../cfg.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "../include/lib.h"
#include "../include/log.h"
#include "../include/mem.h"

#include "mpack.h"
#include "mpackread.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Конвертация между Big Endian и Little Endian для 16-битных чисел.
   *
   * @param digit указатель на число в памяти.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool mpack_swap16(void* digit)
  {
    bool r = false;

    if(digit != NULL)
    {
      unsigned char *p = (unsigned char*)digit;
      unsigned char temp;

      temp = p[0];
      p[0] = p[1];
      p[1] = temp;

      r = true;
    }

    return r;
  }

  /**
   * Конвертация между Big Endian и Little Endian для 32-битных чисел.
   *
   * @param digit указатель на число в памяти.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool mpack_swap32(void* digit)
  {
    bool r = false;

    if(digit != NULL)
    {
      unsigned char *p = (unsigned char*)digit;
      unsigned char temp;

      temp = p[0];
      p[0] = p[3];
      p[3] = temp;

      temp = p[1];
      p[1] = p[2];
      p[2] = temp;

      r = true;
    }

    return r;
  }

  /**
   * Конвертация между Big Endian и Little Endian для 64-битных чисел.
   *
   * @param digit указатель на число в памяти.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  static bool mpack_swap64(void* digit)
  {
    bool r = false;

    if(digit != NULL)
    {
      unsigned char *p = (unsigned char*)digit;
      unsigned char temp;

      temp = p[0];
      p[0] = p[7];
      p[7] = temp;

      temp = p[1];
      p[1] = p[6];
      p[6] = temp;

      temp = p[2];
      p[2] = p[5];
      p[5] = temp;

      temp = p[3];
      p[3] = p[4];
      p[4] = temp;

      r = true;
    }

    return r;
  }

  /**
   * Получение формата объекта MessagePack.
   *
   * @param mpack контекст получения данных.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки.
  */
  static int mpack_receive_format(mpack_read_t* mpack)
  {
    if(mpack == NULL)
    {
      return mpack_code_error_invalid_argument;
    }

    if(mpack->fn_read == NULL)
    {
      return mpack_code_error_invalid_callback;
    }

    if(mpack->fn_read(mpack->fn_read_param, &mpack->format, 1) < 1)
    {
      return mpack_code_error_not_enough_data;
    }

    mpack->status = mpack_read_status_process_format;
    return mpack_code_ok;
  }

  /**
   * Обработка полученного формата объекта MessagePack.
   *
   * @param mpack контекст получения данных.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки.
  */
  static int mpack_process_format(mpack_read_t* mpack)
  {
    if(mpack == NULL)
    {
      return mpack_code_error_invalid_argument;
    }

    uint08_t format = mpack->format;

    if((format & 0x80) == mpack_format_fixint_positive)
    {
      mpack->object.type = mpack_type_int;
      mpack->object.i64  = (int64_t)(int08_t)format;
      mpack->status      = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }

    if((format & 0xe0) == mpack_format_fixint_negative)
    {
      mpack->object.type = mpack_type_int;
      mpack->object.i64  = (int64_t)(int08_t)format;
      mpack->status      = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }

    if((format & 0xf0) == mpack_format_fixarray)
    {
      mpack->object.type      = mpack_type_array;
      mpack->object.arr_count = (uint32_t)(uint08_t)(format & 0x0f);
      mpack->status           = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }

    if((format & 0xf0) == mpack_format_fixmap)
    {
      mpack->object.type      = mpack_type_map;
      mpack->object.map_count = (uint32_t)(uint08_t)(format & 0x0f);
      mpack->status           = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }

    if((format & 0xe0) == mpack_format_fixstr)
    {
      mpack->object.type     = mpack_type_str;
      mpack->object.str_size = (uint32_t)(uint08_t)(format & 0x1f);
      mpack->status          = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }

    if(format == mpack_format_nil)
    {
      mpack->object.type = mpack_type_nil;
      mpack->status      = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }

#   if(0)
    if(format == mpack_format_reserved)
    {
      mpack->object.type = mpack_type_no;
      mpack->status      = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }
#   endif

    if(format == mpack_format_false)
    {
      mpack->object.type = mpack_type_bool;
      mpack->object.bln  = false;
      mpack->status      = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }

    if(format == mpack_format_true)
    {
      mpack->object.type = mpack_type_bool;
      mpack->object.bln  = true;
      mpack->status      = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }

    if(format == mpack_format_bin8)
    {
      mpack->object.type = mpack_type_bin;
      mpack->number_need = 1;
      mpack->number_read = 0;
      mpack->status      = mpack_read_status_receive_number;
      return mpack_code_ok;
    }

    if(format == mpack_format_bin16)
    {
      mpack->object.type = mpack_type_bin;
      mpack->number_need = 2;
      mpack->number_read = 0;
      mpack->status      = mpack_read_status_receive_number;
      return mpack_code_ok;
    }

    if(format == mpack_format_bin32)
    {
      mpack->object.type = mpack_type_bin;
      mpack->number_need = 4;
      mpack->number_read = 0;
      mpack->status      = mpack_read_status_receive_number;
      return mpack_code_ok;
    }

    if(format == mpack_format_ext8)
    {
      mpack->object.type = mpack_type_ext;
      mpack->number_need = 1 + 1;
      mpack->number_read = 0;
      mpack->status      = mpack_read_status_receive_number;
      return mpack_code_ok;
    }

    if(format == mpack_format_ext16)
    {
      mpack->object.type = mpack_type_ext;
      mpack->number_need = 2 + 1;
      mpack->number_read = 0;
      mpack->status      = mpack_read_status_receive_number;
      return mpack_code_ok;
    }

    if(format == mpack_format_ext32)
    {
      mpack->object.type = mpack_type_ext;
      mpack->number_need = 4 + 1;
      mpack->number_read = 0;
      mpack->status      = mpack_read_status_receive_number;
      return mpack_code_ok;
    }

    if(format == mpack_format_float32)
    {
      mpack->object.type = mpack_type_float;
      mpack->number_need = 4;
      mpack->number_read = 0;
      mpack->status      = mpack_read_status_receive_number;
      return mpack_code_ok;
    }

    if(format == mpack_format_float64)
    {
      mpack->object.type = mpack_type_double;
      mpack->number_need = 8;
      mpack->number_read = 0;
      mpack->status      = mpack_read_status_receive_number;
      return mpack_code_ok;
    }

    if(format == mpack_format_int8)
    {
      mpack->object.type = mpack_type_int;
      mpack->number_need = 1;
      mpack->number_read = 0;
      mpack->status      = mpack_read_status_receive_number;
      return mpack_code_ok;
    }

    if(format == mpack_format_int16)
    {
      mpack->object.type = mpack_type_int;
      mpack->number_need = 2;
      mpack->number_read = 0;
      mpack->status      = mpack_read_status_receive_number;
      return mpack_code_ok;
    }

    if(format == mpack_format_int32)
    {
      mpack->object.type = mpack_type_int;
      mpack->number_need = 4;
      mpack->number_read = 0;
      mpack->status      = mpack_read_status_receive_number;
      return mpack_code_ok;
    }

    if(format == mpack_format_int64)
    {
      mpack->object.type = mpack_type_int;
      mpack->number_need = 8;
      mpack->number_read = 0;
      mpack->status      = mpack_read_status_receive_number;
      return mpack_code_ok;
    }

    if(format == mpack_format_uint8)
    {
      mpack->object.type = mpack_type_uint;
      mpack->number_need = 1;
      mpack->number_read = 0;
      mpack->status      = mpack_read_status_receive_number;
      return mpack_code_ok;
    }

    if(format == mpack_format_uint16)
    {
      mpack->object.type = mpack_type_uint;
      mpack->number_need = 2;
      mpack->number_read = 0;
      mpack->status      = mpack_read_status_receive_number;
      return mpack_code_ok;
    }

    if(format == mpack_format_uint32)
    {
      mpack->object.type = mpack_type_uint;
      mpack->number_need = 4;
      mpack->number_read = 0;
      mpack->status      = mpack_read_status_receive_number;
      return mpack_code_ok;
    }

    if(format == mpack_format_uint64)
    {
      mpack->object.type = mpack_type_uint;
      mpack->number_need = 8;
      mpack->number_read = 0;
      mpack->status      = mpack_read_status_receive_number;
      return mpack_code_ok;
    }

    if(format == mpack_format_fixext1)
    {
      mpack->object.type = mpack_type_ext;
      mpack->number_need = 1;
      mpack->number_read = 0;
      mpack->binary_need = 1;
      mpack->binary_read = 0;
      mpack->status      = mpack_read_status_receive_number;
      return mpack_code_ok;
    }

    if(format == mpack_format_fixext2)
    {
      mpack->object.type = mpack_type_ext;
      mpack->number_need = 1;
      mpack->number_read = 0;
      mpack->binary_need = 2;
      mpack->binary_read = 0;
      mpack->status      = mpack_read_status_receive_number;
      return mpack_code_ok;
    }

    if(format == mpack_format_fixext4)
    {
      mpack->object.type = mpack_type_ext;
      mpack->number_need = 1;
      mpack->number_read = 0;
      mpack->binary_need = 4;
      mpack->binary_read = 0;
      mpack->status      = mpack_read_status_receive_number;
      return mpack_code_ok;
    }

    if(format == mpack_format_fixext8)
    {
      mpack->object.type = mpack_type_ext;
      mpack->number_need = 1;
      mpack->number_read = 0;
      mpack->binary_need = 8;
      mpack->binary_read = 0;
      mpack->status      = mpack_read_status_receive_number;
      return mpack_code_ok;
    }

    if(format == mpack_format_fixext16)
    {
      mpack->object.type = mpack_type_ext;
      mpack->number_need = 1;
      mpack->number_read = 0;
      mpack->binary_need = 16;
      mpack->binary_read = 0;
      mpack->status      = mpack_read_status_receive_number;
      return mpack_code_ok;
    }

    if(format == mpack_format_str8)
    {
      mpack->object.type = mpack_type_str;
      mpack->number_need = 1;
      mpack->number_read = 0;
      mpack->status      = mpack_read_status_receive_number;
      return mpack_code_ok;
    }

    if(format == mpack_format_str16)
    {
      mpack->object.type = mpack_type_str;
      mpack->number_need = 2;
      mpack->number_read = 0;
      mpack->status      = mpack_read_status_receive_number;
      return mpack_code_ok;
    }

    if(format == mpack_format_str32)
    {
      mpack->object.type = mpack_type_str;
      mpack->number_need = 4;
      mpack->number_read = 0;
      mpack->status      = mpack_read_status_receive_number;
      return mpack_code_ok;
    }

    if(format == mpack_format_array16)
    {
      mpack->object.type = mpack_type_array;
      mpack->number_need = 2;
      mpack->number_read = 0;
      mpack->status      = mpack_read_status_receive_number;
      return mpack_code_ok;
    }

    if(format == mpack_format_array32)
    {
      mpack->object.type = mpack_type_array;
      mpack->number_need = 4;
      mpack->number_read = 0;
      mpack->status      = mpack_read_status_receive_number;
      return mpack_code_ok;
    }

    if(format == mpack_format_map16)
    {
      mpack->object.type = mpack_type_map;
      mpack->number_need = 2;
      mpack->number_read = 0;
      mpack->status      = mpack_read_status_receive_number;
      return mpack_code_ok;
    }

    if(format == mpack_format_map32)
    {
      mpack->object.type = mpack_type_map;
      mpack->number_need = 4;
      mpack->number_read = 0;
      mpack->status      = mpack_read_status_receive_number;
      return mpack_code_ok;
    }

    return mpack_code_error_invalid_format;
  }

  /**
   * Получение цифровых данных объекта MessagePack.
   *
   * @param mpack контекст получения данных.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки.
  */
  static int mpack_receive_number(mpack_read_t* mpack)
  {
    if(mpack == NULL)
    {
      return mpack_code_error_invalid_argument;
    }

    if(mpack->fn_read == NULL)
    {
      return mpack_code_error_invalid_callback;
    }

    if(mpack->number_need > sizeof(mpack->number_data))
    {
      return mpack_code_error_internal;
    }

    if(mpack->number_need > mpack->number_read)
    {
      uint32_t required = mpack->number_need - mpack->number_read;
      uint32_t received = mpack->fn_read(mpack->fn_read_param, &mpack->number_data[mpack->number_read], required);
      if(received > required)
      {
        return mpack_code_error_internal;
      }
      mpack->number_read += received;
    }

    if(mpack->number_need > mpack->number_read)
    {
      return mpack_code_error_not_enough_data;
    }

    mpack->status = mpack_read_status_process_number;
    return mpack_code_ok;
  }

  /**
   * Обработка полученных цифровых данных объекта MessagePack.
   *
   * @param mp контекст получения данных.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки.
  */
  static int mpack_process_number(mpack_read_t* mpack)
  {
    if(mpack == NULL)
    {
      return mpack_code_error_invalid_argument;
    }

    uint08_t endian = mpack->endian;
    uint08_t format = mpack->format;
    
    if(format == mpack_format_bin8)
    {
      mpack->object.bin_size = *(uint08_t*)&mpack->number_data[0];
      mpack->status          = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }

    if(format == mpack_format_bin16)
    {
      if(endian == mpack_endian_little && mpack_swap16(mpack->number_data) == false)return mpack_code_error_internal;
      mpack->object.bin_size = *(uint16_t*)&mpack->number_data[0];
      mpack->status           = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }

    if(format == mpack_format_bin32)
    {
      if(endian == mpack_endian_little && mpack_swap32(mpack->number_data) == false)return mpack_code_error_internal;
      mpack->object.bin_size = *(uint32_t*)&mpack->number_data[0];
      mpack->status          = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }

    if(format == mpack_format_ext8)
    {
      mpack->object.ext_size = *(uint08_t*)&mpack->number_data[0];
      mpack->object.ext_type = *(uint08_t*)&mpack->number_data[1];
      mpack->status          = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }

    if(format == mpack_format_ext16)
    {
      if(endian == mpack_endian_little && mpack_swap16(mpack->number_data) == false)return mpack_code_error_internal;
      mpack->object.ext_size = *(uint16_t*)&mpack->number_data[0];
      mpack->object.ext_type = *(uint08_t*)&mpack->number_data[2];
      mpack->status          = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }

    if(format == mpack_format_ext32)
    {
      if(endian == mpack_endian_little && mpack_swap32(mpack->number_data) == false)return mpack_code_error_internal;
      mpack->object.ext_size = *(uint32_t*)&mpack->number_data[0];
      mpack->object.ext_type = *(uint08_t*)&mpack->number_data[4];
      mpack->status          = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }

    if(format == mpack_format_float32)
    {
      if(endian == mpack_endian_little && mpack_swap32(mpack->number_data) == false)return mpack_code_error_internal;
      mpack->object.flt = *(float*)&mpack->number_data[0];
      mpack->status     = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }

    if(format == mpack_format_float64)
    {
      if(endian == mpack_endian_little && mpack_swap64(mpack->number_data) == false)return mpack_code_error_internal;
      mpack->object.dbl = *(double*)&mpack->number_data[0];
      mpack->status     = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }

    if(format == mpack_format_int8)
    {
      mpack->object.i64 = *(int08_t*)&mpack->number_data[0];
      mpack->status     = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }

    if(format == mpack_format_int16)
    {
      if(endian == mpack_endian_little && mpack_swap16(mpack->number_data) == false)return mpack_code_error_internal;
      mpack->object.i64 = *(int16_t*)&mpack->number_data[0];
      mpack->status     = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }

    if(format == mpack_format_int32)
    {
      if(endian == mpack_endian_little && mpack_swap32(mpack->number_data) == false)return mpack_code_error_internal;
      mpack->object.i64 = *(int32_t*)&mpack->number_data[0];
      mpack->status     = mpack_read_status_xchange_object;;
      return mpack_code_ok;
    }

    if(format == mpack_format_int64)
    {
      if(endian == mpack_endian_little && mpack_swap64(mpack->number_data) == false)return mpack_code_error_internal;
      mpack->object.i64 = *(int64_t*)&mpack->number_data[0];
      mpack->status     = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }

    if(format == mpack_format_uint8)
    {
      mpack->object.u64 = *(uint08_t*)&mpack->number_data[0];
      mpack->status     = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }

    if(format == mpack_format_uint16)
    {
      if(endian == mpack_endian_little && mpack_swap16(mpack->number_data) == false)return mpack_code_error_internal;
      mpack->object.u64 = *(uint16_t*)&mpack->number_data[0];
      mpack->status     = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }

    if(format == mpack_format_uint32)
    {
      if(endian == mpack_endian_little && mpack_swap32(mpack->number_data) == false)return mpack_code_error_internal;
      mpack->object.u64 = *(uint32_t*)&mpack->number_data[0];
      mpack->status     = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }

    if(format == mpack_format_uint64)
    {
      if(endian == mpack_endian_little && mpack_swap64(mpack->number_data) == false)return mpack_code_error_internal;
      mpack->object.u64 = *(uint64_t*)&mpack->number_data[0];
      mpack->status     = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }

    if(format == mpack_format_fixext1)
    {
      mpack->object.ext_type = *(uint08_t*)&mpack->number_data[0];
      mpack->status          = mpack_read_status_xchange_object;
     return mpack_code_ok;
    }

    if(format == mpack_format_fixext2)
    {
      mpack->object.ext_type = *(uint08_t*)&mpack->number_data[0];
      mpack->status          = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }

    if(format == mpack_format_fixext4)
    {
      mpack->object.ext_type = *(uint08_t*)&mpack->number_data[0];
      mpack->status          = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }

    if(format == mpack_format_fixext8)
    {
      mpack->object.ext_type = *(uint08_t*)&mpack->number_data[0];
      mpack->status          = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }

    if(format == mpack_format_fixext16)
    {
      mpack->object.ext_type = *(uint08_t*)&mpack->number_data[0];
      mpack->status          = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }

    if(format == mpack_format_str8)
    {
      mpack->object.str_size = *(uint08_t*)&mpack->number_data[0];
      mpack->status          = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }

    if(format == mpack_format_str16)
    {
      if(endian == mpack_endian_little && mpack_swap16(mpack->number_data) == false)return mpack_code_error_internal;
      mpack->object.str_size = *(uint16_t*)&mpack->number_data[0];
      mpack->status          = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }

    if(format == mpack_format_str32)
    {
      if(endian == mpack_endian_little && mpack_swap32(mpack->number_data) == false)return mpack_code_error_internal;
      mpack->object.str_size = *(uint32_t*)&mpack->number_data[0];
      mpack->status          = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }

    if(format == mpack_format_array16)
    {
      if(endian == mpack_endian_little && mpack_swap16(mpack->number_data) == false)return mpack_code_error_internal;
      mpack->object.arr_count = *(uint16_t*)&mpack->number_data[0];
      mpack->status           = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }

    if(format == mpack_format_array32)
    {
      if(endian == mpack_endian_little && mpack_swap32(mpack->number_data) == false)return mpack_code_error_internal;
      mpack->object.arr_count = *(uint32_t*)&mpack->number_data[0];
      mpack->status           = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }

    if(format == mpack_format_map16)
    {
      if(endian == mpack_endian_little && mpack_swap16(mpack->number_data) == false)return mpack_code_error_internal;
      mpack->object.map_count = *(uint16_t*)&mpack->number_data[0];
      mpack->status           = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }

    if(format == mpack_format_map32)
    {
      if(endian == mpack_endian_little && mpack_swap32(mpack->number_data) == false)return mpack_code_error_internal;
      mpack->object.map_count = *(uint32_t*)&mpack->number_data[0];
      mpack->status           = mpack_read_status_xchange_object;
      return mpack_code_ok;
    }

    return mpack_code_error_invalid_format;
  }

  /**
   * Получение информации об объекте MessagePack.
   *
   * @param mpack контекст получения данных.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки.
  */
  static int mpack_receive_object(mpack_read_t* mpack)
  {
    if(mpack == NULL)
    {
      return mpack_code_error_invalid_argument;
    }

    if(mpack->status == mpack_read_status_xchange_object)
    {
      return mpack_code_ok;
    }

    if(mpack->status == mpack_read_status_receive_format)
    {
      int r = mpack_receive_format(mpack);
      if( r != mpack_code_ok ) return r;
    }

    if(mpack->status == mpack_read_status_process_format)
    {
      int r = mpack_process_format(mpack);
      if( r != mpack_code_ok ) return r;
    }

    if(mpack->status == mpack_read_status_receive_number)
    {
      int r = mpack_receive_number(mpack);
      if( r != mpack_code_ok ) return r;
    }

    if(mpack->status == mpack_read_status_process_number)
    {
      int r = mpack_process_number(mpack);
      if( r != mpack_code_ok ) return r;
    }

    return mpack_code_ok;
  }

  /**
   * Получение бинарных данных объекта MessagePack.
   *
   * @param mpack контекст получения данных.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки.
  */
  static int mpack_receive_object_binary(mpack_read_t* mpack, void* buffer, uint32_t buffer_size)
  {
    if(mpack == NULL)
    {
      return mpack_code_error_invalid_argument;
    }

    if(mpack->fn_read == NULL)
    {
      return mpack_code_error_invalid_callback;
    }

    if(mpack->binary_need > buffer_size)
    {
      return mpack_code_error_buffer_too_small;
    }

    if(mpack->binary_need > mpack->binary_read)
    {
      uint32_t required = mpack->binary_need - mpack->binary_read;
      uint32_t received = mpack->fn_read(mpack->fn_read_param, &((unsigned char*)buffer)[mpack->binary_read], required);
      if(received > required)
      {
        return mpack_code_error_internal;
      }
      mpack->binary_read += received;
    }

    if(mpack->binary_need > mpack->binary_read)
    {
      return mpack_code_error_not_enough_data;
    }

    return mpack_code_ok;
  }

  static bool mpack_recv_reset(mpack_read_t* mpack)
  {
    bool r = false;

    if(mpack != NULL)
    {
      mpack->status      = mpack_read_status_receive_format;
      mpack->format      = 0;
      memzero(mpack->number_data, sizeof(mpack->number_data));
      mpack->number_need = 0;
      mpack->number_read = 0;
      mpack->binary_need = 0;
      mpack->binary_read = 0;
      r = true;
    }

    return r;
  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

///////////////////////////////////////////////////////////////////////////////////////////////////

int apilib::mpack_get_int_as_int08(mpack_read_t* mpack, int08_t* value)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL || value == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status != mpack_read_status_xchange_object)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    if(mpack->object.type == mpack_type_int)
    {
      int64_t i = mpack->object.i64;
      if(int08_min <= i && i <= int08_max)
      {
        *value = (int08_t)i;
        r = mpack_code_ok;
        break;
      }
      else
      {
        r = mpack_code_error_buffer_too_small;
        break;
      }
    }

    if(mpack->object.type == mpack_type_uint)
    {
      uint64_t u = mpack->object.u64;
      if(u <= int08_max)
      {
        *value = (int08_t)u;
        r = mpack_code_ok;
        break;
      }
      else
      {
        r = mpack_code_error_buffer_too_small;
        break;
      }
    }

    r = mpack_code_error_invalid_type;
  }
  while(0);

  return r;
}

int apilib::mpack_get_int_as_int16(mpack_read_t* mpack, int16_t* value)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL || value == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status != mpack_read_status_xchange_object)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    if(mpack->object.type == mpack_type_int)
    {
      int64_t i = mpack->object.i64;
      if(int16_min <= i && i <= int16_max)
      {
        *value = (int16_t)i;
        r = mpack_code_ok;
        break;
      }
      else
      {
        r = mpack_code_error_buffer_too_small;
        break;
      }
    }

    if(mpack->object.type == mpack_type_uint)
    {
      uint64_t u = mpack->object.u64;
      if(u <= int16_max)
      {
        *value = (int16_t)u;
        r = mpack_code_ok;
        break;
      }
      else
      {
        r = mpack_code_error_buffer_too_small;
        break;
      }
    }

    r = mpack_code_error_invalid_type;
  }
  while(0);

  return r;
}

int apilib::mpack_get_int_as_int32(mpack_read_t* mpack, int32_t* value)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL || value == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status != mpack_read_status_xchange_object)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    if(mpack->object.type == mpack_type_int)
    {
      int64_t i = mpack->object.i64;
      if(int32_min <= i && i <= int32_max)
      {
        *value = (int32_t)i;
        r = mpack_code_ok;
        break;
      }
      else
      {
        r = mpack_code_error_buffer_too_small;
        break;
      }
    }

    if(mpack->object.type == mpack_type_uint)
    {
      uint64_t u = mpack->object.u64;
      if(u <= int32_max)
      {
        *value = (int32_t)u;
        r = mpack_code_ok;
        break;
      }
      else
      {
        r = mpack_code_error_buffer_too_small;
        break;
      }
    }

    r = mpack_code_error_invalid_type;
  }
  while(0);

  return r;
}

int apilib::mpack_get_int_as_int64(mpack_read_t* mpack, int64_t* value)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL || value == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status != mpack_read_status_xchange_object)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    if(mpack->object.type == mpack_type_int)
    {
      int64_t i = mpack->object.i64;
      if(int64_min <= i && i <= int64_max)
      {
        *value = (int64_t)i;
        r = mpack_code_ok;
        break;
      }
      else
      {
        r = mpack_code_error_buffer_too_small;
        break;
      }
    }

    if(mpack->object.type == mpack_type_uint)
    {
      uint64_t u = mpack->object.u64;
      if(u <= int64_max)
      {
        *value = (int64_t)u;
        r = mpack_code_ok;
        break;
      }
      else
      {
        r = mpack_code_error_buffer_too_small;
        break;
      }
    }

    r = mpack_code_error_invalid_type;
  }
  while(0);

  return r;
}

int apilib::mpack_get_int_as_uint08(mpack_read_t* mpack, uint08_t* value)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL || value == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status != mpack_read_status_xchange_object)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    if(mpack->object.type == mpack_type_int)
    {
      int64_t i = mpack->object.i64;
      if(0 <= i && i <= uint08_max)
      {
        *value = (uint08_t)i;
        r = mpack_code_ok;
        break;
      }
      else
      {
        r = mpack_code_error_buffer_too_small;
        break;
      }
    }

    if(mpack->object.type == mpack_type_uint)
    {
      uint64_t u = mpack->object.u64;
      if(u <= uint08_max)
      {
        *value = (uint08_t)u;
        r = mpack_code_ok;
        break;
      }
      else
      {
        r = mpack_code_error_buffer_too_small;
        break;
      }
    }

    r = mpack_code_error_invalid_type;
  }
  while(0);

  return r;
}

int apilib::mpack_get_int_as_uint16(mpack_read_t* mpack, uint16_t* value)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL || value == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status != mpack_read_status_xchange_object)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    if(mpack->object.type == mpack_type_int)
    {
      int64_t i = mpack->object.i64;
      if(0 <= i && i <= uint16_max)
      {
        *value = (uint16_t)i;
        r = mpack_code_ok;
        break;
      }
      else
      {
        r = mpack_code_error_buffer_too_small;
        break;
      }
    }

    if(mpack->object.type == mpack_type_uint)
    {
      uint64_t u = mpack->object.u64;
      if(u <= uint16_max)
      {
        *value = (uint16_t)u;
        r = mpack_code_ok;
        break;
      }
      else
      {
        r = mpack_code_error_buffer_too_small;
        break;
      }
    }

    r = mpack_code_error_invalid_type;
  }
  while(0);

  return r;
}

int apilib::mpack_get_int_as_uint32(mpack_read_t* mpack, uint32_t* value)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL || value == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status != mpack_read_status_xchange_object)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    if(mpack->object.type == mpack_type_int)
    {
      int64_t i = mpack->object.i64;
      if(0 <= i && i <= uint32_max)
      {
        *value = (uint32_t)i;
        r = mpack_code_ok;
        break;
      }
      else
      {
        r = mpack_code_error_buffer_too_small;
        break;
      }
    }

    if(mpack->object.type == mpack_type_uint)
    {
      uint64_t u = mpack->object.u64;
      if(u <= uint32_max)
      {
        *value = (uint32_t)u;
        r = mpack_code_ok;
        break;
      }
      else
      {
        r = mpack_code_error_buffer_too_small;
        break;
      }
    }

    r = mpack_code_error_invalid_type;
  }
  while(0);

  return r;
}

int apilib::mpack_get_int_as_uint64(mpack_read_t* mpack, uint64_t* value)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL || value == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status != mpack_read_status_xchange_object)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    if(mpack->object.type == mpack_type_int)
    {
      int64_t i = mpack->object.i64;
      if(0 <= i && i <= uint64_max)
      {
        *value = i;
        r = mpack_code_ok;
        break;
      }
      else
      {
        r = mpack_code_error_buffer_too_small;
        break;
      }
    }

    if(mpack->object.type == mpack_type_uint)
    {
      uint64_t u = mpack->object.u64;
      if(u <= uint64_max)
      {
        *value = u;
        r = mpack_code_ok;
        break;
      }
      else
      {
        r = mpack_code_error_buffer_too_small;
        break;
      }
    }

    r = mpack_code_error_invalid_type;
  }
  while(0);

  return r;
}

int apilib::mpack_get_nil(mpack_read_t* mpack)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status != mpack_read_status_xchange_object)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    if(mpack->object.type == mpack_type_nil)
    {
      r = mpack_code_ok;
      break;
    }
    else
    {
      r = mpack_code_error_invalid_type;
      break;
    }
  }
  while(0);

  return r;
}

int apilib::mpack_get_bool(mpack_read_t* mpack, bool* value)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL || value == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status != mpack_read_status_xchange_object)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    if(mpack->object.type == mpack_type_bool)
    {
      *value = mpack->object.bln;
      r = mpack_code_ok;
      break;
    }
    else
    {
      r = mpack_code_error_invalid_type;
      break;
    }
  }
  while(0);

  return r;
}

int apilib::mpack_get_float_as_float32(mpack_read_t* mpack, float* value)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL || value == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status != mpack_read_status_xchange_object)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    if(mpack->object.type == mpack_type_float)
    {
      *value = mpack->object.flt;
      r = mpack_code_ok;
      break;
    }
    else
    {
      r = mpack_code_error_invalid_type;
      break;
    }
  }
  while(0);

  return r;
}

int apilib::mpack_get_float_as_float64(mpack_read_t* mpack, double* value)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL || value == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status != mpack_read_status_xchange_object)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    if(mpack->object.type == mpack_type_double)
    {
      *value = mpack->object.dbl;
      r = mpack_code_ok;
      break;
    }
    else
    {
      r = mpack_code_error_invalid_type;
      break;
    }
  }
  while(0);

  return r;
}

int apilib::mpack_get_str_info(mpack_read_t* mpack, uint32_t* size)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL || size == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status != mpack_read_status_xchange_object)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    if(mpack->object.type == mpack_type_str)
    {
      *size = mpack->object.str_size;
      r = mpack_code_ok;
      break;
    }
    else
    {
      r = mpack_code_error_invalid_type;
      break;
    }
  }
  while(0);

  return r;
}

int apilib::mpack_get_bin_info(mpack_read_t* mpack, uint32_t* size)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL || size == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status != mpack_read_status_xchange_object)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    if(mpack->object.type == mpack_type_bin)
    {
      *size = mpack->object.bin_size;
      r = mpack_code_ok;
      break;
    }
    else
    {
      r = mpack_code_error_invalid_type;
      break;
    }
  }
  while(0);

  return r;
}

int apilib::mpack_get_ext_info(mpack_read_t* mpack, uint32_t* size, uint08_t* type)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL || size == NULL || type == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status != mpack_read_status_xchange_object)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    if(mpack->object.type == mpack_type_ext)
    {
      *size = mpack->object.ext_size;
      *type = mpack->object.ext_type;
      r = mpack_code_ok;
      break;
    }
    else
    {
      r = mpack_code_error_invalid_type;
      break;
    }
  }
  while(0);

  return r;
}

int apilib::mpack_get_array_count(mpack_read_t* mpack, uint32_t* count)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL || count == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status != mpack_read_status_xchange_object)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    if(mpack->object.type == mpack_type_array)
    {
      *count = mpack->object.arr_count;
      r = mpack_code_ok;
      break;
    }
    else
    {
      r = mpack_code_error_invalid_type;
      break;
    }
  }
  while(0);

  return r;
}

int apilib::mpack_get_map_count(mpack_read_t* mpack, uint32_t* count)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL || count == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status != mpack_read_status_xchange_object)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    if(mpack->object.type == mpack_type_map)
    {
      *count = mpack->object.map_count;
      r = mpack_code_ok;
      break;
    }
    else
    {
      r = mpack_code_error_invalid_type;
      break;
    }
  }
  while(0);

  return r;
}

///////////////////////////////////////////////////////////////////////////////////////////////////