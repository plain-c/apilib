#ifndef splicejumper32_h__
#define splicejumper32_h__

///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Переключатель отвечает за передачу управления оригинальной процедуре либо перехватчику.
 * Управление передается перехватчику процедуры для потока вызывающего процедуру первично.
 * Управление передается оригинальной процедуре для потока вызывающего процедуру вторично.
 *
 * Поддерживается отключение передачи управления перехватчику как глобально для всех потоков,
 * так и локально для кокретного потока, независимо первичный вызов или вторичный.
*/

///////////////////////////////////////////////////////////////////////////////////////////////////

#if(APILIB_32 != 0)

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Бинарный код переключателя для 32-битного окружения.
   * Отвечает за передачу управления внутри потока вызывающего перехваченную процедуру.
  */
# pragma pack(1)
  struct splicejumper_bincode_t
  {
    unsigned char dbg;                    ///    int   3               0xCC
    unsigned char dummy00;                ///    pusha                 0x60
    unsigned char dummy01[4];             ///    mov   edx, [esp+8*4]  0x8B,0x54,0x24,0x20
    struct
    {
      unsigned char dummy;                ///    mov   ecx, ???        0xB9
      const void*   jumper;               ///    ??? -> контекст переключателя.
    } s1;
    struct
    {
      unsigned char dummy;                ///    mov   eax, ???        0xB8
      const void*   jumper_enter_bincode; ///    ??? -> обработчик входа в переключатель.
    } s2;
    unsigned char dummy02[2];             ///    call  eax             0xFF,0xD0
    unsigned char dummy03[4];             ///    mov   [esp+8*4], eax  0x89,0x44,0x24,0x20
    unsigned char dummy04;                ///    popa                  0x61
    unsigned char dummy05;                ///    pop   eax             0x58
    unsigned char dummy06[2];             ///    call  eax             0xFF,0xD0
    unsigned char dummy07;                ///    push  eax             0x50
    unsigned char dummy08;                ///    pusha                 0x60
    struct
    {
      unsigned char dummy;                ///    mov   ecx, ???        0xB9
      const void*   jumper;               ///    ??? -> контекст переключателя.
    } s3;
    struct
    {
      unsigned char dummy;                ///    mov   eax, ???        0xB8
      const void*   jumper_leave_bincode; ///    ??? -> обработчик выхода из переключателя.
    } s4;
    unsigned char dummy09[2];             ///    call  eax             0xFF,0xD0
    unsigned char dummy10[4];             ///    mov   [esp+8*4], eax  0x89,0x44,0x24,0x20
    unsigned char dummy11;                ///    popa                  0x61
    unsigned char dummy12;                ///    retn                  0xC3
  };
# pragma pack()

  /**
   * Контекст адреса возврата для потока исполняющего бинарный код переключателя.
  */
  struct splicejumper_retaddr_t
  {
    spliceimport_tid_t thread_id;
    void*              return_address;
  };

  /**
   * Контекст игнорируемого потока для которого выключен перехват.
  */
  struct splicejumper_skipped_t
  {
    spliceimport_tid_t thread_id;
  };

  /**
   * Контекст переключателя для 32-битного окружения.
   * Содержит бинарный код, который должен быть размещен в памяти с правами на исполнение.
  */
  struct splicejumper_t
  {
    bool                    active;         // Активен ли переключатель.
    void*                   bridge;         // Адрес для вызова оригинальной процедуры.
    void*                   hooker;         // Адрес для вызова перехватчика процедуры.

    spliceimport_cs_t       retaddr_cs;     // Критическая секция для эксклюзивного доступа.
    splicejumper_retaddr_t* retaddr_array;  // Массив адресов возврата.
    size_t                  retaddr_count;  // Кол-во адресов возврата.

    spliceimport_cs_t       skipped_cs;     // Критическая секция для эксклюзивного доступа.
    splicejumper_skipped_t* skipped_array;  // Массив потоков перехват для которых выключен.
    size_t                  skipped_count;  // Кол-во потоков перехват для которых выключен.

    splicejumper_bincode_t  bincode;        // Бинарный код переключателя.
  };

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * Создание контекста переключателя.
   * 
   * @param jumper контекст переключателя.
   * @param bridge адрес для вызова оригинальной процедуры.
   * @param hooker адрес для вызова перехватчика процедуры.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  bool splicejumper_create(splicejumper_t* jumper, void* bridge, void* hooker);

  /**
   * Разрушение контекста переключателя.
   *
   * Перед разрушением контекста необходимо убедиться в его готовности к разрушению используя 
   * splicejumper_destroy_available(), проверив количество используемых адресов возврата, чтобы
   * не нарушить выполнение потока посетившего переключатель, но не совершившего из него выход.
   *
   * @param jumper контекст переключателя. 
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  bool splicejumper_destroy(splicejumper_t* jumper);

  /**
   * Проверка готовности к разрушению контекста переключателя.
   *
   * @param jumper контекст переключателя.
   *
   * @return true контекст готов к разрушению, false не готов к разрушению или в случае ошибки.
  */
  bool splicejumper_destroy_available(splicejumper_t* jumper);

  /**
   * Получение активности переключателя для всех потоков.
   *
   * @param jumper контекст переключателя.
   *
   * @return true контекст активен, false контекст не активен или в случае ошибки.
  */
  bool splicejumper_get_active(splicejumper_t* jumper);

  /**
   * Установка активности переключателя для всех потоков.
   *
   * @param jumper контекст переключателя.
   * @param active активность.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  bool splicejumper_set_active(splicejumper_t* jumper, bool active);

  /**
   * Получение активности переключателя для текущего потока.
   *
   * @param jumper контекст переключателя.
   *
   * @return true контекст активен, false контекст не активен или в случае ошибки.
  */
  bool splicejumper_get_active_for_current_thread(splicejumper_t* jumper);

  /**
   * Установка активности переключателя для текущего потока.
   *
   * @param jumper контекст переключателя.
   * @param active активность.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  bool splicejumper_set_active_for_current_thread(splicejumper_t* jumper, bool active);

  /**
   * Получение адреса для вызова оригинальной процедуры.
   *
   * @param jumper контекст переключателя.
   *
   * @return адрес для вызова оригинальной процедуры в случае успеха, NULL в случае ошибки.
  */
  void* splicejumper_get_brdige(splicejumper_t* jumper);

  /**
   * Установка адреса для вызова оригинальной процедуры.
   *
   * @param jumper контекст переключателя.
   * @param bridge адрес для вызова оригинальной процедуры.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  bool splicejumper_set_bridge(splicejumper_t* jumper, void* bridge);

  /**
   * Получение адреса для вызова перехватчика процедуры.
   *
   * @param jumper контекст переключателя.
   *
   * @return адрес для вызова перехватчика процедуры в случае успеха, NULL в случае ошибки.
  */
  void* splicejumper_get_hooker(splicejumper_t* jumper);

  /**
   * Установка адреса для вызова перехватчика процедуры.
   *
   * @param jumper контекст переключателя.
   * @param hooker адрес для вызова перехватчика процедуры.
   *
   * @return true в случае успеха, false в случае ошибки.
  */
  bool splicejumper_set_hooker(splicejumper_t* jumper, void* hooker);

  /**
   * Получение дислокации переключателя.
   *
   * @param jumper контекст переключателя.
   *
   * @return дислокация переключателя в случае успеха, NULL в случае ошибки.
  */
  void* splicejumper_get_location(splicejumper_t* jumper);

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

#endif // #if(APILIB_32 != 0)

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // splicejumper32_h__