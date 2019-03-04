#ifndef __GPIO_MILANDR_H__
#define __GPIO_MILANDR_H__

#include <gpio/gpio-interface.h>

typedef struct _milandr_gpio_t milandr_gpio_t;
typedef struct _milandr_gpio_hndl_list_item_t milandr_gpio_hndl_list_item_t;

struct _milandr_gpio_hndl_list_item_t
{
    list_t              item;
    
    milandr_gpio_t       *pin;
    gpio_handler_t      handler;
    void                *handler_arg;
};

struct _milandr_gpio_t
{
    gpioif_t            gpioif;
    
    GPIO_t              *reg;
    unsigned            port;
    unsigned            pin_n;
    
    int                 irq_handler_idx;
    int                 int_pending;
    
    milandr_gpio_hndl_list_item_t hndl_item;
};

typedef enum
{
    GPIO_PORT_A,
    GPIO_PORT_B,
    GPIO_PORT_C,
    GPIO_PORT_D,
    GPIO_PORT_E,
    GPIO_PORT_F
} gpio_port_t;

// Ножка настраивается в режим порта
#define GPIO_FLAGS_FUNC_PORT          0
// Ножка настраивается в режим основной функции
#define GPIO_FLAGS_FUNC_MAIN          1
// Ножка настраивается в режим альтернативной функции
#define GPIO_FLAGS_FUNC_ALT           2
// Ножка настраивается в режим альтернативной функции
#define GPIO_FLAGS_FUNC_REDEF         3
// Ножка настраивается в режим альтернативной функции
#define GPIO_FLAGS_FUNC_ANALOG        4
// Ножка настраивается в режим цифрового входа
#define GPIO_FLAGS_INPUT              (0 << 4)
// Ножка настраивается в режим цифрового выхода
#define GPIO_FLAGS_OUTPUT             (1 << 4)

// Включение подтягивающего к питанию резистора
#define GPIO_FLAGS_PULL_UP            (1 << 6)
// Включение подтягивающего к земле резистора
#define GPIO_FLAGS_PULL_DOWN          (1 << 7)
// Режим работы цифрового выхода - открытый коллектор (если флаг не 
// указан, то push-pul)
#define GPIO_FLAGS_OPEN_DRAIN         (1 << 8)
// Режим работы цифрового входа - триггер Шмитта
#define GPIO_FLAGS_SCHMITT            (1 << 9)
// Включение входного фильтра импульсов до 10 нс
#define GPIO_FLAGS_IN_FILTER          (1 << 10)

// Крутизна фронта для цифровых выходов и альтернативных функций
#define GPIO_FLAGS_LOW_SPEED          (1 << 11)
#define GPIO_FLAGS_MEDIUM_SPEED       (2 << 11)
#define GPIO_FLAGS_HIGH_SPEED         (3 << 11)

// Служебные маски
#define _IO_MODE_MASK            0x007
#define _GET_POWER(flags)        (((flags) >> 11) & 0x3)


void milandr_gpio_init(milandr_gpio_t *gpio, unsigned port, unsigned pin, unsigned flags);

#endif // __GPIO_MILANDR_H__
