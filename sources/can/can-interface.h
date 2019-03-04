/*
 * CAN interface.
 *
 * Copyright (C) 2017 Dmitry Podkhvatilin <vatilin@gmail.com>
 *
 * This file is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You can redistribute this file and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software Foundation;
 * either version 2 of the License, or (at your discretion) any later version.
 * See the accompanying file "COPYING.txt" for more details.
 *
 * As a special exception to the GPL, permission is granted for additional
 * uses of the text contained in this file.  See the accompanying file
 * "COPY-UOS.txt" for details.
 */
#ifndef __CAN_INTERFACE_H__
#define __CAN_INTERFACE_H__

// !!! ПЕРЕПИСАТЬ ДЛЯ ДРАЙВЕРА CAN

//
//        Интерфейс драйвера контроллера SPI в режиме "Мастер"
//
//    Данный заголовочный файл предназначен для унификации интерфейса драйверов
// контроллеров SPI различных производителей. Унификация интерфейса нужна для
// возможности написания ПЕРЕНОСИМЫХ драйверов контроллеров более высокого 
// уровня, использующих SPI в качестве технологической линии связи (например, 
// драйвер флеш-памяти SD в режиме SPI, драйверы различных EEPROM, подключаемых 
// по SPI, драйвер USB-контроллера MAX3421 и т.д.).
//
//    Для разработки драйвера SPI, совместимого с данным интерфейсом, необходимо
// выполнить несколько шагов:
//
// 1) Объявить структуру драйвера SPI, "наследующую" данную структуру, т.е.
// содержащую в качестве первого поля переменную структуры spimif_t, например:
//
//    struct _manufacturer_spim_t {
//
//       spimif_t        spimif;
//
//       ... // другие поля, необходимые для драйвера SPI.
//    };
//    typedef struct _manufacturer_spim_t manufacturer_spim_t;
//
// Имя структуры, разумеется, может быть произвольным.
//
// 2) Написать функцию приёма-передачи для данного контроллера SPI вида:
//
//    int manufacturer_trx (spimif_t *spi, spi_message_t *msg)
//    {
//          // Преобразуем указатель к структуре нашего аппаратного драйвера
//          // (мы уверены, что нам передают указатель на правильную структуру).
//          manufacturer_spim_t *mspi = (manufacturer_spim_t *) spi;
//
//          ... // Код, реализующий обмен. Используется структура аппаратного
//              // драйвера для хранения нужной для работы информации.
//    }
//
// Необходимо учитывать, что информация о скорости передачи и режиме работы
// контроллера содержится в структуре сообщения spi_message_t, а не в структуре
// драйвера.
// Функция должна возвращать коды результата, объявленные в данном файле.
// Если поле msg->tx_data равно 0, то в линию должно быть выдано msg->word_count
// нулей.
// Если поле msg->rx_data равно 0, то принятые данные должны быть отброшены.
//
// 3) Написать функцию инициализации драйвера SPI. Параметры могут быть любыми,
// данным интерфейсом они никак не регламентируются. Но логично, что одним из
// параметров должен быть указатель на структуру драйвера, содержащую в себе 
// структуру spimif_t. Таким образом, функция инициализации должна выглядеть
// примерно так:
//
//    int manuf_spi_init(manufacturer_spim_t *spi, ...)
//    {
//          spi->spimif.trx = manufacturer_trx;
//          ...
//    }
//
// В функции инициализации обязательно должен быть установлен указатель на 
// функцию приёма-передачи.
//
// 4) Для драйверов SPI предусмотрено макроопределение SPI_NO_DMA, которое,
// если задано, предполагает отключение использования каналов прямого доступа 
// в драйвере SPI. Реализовывать ли такую реакцию на данное макроопределение
// или нет - решение лежит на разработчике конкретного драйвера SPI.
//
// В качестве примеров можно использовать драйверы SPI для процессоров фирмы
// Элвис и Миландр: sources/elvees/spi.*, sources/milandr/spi.* .
//
//
//    Использование драйвера:
//
// 1) Объявить переменную структуры драйвера SPI:
//      manufacturer_spim_t spim;
// 2) Вызвать для неё функцию инициализации. Логично, если это произойдёт в
// функции uos_init(), хотя это и не обязательно:
//      void uos_init()
//      {
//          manuf_spi_init(&spim, ...);
//          ...
//      }
// 3) Объявить переменную сообщения SPI. Проинициализировать её поля и вызвать
// функцию spim_trx:
//
//      spi_message_t msg;
//      uint8_t rxbuf[16];  // буфер для принятых данных
//      uint8_t txbuf[16];  // буфер для выдаваемых данных
//
//      ...
//
//      msg.rx_data = rxbuf;
//      msg.tx_data = txbuf;
//      msg.word_count = 16;
//      msg.freq = 4000000; // битовая скорость в герцах
//      msg.mode = SPI_MODE_CPHA | SPI_MODE_NB_BITS(8); // это для примера
//
//      spim_trx((spimif_t *) &spim, &msg);
//
// Возможные установки режима приёма-передачи приведены ниже в данном файле.
// Поля rx_data и tx_data сообщения могут быть нулевыми, если, соответственно,
// принятые данные не представляют интереса или передаваемые данные могут быть
// просто нулевыми.
//


