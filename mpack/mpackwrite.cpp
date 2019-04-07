#include "../cfg.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "../include/lib.h"
#include "../include/log.h"
#include "../include/mem.h"

#include "mpack.h"
#include "mpackwrite.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Запись построенного объекта в буфере.
   *
   * В случае неполной записи объекта требуется выполнить повторный вызов процедуры.
   *
   * @param mpack контекст записи.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки,
   *         mpack_code_partial_complete объект записан неполностью.
  */
  static int mpack_write_object_buffer(mpack_write_t* mpack)
  {
    int r = mpack_code_error_internal;

    do 
    {
      if(mpack == NULL)
      {
        r = mpack_code_error_invalid_argument;
        break;
      }

      if(mpack->fn_write == NULL)
      {
        r = mpack_code_error_invalid_callback;
        break;
      }

      if(mpack->status != mpack_write_status_write_buffer)
      {
        r = mpack_code_error_invalid_operation;
        break;
      }

      if(mpack->buffer_amount == mpack->buffer_offset)
      {
        mpack->status = (mpack->binary_amount != mpack->binary_offset) ? mpack_write_status_write_binary : mpack_write_status_build_buffer;
        r = mpack_code_ok;
        break;
      }

      if(mpack->buffer_amount > mpack->buffer_offset)
      {
        uint32_t buffer_size = mpack->buffer_amount - mpack->buffer_offset;
        uint32_t buffer_done = mpack->fn_write(mpack->fn_write_param, &mpack->buffer_data[mpack->buffer_offset], buffer_size);
        if(buffer_done > buffer_size)
        {
          mpack->status = mpack_write_status_fatal_error;
          r = mpack_code_error_internal;
          break;
        }
        mpack->buffer_offset += buffer_done;
      }

      if(mpack->buffer_amount == mpack->buffer_offset)
      {
        mpack->status = (mpack->binary_amount != mpack->binary_offset) ? mpack_write_status_write_binary : mpack_write_status_build_buffer;
        r = mpack_code_ok;
        break;
      }

      if(mpack->buffer_amount > mpack->buffer_offset)
      {
        r = mpack_code_partial_complete;
        break;
      }
    }
    while(0);

    return r;
  }

  /**
   * Запись построенного объекта в буфере.
   *
   * При неполной записи объекта требуется выполнять повторный вызов процедуры учитывая при этом
   * количество записанных данных.
   *
   * @param mpack   контекст записи.
   * @param data    бинарные данные.
   * @param size    размер бинарных данных в байтах.
   * @param written количество записанных бинарных данных в байтах.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки,
   *         mpack_code_partial_complete объект записан неполностью.
  */
  static int mpack_write_object_binary(mpack_write_t* mpack, const void* data, uint32_t size, uint32_t* written)
  {
    int r = mpack_code_error_internal;

    do 
    {
      if(mpack == NULL)
      {
        r = mpack_code_error_invalid_argument;
        break;
      }

      if(mpack->fn_write == NULL)
      {
        r = mpack_code_error_invalid_callback;
        break;
      }

      if(mpack->status != mpack_write_status_write_binary)
      {
        r = mpack_code_error_invalid_operation;
        break;
      }

      if(mpack->binary_amount == mpack->binary_offset)
      {
        mpack->status = mpack_write_status_build_buffer;
        r = mpack_code_ok;
        break;
      }

      if(mpack->binary_amount > mpack->binary_offset)
      {
        uint32_t binary_size = 0;
        uint32_t binary_done = 0;

        if(data != NULL && size != 0)
        {
          binary_size = mpack->binary_amount - mpack->binary_offset;
          binary_done = mpack->fn_write(mpack->fn_write_param, data, ((binary_size < size) ? binary_size : size));
        }

        if(binary_done > binary_size)
        {
          mpack->status = mpack_write_status_fatal_error;
          r = mpack_code_error_internal;
          break;
        }

        if(written != NULL)
        {
          *written = binary_done;
        }

        mpack->binary_offset += binary_done;
      }

      if(mpack->binary_amount == mpack->binary_offset)
      {
        mpack->status = mpack_write_status_build_buffer;
        r = mpack_code_ok;
        break;
      }

      if(mpack->binary_amount > mpack->binary_offset)
      {
        r = mpack_code_partial_complete;
        break;
      }
    }
    while(0);

    return r;
  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

///////////////////////////////////////////////////////////////////////////////////////////////////

int apilib::mpack_reset_write_context(mpack_write_t* mpack)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status == mpack_write_status_fatal_error)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    if((mpack->buffer_offset != 0 && mpack->buffer_offset != mpack->buffer_amount) ||
       (mpack->binary_offset != 0 && mpack->binary_offset != mpack->binary_amount))
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    mpack->status        = mpack_write_status_build_buffer;
    mpack->buffer_offset = 0;
    mpack->buffer_amount = 0;
    mpack->binary_offset = 0;
    mpack->binary_amount = 0;
    r = mpack_code_ok;
  }
  while(0);

  return r;
}

