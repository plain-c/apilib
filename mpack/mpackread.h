#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Статус контекста чтения данных.
  */
  enum mpack_read_status_t
  {
    mpack_read_status_receive_format = 0x00, // Получение формата представления данных.
    mpack_read_status_process_format = 0x01, // Обработка формата представления данных.
    mpack_read_status_receive_number = 0x02, // Получение числовых данных.
    mpack_read_status_process_number = 0x03, // Обработка числовых данных.
    mpack_read_status_xchange_object = 0x04, // Обмен информацией и бинарными данными объекта.
  };

  /**
   * Прототип процедуры чтения данных.
   *
   * @param param параметр процедуры.
   * @param data  буфер для чтения данных, NULL если данные необходимо проигнорировать.
   * @param size  требуемый размер данных, который необходимо прочесть или проигнорировать.
   *
   * @return количество прочитанных или проигнорированных данных.
  */
  typedef uint32_t (__stdcall* mpack_fn_read)(void* param, void* data, uint32_t size);

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
  
  /**
   * Контекст чтения данных.
  */
  struct mpack_read_t
  {
    uint08_t endian;                // Порядок байт в текущей системе.
    uint08_t status;                // Статус контекста чтения данных (mpack_read_status_t).

    uint08_t format;                // Формат хранимых данных (mpack_format_t).
    uint08_t number_data[10];       // Буфер для числовых данных.
    uint32_t number_need;           // Необходимое количество числовых данных.
    uint32_t number_read;           // Накопленное количество числовых данных.
    uint32_t binary_need;           // Необходимое количество бинарных данных.
    uint32_t binary_read;           // Накопленное количество бинарных данных.

    struct 
    {
      uint08_t type;                // Тип представления данных (mpack_type_t).

      union
      {
        int64_t  i64;               // Число со знаком (64-бит).
        uint64_t u64;               // Число без знака (64-бит).
        bool     bln;               // Логическое значение.
        float    flt;               // Число с плавающей запятой одинарной точности (32-бит).
        double   dbl;               // Число с плавающей запятой двойной точности (64-бит).

        uint32_t str_size;          // Рармер строки в байтах.
        uint32_t bin_size;          // Размер бинарных данных в байтах.
        
        struct
        {
          uint32_t ext_size;        // Размер расширенных бинарных данных в байтах.
          uint08_t ext_type;        // Тип расширенных бинарных данных.
        };

        uint32_t arr_count;         // Количество объектов в массиве.
        uint32_t map_count;         // Количество парных объектов в карте.
      };
    } object;
    
    mpack_fn_read fn_read;          // Процедура чтения данных.
    void*         fn_read_param;    // Параметр передаваемый в процедуру чтения данных.
  };

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  int mpack_get_int_as_int08(mpack_read_t* mpack, int08_t* value);
  int mpack_get_int_as_int16(mpack_read_t* mpack, int16_t* value);
  int mpack_get_int_as_int32(mpack_read_t* mpack, int32_t* value);
  int mpack_get_int_as_int64(mpack_read_t* mpack, int64_t* value);
  int mpack_get_int_as_uint08(mpack_read_t* mpack, uint08_t* value);
  int mpack_get_int_as_uint16(mpack_read_t* mpack, uint16_t* value);
  int mpack_get_int_as_uint32(mpack_read_t* mpack, uint32_t* value);
  int mpack_get_int_as_uint64(mpack_read_t* mpack, uint64_t* value);
  int mpack_get_nil(mpack_read_t* mpack);
  int mpack_get_bool(mpack_read_t* mpack, bool* value);
  int mpack_get_float_as_float32(mpack_read_t* mpack, float* value);
  int mpack_get_float_as_float64(mpack_read_t* mpack, double* value);
  int mpack_get_str_info(mpack_read_t* mpack, uint32_t* size);
  int mpack_get_bin_info(mpack_read_t* mpack, uint32_t* size);
  int mpack_get_ext_info(mpack_read_t* mpack, uint32_t* size, uint08_t* type);
  int mpack_get_array_count(mpack_read_t* mpack, uint32_t* count);
  int mpack_get_map_count(mpack_read_t* mpack, uint32_t* count);
  
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

///////////////////////////////////////////////////////////////////////////////////////////////////