//
// Коды результата
//

// Успешно
#define CAN_ERR_OK              0
// Возвращается в случае, если параметр содержит недопустимое значение.
#define CAN_ERR_BAD_PARAM       -1
// Возвращается, если указан недопустимый номер порта (контроллера) CAN.
#define CAN_ERR_BAD_PORT        -2
// Возвращается, если произошла ошибка при взаимодействии с контроллером по 
// шине связи (например, по SPI).
#define CAN_ERR_IO              -3
// Ошибка возвращается, если превышено допустимое число приёмных фильтров
#define CAN_TOO_MANY_FILTERS    -4


#define CAN_TIMING_SET_PS1(ps1)               ((ps1) << 0)
#define CAN_TIMING_SET_PS2(ps2)               ((ps2) << 4)
#define CAN_TIMING_SET_PRS(prs)               ((prs) << 8)
#define CAN_TIMING_SET_SJW(sjw)               ((sjw) << 12)
#define CAN_TIMING_GET_PS1(timing)            ((timing >> 0) & 0xF)
#define CAN_TIMING_GET_PS2(timing)            ((timing >> 4) & 0xF)
#define CAN_TIMING_GET_PRS(timing)            ((timing >> 8) & 0xF)
#define CAN_TIMING_GET_SJW(timing)            ((timing >> 12) & 0xF)


typedef struct _canif_t canif_t;
typedef struct _can_frame_t can_frame_t;

//
// Базовый тип интерфейса CAN
//
struct _canif_t
{
    // Мьютекс для синхронизации
    mutex_t     lock;

    // Установка времянки бита.
    // Бит состоит из четырёх участков, длины которых измеряются в квантах (TQ):
    //   sync     - сегмент синхронизации, всегда 1 TQ
    //   prs      - сегмент распространения, компенсирует задержки распространения между
    //              узлами, 1 - 8 TQ
    //   ps1      - сегмент фазы 1, 1 - 8 TQ
    //   ps2      - сегмент фазы 2, 2 - 8 TQ
    //   Таким образом бит разбивается на sync_seg + prop_seg + ps1 + ps2 квантов.
    //   Точка считывания значения бита лежит в конце фазы 1 (или начале фазы 2 - одно и то же).
    //   sjw      - максимально допустимый скачок в квантах при подстройке приёмника
    //              под принимаемый сигнал, 1 - 4 TQ
    //   baud_rate- бодовая скорость (частота выдачи битов).
    //   Длина одного бита рассчитывается по формуле
    //   Tbit = (1 + prog_seg + ps1 + ps2) * 1 / bit_freq.
    // Параметр timing должен содержать значения prs, ps1, ps2, sjw, устанавливаемые
    // с помощью макросов CAN_TIMING_SET_*.
    // При задании timing == 0 функция должна установить тайминг по умолчанию для
    // данной бодовой скорости.
    int (* set_timing)(canif_t *can, unsigned baud_rate, unsigned timing);
        
    // Выдача кадров CAN.
    //   buffer         - буфер с кадрами
    //   nb_of_frames   - количество кадров в буфере
    //   возвращает число выданных кадров или код ошибки (отрицательное число).
    int (* output)(canif_t *can, const can_frame_t *buffer, int nb_of_frames);
    
