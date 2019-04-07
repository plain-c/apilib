#ifndef spliceplaces64_h__
#define spliceplaces64_h__

///////////////////////////////////////////////////////////////////////////////////////////////////

#if(APILIB_64 != 0)

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Размеры областей памяти.
  */
  enum spliceplaces_size_t
  {
    spliceplaces_relay_longjump_size = 14,
    spliceplaces_relay_nearjump_size = 5,
    spliceplaces_relay_hotpatch_size = 2,
    spliceplaces_tramp_longjump_size = 14,
    spliceplaces_tramp_nearjump_size = 5,
    spliceplaces_tramp_minijump_size = 2,
  };

  /**
   * Используемые области памяти.
  */
  enum spliceplaces_used_t
  {
    spliceplaces_relay_longjump_used = 0x01,
    spliceplaces_relay_nearjump_used = 0x02,
    spliceplaces_relay_hotpatch_used = 0x04,
    spliceplaces_tramp_longjump_used = 0x08,
    spliceplaces_tramp_nearjump_used = 0x10,
    spliceplaces_tramp_minijump_used = 0x20,
  };

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Контекст области для размещения прыжка в пределах 64-бит.
   * Заменяет неиспользуемые данные над процедурой размером 14 байт.
  */
  struct spliceplaces_relay_longjump_t
  {
    const void*   address;
    unsigned char original[spliceplaces_relay_longjump_size];
    unsigned char modified[spliceplaces_relay_longjump_size];
  };

  /**
   * Контекст области для размещения прыжка в пределах 32-бит.
   * Заменяет неиспользуемые данные над процедурой размером 5 байт.
  */
  struct spliceplaces_relay_nearjump_t
  {
    const void*   address;
    unsigned char original[spliceplaces_relay_nearjump_size];
    unsigned char modified[spliceplaces_relay_nearjump_size];
  };

  /**
   * Контекст области для размещения прыжка в пределах 8-бит.
   * Заменяет неиспользуемые данные внутри процедуры размером 2 байта.
  */
  struct spliceplaces_relay_hotpatch_t
  {
    const void*   address;
    unsigned char original[spliceplaces_relay_hotpatch_size];
    unsigned char modified[spliceplaces_relay_hotpatch_size];
  };

  /**
   * Контекст области для размещения прыжка в пределах 64-бит.
   * Заменяет используемые данные внутри процедуры размером 14 байт.
  */
  struct spliceplaces_tramp_longjump_t
  {
    const void*   address;
    unsigned char original[spliceplaces_tramp_longjump_size];
    unsigned char modified[spliceplaces_tramp_longjump_size];
  };

  /**
   * Контекст области для размещения прыжка в пределах 32-бит.
   * Заменяет используемые данные внутри процедуры размером 5 байт.
  */
  struct spliceplaces_tramp_nearjump_t
  {
    const void*   address;
    unsigned char original[spliceplaces_tramp_nearjump_size];
    unsigned char modified[spliceplaces_tramp_nearjump_size];
  };

  /**
   * Контекст области для размещения прыжка в пределах 8-бит.
   * Заменяет используемые данные внутри процедуры размером 2 байта.
  */
  struct spliceplaces_tramp_minijump_t
  {
    const void*   address;
    unsigned char original[spliceplaces_tramp_minijump_size];
    unsigned char modified[spliceplaces_tramp_minijump_size];
  };

  /**
   * Контекст областей в памяти на пути от перехватываемой процедуры к переключателю.
  */
  struct spliceplaces_t
  {
    unsigned char used;

    spliceplaces_relay_longjump_t relay_longjump;
    spliceplaces_relay_nearjump_t relay_nearjump;
    spliceplaces_relay_hotpatch_t relay_hotpatch;
    spliceplaces_tramp_longjump_t tramp_longjump;
    spliceplaces_tramp_nearjump_t tramp_nearjump;
    spliceplaces_tramp_minijump_t tramp_minijump;
  };

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Создание контекста областей.
   *
   * @param places         контекст областей.
   * @param target         адрес процедуры.
   * @param jumper         адрес переключателя.
   * @param bridge_address инструкции которые необходимо перенести на мост.
   * @param bridge_movecnt количество инструкций в байтах которые необходимо перенести на мост.
   * 
   * @return true в случае успеха, false в случае ошибки.
  */
  bool spliceplaces_create(spliceplaces_t* places, void* target, void* jumper, void** bridge_address, size_t* bridge_movecnt);

  /**
   * Разрушение контекста областей.
   *
   * @param places контекст областей.
   * 
   * @return true в случае успеха, false в случае ошибки.
  */
  bool spliceplaces_destroy(spliceplaces_t* places);

  /**
   * Проверка готовности к разрушению контекста областей.
   *
   * @param places контекст областей.
   *
   * @return true контекст готов к разрушению, false не готов к разрушению или в случае ошибки.
  */
  bool spliceplaces_destroy_available(spliceplaces_t* places);

  /**
   * Запись оригинальных данных.
   *
   * @param places контекст областей.
   * 
   * @return true в случае успеха, false в случае ошибки.
  */
  bool spliceplaces_write_original_bytes(spliceplaces_t* places);

  /**
   * Запись модифицированных данных.
   *
   * @param places контекст областей.
   * 
   * @return true в случае успеха, false в случае ошибки.
  */
  bool spliceplaces_write_modified_bytes(spliceplaces_t* places);

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

#endif // #if(APILIB_64 != 0)

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // spliceplaces64_h__