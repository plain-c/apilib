#ifndef splice_h__
#define splice_h__

///////////////////////////////////////////////////////////////////////////////////////////////////

#if( APILIB_32 != 0 || APILIB_64 != 0 )

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Параметры сплайсинга.
  */
  enum splice_param_t
  {
    splice_param_no              = 0x00,
    splice_param_search_redirect = 0x01, // Поиск редиректа процедуры.
    splice_param_create_disabled = 0x02, // Создание выключенного перехвата.
  };

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Контекст сплайсинга.
  */
  struct splice_t;

  /**
   * Процедура уведомления о создании контекста при установке перехвата.
   *
   * @param target адрес оригинальной процедуры.
   * @param hooker адрес для вызова перехватчика процедуры.
   * @param splice адрес созданного контекста.
   * @param bridge адрес для вызова оригинальной процедуры.
  */
  typedef void (__fastcall* splice_notify_t)(void* target, void* hooker, splice_t* splice, void* bridge);

  /**
   * Элемент описывающий перехватываемую процедуру.
  */
  struct spliceitem_t
  {
    void*           target; // Адрес оригинальной процедуры.
    void*           hooker; // Адрес перехватчика процедуры.
    uint32_t        params; // Параметры сплайсинга.
    splice_notify_t notify; // Процедура уведомляющая о созданном контексте сплайсинга.
    splice_t*       splice; // Контекст сплайсинга создаваемый при установке перехвата.
    void*           bridge; // Адрес моста для вызова оригинальной процедуры после перехвата.
  };

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Поиск адреса редиректа процедуры.
   *
   * @param target адрес перехватываемой процедуры.
   *
   * @return адрес редиректа процедуры в случае успеха, NULL в случае ошибки.
  */
  void* splice_search_redirect(void* target);

  /**
   * Создание контекста сплайсинга.
   *
   * @param target адрес оригинальной процедуры.
   * @param hooker адрес перехватчика процедуры.
   *
   * @return контекст сплайсинга в случае успеха, NULL в случае ошибки.
  */
  splice_t* splice_create(void* target, void* hooker);

  /**
   * Разрушение контекста сплайсинга.
   *
   * @param splice контекст сплайсинга.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  bool splice_destroy(splice_t* splice);

  /**
   * Установка сплайсинга в память.
   *
   * @param splice контекст сплайсинга.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  bool splice_write_modified_bytes(splice_t* splice);

  /**
   * Удаление сплайсинга из памяти.
   *
   * @param splice контекст сплайсинга.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  bool splice_write_original_bytes(splice_t* splice);

  /**
   * Включение перехвата для всех потоков.
   *
   * @param splice контекст сплайсинга.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  bool splice_enable(splice_t* splice);

  /**
   * Выключение перехвата для всех потоков.
   *
   * @param splice контекст сплайсинга.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  bool splice_disable(splice_t* splice);

  /**
   * Включение перехвата для текущего потока.
   *
   * @param splice контекст сплайсинга.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  bool splice_enable_for_current_thread(splice_t* splice);

  /**
   * Выключение перехвата для текущего потока.
   *
   * @param splice контекст сплайсинга.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  bool splice_disable_for_current_thread(splice_t* splice);

  /**
   * Получение адреса моста.
   *
   * @param splice контекст сплайсинга.
   *
   * @return адрес моста в случае успеха, NULL в случае ошибки.
  */
  void* splice_get_bridge_location(splice_t* splice);

  /**
   * Установка перехвата одной процедуры.
   *
   * @param entry описание перехватываемой процедуры.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  bool splice_entry_hook(spliceitem_t* item);

  /**
   * Удаление перехвата одной процедуры.
   *
   * @param entry описание перехватываемой процедуры.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  bool splice_entry_unhook(spliceitem_t* item);

  /**
   * Установка перехвата группы процедур.
   *
   * @param items       массив описывающий перехватываемые процедуры.
   * @param items_count количество перехватываемых процедур в массиве.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  bool splice_group_hook(spliceitem_t* items, size_t items_count);

  /**
   * Удаление перехвата группы процедур.
   *
   * @param items       массив описывающий перехватываемые процедуры.
   * @param items_count количество перехватываемых процедур в массиве.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  bool splice_group_unhook(spliceitem_t* items, size_t items_count);

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

#endif //#if( APILIB_32 != 0 || APILIB_64 != 0 )

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // splice_h__