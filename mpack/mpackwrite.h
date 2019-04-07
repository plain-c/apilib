#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Статус контекста записи данных.
  */
  enum mpack_write_status_t
  {
    mpack_write_status_build_buffer = 0x00, // Построение объекта с записью в буфер.
    mpack_write_status_write_buffer = 0x01, // Запись буфера.
    mpack_write_status_write_binary = 0x02, // Запись бинарных данных объекта.
    mpack_write_status_fatal_error  = 0x03, // Контекст записи данных поврежден.
  };

  /**
   * Прототип процедуры записи данных.
   *
   * @param param параметр процедуры.
   * @param data  записываемые данные.
   * @param size  размер записываемых данных.
   *
   * @return количество записанных данных, 0 не является ошибкой.
  */
  typedef uint32_t (__stdcall* mpack_fn_write)(void* param, const void* data, uint32_t size);

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Контекст записи данных.
  */
  struct mpack_write_t
  {
    uint08_t endian;                // Порядок байт в текущей системе.
    uint08_t status;                // Статус контекста записи данных (mpack_write_status_t).

    uint08_t buffer_data[10];       // Буфер содержащий формат объекта и его числовые данные.
    uint32_t buffer_offset;         // Количество записанных данных из буфера.
    uint32_t buffer_amount;         // Общее количество данных в буфере.
    uint32_t binary_offset;         // Количество записанных бинарных данных.
    uint32_t binary_amount;         // Общее количество бинарных данных.

    mpack_fn_write fn_write;        // Процедура записи данных.
    void*          fn_write_param;  // Параметр передаваемый в процедуру записи данных.
  };

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Сброс контекста записи данных к исходному состоянию.
   *
   * Сброс контекста может потребоваться в случае отмены записи построенного объекта в буфере,
   * когда ни одного байта записано не было. Сброс контекста невозможен если какие-либо данные
   * были записаны и записаны не полностью. Так же сброс контекста невозможен в случае его 
   * повреждения, о чем говорит статус самого контекста, в таком случае требуется его разрушение.
   *
   * @param mpack контекст записи данных.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки.
  */
  int mpack_reset_write_context(mpack_write_t* mpack);

  /**
   * Запись объекта.
   *
   * Запись объекта через процедуру {@link mpack_fn_write} указанную для созданного контекста.
   * На первом этапе происходит запись данных из буфера.
   * На втором этапе происходит запись бинарных данных для таких объектов как 'str', 'bin, 'ext'.
   * После завершения каждого из этапов происходит автоматическая смена статуса контекста.
   *
   * При неполной записи объекта требуется выполнять повторный вызов процедуры учитывая при этом
   * количество записанных данных.
   *
   * @param mpack   контекст записи данных.
   * @param data    бинарные данные, NULL если данные отсутствуют.
   * @param size    размер бинарных данных в байтах, 0 если данные отсутствуют.
   * @param written количество записанных бинарных данных в байтах {@link data}.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки,
   *         mpack_code_partial_complete объект записан неполностью.
  */
  int mpack_write_object(mpack_write_t* mpack, const void* data, uint32_t size, uint32_t* written);

  /**
   * Построение объекта числа со знаком с записью в буфер (08-bit signed integer).
   * Для записи созданного объекта следует использовать {@link mpack_write_object}.
   *
   * @param mpack конткест записи данных.
   * @param value число со знаком.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки.
  */
  int mpack_build_int_from_int08(mpack_write_t* mpack, int08_t value);

  /**
   * Построение объекта числа со знаком с записью в буфер (16-bit signed integer).
   * Для записи созданного объекта следует использовать {@link mpack_write_object}.
   *
   * @param mpack конткест записи данных.
   * @param value число со знаком.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки.
  */
  int mpack_build_int_from_int16(mpack_write_t* mpack, int16_t value);

  /**
   * Построение объекта числа со знаком с записью в буфер (32-bit signed integer).
   * Для записи созданного объекта следует использовать {@link mpack_write_object}.
   *
   * @param mpack конткест записи данных.
   * @param value число со знаком.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки.
  */
  int mpack_build_int_from_int32(mpack_write_t* mpack, int32_t value);

  /**
   * Построение объекта числа со знаком с записью в буфер (64-bit signed integer).
   * Для записи созданного объекта следует использовать {@link mpack_write_object}.
   *
   * @param mpack конткест записи данных.
   * @param value число со знаком.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки.
  */
  int mpack_build_int_from_int64(mpack_write_t* mpack, int64_t value);

  /**
   * Построение объекта числа без знака с записью в буфер (08-bit unsigned integer).
   * Для записи созданного объекта следует использовать {@link mpack_write_object}.
   *
   * @param mpack конткест записи данных.
   * @param value число без знака.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки.
  */
  int mpack_build_int_from_uint08(mpack_write_t* mpack, uint08_t value);

  /**
   * Построение объекта числа без знака с записью в буфер (16-bit unsigned integer).
   * Для записи созданного объекта следует использовать {@link mpack_write_object}.
   *
   * @param mpack конткест записи данных.
   * @param value число без знака.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки.
  */
  int mpack_build_int_from_uint16(mpack_write_t* mpack, uint16_t value);

  /**
   * Построение объекта числа без знака с записью в буфер (32-bit unsigned integer).
   * Для записи созданного объекта следует использовать {@link mpack_write_object}.
   *
   * @param mpack конткест записи данных.
   * @param value число без знака.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки.
  */
  int mpack_build_int_from_uint32(mpack_write_t* mpack, uint32_t value);

  /**
   * Построение объекта числа без знака с записью в буфер (64-bit unsigned integer).
   * Для записи созданного объекта следует использовать {@link mpack_write_object}.
   *
   * @param mpack конткест записи данных.
   * @param value число без знака.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки.
  */
  int mpack_build_int_from_uint64(mpack_write_t* mpack, uint64_t value);

  /**
   * Построение объекта пустого типа с записью в буфер (nil).
   * Для записи созданного объекта следует использовать {@link mpack_write_object}.
   *
   * @param mpack конткест записи данных.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки.
  */
  int mpack_build_nil(mpack_write_t* mpack);

  /**
   * Построение объекта логического значения с записью в буфер (boolean).
   * Для записи созданного объекта следует использовать {@link mpack_write_object}.
   *
   * @param mpack конткест записи данных.
   * @param value логическое значение.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки.
  */
  int mpack_build_bool(mpack_write_t* mpack, bool value);

  /**
   * Построение объекта числа с плавающей запятой с записью в буфер (32-bit single precision).
   * Для записи созданного объекта следует использовать {@link mpack_write_object}.
   *
   * @param mpack конткест записи данных.
   * @param value число с плавающей запятой.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки.
  */
  int mpack_build_float_as_float32(mpack_write_t* mpack, float value);

  /**
   * Построение объекта числа с плавающей запятой с записью в буфер (64-bit double precision).
   * Для записи созданного объекта следует использовать {@link mpack_write_object}.
   *
   * @param mpack конткест записи данных.
   * @param value число с плавающей запятой.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки.
  */
  int mpack_build_float_as_float64(mpack_write_t* mpack, double value);

  /**
   * Построение информации о строковом объекте с записью в буфер (str).
   * Для записи созданного объекта следует использовать {@link mpack_write_object}.
   *
   * @param mpack конткест записи данных.
   * @param size  размер строки в байтах.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки.
  */
  int mpack_build_str_info(mpack_write_t* mpack, uint32_t size);

  /**
   * Построение информации о бинарном объекте с записью в буфер (bin).
   * Для записи созданного объекта следует использовать {@link mpack_write_object}.
   *
   * @param mpack конткест записи данных.
   * @param size  размер бинарных данных в байтах.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки.
  */
  int mpack_build_bin_info(mpack_write_t* mpack, uint32_t size);

  /**
   * Построение информации о бинарном расширенном объекте с записью в буфер (ext).
   * Для записи созданного объекта следует использовать {@link mpack_write_object}.
   *
   * @param mpack конткест записи данных.
   * @param size  размер бинарных данных в байтах.
   * @oaram type  тип расширенного объекта.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки.
  */
  int mpack_build_ext_info(mpack_write_t* mpack, uint32_t size, uint08_t type);

  /**
   * Построение информации о массиве объектов с записью в буфер (array).
   * Для записи созданного объекта следует использовать {@link mpack_write_object}.
   *
   * @param mpack контекст записи данных.
   * @param count количество объектов в массиве.
   * 
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки.
  */
  int mpack_build_array(mpack_write_t* mpack, uint32_t count);

  /**
   * Построение информации о карте объектов с записью в буфер (map).
   * Для записи созданного объекта следует использовать {@link mpack_write_object}.
   *
   * @param mpack контекст записи данных.
   * @param count количество парных объектов в карте.
   * 
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки.
  */
  int mpack_build_map(mpack_write_t* mpack, uint32_t count);
  
  /**
   * Запись объекта числа со знаком (08-bit signed integer).
   *
   * Запись объекта полностью, включая его первоначальное построение в буфере.
   * В случае неполной записи следует использовать {@link mpack_write_object}.
   *
   * @param mpack конткест записи данных.
   * @param value число со знаком.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки,
   *         mpack_code_partial_complete в случае неполной записи объекта.
  */
  int mpack_write_int_from_int08(mpack_write_t* mpack, int08_t value);

  /**
   * Запись объекта числа со знаком (16-bit signed integer).
   *
   * Запись объекта полностью, включая его первоначальное построение в буфере.
   * В случае неполной записи следует использовать {@link mpack_write_object}.
   *
   * @param mpack конткест записи данных.
   * @param value число со знаком.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки,
   *         mpack_code_partial_complete в случае неполной записи объекта.
  */
  int mpack_write_int_from_int16(mpack_write_t* mpack, int16_t value);
  
  /**
   * Запись объекта числа со знаком (32-bit signed integer).
   *
   * Запись объекта полностью, включая его первоначальное построение в буфере.
   * В случае неполной записи следует использовать {@link mpack_write_object}.
   *
   * @param mpack конткест записи данных.
   * @param value число со знаком.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки,
   *         mpack_code_partial_complete в случае неполной записи объекта.
  */
  int mpack_write_int_from_int32(mpack_write_t* mpack, int32_t value);

  /**
   * Запись объекта числа со знаком (64-bit signed integer).
   *
   * Запись объекта полностью, включая его первоначальное построение в буфере.
   * В случае неполной записи следует использовать {@link mpack_write_object}.
   *
   * @param mpack конткест записи данных.
   * @param value число со знаком.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки,
   *         mpack_code_partial_complete в случае неполной записи объекта.
  */
  int mpack_write_int_from_int64(mpack_write_t* mpack, int64_t value);

  /**
   * Запись объекта числа без знака (08-bit unsigned integer).
   *
   * Запись объекта полностью, включая его первоначальное построение в буфере.
   * В случае неполной записи следует использовать {@link mpack_write_object}.
   *
   * @param mpack конткест записи данных.
   * @param value число без знака.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки,
   *         mpack_code_partial_complete в случае неполной записи объекта.
  */
  int mpack_write_int_from_uint08(mpack_write_t* mpack, uint08_t value);

  /**
   * Запись объекта числа без знака (16-bit unsigned integer).
   *
   * Запись объекта полностью, включая его первоначальное построение в буфере.
   * В случае неполной записи следует использовать {@link mpack_write_object}.
   *
   * @param mpack конткест записи данных.
   * @param value число без знака.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки,
   *         mpack_code_partial_complete в случае неполной записи объекта.
  */
  int mpack_write_int_from_uint16(mpack_write_t* mpack, uint16_t value);

  /**
   * Запись объекта числа без знака (32-bit unsigned integer).
   *
   * Запись объекта полностью, включая его первоначальное построение в буфере.
   * В случае неполной записи следует использовать {@link mpack_write_object}.
   *
   * @param mpack конткест записи данных.
   * @param value число без знака.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки,
   *         mpack_code_partial_complete в случае неполной записи объекта.
  */
  int mpack_write_int_from_uint32(mpack_write_t* mpack, uint32_t value);

  /**
   * Запись объекта числа без знака (64-bit unsigned integer).
   *
   * Запись объекта полностью, включая его первоначальное построение в буфере.
   * В случае неполной записи следует использовать {@link mpack_write_object}.
   *
   * @param mpack конткест записи данных.
   * @param value число без знака.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки,
   *         mpack_code_partial_complete в случае неполной записи объекта.
  */
  int mpack_write_int_from_uint64(mpack_write_t* mpack, uint64_t value);

  /**
   * Запись объекта пустого типа (nil).
   *
   * Запись объекта полностью, включая его первоначальное построение в буфере.
   * В случае неполной записи следует использовать {@link mpack_write_object}.
   *
   * @param mpack конткест записи данных.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки,
   *         mpack_code_partial_complete в случае неполной записи объекта.
  */
  int mpack_write_nil(mpack_write_t* mpack);

  /**
   * Запись объекта логического значения (boolean).
   *
   * Запись объекта полностью, включая его первоначальное построение в буфере.
   * В случае неполной записи следует использовать {@link mpack_write_object}.
   *
   * @param mpack конткест записи данных.
   * @param value логическое значение.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки,
   *         mpack_code_partial_complete в случае неполной записи объекта.
  */
  int mpack_write_bool(mpack_write_t* mpack, bool value);

  /**
   * Запись объекта числа с плавающей запятой (32-bit single precision).
   *
   * Запись объекта полностью, включая его первоначальное построение в буфере.
   * В случае неполной записи следует использовать {@link mpack_write_object}.
   *
   * @param mpack конткест записи данных.
   * @param value число с плавающей запятой.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки,
   *         mpack_code_partial_complete в случае неполной записи объекта.
  */
  int mpack_write_float_from_float32(mpack_write_t* mpack, float value);

  /**
   * Запись объекта числа с плавающей запятой (64-bit double precision).
   *
   * Запись объекта полностью, включая его первоначальное построение в буфере.
   * В случае неполной записи следует использовать {@link mpack_write_object}.
   *
   * @param mpack конткест записи данных.
   * @param value число с плавающей запятой.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки,
   *         mpack_code_partial_complete в случае неполной записи объекта.
  */
  int mpack_write_float_from_float64(mpack_write_t* mpack, double value);

  /**
   * Запись объекта строки (str).
   *
   * Запись объекта полностью, включая его первоначальное построение в буфере.
   * В случае неполной записи следует использовать {@link mpack_write_object}.
   *
   * @param mpack   конткест записи данных.
   * @param data    данные строки.
   * @param size    размер строки в байтах.
   * @param written количество записанных данных строки в байтах.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки,
   *         mpack_code_partial_complete в случае неполной записи объекта.
  */
  int mpack_write_str(mpack_write_t* mpack, const void* data, uint32_t size, uint32_t* written);

  /**
   * Запись объекта бинарных данных (bin).
   *
   * Запись объекта полностью, включая его первоначальное построение в буфере.
   * В случае неполной записи следует использовать {@link mpack_write_object}.
   *
   * @param mpack   конткест записи данных.
   * @param data    бинарные данные.
   * @param size    размер бинарных данных в байтах.
   * @param written количество записанных бинарных данных в байтах.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки,
   *         mpack_code_partial_complete в случае неполной записи объекта.
  */
  int mpack_write_bin(mpack_write_t* mpack, const void* data, uint32_t size, uint32_t* written);

  /**
   * Запись объекта бинарных расширенных данных (ext).
   *
   * Запись объекта полностью, включая его первоначальное построение в буфере.
   * В случае неполной записи следует использовать {@link mpack_write_object}.
   *
   * @param mpack   конткест записи данных.
   * @param data    бинарные данные.
   * @param size    размер бинарных данных в байтах.
   * @oaram type    тип бинарных данных.
   * @param written количество записанных бинарных данных в байтах.
   *
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки,
   *         mpack_code_partial_complete в случае неполной записи объекта.
  */
  int mpack_write_ext(mpack_write_t* mpack, const void* data, uint32_t size, uint08_t type, uint32_t* written);

  /**
   * Запись объекта массива (array).
   *
   * Запись объекта полностью, включая его первоначальное построение в буфере.
   * В случае неполной записи следует использовать {@link mpack_write_object}.
   *
   * @param mpack контекст записи данных.
   * @param count количество объектов в массиве.
   * 
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки,
   *         mpack_code_partial_complete в случае неполной записи объекта.
  */
  int mpack_write_array(mpack_write_t* mpack, uint32_t count);

  /**
   * Запись объекта карты (map).
   *
   * Запись объекта полностью, включая его первоначальное построение в буфере.
   * В случае неполной записи следует использовать {@link mpack_write_object}.
   *
   * @param mpack контекст записи данных.
   * @param count количество парных объектов в карте.
   * 
   * @return mpack_code_ok в случае успеха, mpack_code_error_* в случае ошибки,
   *         mpack_code_partial_complete в случае неполной записи объекта.
  */
  int mpack_write_map(mpack_write_t* mpack, uint32_t count);

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

///////////////////////////////////////////////////////////////////////////////////////////////////