int apilib::mpack_write_object(mpack_write_t* mpack, const void* data, uint32_t size, uint32_t* written)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->fn_write == NULL)
    {
      r = mpack_code_error_invalid_callback;
      break;
    }

    if(mpack->status != mpack_write_status_write_buffer &&
       mpack->status != mpack_write_status_write_binary)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    if(mpack->status == mpack_write_status_write_buffer)
    {
      r = mpack_write_object_buffer(mpack);
      if(r != mpack_code_ok)break;
    }

    if(mpack->status == mpack_write_status_write_binary)
    {
      r = mpack_write_object_binary(mpack, data, size, written);
      if(r != mpack_code_ok)break;
    }
  }
  while(0);

  return r;
}

int apilib::mpack_build_int_from_int08(mpack_write_t* mpack, int08_t value)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status != mpack_write_status_build_buffer)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    if(-32 <= value && value <= 127)
    {
      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = (uint08_t)value;
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1;
      r = mpack_code_ok;
      break;
    }

  //if(int08_min <= value && value <= int08_max)
    {
      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_int8;
      mpack->buffer_data[1] = (uint08_t)value;
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 1;
      r = mpack_code_ok;
      break;
    }
  }
  while(0);

  return r;
}

int apilib::mpack_build_int_from_int16(mpack_write_t* mpack, int16_t value)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status != mpack_write_status_build_buffer)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    if(-32 <= value && value <= 127)
    {
      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = (uint08_t)value;
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1;
      r = mpack_code_ok;
      break;
    }

    if(int08_min <= value && value <= int08_max)
    {
      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_int8;
      mpack->buffer_data[1] = (uint08_t)value;
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 1;
      r = mpack_code_ok;
      break;
    }

  //if(int16_min <= value && value <= int16_max)
    {
      int16_t temp = value;

      if((mpack->endian == mpack_endian_unknown) ||
         (mpack->endian == mpack_endian_little && mpack_swap16(&temp) == false))
      {
        r = mpack_code_error_internal;
        break;
      }

      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_int16;
      mpack->buffer_data[1] = (uint08_t)((uint08_t*)&temp)[0];
      mpack->buffer_data[2] = (uint08_t)((uint08_t*)&temp)[1];
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 2;

      r = mpack_code_ok;
      break;
    }
  }
  while(0);

  return r;
}

int apilib::mpack_build_int_from_int32(mpack_write_t* mpack, int32_t value)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status != mpack_write_status_build_buffer)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    if(-32 <= value && value <= 127)
    {
      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = (uint08_t)value;
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1;
      r = mpack_code_ok;
      break;
    }

    if(int08_min <= value && value <= int08_max)
    {
      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_int8;
      mpack->buffer_data[1] = (uint08_t)value;
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 1;
      r = mpack_code_ok;
      break;
    }

    if(int16_min <= value && value <= int16_max)
    {
      int16_t temp = (int16_t)value;

      if((mpack->endian == mpack_endian_unknown) ||
         (mpack->endian == mpack_endian_little && mpack_swap16(&temp) == false))
      {
        r = mpack_code_error_internal;
        break;
      }

      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_int16;
      mpack->buffer_data[1] = (uint08_t)((uint08_t*)&temp)[0];
      mpack->buffer_data[2] = (uint08_t)((uint08_t*)&temp)[1];
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 2;

      r = mpack_code_ok;
      break;
    }

  //if(int32_min <= value && value <= int32_max)
    {
      int32_t temp = value;

      if((mpack->endian == mpack_endian_unknown) ||
         (mpack->endian == mpack_endian_little && mpack_swap32(&temp) == false))
      {
        r = mpack_code_error_internal;
        break;
      }

      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_int32;
      mpack->buffer_data[1] = (uint08_t)((uint08_t*)&temp)[0];
      mpack->buffer_data[2] = (uint08_t)((uint08_t*)&temp)[1];
      mpack->buffer_data[3] = (uint08_t)((uint08_t*)&temp)[2];
      mpack->buffer_data[4] = (uint08_t)((uint08_t*)&temp)[3];
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 4;

      r = mpack_code_ok;
      break;
    }
  }
  while(0);

  return r;
}

