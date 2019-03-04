#ifndef __STM32F4_GPIO_H__
#define __STM32F4_GPIO_H__

#include <gpio/gpio-interface.h>

typedef struct _stm32f4_gpio_t stm32f4_gpio_t;
typedef struct _stm32f4_gpio_hndl_list_item_t stm32f4_gpio_hndl_list_item_t;

struct _stm32f4_gpio_hndl_list_item_t
{
    list_t              item;
    
    stm32f4_gpio_t       *pin;
    gpio_handler_t      handler;
    void                *handler_arg;
};

struct _stm32f4_gpio_t
{
    gpioif_t            gpioif;
    
    GPIO_t              *reg;
    unsigned            pin_n;
    
    int                 irq_handler_idx;
    int                 int_pending;
    
    stm32f4_gpio_hndl_list_item_t hndl_item;
};


typedef enum
{
    GPIO_PORT_A,
    GPIO_PORT_B,
    GPIO_PORT_C,
    GPIO_PORT_D,
    GPIO_PORT_E,
    GPIO_PORT_F,
    GPIO_PORT_G,
    GPIO_PORT_H,
    GPIO_PORT_I,
    GPIO_PORT_J,
    GPIO_PORT_K
} gpio_port_t;

// Ножка настраивается в режим альтернативной функции
#define GPIO_FLAGS_ALT_SYSTEM         (0x0 << 0)
#define GPIO_FLAGS_ALT_TIM1           (0x1 << 0)
#define GPIO_FLAGS_ALT_TIM2           (0x1 << 0)
#define GPIO_FLAGS_ALT_TIM3           (0x2 << 0)
#define GPIO_FLAGS_ALT_TIM4           (0x2 << 0)
#define GPIO_FLAGS_ALT_TIM5           (0x2 << 0)
#define GPIO_FLAGS_ALT_TIM8           (0x3 << 0)
#define GPIO_FLAGS_ALT_TIM9           (0x3 << 0)
#define GPIO_FLAGS_ALT_TIM10          (0x3 << 0)
#define GPIO_FLAGS_ALT_TIM11          (0x3 << 0)
#define GPIO_FLAGS_ALT_I2C1           (0x4 << 0)
#define GPIO_FLAGS_ALT_I2C2           (0x4 << 0)
#define GPIO_FLAGS_ALT_I2C3           (0x4 << 0)
#define GPIO_FLAGS_ALT_SPI1           (0x5 << 0)
#define GPIO_FLAGS_ALT_SPI2           (0x5 << 0)
#define GPIO_FLAGS_ALT_SPI3           (0x6 << 0)
#define GPIO_FLAGS_ALT_SPI4           (0x5 << 0)
#define GPIO_FLAGS_ALT_SPI5           (0x5 << 0)
#define GPIO_FLAGS_ALT_SPI6           (0x5 << 0)
#define GPIO_FLAGS_ALT_USART1         (0x7 << 0)
#define GPIO_FLAGS_ALT_USART2         (0x7 << 0)
#define GPIO_FLAGS_ALT_USART3         (0x7 << 0)
#define GPIO_FLAGS_ALT_UART4          (0x8 << 0)
#define GPIO_FLAGS_ALT_UART5          (0x8 << 0)
#define GPIO_FLAGS_ALT_USART6         (0x8 << 0)
#define GPIO_FLAGS_ALT_UART7          (0x8 << 0)
#define GPIO_FLAGS_ALT_UART8          (0x8 << 0)
#define GPIO_FLAGS_ALT_CAN1           (0x9 << 0)
#define GPIO_FLAGS_ALT_CAN2           (0x9 << 0)
#define GPIO_FLAGS_ALT_TIM12          (0x9 << 0)
#define GPIO_FLAGS_ALT_TIM13          (0x9 << 0)
#define GPIO_FLAGS_ALT_TIM14          (0x9 << 0)
#define GPIO_FLAGS_ALT_OTG_FS         (0xA << 0)
#define GPIO_FLAGS_ALT_OTG_HS         (0xA << 0)
#define GPIO_FLAGS_ALT_ETH            (0xB << 0)
#define GPIO_FLAGS_ALT_FSMC           (0xC << 0)
#define GPIO_FLAGS_ALT_SDIO           (0xC << 0)
#define GPIO_FLAGS_ALT_OTG_HS_FS      (0xC << 0)
#define GPIO_FLAGS_ALT_DCMI           (0xD << 0)
#define GPIO_FLAGS_ALT_EVENTOUT       (0xF << 0)
// Ножка настраивается в режим цифрового входа
#define GPIO_FLAGS_INPUT              (1 << 4)
// Ножка настраивается в режим цифрового выхода
#define GPIO_FLAGS_OUTPUT             (2 << 4)
// Ножка настраивается в режим аналогового входа-выхода
#define GPIO_FLAGS_ANALOG             (3 << 4)

// Включение подтягивающего к питанию резистора
#define GPIO_FLAGS_PULL_UP            (1 << 6)
// Включение подтягивающего к земле резистора
#define GPIO_FLAGS_PULL_DOWN          (1 << 7)
// Режим работы цифрового выхода - открытый коллектор (если флаг не 
// указан, то push-pull; применимо только для цифрового выхода и 
// альтернативных функций)
#define GPIO_FLAGS_OPEN_DRAIN         (1 << 8)

// Крутизна фронта для цифровых выходов и альтернативных функций
#define GPIO_FLAGS_2MHZ               (0 << 9)  // Low speed
#define GPIO_FLAGS_25MHZ              (1 << 9)  // Medium speed
#define GPIO_FLAGS_50MHZ              (2 << 9)  // Fast speed
#define GPIO_FLAGS_100MHZ             (3 << 9)  // High speed


// Служебные маски
#define _IO_MODE_MASK            0x030
#define _ALT_FUNC_MASK           0x00F
#define _GET_POWER(flags)        (((flags) >> 9) & 0x3)


void stm32f4_gpio_init(stm32f4_gpio_t *gpio, unsigned port, unsigned pin, unsigned flags);

#endif // __STM32F4_GPIO_H__
