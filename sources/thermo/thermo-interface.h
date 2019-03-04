/*
 * Thermosensor interface.
 *
 * Copyright (C) 2018 Dmitry Podkhvatilin <vatilin@gmail.com>
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
#ifndef __THERMO_INTERFACE_H__
#define __THERMO_INTERFACE_H__

// !!! ПЕРЕПИСАТЬ ДЛЯ ДРАЙВЕРА THERMO

//
//        Интерфейс драйвера контроллера THERMO
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
#define THERMO_ERR_OK           0
// Ошибка ввода-вывода
#define THERMO_ERR_IO           -1
// Неверный параметр
#define THERMO_ERR_BAD_PARAM    -2
// Выход температуры за допустимые границы
#define THERMO_ERR_BAD_VALUE    -3


typedef struct _thermoif_t thermoif_t;

//
// Базовый тип интерфейса термодатчика
//
struct _thermoif_t
{
    // В данное поле должен быть записан указатель на аппаратно-зависимую
    // функцию, возвращающую текущее показание температуры в градусах Цельсия,
    // умноженное на 1000 (!). В случае ошибки возвращается значение 
    // (0x7FFFFFFF + код ошибки).
    int (* read)(thermoif_t *thermo);
};

#define to_thermoif(x)   ((thermoif_t*)&(x)->thermoif)

//
// Функции-обёрки для удобства вызова функций интерфейса.
//
static inline __attribute__((always_inline)) 
int thermo_read(thermoif_t *thermo)
{
    return thermo->read(thermo);
}

#endif // __THERMO_INTERFACE_H__