int apilib::mpack_build_int_from_int64(mpack_write_t* mpack, int64_t value)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status != mpack_write_status_build_buffer)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    if(-32 <= value && value <= 127)
    {
      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = (uint08_t)value;
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1;
      r = mpack_code_ok;
      break;
    }

    if(int08_min <= value && value <= int08_max)
    {
      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_int8;
      mpack->buffer_data[1] = (uint08_t)value;
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 1;
      r = mpack_code_ok;
      break;
    }

    if(int16_min <= value && value <= int16_max)
    {
      int16_t temp = (int16_t)value;

      if((mpack->endian == mpack_endian_unknown) ||
         (mpack->endian == mpack_endian_little && mpack_swap16(&temp) == false))
      {
        r = mpack_code_error_internal;
        break;
      }

      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_int16;
      mpack->buffer_data[1] = (uint08_t)((uint08_t*)&temp)[0];
      mpack->buffer_data[2] = (uint08_t)((uint08_t*)&temp)[1];
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 2;

      r = mpack_code_ok;
      break;
    }

    if(int32_min <= value && value <= int32_max)
    {
      int32_t temp = (int32_t)value;

      if((mpack->endian == mpack_endian_unknown) ||
         (mpack->endian == mpack_endian_little && mpack_swap32(&temp) == false))
      {
        r = mpack_code_error_internal;
        break;
      }

      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_int32;
      mpack->buffer_data[1] = (uint08_t)((uint08_t*)&temp)[0];
      mpack->buffer_data[2] = (uint08_t)((uint08_t*)&temp)[1];
      mpack->buffer_data[3] = (uint08_t)((uint08_t*)&temp)[2];
      mpack->buffer_data[4] = (uint08_t)((uint08_t*)&temp)[3];
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 4;

      r = mpack_code_ok;
      break;
    }

  //if(int64_min <= value && value <= int64_max)
    {
      int64_t temp = value;

      if((mpack->endian == mpack_endian_unknown) ||
         (mpack->endian == mpack_endian_little && mpack_swap64(&temp) == false))
      {
        r = mpack_code_error_internal;
        break;
      }

      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_int64;
      mpack->buffer_data[1] = (uint08_t)((uint08_t*)&temp)[0];
      mpack->buffer_data[2] = (uint08_t)((uint08_t*)&temp)[1];
      mpack->buffer_data[3] = (uint08_t)((uint08_t*)&temp)[2];
      mpack->buffer_data[4] = (uint08_t)((uint08_t*)&temp)[3];
      mpack->buffer_data[5] = (uint08_t)((uint08_t*)&temp)[4];
      mpack->buffer_data[6] = (uint08_t)((uint08_t*)&temp)[5];
      mpack->buffer_data[7] = (uint08_t)((uint08_t*)&temp)[6];
      mpack->buffer_data[8] = (uint08_t)((uint08_t*)&temp)[7];
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 8;

      r = mpack_code_ok;
      break;
    }
  }
  while(0);

  return r;
}

int apilib::mpack_build_int_from_uint08(mpack_write_t* mpack, uint08_t value)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status != mpack_write_status_build_buffer)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    if(value <= 127)
    {
      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = (uint08_t)value;
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1;
      r = mpack_code_ok;
      break;
    }

  //if(value <= uint08_max)
    {
      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_uint8;
      mpack->buffer_data[1] = (uint08_t)value;
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 1;
      r = mpack_code_ok;
      break;
    }
  }
  while(0);

  return r;
}

int apilib::mpack_build_int_from_uint16(mpack_write_t* mpack, uint16_t value)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status != mpack_write_status_build_buffer)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    if(value <= 127)
    {
      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = (uint08_t)value;
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1;
      r = mpack_code_ok;
      break;
    }

    if(value <= uint08_max)
    {
      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_uint8;
      mpack->buffer_data[1] = (uint08_t)value;
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 1;
      r = mpack_code_ok;
      break;
    }

  //if(value <= uint16_max)
    {
      uint16_t temp = value;

      if((mpack->endian == mpack_endian_unknown) ||
         (mpack->endian == mpack_endian_little && mpack_swap16(&temp) == false))
      {
        r = mpack_code_error_internal;
        break;
      }

      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_uint16;
      mpack->buffer_data[1] = (uint08_t)((uint08_t*)&temp)[0];
      mpack->buffer_data[2] = (uint08_t)((uint08_t*)&temp)[1];
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 2;

      r = mpack_code_ok;
      break;
    }
  }
  while(0);

  return r;
}

int apilib::mpack_build_int_from_uint32(mpack_write_t* mpack, uint32_t value)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status != mpack_write_status_build_buffer)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    if(value <= 127)
    {
      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = (uint08_t)value;
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1;
      r = mpack_code_ok;
      break;
    }

    if(value <= uint08_max)
    {
      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_uint8;
      mpack->buffer_data[1] = (uint08_t)value;
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 1;
      r = mpack_code_ok;
      break;
    }

    if(value <= uint16_max)
    {
      uint16_t temp = (uint16_t)value;

      if((mpack->endian == mpack_endian_unknown) ||
         (mpack->endian == mpack_endian_little && mpack_swap16(&temp) == false))
      {
        r = mpack_code_error_internal;
        break;
      }

      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_uint16;
      mpack->buffer_data[1] = (uint08_t)((uint08_t*)&temp)[0];
      mpack->buffer_data[2] = (uint08_t)((uint08_t*)&temp)[1];
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 2;

      r = mpack_code_ok;
      break;
    }

  //if(value <= uint32_max)
    {
      uint32_t temp = value;

      if((mpack->endian == mpack_endian_unknown) ||
         (mpack->endian == mpack_endian_little && mpack_swap32(&temp) == false))
      {
        r = mpack_code_error_internal;
        break;
      }

      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_uint32;
      mpack->buffer_data[1] = (uint08_t)((uint08_t*)&temp)[0];
      mpack->buffer_data[2] = (uint08_t)((uint08_t*)&temp)[1];
      mpack->buffer_data[3] = (uint08_t)((uint08_t*)&temp)[2];
      mpack->buffer_data[4] = (uint08_t)((uint08_t*)&temp)[3];
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 4;

      r = mpack_code_ok;
      break;
    }
  }
  while(0);

  return r;
}

