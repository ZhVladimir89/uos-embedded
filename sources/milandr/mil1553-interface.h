/*
 * MIL-STD 1553 driver interface.
 *
 * Copyright (C) 2015 Dmitry Podkhvatilin <vatilin@gmail.com>
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
#ifndef __MIL1553_INTERFACE_H__
#define __MIL1553_INTERFACE_H__

#ifndef MIL_MAX_SLOTS
#define MIL_MAX_SLOTS       16
#endif

//
// Коды результата
//

// Успешно
#define MIL_ERR_OK               0
#define MIL_ERR_NOT_SUPP        -1
#define MIL_ERR_BAD_ARG         -2
#define MIL_ERR_NOT_PERM        -3
#define MIL_ERR_NO_MEM          -4

//
// Режимы работы контроллера MIL-STD 1553
//
#define MIL_MODE_UNDEFINED          0
#define MIL_MODE_BC_MAIN            1
#define MIL_MODE_BC_RSRV            2
#define MIL_MODE_RT                 3
#define MIL_MODE_MON                4

#define MIL_SUBADDR_WORDS_COUNT     32

#define MIL_DATA_LENGTH     		1024

enum { MIL_SINGLE_MSG_COUNT = 64 };

//! Режимы передачи данных между узлами MIL-STD 1553.
typedef enum
{
    MIL_SLOT_BC_RT,              //!< Передача данных КШ->ОУ
    MIL_SLOT_RT_BC,              //!< Передача данных ОУ->КШ
    MIL_SLOT_RT_RT,              //!< Передача данных ОУ->ОУ
    MIL_SLOT_NO_TRANS            //!< Нет передачи
} mil_trans_mode_t;

//! Значения регистра MSG в режиме ОУ и М.
typedef enum
{
    // Команды обмена данными
    //! Команда приёма КШ-ОУ, не групповая (формат сообщения 1)
    MSG_DATARECV__BC_RT__SINGLE = 1,
    //! Команда приёма  КШ-ОУ, групповая (формат сообщения 7)
    MSG_DATARECV__BC_RT__GROUP = 0x80,
    //! Команда приёма ОУ-ОУ, не групповая (формат сообщения 3)
    MSG_DATARECV__RT_RT__SINGLE = 4,
    //! Команда приёма ОУ-ОУ, групповая (формат сообщения 8)
    MSG_DATARECV__RT_RT__GROUP = 0x100,
    //! Команда передачи ОУ-КШ (формат сообщения 2)
    MSG_DATASEND__RT_BC__SINGLE = 0x402,
    //! Команда передачи ОУ-ОУ, не групповая (формат сообщения 3)
    MSG_DATASEND__RT_RT__SINGLE = 0x1008,
    //! Команда передачи ОУ-ОУ, групповая (формат сообщения 8)
    MSG_DATASEND__RT_RT__GROUP = 0x200,
    // Команды управления
    //! Код 0-15 K=1 нет данных, не групповая (формат сообщения 4)
    MSG_CMD_WITH_NODATA__0_0xf__SINGLE = 0x410,
    //! Код 0-15 K=1 нет данных, групповая (формат сообщения 9)
    MSG_CMD_WITH_NODATA__0_0xf__GROUP = 0x400,
    //! Код 16-31 K=1 с данными, не групповая (формат сообщения 5)
    MSG_CMD_WITH_NODATA__0x10_0x1f__DATAWORD_EXPECTED__SINGLE = 0x2420,
    //! Код 16-31 К=0 с данными, не групповая (формат сообщения 6)
    MSG_CMD_WITH_DATA__0x10_0x1f__SINGLE = 0x40,
    //! Код 16-31 К=0 с данными, групповая (формат сообщения 10)
    MSG_CMD_WITH_DATA__0x10_0x1f__GROUP = 0x800
} MSG_COMMANDS;

typedef enum
{
    CMD_TakeInterfaceControl = 0,  //!< Принять управление интерфейсом
    CMD_Synchronize = 1,  //!< Синхронизация
    CMD_SendAnswerWord = 2,  //!< Передать ОС
    CMD_SelfCheck = 3,  //!< Начать самоконтроль ОУ
    CMD_LockSender = 4,  //!< Блокировать передатчик
    CMD_UnlockSender = 5,  //!< Разблокировать передатчик
    CMD_LockInvalidRtFlag = 6,  //!< Блокировать признак неисправности ОУ
    CMD_UnlockInvalidRtFlag = 7,  //!< Разблокировать признак неисправности ОУ
    CMD_SetRtInitialState = 8,  //!< Установить ОУ в исходное состояние
    CMD_SendVectorWord = 0x10,  //!< Передать векторное слово
    CMD_SynchronizeWithDataWord = 0x11,  //!< Синхронизация (с СД)
    CMD_SendLastCommand = 0x12,  //!< Передать последнюю команду
    CMD_SendRtInternalCheckWord = 0x13,  //!< Передать слово ВСК (встроенная система контроля) ОУ
    CMD_LockSenderN = 0x14,  //!< Блокировать i-й передатчик
    CMD_UnlockSenderN = 0x15  //!< Разблокировать i-й передатчик
} BC_COMMAND_CODES;

struct command_t {
    unsigned mode           : 2;  //!< Режим передачи данных из списка mil_request_mode_t
    unsigned addr           : 5;  //!< Адрес ОУ для режимов КШ->ОУ
    unsigned req_pattern    : 5;  //!< Признак команды управления. Должен быть равен b00000 либо b11111.
    unsigned command        : 5;  //!< Команда управления.
    unsigned reserve2       : 5;  //!< Адрес ОУ источника для режима ОУ->ОУ.
    unsigned reserve3       : 5;  //!< Подадрес для адреса, указанного в поле \a addr_src.
    unsigned reserve        : 5;  //!< не используется
};

typedef struct command_t command_t;

//! Дескриптор слота
typedef struct _mil_slot_desc_t
{
    union {
        struct {
            unsigned transmit_mode  : 2;  //!< Режим передачи данных из списка mil_trans_mode_t.
            unsigned addr           : 5;  //!< Адрес ОУ для режимов КШ->ОУ и ОУ->КШ. Адрес ОУ приёмника для режима ОУ->ОУ.
            unsigned subaddr        : 5;  //!< Подадрес для адреса, указанного в поле \a addr.
            unsigned words_count    : 5;  //!< Количество слов данных для передачи.
            unsigned addr_src       : 5;  //!< Адрес ОУ источника для режима ОУ->ОУ.
            unsigned subaddr_src    : 5;  //!< Подадрес для адреса, указанного в поле \a addr_src.
            unsigned reserve        : 5;  //!< не используется
        };
        unsigned raw;
	command_t command;
    };
} mil_slot_desc_t;


typedef struct _mil_slot_t mil_slot_t;
struct _mil_slot_t
{
    mil_slot_desc_t     desc;
    uint16_t            *data;
    mil_slot_t          *next;
};

//
// Базовый тип интерфейса MIL-STD 1553
//
typedef struct _mil1553if_t mil1553if_t;
struct _mil1553if_t
{
    // Мьютекс для синхронизации
    mutex_t     mutex;

    int (* set_mode)(mil1553if_t *mil, unsigned mode);
    int (* start)(mil1553if_t *mil);
    int (* stop)(mil1553if_t *mil);
    int (* is_started)(mil1553if_t *mil);
    int (* read_subaddr)(mil1553if_t *mil, int subaddr, void *data, int nb_words);
    int (* write_subaddr)(mil1553if_t *mil, int subaddr, void *data, int nb_words);
    int (* lock)(mil1553if_t *mil);
    int (* unlock)(mil1553if_t *mil);

    int (* bc_set_cyclogram)(mil1553if_t *mil, mil_slot_t *head, unsigned nb_slots);
    int (* bc_set_period)(mil1553if_t *mil, unsigned period_ms);
    int (* bc_urgent_send)(mil1553if_t *mil, mil_slot_desc_t descr, void *data);
    int (* bc_ordinary_send)(mil1553if_t *mil, int slot_number, void *data);
};

//
// Функции-обёртки для удобства вызова функции приёма-передачи.
//
static inline __attribute__((always_inline)) 
int mil1553_set_mode(mil1553if_t *mil, unsigned mode)
{
    return mil->set_mode(mil, mode);
}

static inline __attribute__((always_inline)) 
int mil1553_start(mil1553if_t *mil)
{
    return mil->start(mil);
}

static inline __attribute__((always_inline)) 
int mil1553_stop(mil1553if_t *mil)
{
    return mil->stop(mil);
}

static inline __attribute__((always_inline)) 
int mil1553_is_started(mil1553if_t *mil)
{
    return mil->is_started(mil);
}

static inline __attribute__((always_inline)) 
int mil1553_read_subaddr(mil1553if_t *mil, int subaddr, void *data, int size)
{
    return mil->read_subaddr(mil, subaddr, data, size);
}

static inline __attribute__((always_inline)) 
int mil1553_write_subaddr(mil1553if_t *mil, int subaddr, void *data, int size)
{
    return mil->write_subaddr(mil, subaddr, data, size);
}

static inline __attribute__((always_inline))
int mil1553_lock(mil1553if_t *mil)
{
    return mil->lock(mil);
}

static inline __attribute__((always_inline))
int mil1553_unlock(mil1553if_t *mil)
{
    return mil->unlock(mil);
}

/*
static inline __attribute__((always_inline))
int mil1553_bc_cyclogram_add_slot(mil1553if_t *mil, mil_slot_t *slot)
{
    return mil->bc_cyclogram_add_slot(mil, slot);
}

static inline __attribute__((always_inline))
int mil1553_bc_cyclogram_clear(mil1553if_t *mil)
{
    return mil->bc_cyclogram_clear(mil);
}
*/

static inline __attribute__((always_inline))
int mil1553_bc_set_cyclogram(mil1553if_t *mil, mil_slot_t *head, unsigned nb_slots)
{
    return mil->bc_set_cyclogram(mil, head, nb_slots);
}

static inline __attribute__((always_inline))
int mil1553_bc_set_period(mil1553if_t *mil, unsigned period_ms)
{
    return mil->bc_set_period(mil, period_ms);
}

static inline __attribute__((always_inline))
int mil1553_bc_urgent_send(mil1553if_t *mil, mil_slot_desc_t descr, void *data)
{
    return mil->bc_urgent_send(mil, descr, data);
}

static inline __attribute__((always_inline))
int mil1553_bc_ordinary_send(mil1553if_t *mil, int slot_number, void *data)
{
    return mil->bc_ordinary_send(mil, slot_number, data);
}
#endif