    // Приём кадров CAN.
    //   buffer         - буфер с кадрами
    //   nb_of_frames   - количество кадров в буфере
    //   non_block      - если не равно 0, то функция выполняется в неблокирующем режиме:
    //                    завершается сразу с нулевым возвратом, если нет принятых кадров.
    //   возвращает число принятых кадров или код ошибки (отрицательное число).
    // Функция должна выдавать только те кадры, которые успешно прошли фильтр.
    int (* input)(canif_t *can, can_frame_t *buffer, int nb_of_frames, int non_block);
    
    // Добавление фильтра.
    // Выдаются из драйвера в прикладное ПО только те кадры, идентификаторы которых
    // удовлетворяет условию id & mask == pattern хотя бы для одного из фильтров.
    // Флаги, хранимые в идентификаторе кадра, должны при этом учитываться.
    // По умолчанию принимаются все пакеты.
    // Маска и фильтр могут содержать бит-признак расширенного формата идентификатора.
    int (* add_filter)(canif_t *can, uint32_t mask, uint32_t pattern);
    
    // Сброс всех фильтров.
    int (* clear_filters)(canif_t *can);
    
    // Получение информации о состоянии контроллера (CAN_ERROR_ACTIVE, ...).
    int (* get_state)(canif_t *can);
    
    // Переключение режима энергопотребления контроллера.
    // При переключениях контроллера должны сохраняться (или восстанавливаться) все 
    // установленные фильтры.
    int (* switch_mode)(canif_t *can, int mode);
    
    // Получение текущего режима энергопотребления.
    int (* get_mode)(canif_t *can);
    
    // Сброс контроллера.
    int (* reset)(canif_t *can);
};

#define to_canif(x)   ((canif_t*)&(x)->canif)

//
// Состояния контроллера CAN
//
enum {
    CAN_ERROR_ACTIVE,
    CAN_ERROR_PASSIVE,
    CAN_BUS_OFF
};

//
// Состояние интерфейса с точки зрения потребления энергии
//
enum {
    IFACE_MODE_SHUTDOWN,            // отключение (наименьшее потребление энергии)
    IFACE_MODE_SLEEP,               // сон
    IFACE_MODE_OPERATION            // работа (наибольшее потребление энергии)
};

//
// Флаги поля идентификатора
//
#define CAN_ID_MASK         0x1FFFFFFF  // Маска для выделения непосредственно идентификатора
#define	CAN_ID_RTR          (1 << 29)   // Удалённый запрос кадра
#define	CAN_ID_EXT          (1 << 30)   // Расширенный формат

//
// Обобщённый формат кадра CAN.
//
struct _can_frame_t {
    // Идентификатор кадра и флаги
	uint32_t id;
	// Поле длины (допустимые значения: от 0 до 8)
	unsigned dlc;
    // Данные
    union {
        uint8_t     data[8];
        uint32_t    data32[2];
    };
};


//
// Функции-обёрка для удобства вызова функции драйвера CAN
//

static inline __attribute__((always_inline)) 
int can_set_timing(canif_t *can, unsigned baud_rate, unsigned timing)
{
    return can->set_timing(can, baud_rate, timing);
}

static inline __attribute__((always_inline)) 
int can_output(canif_t *can, const can_frame_t *buffer, int nb_of_frames)
{
    return can->output(can, buffer, nb_of_frames);
}

static inline __attribute__((always_inline)) 
int can_input(canif_t *can, can_frame_t *buffer, int nb_of_frames, int non_block)
{
    return can->input(can, buffer, nb_of_frames, non_block);
}

static inline __attribute__((always_inline)) 
int can_add_filter(canif_t *can, unsigned mask, unsigned pattern)
{
    return can->add_filter(can, mask, pattern);
}

static inline __attribute__((always_inline)) 
int can_clear_filters(canif_t *can)
{
    return can->clear_filters(can);
}

static inline __attribute__((always_inline)) 
int can_get_state(canif_t *can)
{
    return can->get_state(can);
}

static inline __attribute__((always_inline)) 
int can_switch_mode(canif_t *can, int mode)
{
    return can->switch_mode(can, mode);
}

static inline __attribute__((always_inline)) 
int can_get_mode(canif_t *can)
{
    return can->get_mode(can);
}

static inline __attribute__((always_inline)) 
int can_reset(canif_t *can)
{
    return can->reset(can);
}


#endif