int apilib::mpack_build_int_from_uint64(mpack_write_t* mpack, uint64_t value)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status != mpack_write_status_build_buffer)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    if(value <= 127)
    {
      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = (uint08_t)value;
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1;
      r = mpack_code_ok;
      break;
    }

    if(value <= uint08_max)
    {
      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_uint8;
      mpack->buffer_data[1] = (uint08_t)value;
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 1;
      r = mpack_code_ok;
      break;
    }

    if(value <= uint16_max)
    {
      uint16_t temp = (uint16_t)value;

      if((mpack->endian == mpack_endian_unknown) ||
         (mpack->endian == mpack_endian_little && mpack_swap16(&temp) == false))
      {
        r = mpack_code_error_internal;
        break;
      }
     
      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_uint16;
      mpack->buffer_data[1] = (uint08_t)((uint08_t*)&temp)[0];
      mpack->buffer_data[2] = (uint08_t)((uint08_t*)&temp)[1];
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 2;

      r = mpack_code_ok;
      break;
    }

    if(value <= uint32_max)
    {
      uint32_t temp = (uint32_t)value;

      if((mpack->endian == mpack_endian_unknown) ||
         (mpack->endian == mpack_endian_little && mpack_swap32(&temp) == false))
      {
        r = mpack_code_error_internal;
        break;
      }
      
      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_uint32;
      mpack->buffer_data[1] = (uint08_t)((uint08_t*)&temp)[0];
      mpack->buffer_data[2] = (uint08_t)((uint08_t*)&temp)[1];
      mpack->buffer_data[3] = (uint08_t)((uint08_t*)&temp)[2];
      mpack->buffer_data[4] = (uint08_t)((uint08_t*)&temp)[3];
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 4;

      r = mpack_code_ok;
      break;
    }

  //if(value <= uint64_max)
    {
      uint64_t temp = value;

      if((mpack->endian == mpack_endian_unknown) ||
         (mpack->endian == mpack_endian_little && mpack_swap64(&temp) == false))
      {
        r = mpack_code_error_internal;
        break;
      }

      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_uint64;
      mpack->buffer_data[1] = (uint08_t)((uint08_t*)&temp)[0];
      mpack->buffer_data[2] = (uint08_t)((uint08_t*)&temp)[1];
      mpack->buffer_data[3] = (uint08_t)((uint08_t*)&temp)[2];
      mpack->buffer_data[4] = (uint08_t)((uint08_t*)&temp)[3];
      mpack->buffer_data[5] = (uint08_t)((uint08_t*)&temp)[4];
      mpack->buffer_data[6] = (uint08_t)((uint08_t*)&temp)[5];
      mpack->buffer_data[7] = (uint08_t)((uint08_t*)&temp)[6];
      mpack->buffer_data[8] = (uint08_t)((uint08_t*)&temp)[7];
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 8;

      r = mpack_code_ok;
      break;
    }
  }
  while(0);

  return r;
}

int apilib::mpack_build_nil(mpack_write_t* mpack)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status != mpack_write_status_build_buffer)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    mpack->status         = mpack_write_status_write_buffer;
    mpack->buffer_data[0] = mpack_format_nil;
    mpack->buffer_offset  = 0;
    mpack->buffer_amount  = 1;
    r = mpack_code_ok;
    break;
  }
  while(0);

  return r;
}

int apilib::mpack_build_bool(mpack_write_t* mpack, bool value)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status != mpack_write_status_build_buffer)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    if(value == true)
    {
      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_true;
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1;
      r = mpack_code_ok;
      break;
    }

    if(value == false)
    {
      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_false;
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1;
      r = mpack_code_ok;
      break;
    }
  }
  while(0);

  return r;
}

int apilib::mpack_build_float_as_float32(mpack_write_t* mpack, float value)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status != mpack_write_status_build_buffer)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    float temp = value;

    if((mpack->endian == mpack_endian_unknown) ||
       (mpack->endian == mpack_endian_little && mpack_swap32(&temp) == false))
    {
      r = mpack_code_error_internal;
      break;
    }

    mpack->status         = mpack_write_status_write_buffer;
    mpack->buffer_data[0] = mpack_format_float32;
    mpack->buffer_data[1] = (uint08_t)((uint08_t*)&temp)[0];
    mpack->buffer_data[2] = (uint08_t)((uint08_t*)&temp)[1];
    mpack->buffer_data[3] = (uint08_t)((uint08_t*)&temp)[2];
    mpack->buffer_data[4] = (uint08_t)((uint08_t*)&temp)[3];
    mpack->buffer_offset  = 0;
    mpack->buffer_amount  = 1 + 4;

    r = mpack_code_ok;
    break;
  }
  while(0);

  return r;
}

