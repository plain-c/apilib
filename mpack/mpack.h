#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Код результата процедур.
  */
  enum mpack_code_t
  {
    mpack_code_partial_complete        = ( 1),
    mpack_code_ok                      = ( 0), // Успешно выполнено.
    mpack_code_error_internal          = (-1), // Внутренняя ошибка.
    mpack_code_error_invalid_argument  = (-2), // Некорректный аргумент при вызове процедуры.
    mpack_code_error_invalid_callback  = (-3), // Некорректный коллбэк указанный в контексте.
    mpack_code_error_invalid_operation = (-4), // Некорректная операция в текущий момент.
    mpack_code_error_invalid_format    = (-5), // Некорректный формат объекта.
    mpack_code_error_invalid_type      = (-6), // Некорректный тип объекта.
    mpack_code_error_not_enough_data   = (-7), // Недостаточно данных для обработки объекта.
    mpack_code_error_buffer_too_small  = (-8), // Недостаточный размер буфера для записи данных.
  };

  /**
   * Порядок байт.
  */
  enum mpack_endian_t
  {
    mpack_endian_unknown = 0x00,
    mpack_endian_big     = 0x01,
    mpack_endian_little  = 0x02,
  };
 
  /**
   * Маска для определения формата хранения данных.
  */
  enum mpack_mask_t                    //      first byte
  {                                    // (in bin)    (in hex)
    mpack_mask_fixint_positive = 0x80, // 0xxxxxxx	0x00 - 0x7f
    mpack_mask_fixint_negative = 0xe0, //	111xxxxx	0xe0 - 0xff
    mpack_mask_fixstr          = 0xe0, // 101xxxxx	0xa0 - 0xbf
    mpack_mask_fixmap          = 0xf0, // 1000xxxx	0x80 - 0x8f
    mpack_mask_fixarray        = 0xf0, // 1001xxxx	0x90 - 0x9f
  };

  /**
   * Формат хранения данных.
  */
  enum mpack_format_t                    //      first byte
  {                                      // (in bin)    (in hex)
    mpack_format_fixint_positive = 0x00, // 0xxxxxxx  0x00 - 0x7f
    mpack_format_fixmap          = 0x80, // 1000xxxx  0x80 - 0x8f
    mpack_format_fixarray        = 0x90, // 1001xxxx  0x90 - 0x9f
    mpack_format_fixstr          = 0xa0, // 101xxxxx  0xa0 - 0xbf
    mpack_format_nil             = 0xc0, // 11000000  0xc0
    mpack_format_reserved        = 0xc1, // 11000001  0xc1 (never used)
    mpack_format_false           = 0xc2, // 11000010  0xc2
    mpack_format_true            = 0xc3, // 11000011  0xc3
    mpack_format_bin8            = 0xc4, // 11000100  0xc4
    mpack_format_bin16           = 0xc5, // 11000101  0xc5
    mpack_format_bin32           = 0xc6, // 11000110  0xc6
    mpack_format_ext8            = 0xc7, // 11000111  0xc7
    mpack_format_ext16           = 0xc8, // 11001000  0xc8
    mpack_format_ext32           = 0xc9, // 11001001  0xc9
    mpack_format_float32         = 0xca, // 11001010  0xca
    mpack_format_float64         = 0xcb, // 11001011  0xcb
    mpack_format_uint8           = 0xcc, // 11001100  0xcc
    mpack_format_uint16          = 0xcd, // 11001101  0xcd
    mpack_format_uint32          = 0xce, // 11001110  0xce
    mpack_format_uint64          = 0xcf, // 11001111  0xcf
    mpack_format_int8            = 0xd0, // 11010000  0xd0
    mpack_format_int16           = 0xd1, // 11010001  0xd1
    mpack_format_int32           = 0xd2, // 11010010  0xd2
    mpack_format_int64           = 0xd3, // 11010011  0xd3
    mpack_format_fixext1         = 0xd4, // 11010100  0xd4
    mpack_format_fixext2         = 0xd5, // 11010101  0xd5
    mpack_format_fixext4         = 0xd6, // 11010110  0xd6
    mpack_format_fixext8         = 0xd7, // 11010111  0xd7
    mpack_format_fixext16        = 0xd8, // 11011000  0xd8
    mpack_format_str8            = 0xd9, // 11011001  0xd9
    mpack_format_str16           = 0xda, // 11011010  0xda
    mpack_format_str32           = 0xdb, // 11011011  0xdb
    mpack_format_array16         = 0xdc, // 11011100  0xdc
    mpack_format_array32         = 0xdd, // 11011101  0xdd
    mpack_format_map16           = 0xde, // 11011110  0xde
    mpack_format_map32           = 0xdf, // 11011111  0xdf
    mpack_format_fixint_negative = 0xe0, // 111xxxxx  0xe0 - 0xff
  };

  /**
   * Тип представления данных.
  */
  enum mpack_type_t
  {
    mpack_type_no      = 0x00, // Invalid type.
    mpack_type_int     = 0x01, // Integer represents an integer (signed).
    mpack_type_uint    = 0x02, // Integer represents an integer (unsigned).
    mpack_type_nil     = 0x03, // Nil represents nil.
    mpack_type_bool    = 0x04, // Boolean represents true or false.
    mpack_type_float   = 0x05, // Float represents a IEEE 754 single precision floating point number including NaN and Infinity (32-bit).
    mpack_type_double  = 0x06, // Float represents a IEEE 754 double precision floating point number including NaN and Infinity (64-bit).
    mpack_type_str     = 0x07, // String extending Raw type represents a UTF-8 string.
    mpack_type_bin     = 0x08, // Binary extending Raw type represents a byte array.
    mpack_type_ext     = 0x09, // Extension represents a tuple of type information and a byte array where type information is an integer whose meaning is defined by applications.
    mpack_type_array   = 0x0A, // Array represents a sequence of objects.
    mpack_type_map     = 0x0B, // Map represents key-value pairs of objects.
  };
 
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
  
  bool mpack_swap16(void* digit);
  bool mpack_swap32(void* digit);
  bool mpack_swap64(void* digit);

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

///////////////////////////////////////////////////////////////////////////////////////////////////