int apilib::mpack_build_float_as_float64(mpack_write_t* mpack, double value)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status != mpack_write_status_build_buffer)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    double temp = value;

    if((mpack->endian == mpack_endian_unknown) ||
       (mpack->endian == mpack_endian_little && mpack_swap64(&temp) == false))
    {
      r = mpack_code_error_internal;
      break;
    }

    mpack->status         = mpack_write_status_write_buffer;
    mpack->buffer_data[0] = mpack_format_float64;
    mpack->buffer_data[1] = (uint08_t)((uint08_t*)&temp)[0];
    mpack->buffer_data[2] = (uint08_t)((uint08_t*)&temp)[1];
    mpack->buffer_data[3] = (uint08_t)((uint08_t*)&temp)[2];
    mpack->buffer_data[4] = (uint08_t)((uint08_t*)&temp)[3];
    mpack->buffer_data[5] = (uint08_t)((uint08_t*)&temp)[4];
    mpack->buffer_data[6] = (uint08_t)((uint08_t*)&temp)[5];
    mpack->buffer_data[7] = (uint08_t)((uint08_t*)&temp)[6];
    mpack->buffer_data[8] = (uint08_t)((uint08_t*)&temp)[7];
    mpack->buffer_offset  = 0;
    mpack->buffer_amount  = 1 + 8;

    r = mpack_code_ok;
    break;
  }
  while(0);

  return r;
}

int apilib::mpack_build_str_info(mpack_write_t* mpack, uint32_t size)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status != mpack_write_status_build_buffer)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    if(size <= 0x0f)
    {
      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = (uint08_t)(mpack_format_fixstr) | (uint08_t)(size & 0x0f);
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1;
      mpack->binary_offset  = 0;
      mpack->binary_amount  = size;
      r = mpack_code_ok;
      break;
    }

    if(size <= uint08_max)
    {
      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_str8;
      mpack->buffer_data[1] = (uint08_t)size;
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 1;
      mpack->binary_offset  = 0;
      mpack->binary_amount  = size;
      r = mpack_code_ok;
      break;
    }

    if(size <= uint16_max)
    {
      uint16_t temp = (uint16_t)size;

      if((mpack->endian == mpack_endian_unknown) ||
         (mpack->endian == mpack_endian_little && mpack_swap16(&temp) == false))
      {
        r = mpack_code_error_internal;
        break;
      }

      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_str16;
      mpack->buffer_data[1] = (uint08_t)((uint08_t*)&temp)[0];
      mpack->buffer_data[2] = (uint08_t)((uint08_t*)&temp)[1];
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 2;
      mpack->binary_offset  = 0;
      mpack->binary_amount  = size;

      r = mpack_code_ok;
      break;
    }

    if(size <= uint32_max)
    {
      uint32_t temp = size;

      if((mpack->endian == mpack_endian_unknown) ||
         (mpack->endian == mpack_endian_little && mpack_swap32(&temp) == false))
      {
        r = mpack_code_error_internal;
        break;
      }

      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_str32;
      mpack->buffer_data[1] = (uint08_t)((uint08_t*)&temp)[0];
      mpack->buffer_data[2] = (uint08_t)((uint08_t*)&temp)[1];
      mpack->buffer_data[3] = (uint08_t)((uint08_t*)&temp)[2];
      mpack->buffer_data[4] = (uint08_t)((uint08_t*)&temp)[3];
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 4;
      mpack->binary_offset  = 0;
      mpack->binary_amount  = size;

      r = mpack_code_ok;
      break;
    }
  } 
  while(0);

  return r;
}

int apilib::mpack_build_bin_info(mpack_write_t* mpack, uint32_t size)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status != mpack_write_status_build_buffer)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    if(size <= uint08_max)
    {
      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_bin8;
      mpack->buffer_data[1] = (uint08_t)size;
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 1;
      mpack->binary_offset  = 0;
      mpack->binary_amount  = size;
      r = mpack_code_ok;
      break;
    }

    if(size <= uint16_max)
    {
      uint16_t temp = (uint16_t)size;

      if((mpack->endian == mpack_endian_unknown) ||
         (mpack->endian == mpack_endian_little && mpack_swap16(&temp) == false))
      {
        r = mpack_code_error_internal;
        break;
      }

      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_bin16;
      mpack->buffer_data[1] = (uint08_t)((uint08_t*)&temp)[0];
      mpack->buffer_data[2] = (uint08_t)((uint08_t*)&temp)[1];
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 2;
      mpack->binary_offset  = 0;
      mpack->binary_amount  = size;

      r = mpack_code_ok;
      break;
    }

    if(size <= uint32_max)
    {
      uint32_t temp = size;

      if((mpack->endian == mpack_endian_unknown) ||
         (mpack->endian == mpack_endian_little && mpack_swap32(&temp) == false))
      {
        r = mpack_code_error_internal;
        break;
      }

      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_bin32;
      mpack->buffer_data[1] = (uint08_t)((uint08_t*)&temp)[0];
      mpack->buffer_data[2] = (uint08_t)((uint08_t*)&temp)[1];
      mpack->buffer_data[3] = (uint08_t)((uint08_t*)&temp)[2];
      mpack->buffer_data[4] = (uint08_t)((uint08_t*)&temp)[3];
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 4;
      mpack->binary_offset  = 0;
      mpack->binary_amount  = size;

      r = mpack_code_ok;
      break;
    }
  }
  while(0);

  return r;
}

int apilib::mpack_build_ext_info(mpack_write_t* mpack, uint32_t size, uint08_t type)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status != mpack_write_status_build_buffer)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    if(size == 1)
    {
      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_fixext1;
      mpack->buffer_data[1] = type;
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 1;
      mpack->binary_offset  = 0;
      mpack->binary_amount  = size;
      r = mpack_code_ok;
      break;
    }

    if(size == 2)
    {
      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_fixext2;
      mpack->buffer_data[1] = type;
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 1;
      mpack->binary_offset  = 0;
      mpack->binary_amount  = size;
      r = mpack_code_ok;
      break;
    }

    if(size == 4)
    {
      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_fixext4;
      mpack->buffer_data[1] = type;
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 1;
      mpack->binary_offset  = 0;
      mpack->binary_amount  = size;
      r = mpack_code_ok;
      break;
    }

    if(size == 8)
    {
      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_fixext8;
      mpack->buffer_data[1] = type;
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 1;
      mpack->binary_offset  = 0;
      mpack->binary_amount  = size;
      r = mpack_code_ok;
      break;
    }

    if(size == 16)
    {
      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_fixext16;
      mpack->buffer_data[1] = type;
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 1;
      mpack->binary_offset  = 0;
      mpack->binary_amount  = size;
      r = mpack_code_ok;
      break;
    }

    if(size <= uint08_max)
    {
      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_ext8;
      mpack->buffer_data[1] = (uint08_t)size;
      mpack->buffer_data[2] = type;
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 1 + 1;
      mpack->binary_offset  = 0;
      mpack->binary_amount  = size;
      r = mpack_code_ok;
      break;
    }

    if(size <= uint16_max)
    {
      uint16_t temp = (uint16_t)size;

      if((mpack->endian == mpack_endian_unknown) ||
         (mpack->endian == mpack_endian_little && mpack_swap16(&temp) == false))
      {
        r = mpack_code_error_internal;
        break;
      }

      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_ext16;
      mpack->buffer_data[1] = (uint08_t)((uint08_t*)&temp)[0];
      mpack->buffer_data[2] = (uint08_t)((uint08_t*)&temp)[1];
      mpack->buffer_data[3] = type;
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 2 + 1;
      mpack->binary_offset  = 0;
      mpack->binary_amount  = size;

      r = mpack_code_ok;
      break;
    }

    if(size <= uint32_max)
    {
      uint32_t temp = size;

      if((mpack->endian == mpack_endian_unknown) ||
         (mpack->endian == mpack_endian_little && mpack_swap32(&temp) == false))
      {
        r = mpack_code_error_internal;
        break;
      }

      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_ext32;
      mpack->buffer_data[1] = (uint08_t)((uint08_t*)&temp)[0];
      mpack->buffer_data[2] = (uint08_t)((uint08_t*)&temp)[1];
      mpack->buffer_data[3] = (uint08_t)((uint08_t*)&temp)[2];
      mpack->buffer_data[4] = (uint08_t)((uint08_t*)&temp)[3];
      mpack->buffer_data[5] = type;
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 4 + 1;
      mpack->binary_offset  = 0;
      mpack->binary_amount  = size;

      r = mpack_code_ok;
      break;
    }
  }
  while(0);

  return r;
}

int apilib::mpack_build_array(mpack_write_t* mpack, uint32_t count)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status != mpack_write_status_build_buffer)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    if(count <= 0x0f)
    {
      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = (uint08_t)(mpack_format_fixarray) | (uint08_t)(count & 0x0f);
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1;
      r = mpack_code_ok;
      break;
    }

    if(count <= uint16_max)
    {
      uint16_t temp = (uint16_t)count;

      if((mpack->endian == mpack_endian_unknown) ||
         (mpack->endian == mpack_endian_little && mpack_swap16(&temp) == false))
      {
        r = mpack_code_error_internal;
        break;
      }

      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_array16;
      mpack->buffer_data[1] = (uint08_t)((uint08_t*)&temp)[0];
      mpack->buffer_data[2] = (uint08_t)((uint08_t*)&temp)[1];
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 2;

      r = mpack_code_ok;
      break;
    }

    if(count <= uint32_max)
    {
      uint32_t temp = count;

      if((mpack->endian == mpack_endian_unknown) ||
         (mpack->endian == mpack_endian_little && mpack_swap32(&temp) == false))
      {
        r = mpack_code_error_internal;
        break;
      }

      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_array32;
      mpack->buffer_data[1] = (uint08_t)((uint08_t*)&temp)[0];
      mpack->buffer_data[2] = (uint08_t)((uint08_t*)&temp)[1];
      mpack->buffer_data[3] = (uint08_t)((uint08_t*)&temp)[2];
      mpack->buffer_data[4] = (uint08_t)((uint08_t*)&temp)[3];
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 4;

      r = mpack_code_ok;
      break;
    }
  }
  while(0);

  return r;
}

int apilib::mpack_build_map(mpack_write_t* mpack, uint32_t count)
{
  int r = mpack_code_error_internal;

  do 
  {
    if(mpack == NULL)
    {
      r = mpack_code_error_invalid_argument;
      break;
    }

    if(mpack->status != mpack_write_status_build_buffer)
    {
      r = mpack_code_error_invalid_operation;
      break;
    }

    if(count <= 0x0f)
    {
      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = (uint08_t)(mpack_format_fixmap) | (uint08_t)(count & 0x0f);
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1;
      r = mpack_code_ok;
      break;
    }

    if(count <= uint16_max)
    {
      uint16_t temp = (uint16_t)count;

      if((mpack->endian == mpack_endian_unknown) ||
         (mpack->endian == mpack_endian_little && mpack_swap16(&temp) == false))
      {
        r = mpack_code_error_internal;
        break;
      }

      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_map16;
      mpack->buffer_data[1] = (uint08_t)((uint08_t*)&temp)[0];
      mpack->buffer_data[2] = (uint08_t)((uint08_t*)&temp)[1];
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 2;

      r = mpack_code_ok;
      break;
    }

    if(count <= uint32_max)
    {
      uint32_t temp = count;

      if((mpack->endian == mpack_endian_unknown) ||
         (mpack->endian == mpack_endian_little && mpack_swap32(&temp) == false))
      {
        r = mpack_code_error_internal;
        break;
      }

      mpack->status         = mpack_write_status_write_buffer;
      mpack->buffer_data[0] = mpack_format_map32;
      mpack->buffer_data[1] = (uint08_t)((uint08_t*)&temp)[0];
      mpack->buffer_data[2] = (uint08_t)((uint08_t*)&temp)[1];
      mpack->buffer_data[3] = (uint08_t)((uint08_t*)&temp)[2];
      mpack->buffer_data[4] = (uint08_t)((uint08_t*)&temp)[3];
      mpack->buffer_offset  = 0;
      mpack->buffer_amount  = 1 + 4;

      r = mpack_code_ok;
      break;
    }
  }
  while(0);

  return r;
}

int apilib::mpack_write_int_from_int08(mpack_write_t* mpack, int08_t value)
{
  int r = mpack_code_error_internal;

  do 
  {
    r = mpack_build_int_from_int08(mpack, value);
    if( r != mpack_code_ok )break;;

    r = mpack_write_object_buffer(mpack);
    if( r != mpack_code_ok )break;
  }
  while(0);

  return r;
}

int apilib::mpack_write_int_from_int16(mpack_write_t* mpack, int16_t value)
{
  int r = mpack_code_error_internal;

  do 
  {
    r = mpack_build_int_from_int16(mpack, value);
    if(r != mpack_code_ok)break;
    
    r = mpack_write_object_buffer(mpack);
    if(r != mpack_code_ok)break;
  }
  while(0);

  return r;
}

int apilib::mpack_write_int_from_int32(mpack_write_t* mpack, int32_t value)
{
  int r = mpack_code_error_internal;

  do 
  {
    r = mpack_build_int_from_int32(mpack, value);
    if(r != mpack_code_ok)break;

    r = mpack_write_object_buffer(mpack);
    if(r != mpack_code_ok)break;
  }
  while(0);

  return r;
}

int apilib::mpack_write_int_from_int64(mpack_write_t* mpack, int64_t value)
{
  int r = mpack_code_error_internal;

  do 
  {
    r = mpack_build_int_from_int64(mpack, value);
    if(r != mpack_code_ok)break;

    r = mpack_write_object_buffer(mpack);
    if(r != mpack_code_ok)break;
  }
  while(0);

  return r;
}

int apilib::mpack_write_int_from_uint08(mpack_write_t* mpack, uint08_t value)
{
  int r = mpack_code_error_internal;

  do 
  {
    r = mpack_build_int_from_uint08(mpack, value);
    if(r != mpack_code_ok)break;

    r = mpack_write_object_buffer(mpack);
    if(r != mpack_code_ok)break;
  }
  while(0);

  return r;
}

int apilib::mpack_write_int_from_uint16(mpack_write_t* mpack, uint16_t value)
{
  int r = mpack_code_error_internal;

  do 
  {
    r = mpack_build_int_from_uint16(mpack, value);
    if(r != mpack_code_ok)break;

    r = mpack_write_object_buffer(mpack);
    if(r != mpack_code_ok)break;
  }
  while(0);

  return r;
}
int apilib::mpack_write_int_from_uint32(mpack_write_t* mpack, uint32_t value)
{
  int r = mpack_code_error_internal;

  do 
  {
    r = mpack_build_int_from_uint32(mpack, value);
    if(r != mpack_code_ok)break;

    r = mpack_write_object_buffer(mpack);
    if(r != mpack_code_ok)break;
  }
  while(0);

  return r;
}

int apilib::mpack_write_int_from_uint64(mpack_write_t* mpack, uint64_t value)
{
  int r = mpack_code_error_internal;

  do 
  {
    r = mpack_build_int_from_uint64(mpack, value);
    if(r != mpack_code_ok)break;

    r = mpack_write_object_buffer(mpack);
    if(r != mpack_code_ok)break;
  }
  while(0);

  return r;
}

int apilib::mpack_write_nil(mpack_write_t* mpack)
{
  int r = mpack_code_error_internal;

  do 
  {
    r = mpack_build_nil(mpack);
    if(r != mpack_code_ok)break;

    r = mpack_write_object_buffer(mpack);
    if(r != mpack_code_ok)break;
  }
  while(0);

  return r;
}

int apilib::mpack_write_bool(mpack_write_t* mpack, bool value)
{
  int r = mpack_code_error_internal;

  do 
  {
    r = mpack_build_bool(mpack, value);
    if(r != mpack_code_ok)break;

    r = mpack_write_object_buffer(mpack);
    if(r != mpack_code_ok)break;
  }
  while(0);

  return r;
}

int apilib::mpack_write_float_from_float32(mpack_write_t* mpack, float value)
{
  int r = mpack_code_error_internal;

  do 
  {
    r = mpack_build_float_as_float32(mpack, value);
    if(r != mpack_code_ok)break;
    
    r = mpack_write_object_buffer(mpack);
    if(r != mpack_code_ok)break;
  }
  while(0);

  return r;
}

int apilib::mpack_write_float_from_float64(mpack_write_t* mpack, double value)
{
  int r = mpack_code_error_internal;

  do 
  {
    r = mpack_build_float_as_float64(mpack, value);
    if(r != mpack_code_ok)break;

    r = mpack_write_object_buffer(mpack);
    if(r != mpack_code_ok)break;
  }
  while(0);

  return r;
}

int apilib::mpack_write_str(mpack_write_t* mpack, const void* data, uint32_t size, uint32_t* written)
{
  int r = mpack_code_error_internal;

  do 
  {
    r = mpack_build_str_info(mpack, size);
    if(r != mpack_code_ok)break;

    r = mpack_write_object_buffer(mpack);
    if(r != mpack_code_ok)break;

    r = mpack_write_object_binary(mpack, data, size, written);
    if(r != mpack_code_ok)break;
  }
  while(0);

  return r;
}

int apilib::mpack_write_bin(mpack_write_t* mpack, const void* data, uint32_t size, uint32_t* written)
{
  int r = mpack_code_error_internal;

  do 
  {
    r = mpack_build_bin_info(mpack, size);
    if(r != mpack_code_ok)break;

    r = mpack_write_object_buffer(mpack);
    if(r != mpack_code_ok)break;

    r = mpack_write_object_binary(mpack, data, size, written);
    if(r != mpack_code_ok)break;
  }
  while(0);

  return r;
}

int apilib::mpack_write_ext(mpack_write_t* mpack, const void* data, uint32_t size, uint08_t type, uint32_t* written)
{
  int r = mpack_code_error_internal;

  do 
  {
    r = mpack_build_ext_info(mpack, size, type);
    if(r != mpack_code_ok)break;
    
    r = mpack_write_object_buffer(mpack);
    if(r != mpack_code_ok)break;

    r = mpack_write_object_binary(mpack, data, size, written);
    if(r != mpack_code_ok)break;
  }
  while(0);

  return r;
}

int apilib::mpack_write_array(mpack_write_t* mpack, uint32_t count)
{
  int r = mpack_code_error_internal;

  do 
  {
    r = mpack_build_array(mpack, count);
    if(r != mpack_code_ok)break;

    r = mpack_write_object_buffer(mpack);
    if(r != mpack_code_ok)break;
  }
  while(0);

  return r;
}

int apilib::mpack_write_map(mpack_write_t* mpack, uint32_t count)
{
  int r = mpack_code_error_internal;

  do 
  {
    r = mpack_build_map(mpack, count);
    if(r != mpack_code_ok)break;

    r = mpack_write_object_buffer(mpack);
    if(r != mpack_code_ok)break;
  }
  while(0);

  return r;
}

///////////////////////////////////////////////////////////////////////////////////////////////////