/*
 * Register definitions for Milandr 1986BE9x.
 *
 * Copyright (C) 2010 Serge Vakulenko, <serge@vak.ru>
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
typedef volatile unsigned int arm_reg_t;

/*
 * Memory map
 */
#define ARM_SRAM_BASE       0x20000000  /* Internal static memory */
#define ARM_PERIPH_BASE     0x40000000  /* Peripheral registers */
#define ARM_EXTBUS0_BASE    0x60000000  /* Access to external bus 0 */
#define ARM_EXTBUS1_BASE    0x90000000  /* Access to external bus 1 */
#define ARM_SYSTEM_BASE     0xE0000000  /* Core registers */

#define ARM_SRAM_SIZE       (32*1024)   /* 32 kbytes */

#include <runtime/cortex-m/io-cortex-m.h>

/*
 * Peripheral memory map
 */
#define ARM_CAN1_BASE           (ARM_PERIPH_BASE + 0x00000)
#define ARM_CAN2_BASE           (ARM_PERIPH_BASE + 0x08000)
#define ARM_USB_BASE            (ARM_PERIPH_BASE + 0x10000)
#define ARM_EEPROM_BASE         (ARM_PERIPH_BASE + 0x18000)
#define ARM_RSTCLK_BASE         (ARM_PERIPH_BASE + 0x20000)
#define ARM_DMA_BASE            (ARM_PERIPH_BASE + 0x28000)
#define ARM_UART1_BASE          (ARM_PERIPH_BASE + 0x30000)
#define ARM_UART2_BASE          (ARM_PERIPH_BASE + 0x38000)
#define ARM_SSP1_BASE           (ARM_PERIPH_BASE + 0x40000)
#define ARM_I2C1_BASE           (ARM_PERIPH_BASE + 0x50000)
#define ARM_POWER_BASE          (ARM_PERIPH_BASE + 0x58000)
#define ARM_WWDT_BASE           (ARM_PERIPH_BASE + 0x60000)
#define ARM_IWDT_BASE           (ARM_PERIPH_BASE + 0x68000)
#define ARM_TIMER1_BASE         (ARM_PERIPH_BASE + 0x70000)
#define ARM_TIMER2_BASE         (ARM_PERIPH_BASE + 0x78000)
#define ARM_TIMER3_BASE         (ARM_PERIPH_BASE + 0x80000)
#define ARM_ADC_BASE            (ARM_PERIPH_BASE + 0x88000)
#define ARM_DAC_BASE            (ARM_PERIPH_BASE + 0x90000)
#define ARM_COMP_BASE           (ARM_PERIPH_BASE + 0x98000)
#define ARM_SSP2_BASE           (ARM_PERIPH_BASE + 0xA0000)
#define ARM_GPIOA_BASE          (ARM_PERIPH_BASE + 0xA8000)
#define ARM_GPIOB_BASE          (ARM_PERIPH_BASE + 0xB0000)
#define ARM_GPIOC_BASE          (ARM_PERIPH_BASE + 0xB8000)
#define ARM_GPIOD_BASE          (ARM_PERIPH_BASE + 0xC0000)
#define ARM_GPIOE_BASE          (ARM_PERIPH_BASE + 0xC8000)
#define ARM_BKP_BASE            (ARM_PERIPH_BASE + 0xD8000)
#define ARM_GPIOF_BASE          (ARM_PERIPH_BASE + 0xE8000)
#define ARM_EXT_BUS_BASE        (ARM_PERIPH_BASE + 0xF0050)

typedef enum
{
  CAN1_IRQn                   = 0,   /* CAN1 Interrupt */
  CAN2_IRQn                   = 1,   /* CAN2 Interrupt */
  USB_IRQn                    = 2,   /* USB Host Interrupt */
  DMA_IRQn                    = 5,   /* DMA Interrupt */
  UART1_IRQn                  = 6,   /* UART1 Interrupt */
  UART2_IRQn                  = 7,   /* UART2 Interrupt */
  SSP1_IRQn                   = 8,   /* SSP1 Interrupt */
  MDR_I2C_IRQn                = 10,  /* MDR_I2C Interrupt */
  POWER_IRQn                  = 11,  /* POWER Detecor Interrupt */
  WWDG_IRQn                   = 12,  /* Window Watchdog Interrupt */
  TIMER1_IRQn                 = 14,  /* Timer1 Interrupt */
  TIMER2_IRQn                 = 15,  /* Timer2 Interrupt */
  TIMER3_IRQn                 = 16,  /* Timer3 Interrupt */
  ADC_IRQn                    = 17,  /* ADC Interrupt */
  SSP3_IRQn                   = 19,  /* SSP3 Interrupt */
  SSP2_IRQn                   = 20,  /* SSP2 Interrupt */
  BKP_IRQn                    = 27,  /* BACKUP Interrupt */
  EXT_INT1_IRQn               = 28,  /* EXT_INT1 Interrupt */
  EXT_INT2_IRQn               = 29,  /* EXT_INT2 Interrupt */
  EXT_INT3_IRQn               = 30,  /* EXT_INT3 Interrupt */
  EXT_INT4_IRQn               = 31,  /* EXT_INT4 Interrupt */
  ARCH_TIMER_IRQ              = 32,	 /* SysTick */
  ARCH_INTERRUPTS
} IRQn_t;

/*------------------------------------------------------
 * Регистры портов ввода-вывода
 */
typedef struct
{
    arm_reg_t DATA;     /* Данные для выдачи и чтения */
    arm_reg_t OE;       /* Направление, 1 - выход */
    arm_reg_t FUNC;     /* Выбор функции, два бита на порт */
    arm_reg_t ANALOG;   /* Режим работы, 1 - цифровой */
    arm_reg_t PULL;     /* Подтяжка вверх [31:16] и отключение подтяжки вниз [15:0] */
    arm_reg_t PD;       /* Триггер Шмитта входа [31:16] или открытый сток выхода [15:0] */
    arm_reg_t PWR;      /* Скорость фронта выхода, два бита на порт */
    arm_reg_t GFEN;     /* Фильтрация входа */
} GPIO_t;

#define ARM_GPIOA       ((GPIO_t*) ARM_GPIOA_BASE)
#define ARM_GPIOB       ((GPIO_t*) ARM_GPIOB_BASE)
#define ARM_GPIOC       ((GPIO_t*) ARM_GPIOC_BASE)
#define ARM_GPIOD       ((GPIO_t*) ARM_GPIOD_BASE)
#define ARM_GPIOE       ((GPIO_t*) ARM_GPIOE_BASE)
#define ARM_GPIOF       ((GPIO_t*) ARM_GPIOF_BASE)

/*
 * Регистр OE: направление передачи данных на выводах порта
 */
#define ARM_GPIO_IN(n)      (0 << (n))
#define ARM_GPIO_OUT(n)     (1 << (n))

/*
 * Регистр ANALOG: режим работы контроллера (аналоговый или цифровой)
 */
#define ARM_ANALOG(n)       (0 << (n))
#define ARM_DIGITAL(n)      (1 << (n))

/*
 * Регистр FUNC: выбор функции порта
 */
#define ARM_FUNC_MASK(n)    (3 << ((n)*2))
#define ARM_FUNC_PORT(n)    (0 << ((n)*2))  /* порт */
#define ARM_FUNC_MAIN(n)    (1 << ((n)*2))  /* основная функция */
#define ARM_FUNC_ALT(n)     (2 << ((n)*2))  /* альтернативная функция */
#define ARM_FUNC_REDEF(n)   (3 << ((n)*2))  /* переопределённая функция */

#define ARM_FUNC(n,f)       (f << ((n)*2))  /* установка выбранной функции */

/*
 * Регистр PWR: скорость фронта порта вывода
 */
#define ARM_PWR_MASK(n)     (3 << ((n)*2))
#define ARM_PWR_SLOW(n)     (1 << ((n)*2))  /* медленный фронт */
#define ARM_PWR_FAST(n)     (2 << ((n)*2))  /* быстрый фронт */
#define ARM_PWR_FASTEST(n)  (3 << ((n)*2))  /* максимально быстрый фронт */

/*
 * Регистр PULL: разрешение подтяжки вверх или вниз
 */
#define ARM_PULL_UP(n)      (1 << ((n) + 16))  /* подтяжка в питание включена (n = 0..15) */
#define ARM_PULL_DOWN(n)    (1 << (n))         /* подтяжка в ноль включена (n = 0..15) */

/*
 * Регистр PD: триггер Шмитта входа или открытый сток выхода
 */
#define ARM_PD_SCHMITT(n)   (1 << ((n) + 16))  /* триггер Шмитта включен гистерезис 400 мВ (n = 0..15) */
#define ARM_PD_OD(n)        (1 << (n))         /* открытый сток (n = 0..15) */

/*
 * Регистр GFEN: включение входного фильтра импульсов до 10 нс
 */
#define ARM_GFEN(n)         (1 << (n))         /* фильтр включен (n = 0..15) */


/*------------------------------------------------------
 * External bus
 */
typedef struct
{
    arm_reg_t NAND_CYCLES;
    arm_reg_t CONTROL;
} EXTBUS_t;

#define ARM_EXTBUS      ((EXTBUS_t*) ARM_EXT_BUS_BASE)

/*
 * Регистр EXTBUS CONTROL: управление внешней системной шиной
 */
#define ARM_EXTBUS_ROM      (1 << 0)    /* Память ROM - только чтение */
#define ARM_EXTBUS_RAM      (1 << 1)    /* Память RAM - чтение и запись */
#define ARM_EXTBUS_NAND     (1 << 2)    /* Память NAND Flash */
#define ARM_EXTBUS_CPOL     (1 << 3)    /* Отрицательная полярность CLOCK */
#define ARM_EXTBUS_DONE     (1 << 7)    /* Операция памяти NAND завершена */
#define ARM_EXTBUS_WS(x)    ((x) << 12) /* Длительность цикла = ws+3 */

/*------------------------------------------------------
 * Clock management
 */
typedef struct
{
    arm_reg_t CLOCK_STATUS;
    arm_reg_t PLL_CONTROL;
    arm_reg_t HS_CONTROL;
    arm_reg_t CPU_CLOCK;
    arm_reg_t USB_CLOCK;
    arm_reg_t ADC_MCO_CLOCK;
    arm_reg_t RTC_CLOCK;
    arm_reg_t PER_CLOCK;
    arm_reg_t CAN_CLOCK;
    arm_reg_t TIM_CLOCK;
    arm_reg_t UART_CLOCK;
    arm_reg_t SSP_CLOCK;
} RSTCLK_t;

#define ARM_RSTCLK      ((RSTCLK_t*) ARM_RSTCLK_BASE)

/*
 * Регистр CLOCK_STATUS: состояние блока управления тактовой частотой.
 */
#define ARM_CLOCK_STATUS_PLL_USB_RDY    (1 << 0) /* USB PLL запущена и стабильна */
#define ARM_CLOCK_STATUS_PLL_CPU_RDY    (1 << 1) /* CPU PLL запущена и стабильна */
#define ARM_CLOCK_STATUS_HSE_RDY        (1 << 2) /* осциллятор HSE запущен и стабилен */

/*
 * Регистр PLL_CONTROL: управление блоками умножения частоты.
 */
#define ARM_PLL_CONTROL_USB_ON      (1 << 0)    /* USB PLL включена */
#define ARM_PLL_CONTROL_USB_RLD     (1 << 1)    /* Перезапуск USB PLL */
#define ARM_PLL_CONTROL_CPU_ON      (1 << 2)    /* CPU PLL включена */
#define ARM_PLL_CONTROL_CPU_RLD     (1 << 3)    /* Перезапуск CPU PLL */
#define ARM_PLL_CONTROL_USB_MUL(n)  (((n)-1) << 4)  /* Коэффициент умножения для USB PLL */
#define ARM_PLL_CONTROL_CPU_MUL(n)  (((n)-1) << 8)  /* Коэффициент умножения для CPU PLL */

/*
 * Регистр USB_CLOCK
 */
/* Биты выбора источника для USB_C1 */
#define ARM_USB_C1_SEL(x)   ((x) << 0)
/* Биты выбора источника для USB_C2 */
#define ARM_USB_C2_SEL(x)   ((x) << 2)
/* Биты выбора делителя для USB_C3 */
#define ARM_USB_C3_SEL(x)   ((x) << 4)
/* Бит разрешения тактирования USB */
#define ARM_USB_CLK_EN      (1 << 8)
/* Возможные значения для USB_C1 */
#define ARM_USB_HSI         0
#define ARM_USB_HSI_DIV2    1
#define ARM_USB_HSE         2
#define ARM_USB_HSE_DIV2    3
/* Возможные значения для USB_C2 */
#define ARM_USB_C1          0
#define ARM_USB_PLLUSBo     1
/* Возможные значения для USB_C3 */
#define ARM_USB_C2          0
#define ARM_USB_C2_DIV2     1

/*
 * Регистр ADC_MCO_CLOCK: управление блоками АЦП
 */
/* Выбор источника для ADC_C1. Возможные значения см. ниже */
#define ARM_ADC_C1_SEL(x)   ((x) << 0)
/* Выбор источника для ADC_C2. Возможные значения см. ниже */
#define ARM_ADC_C2_SEL(x)   ((x) << 4)
/* Выбор источника для ADC_C3.
 * Делитель для ADC_C3 опеределяется из выражения ADC_C3 = ADC_C2/(ADC_C3_SEL+1) */
#define ARM_ADC_C3_SEL(x)   ((x) << 8)
/* Разрешение тактовой частоты */
#define ARM_ADC_CLK_EN      (1 << 13)
/* Возможные источники тактовой частоты для ADC_C1 */
#define ARM_ADC_CPU_C1      0
#define ARM_ADC_USB_C1      1
#define ARM_ADC_CPU_C2      2
#define ARM_ADC_USB_C2      3
/* Выбор источника для ADC_C2 */
#define ARM_ADC_LSE         0
#define ARM_ADC_LSI         1
#define ARM_ADC_ADC_C1      2
#define ARM_ADC_HSI_C1      3

/*
 * Регистр PER_CLOCK: включение тактирования периферийных блоков.
 */
#define ARM_PER_CLOCK_CAN1              (1 << 0)
#define ARM_PER_CLOCK_CAN2              (1 << 1)
#define ARM_PER_CLOCK_USB               (1 << 2)
#define ARM_PER_CLOCK_EEPROM            (1 << 3)
#define ARM_PER_CLOCK_RSTCLK            (1 << 4)
#define ARM_PER_CLOCK_DMA               (1 << 5)
#define ARM_PER_CLOCK_UART1             (1 << 6)
#define ARM_PER_CLOCK_UART2             (1 << 7)
#define ARM_PER_CLOCK_SSP1              (1 << 8)
#define ARM_PER_CLOCK_I2C1              (1 << 10)
#define ARM_PER_CLOCK_POWER             (1 << 11)
#define ARM_PER_CLOCK_WWDT              (1 << 12)
#define ARM_PER_CLOCK_IWDT              (1 << 13)
#define ARM_PER_CLOCK_TIMER1            (1 << 14)
#define ARM_PER_CLOCK_TIMER2            (1 << 15)
#define ARM_PER_CLOCK_TIMER3            (1 << 16)
#define ARM_PER_CLOCK_ADC               (1 << 17)
#define ARM_PER_CLOCK_DAC               (1 << 18)
#define ARM_PER_CLOCK_COMP              (1 << 19)
#define ARM_PER_CLOCK_SSP2              (1 << 20)
#define ARM_PER_CLOCK_GPIOA             (1 << 21)
#define ARM_PER_CLOCK_GPIOB             (1 << 22)
#define ARM_PER_CLOCK_GPIOC             (1 << 23)
#define ARM_PER_CLOCK_GPIOD             (1 << 24)
#define ARM_PER_CLOCK_GPIOE             (1 << 25)
#define ARM_PER_CLOCK_BKP               (1 << 27)
#define ARM_PER_CLOCK_GPIOF             (1 << 29)
#define ARM_PER_CLOCK_EXT_BUS           (1 << 30)

/*
 * Регистр HS_CONTROL: управление высокочастотным генератором и осциллятором.
 */
#define ARM_HS_CONTROL_HSE_ON   (1 << 0)    /* Осциллятор HSE включён */
#define ARM_HS_CONTROL_HSE_BYP  (1 << 1)    /* Режим внешнего генератора */

/*
 * Регистр CPU_CLOCK: управление тактовой частотой.
 */
#define ARM_CPU_CLOCK_C1_HSI        (0 << 0)   /* Выбор источника для CPU C1: HSI */
#define ARM_CPU_CLOCK_C1_HSI_DIV2   (1 << 0)   /* HSI/2 */
#define ARM_CPU_CLOCK_C1_HSE        (2 << 0)   /* HSE */
#define ARM_CPU_CLOCK_C1_HSE_DIV2   (3 << 0)   /* HSE/2 */
#define ARM_CPU_CLOCK_C2_PLLCPUO    (1 << 2)   /* Выбор источника для CPU C2: PLLCPUo */
#define ARM_CPU_CLOCK_C3_C2         (0 << 4)   /* Выбор делителя для CPU C3: CPU C3 = CPU C2 */
#define ARM_CPU_CLOCK_C3_C2_DIV2    (8 << 4)   /* CPU C2 / 2 */
#define ARM_CPU_CLOCK_C3_C2_DIV4    (9 << 4)   /* CPU C2 / 4 */
#define ARM_CPU_CLOCK_C3_C2_DIV8    (10 << 4)  /* CPU C2 / 8 */
#define ARM_CPU_CLOCK_C3_C2_DIV16   (11 << 4)  /* CPU C2 / 16 */
#define ARM_CPU_CLOCK_C3_C2_DIV32   (12 << 4)  /* CPU C2 / 32 */
#define ARM_CPU_CLOCK_C3_C2_DIV64   (13 << 4)  /* CPU C2 / 64 */
#define ARM_CPU_CLOCK_C3_C2_DIV128  (14 << 4)  /* CPU C2 / 128 */
#define ARM_CPU_CLOCK_C3_C2_DIV256  (15 << 4)  /* CPU C2 / 256 */
#define ARM_CPU_CLOCK_HCLK_HSI      (0 << 8)   /* Выбор источника для HCLK: HSI */
#define ARM_CPU_CLOCK_HCLK_C3       (1 << 8)   /* CPU C3 */
#define ARM_CPU_CLOCK_HCLK_LSE      (2 << 8)   /* LSE */
#define ARM_CPU_CLOCK_HCLK_LSI      (3 << 8)   /* LSI */

/*
 * Регистр CAN_CLOCK: управление тактовой частотой CAN
 * Делитель тактовой частоты:
 *  0 - HCLK
 *  1 - HCLK/2
 *  2 - HCLK/4
 *  ...
 *  7 - HCLK/128
 */
#define ARM_CAN_CLOCK_BRG1(n)   (n)         /* Делитель тактовой частоты CAN1 */
#define ARM_CAN_CLOCK_BRG2(n)   ((n) << 8)  /* Делитель тактовой частоты CAN2 */
#define ARM_CAN_CLOCK_EN1       (1 << 24)   /* Разрешение тактовой частоты на CAN1 */
#define ARM_CAN_CLOCK_EN2       (1 << 25)   /* Разрешение тактовой частоты на CAN2 */

/*
 * Регистр UART_CLOCK: управление тактовой частотой UART
 * Делитель тактовой частоты:
 *  0 - HCLK
 *  1 - HCLK/2
 *  2 - HCLK/4
 *  ...
 *  7 - HCLK/128
 */
#define ARM_UART_CLOCK_BRG1(n)      (n)         /* Делитель тактовой частоты UART1 */
#define ARM_UART_CLOCK_BRG2(n)      ((n) << 8)  /* Делитель тактовой частоты UART2 */
#define ARM_UART_CLOCK_EN1          (1 << 24)   /* Разрешение тактовой частоты на UART1 */
#define ARM_UART_CLOCK_EN2          (1 << 25)   /* Разрешение тактовой частоты на UART2 */

/*
 * Регистр SSP_CLOCK: управление тактовой частотой SSP
 * Делитель тактовой частоты:
 *  0 - HCLK
 *  1 - HCLK/2
 *  2 - HCLK/4
 *  ...
 *  7 - HCLK/128
 */
#define ARM_SSP_CLOCK_BRG1(n)   (n)         /* Делитель тактовой частоты SSP1 */
#define ARM_SSP_CLOCK_BRG2(n)   ((n) << 8)  /* Делитель тактовой частоты SSP2 */
#define ARM_SSP_CLOCK_EN1       (1 << 24)   /* Разрешение тактовой частоты на SSP1 */
#define ARM_SSP_CLOCK_EN2       (1 << 25)   /* Разрешение тактовой частоты на SSP2 */

/*
 * Регистр TIM_CLOCK: управление тактовой частотой таймеров
 */
#define ARM_TIM1_BRG(x)     ((x) << 0)  /* Делитель тактовой частоты TIM1 */
#define ARM_TIM2_BRG(x)     ((x) << 8)  /* Делитель тактовой частоты TIM2 */
#define ARM_TIM3_BRG(x)     ((x) << 16) /* Делитель тактовой частоты TIM3 */
#define ARM_TIM1_CLK_EN     (1 << 24)   /* Разрешение тактовой частоты на TIM1 */
#define ARM_TIM2_CLK_EN     (1 << 25)   /* Разрешение тактовой частоты на TIM2 */
#define ARM_TIM3_CLK_EN     (1 << 26)   /* Разрешение тактовой частоты на TIM3 */

/*------------------------------------------------------
 * I2C
 */
typedef struct
{
    arm_reg_t PRL;          /* Младшая часть предделителя частоты */
    arm_reg_t PRH;          /* Старшая часть пред делителя частоты */
    arm_reg_t CTR;          /* Управление контроллером I2C */
    arm_reg_t RXD;          /* Принятые данные по I2С */
    arm_reg_t STA;          /* Статус I2C */
    arm_reg_t TXD;          /* Передаваемые данные по I2C */
    arm_reg_t CMD;          /* Управление I2С */
} I2C_t;

#define ARM_I2C             ((I2C_t*) ARM_I2C1_BASE)

/*
 * Регистр I2C CTR: Управление контроллером I2C
 */
#define I2C_EN_I2C          (1 << 7)
#define I2C_EN_INT          (1 << 6)
#define I2C_S_I2C           (1 << 5)

/*
 * Регистр I2C STA: Статус I2C
 */
#define I2C_RX_NACK         (1 << 7)
#define I2C_BUSY            (1 << 6)
#define I2C_LOST_ARB        (1 << 5)
#define I2C_TR_PROG         (1 << 1)
#define I2C_INT             (1 << 0)

/*
 * Регистр I2C CMD: Управление I2С 
 */
#define I2C_START           (1 << 7)
#define I2C_STOP            (1 << 6)
#define I2C_RD              (1 << 5)
#define I2C_WR              (1 << 4)
#define I2C_NACK            (1 << 3)
#define I2C_CLR_INT         (1 << 0)


/*------------------------------------------------------
 * UART
 */
typedef struct
{
    arm_reg_t DR;           /* Данные */
    arm_reg_t RSR;          /* Состояние приёмника и сброс ошибки */
    unsigned reserved0 [4];
    arm_reg_t FR;           /* Флаги */
    unsigned reserved1;
    arm_reg_t ILPR;         /* Управление ИК-обменом */
    arm_reg_t IBRD;         /* Делитель скорости */
    arm_reg_t FBRD;         /* Дробная часть делителя */
    arm_reg_t LCRH;         /* Управление линией */
    arm_reg_t CTL;          /* Управление */
    arm_reg_t IFLS;         /* Порог прерывания FIFO */
    arm_reg_t IM;           /* Маска прерывания */
    arm_reg_t RIS;          /* Состояние прерываний */
    arm_reg_t MIS;          /* Состояние прерываний с маскированием */
    arm_reg_t ICR;          /* Сброс прерывания */
    arm_reg_t DMACTL;       /* Управление DMA */
} UART_t;

typedef struct
{
    arm_reg_t PERIPHID0;
    arm_reg_t PERIPHID1;
    arm_reg_t PERIPHID2;
    arm_reg_t PERIPHID3;
    arm_reg_t PCELLID0;
    arm_reg_t PCELLID1;
    arm_reg_t PCELLID2;
    arm_reg_t PCELLID3;
} UARTTEST_t;

#define ARM_UART1           ((UART_t*) ARM_UART1_BASE)
#define ARM_UART2           ((UART_t*) ARM_UART2_BASE)
#define ARM_UART1TEST       ((UARTTEST_t*) (ARM_UART1_BASE + 0x0FE0))
#define ARM_UART2TEST       ((UARTTEST_t*) (ARM_UART2_BASE + 0x0FE0))

/*
 * Регистр UART DR: данные и флаги
 */
#define ARM_UART_DR_OE      (1 << 11)   /* Переполнение буфера приемника */
#define ARM_UART_DR_BE      (1 << 10)   /* Разрыв линии (break) */
#define ARM_UART_DR_PE      (1 << 9)    /* Ошибка контроля четности */
#define ARM_UART_DR_FE      (1 << 8)    /* Ошибка в структуре кадра */
#define ARM_UART_DR_DATA    0xFF        /* Данные */

/*
 * Регистр UART RSR: состояние приёмника и сброс ошибки
 */
#define ARM_UART_RSR_OE     (1 << 3)    /* Переполнение буфера приемника */
#define ARM_UART_RSR_BE     (1 << 2)    /* Разрыв линии (break) */
#define ARM_UART_RSR_PE     (1 << 1)    /* Ошибка контроля четности */
#define ARM_UART_RSR_FE     (1 << 0)    /* Ошибка в структуре кадра */

/*
 * Регистр UART FR: флаги
 */
#define ARM_UART_FR_RI      (1 << 8)    /* Инверсия линии /UARTRI */
#define ARM_UART_FR_TXFE    (1 << 7)    /* Буфер FIFO передатчика пуст */
#define ARM_UART_FR_RXFF    (1 << 6)    /* Буфер FIFO приемника заполнен */
#define ARM_UART_FR_TXFF    (1 << 5)    /* Буфер FIFO передатчика заполнен */
#define ARM_UART_FR_RXFE    (1 << 4)    /* Буфер FIFO приемника пуст */
#define ARM_UART_FR_BUSY    (1 << 3)    /* UART занят */
#define ARM_UART_FR_DCD     (1 << 2)    /* Инверсия линии /UARTDCD */
#define ARM_UART_FR_DSR     (1 << 1)    /* Инверсия линии /UARTDSR */
#define ARM_UART_FR_CTS     (1 << 0)    /* Инверсия линии /UARTCTS */

/*
 * Регистры UART IBRD и FBRD: делитель скорости
 */
#define ARM_UART_IBRD(mhz,baud) ((mhz) / (baud) / 16)
#define ARM_UART_FBRD(mhz,baud) (((mhz) * 4 / (baud)) & 077)

/*
 * Регистр UART LCRH: управление линией.
 */
#define ARM_UART_LCRH_SPS   (1 << 7)    /* Фиксация значения бита чётности */
#define ARM_UART_LCRH_WLEN5 (0 << 5)    /* Длина слова 5 бит */
#define ARM_UART_LCRH_WLEN6 (1 << 5)    /* Длина слова 6 бит */
#define ARM_UART_LCRH_WLEN7 (2 << 5)    /* Длина слова 7 бит */
#define ARM_UART_LCRH_WLEN8 (3 << 5)    /* Длина слова 8 бит */
#define ARM_UART_LCRH_FEN   (1 << 4)    /* Разрешение работы FIFO */
#define ARM_UART_LCRH_STP2  (1 << 3)    /* Два стоповых бита */
#define ARM_UART_LCRH_EPS   (1 << 2)    /* Чётность (0) или нечётность (1) */
#define ARM_UART_LCRH_PEN   (1 << 1)    /* Разрешение чётности */
#define ARM_UART_LCRH_BRK   (1 << 0)    /* Разрыв линии (break) */

/*
 * Регистр UART CTL: управление.
 */
#define ARM_UART_CTL_CTSEN  (1 << 15)   /* Управление потоком данных по CTS */
#define ARM_UART_CTL_RTSEN  (1 << 14)   /* Управление потоком данных по RTS */
#define ARM_UART_CTL_OUT2   (1 << 13)   /* Инверсия сигнала /UARTOut2 */
#define ARM_UART_CTL_OUT1   (1 << 12)   /* Инверсия сигнала /UARTOut1 */
#define ARM_UART_CTL_RTS    (1 << 11)   /* Инверсия сигнала /UARTRTS */
#define ARM_UART_CTL_DTR    (1 << 10)   /* Инверсия сигнала /UARTDTR */
#define ARM_UART_CTL_RXE    (1 << 9)    /* Прием разрешен */
#define ARM_UART_CTL_TXE    (1 << 8)    /* Передача разрешена */
#define ARM_UART_CTL_LBE    (1 << 7)    /* Шлейф разрешен */
#define ARM_UART_CTL_SIRLP  (1 << 2)    /* ИК-обмен с пониженным энергопотреблением */
#define ARM_UART_CTL_SIREN  (1 << 1)    /* Разрешение ИК передачи данных IrDA SIR */
#define ARM_UART_CTL_UARTEN (1 << 0)    /* Разрешение работы приемопередатчика */

/*
 * Регистр UART IFLS: пороги FIFO.
 */
#define ARM_UART_IFLS_RX_1_8    (0 << 3)    /* Приём: 1/8 буфера */
#define ARM_UART_IFLS_RX_1_4    (1 << 3)    /* Приём: 1/4 буфера */
#define ARM_UART_IFLS_RX_1_2    (2 << 3)    /* Приём: 1/2 буфера */
#define ARM_UART_IFLS_RX_3_4    (3 << 3)    /* Приём: 3/4 буфера */
#define ARM_UART_IFLS_RX_7_8    (4 << 3)    /* Приём: 7/8 буфера */
#define ARM_UART_IFLS_TX_1_8    (0 << 0)    /* Передача: 1/8 буфера */
#define ARM_UART_IFLS_TX_1_4    (1 << 0)    /* Передача: 1/4 буфера */
#define ARM_UART_IFLS_TX_1_2    (2 << 0)    /* Передача: 1/2 буфера */
#define ARM_UART_IFLS_TX_3_4    (3 << 0)    /* Передача: 3/4 буфера */
#define ARM_UART_IFLS_TX_7_8    (4 << 0)    /* Передача: 7/8 буфера */

/*
 * Регистр UART RIS: состояние прерываний.
 * Регистр UART IMSC: маска прерывания.
 * Регистр UART MIS: состояние прерываний с маскированием.
 * Регистр UART ICR: cброс прерывания.
 */
#define ARM_UART_RIS_OE     (1 << 10)   /* Переполнение буфера */
#define ARM_UART_RIS_BE     (1 << 9)    /* Разрыв линии */
#define ARM_UART_RIS_PE     (1 << 8)    /* Ошибка контроля четности */
#define ARM_UART_RIS_FE     (1 << 7)    /* Ошибка в структуре кадра */
#define ARM_UART_RIS_RT     (1 << 6)    /* Таймаут приема данных */
#define ARM_UART_RIS_TX     (1 << 5)    /* Прерывание от передатчика */
#define ARM_UART_RIS_RX     (1 << 4)    /* Прерывание от приемника */
#define ARM_UART_RIS_DSRM   (1 << 3)    /* Изменение состояния /UARTDSR */
#define ARM_UART_RIS_DCDM   (1 << 2)    /* Изменение состояния /UARTDCD */
#define ARM_UART_RIS_CTSM   (1 << 1)    /* Изменение состояния /UARTCTS */
#define ARM_UART_RIS_RIM    (1 << 0)    /* Изменение состояния /UARTRI */

/*
 * Регистр UART DMACTL: управление DMA.
 */
#define ARM_UART_DMACTL_ONERR   (1 << 2)    /* Блокирование при ошибке */
#define ARM_UART_DMACTL_TXE     (1 << 1)    /* Использование ПДП при передаче */
#define ARM_UART_DMACTL_RXE     (1 << 0)    /* Использование ПДП при приеме */

/*------------------------------------------------------
 * Synchronous serial port
 */
typedef struct
{
    arm_reg_t CR0;          /* Управление 0 */
    arm_reg_t CR1;          /* Управление 1 */
    arm_reg_t DR;           /* Буфер данных FIFO */
    arm_reg_t SR;           /* Состояние */
    arm_reg_t CPSR;         /* Делитель тактовой частоты */
    arm_reg_t IM;           /* Маска прерывания */
    arm_reg_t RIS;          /* Состояние прерываний */
    arm_reg_t MIS;          /* Состояние прерываний с учётом маскирования */
    arm_reg_t ICR;          /* Сброс прерываний */
    arm_reg_t DMACR;        /* Управление DMA */
} SSP_t;

typedef struct
{
    arm_reg_t PERIPHID0;
    arm_reg_t PERIPHID1;
    arm_reg_t PERIPHID2;
    arm_reg_t PERIPHID3;
    arm_reg_t PCELLID0;
    arm_reg_t PCELLID1;
    arm_reg_t PCELLID2;
    arm_reg_t PCELLID3;
} SSPTEST_t;

#define ARM_SSP1            ((SSP_t*) ARM_SSP1_BASE)
#define ARM_SSP2            ((SSP_t*) ARM_SSP2_BASE)
#define ARM_SSP1TEST        ((SSPTEST_t*) (ARM_SSP1_BASE + 0x0FE0))
#define ARM_SSP2TEST        ((SSPTEST_t*) (ARM_SSP2_BASE + 0x0FE0))

/*
 * Регистр SSP CR0 - управление 0
 */
#define ARM_SSP_CR0_SCR(n)  ((n) << 8)  /* Скорость обмена */
#define ARM_SSP_CR0_SPH     (1 << 7)    /* Фаза сигнала CLKOUT */
#define ARM_SSP_CR0_SPO     (1 << 6)    /* Полярность сигнала CLKOUT */
#define ARM_SSP_CR0_FRF_SPI (0 << 4)    /* Протокол SPI фирмы Motorola */
#define ARM_SSP_CR0_FRF_SSI (1 << 4)    /* Протокол SSI фирмы Texas Instruments */
#define ARM_SSP_CR0_FRF_MW  (2 << 4)    /* Протокол Microwire фирмы National Semiconductor */
#define ARM_SSP_CR0_DSS(n)  ((n)-1)     /* Размер данных в битах, 4..16 */

/*
 * Регистр SSP CR1 - управление 1
 */
#define ARM_SSP_CR1_SOD     (1 << 3)    /* Запрет TXD в режиме ведомого */
#define ARM_SSP_CR1_MS      (1 << 2)    /* Ведомый режим работы */
#define ARM_SSP_CR1_SSE     (1 << 1)    /* Разрешение работы */
#define ARM_SSP_CR1_LBM     (1 << 0)    /* Режим шлейфа */

/*
 * Регистр SSP SR - состояние
 */
#define ARM_SSP_SR_BSY      (1 << 4)    /* Идёт передача или приём */
#define ARM_SSP_SR_RFF      (1 << 3)    /* Буфер FIFO приемника заполнен */
#define ARM_SSP_SR_RNE      (1 << 2)    /* Буфер FIFO приемника не пуст */
#define ARM_SSP_SR_TNF      (1 << 1)    /* Буфер FIFO передатчика не заполнен */
#define ARM_SSP_SR_TFE      (1 << 0)    /* Буфер FIFO передатчика пуст */

/*
 * Регистры SSP IM, RIS, MIS, ICR - маски и статусы прерывания
 */
#define ARM_SSP_IM_TX       (1 << 3)    /* 50% буфера FIFO передатчика */
#define ARM_SSP_IM_RX       (1 << 2)    /* 50% буфера FIFO приёмника */
#define ARM_SSP_IM_RT       (1 << 1)    /* Таймаут приёмника */
#define ARM_SSP_IM_ROR      (1 << 0)    /* Переполнение буфера приёмника */

/*
 * Регистр SSP DMACR - управление DMA
 */
#define ARM_SSP_DMACR_TX    (1 << 1)    /* Использование DMA при передаче */
#define ARM_SSP_DMACR_RX    (1 << 0)    /* Использование DMA при приёме */

/*------------------------------------------------------
 * Timers
 */
typedef struct
{
    arm_reg_t TIM_CNT;
    arm_reg_t TIM_PSG;
    arm_reg_t TIM_ARR;
    arm_reg_t TIM_CNTRL;
    arm_reg_t TIM_CCR1;
    arm_reg_t TIM_CCR2;
    arm_reg_t TIM_CCR3;
    arm_reg_t TIM_CCR4;
    arm_reg_t TIM_CH1_CNTRL;
    arm_reg_t TIM_CH2_CNTRL;
    arm_reg_t TIM_CH3_CNTRL;
    arm_reg_t TIM_CH4_CNTRL;
    arm_reg_t TIM_CH1_CNTRL1;
    arm_reg_t TIM_CH2_CNTRL1;
    arm_reg_t TIM_CH3_CNTRL1;
    arm_reg_t TIM_CH4_CNTRL1;
    arm_reg_t TIM_CH1_DTG;
    arm_reg_t TIM_CH2_DTG;
    arm_reg_t TIM_CH3_DTG;
    arm_reg_t TIM_CH4_DTG;
    arm_reg_t TIM_BRKETR_CNTRL;
    arm_reg_t TIM_STATUS;
    arm_reg_t TIM_IE;
    arm_reg_t TIM_DMA_RE;
    arm_reg_t TIM_CH1_CNTRL2;
    arm_reg_t TIM_CH2_CNTRL2;
    arm_reg_t TIM_CH3_CNTRL2;
    arm_reg_t TIM_CH4_CNTRL2;
    arm_reg_t TIM_CCR11;
    arm_reg_t TIM_CCR21;
    arm_reg_t TIM_CCR31;
    arm_reg_t TIM_CCR41;
} TIMER_t;

#define ARM_TIMER1      ((TIMER_t*) ARM_TIMER1_BASE)
#define ARM_TIMER2      ((TIMER_t*) ARM_TIMER2_BASE)
#define ARM_TIMER3      ((TIMER_t*) ARM_TIMER3_BASE)

/*
 * Регистр TIM_CNTRL
 */
#define ARM_TIM_CNT_EN       (1 << 0)    /* Разрешение работы таймера */
#define ARM_TIM_ARRB_EN      (1 << 1)    /* Разрешение мгновенного обновления ARR */
#define ARM_TIM_WR_CMPL      (1 << 2)    /* Окончание записи, при задании
                                          * нового значения регистров CNT, PSG и ARR */
#define ARM_TIM_DIR          (1 << 3)    /* Направление счета основного счетчика.
                                          * Возможные значения см. ниже */
#define ARM_TIM_FDTS(x)      ((x) << 4)  /* Частота семплирования данных FDTS */
#define ARM_TIM_CNT_MODE(x)  ((x) << 6)  /* Режим счета основного счетчика */
#define ARM_TIM_EVENT_SEL(x) ((x) << 8)  /* Биты выбора источника событий */

/* Направления счета таймера */
#define ARM_TIM_DIR_UP      0       /* вверх, от 0 до ARR */
#define ARM_TIM_DIR_DOWN    1       /* вниз, от ARR до 0 */

/*
 * Регистры TIM_CHx_CNTRL
 */
#define ARM_TIM_CHFLTR(x)   ((x) << 0)  /* Где зафиксирован сигнал */
#define ARM_TIM_CHSEL(x)    ((x) << 4)  /* Выбор события по входному каналу */
#define ARM_TIM_CHPSC(x)    ((x) << 6)  /* Предварительный делитель входного канала */
#define ARM_TIM_OCCE        (1 << 8)    /* Разрешение работы ETR */
#define ARM_TIM_OCCM(x)     ((x) << 9)  /* Формат выработки сигнала REF в режиме ШИМ */
#define ARM_TIM_BRKEN       (1 << 12)   /* Разрешение сброса по выводу BRK */
#define ARM_TIM_ETREN       (1 << 13)   /* Разрешения сброса по выводу ETR */
#define ARM_TIM_WR_CMPL_CCR (1 << 14)   /* Флаг окончания записи, при задании
                                         * нового значения регистра CCR */
#define ARM_TIM_CAP_nPWM    (1 << 15)   /* Режим работы канала Захват или ШИМ:
                                         * 1 – канал работает в режиме Захват
                                         * 0 – канал работает в режиме ШИМ */

/*
 * Регистры TIM_CHx_CNTRL1
 */
#define ARM_TIM_SELOE(x)    ((x) << 0)  /* Режим работы канала на выход */
#define ARM_TIM_SELO(x)     ((x) << 2)  /* Режим работы выхода канала */
#define ARM_TIM_INV         (1 << 4)    /* Режим выходной инверсии */
#define ARM_TIM_NSELOE(x)   ((x) << 8)  /* Режим работы канала на выход */
#define ARM_TIM_NSELO(x)    ((x) << 10) /* Режим работы выхода канала */
#define ARM_TIM_NINV        (1 << 12)   /* Режим выходной инверсии */

/*
 * Регистры TIM_CHx_CNTRL2
 */
#define ARM_TIM_CHSEL1(x)   ((x) << 0)  /* Выбор события по входному каналу для CAP1 */
#define ARM_TIM_CCR1_EN     (1 << 2)    /* Разрешение работы регистра CCR1 */
#define ARM_TIM_CRRRLD      (1 << 3)    /* Разрешение обновления регистров CCR и CCR1 */

/*
 * Регистры TIM_CHx_DTG
 */
#define ARM_TIM_DTG(x)      ((x) << 0)  /* Предварительный делитель частоты DTG */
#define ARM_TIM_EDTS        (1 << 4)    /* Частота работы DTG */
#define ARM_TIM_DTGx(x)     ((x) << 8)  /* Основной делитель частоты.
                                         * Задержка DTGdel = DTGx*(DTG+1) */

/*
 * Регистр TIM_BRKETR_CNTRL
 */
#define ARM_TIM_BRK_INV         (1 << 0)    /* Инверсия входа BRK */
#define ARM_TIM_ETR_INV         (1 << 1)    /* Инверсия входа ETR */
#define ARM_TIM_ETRPSC(x)       ((x) << 2)  /* Асинхронный пред. делитель внешней частоты */
#define ARM_TIM_ETR_FILTER(x)   ((x) << 4)  /* Цифровой фильтр на входе ETR.
                                             * (Где зафиксирован сигнал) */

/*
 * Регистр TIM_STATUS
 */
#define ARM_TIM_CNT_ZERO_EVENT      (1 << 0)    /* Событие совпадения CNT с нулем */
#define ARM_TIM_CNT_ARR_EVENT       (1 << 1)    /* Событие совпадения CNT с ARR */
#define ARM_TIM_ETR_RE_EVENT        (1 << 2)    /* Событие переднего фронта на входе ETR */
#define ARM_TIM_ETR_FE_EVENT        (1 << 3)    /* Событие заднего фронта на входе ETR */
#define ARM_TIM_BRK_EVENT           (1 << 4)    /* Триггерированное по PCLK состояние входа BRK,
                                                 * 0 – BRK == 0, 1 – BRK == 1 */
#define ARM_TIM_CCR_CAP_EVENT(x)    ((x) << 5)  /* Событие переднего фронта на входе CAP каналов таймера */
#define ARM_TIM_CCR_REF_EVENT(x)    ((x) << 9)  /* Событие переднего фронта на входе REF каналов таймера */
#define ARM_TIM_CCR_CAP1_EVENT(x)   ((x) << 13) /* Событие переднего фронта на входе CAP1 каналов таймера */

/*
 * Регистр TIM_IE
 */
/* Флаг разрешения прерывания по событию совпадения CNT и нуля */
#define ARM_TIM_CNT_ZERO_EVENT_IE   (1 << 0)
/* Флаг разрешения прерывания по событию совпадения CNT и ARR */
#define ARM_TIM_CNT_ARR_EVENT_IE    (1 << 1)
/* Флаг разрешения прерывания по переднему фронту на входе ETR */
#define ARM_TIM_ETR_RE_EVENT_IE     (1 << 2)
/* Флаг разрешения прерывания по заднему фронту на входе ETR */
#define ARM_TIM_ETR_FE_EVENT_IE     (1 << 3)
/* Флаг разрешения прерывания по событию переднего фронта на
 * выходе CAP каналов таймера. n - номер канала */
#define ARM_TIM_CCR_CAP_EVENT_IE(n) ((1 << (n)) << 5)
/* Флаг разрешения прерывания по событию переднего фронта на
 * выходе REF каналов таймера. n - номер канала */
#define ARM_TIM_CCR_CCR_EVENT_IE(n) ((1 << (n)) << 9)
/* Флаг разрешения прерывания по событию переднего фронта на
 * выходе CAP1 каналов таймера. n - номер канала */
#define ARM_TIM_CCR_CAP1_EVENT_IE(n)    ((1 << (n)) << 13)

/*
 * Регистр TIM_DMA_RE
 */
/* Флаг разрешения запроса DMA по событию совпадения CNT и нуля */
#define ARM_TIM_CNT_ZERO_EVENT_RE   (1 << 0)
/* Флаг разрешения запроса DMA по событию совпадения CNT и ARR */
#define ARM_TIM_CNT_ARR_EVENT_RE    (1 << 1)
/* Флаг разрешения запроса DMA по переднему фронту на входе ETR */
#define ARM_TIM_ETR_RE_EVENT_RE     (1 << 2)
/* Флаг разрешения запроса DMA по заднему фронту на входе ETR */
#define ARM_TIM_ETR_FE_EVENT_RE     (1 << 3)
/* Флаг разрешения запроса DMA по событию переднего фронта на
 * выходе CAP каналов таймера. n - номер канала */
#define ARM_TIM_CCR_CAP_EVENT_RE(n) ((1 << (n)) << 5)
/* Флаг разрешения запроса DMA по событию переднего фронта на
 * выходе REF каналов таймера. n - номер канала */
#define ARM_TIM_CCR_CCR_EVENT_RE(n) ((1 << (n)) << 9)
/* Флаг разрешения запроса DMA по событию переднего фронта на
 * выходе CAP1 каналов таймера. n - номер канала */
#define ARM_TIM_CCR_CAP1_EVENT_RE(n)    ((1 << (n)) << 13)

/*------------------------------------------------------
 * Battery backup
 */
typedef struct
{
    arm_reg_t BKP_REG_00;       /* Регистр аварийного сохранения 0 */
    unsigned reserved1 [13];
    arm_reg_t BKP_REG_0E;       /* Регистр аварийного сохранения 14 */
    arm_reg_t BKP_REG_0F;       /* Регистр аварийного сохранения 15 и
                                 * управления блоками RTC, LSE, LSI и HSI */
    arm_reg_t RTC_CNT;          /* Регистр основного счетчика часов реального времени */
    arm_reg_t RTC_DIV;          /* Регистр предварительного делителя основного счетчика */
    arm_reg_t RTC_PRL;          /* Регистр основания счета предварительного делителя */
    arm_reg_t RTC_ALRM;         /* Регистр значения для сравнения основного счетчика и
                                 * выработки сигнала ALRF */
    arm_reg_t RTC_CS;           /* Регистр управления и состояния флагов
                                 * часов реального времени */
} BACKUP_t;

#define ARM_BACKUP      ((BACKUP_t*) ARM_BKP_BASE)

/*
 * Регистр BKP_REG_0E.
 */
#define ARM_BKP_REG_0E_LOW          (7 << 0)    /* Выбор режима работы регулятора 1.8В */
#define ARM_BKP_REG_0E_SELECT_RI    (7 << 3)    /* Выбор дополнительной нагрузки для
                                                 * регулятора 1.8В */
#define ARM_BKP_REG_0E_JTAG_A       (1 << 6)    /* Разрешение работы порта JTAG A */
#define ARM_BKP_REG_0E_JTAG_B       (1 << 7)    /* Разрешение работы порта JTAG B */
#define ARM_BKP_REG_0E_TRIM         (7 << 8)    /* Коэффициент настройки опорного
                                                 * напряжения регулятора */
#define ARM_BKP_REG_0E_FPOR         (1 << 11)   /* Флаг срабатывания POR */

/*------------------------------------------------------
 * Universal Serial Bus
 */
typedef struct
{
    arm_reg_t CTRL;     // Управление очередью нулевой оконечной точки
    arm_reg_t STS;      // Состояние оконечной точки
    arm_reg_t TS;       // Состояние типа передачи оконечной точки
    arm_reg_t NTS;      // Состояние передачи NAK оконечной точки
} EndPointStatusRegs_t;

typedef struct
{
    arm_reg_t RXFD;     // Принятые данные оконечной точки
    unsigned gap1;
    arm_reg_t RXFDC_L;  // Число данных в оконечной точке (мл. часть)
    arm_reg_t RXFDC_H;  // Число данных в оконечной точке (ст. часть)
    arm_reg_t RXFC;     // Управление очередью приема оконечной точки
    unsigned gap2 [11];
    arm_reg_t TXFD;     // Данные для передачи через оконечную точку
    unsigned gap3 [3];
    arm_reg_t TXFC;     // Управление очередью передачи оконечной точки
    unsigned gap4 [11];
} EndPointFifoRegs_t;

typedef struct
{
    // Host Regs
    arm_reg_t HTXC;         // Регистр управления передачей пакетов со стороны хоста
    arm_reg_t HTXT;         // Регистр задания типа передаваемых пакетов со стороны хоста
    arm_reg_t HTXLC;        // Регистр управления линиями шины USB
    arm_reg_t HTXSE;        // Регистр управление автоматической отправки SOF
    arm_reg_t HTXA;         // Регистр задания адреса устройства для отправки пакета
    arm_reg_t HTXE;         // Регистр задания номера оконечной точки для отправки пакета
    arm_reg_t HFN_L;        // Регистр задания номера фрейма для отправки SOF (мл. часть)
    arm_reg_t HFN_H;        // Регистр задания номера фрейма для отправки SOF (ст. часть)
    arm_reg_t HIS;          // Регистр флагов событий контроллера хостa
    arm_reg_t HIM;          // Регистр флагов разрешения прерываний по событиям контроллера хоста
    arm_reg_t HRXS;         // Регистр состояния очереди приема данных хоста
    arm_reg_t HRXP;         // Регистр отображения PID принятого пакета
    arm_reg_t HRXA;         // Регистр отображения адреса устройства, от которого принят пакет
    arm_reg_t HRXE;         // Регистр отображения номер оконечной точки, от которой принят пакет
    arm_reg_t HRXCS;        // Регистр отображения состояния подсоединения устройства
    arm_reg_t HSTM;         // Регистр расчета времени фрейма
    unsigned gap1 [16];
    arm_reg_t HRXFD;        // Данные очереди приема
    unsigned gap2;
    arm_reg_t HRXDC_L;      // Число принятых данных в очереди (мл. часть)
    arm_reg_t HRXDC_H;      // Число принятых данных в очереди (ст. часть)
    arm_reg_t HRXFC;        // Управление очередью приема
    unsigned gap3 [11];
    arm_reg_t HTXFD;        // Данные для передачи
    unsigned gap4 [3];
    arm_reg_t HTXFC;        // Управление очередью передачи
    unsigned gap5 [11];

    // Slave Regs
    EndPointStatusRegs_t SEPS [4];

    arm_reg_t SC;           // Управление контроллеров SLAVE
    arm_reg_t SLS;          // Отображение состояния линий USB шины
    arm_reg_t SIS;          // Флаги событий контроллера SLAVE
    arm_reg_t SIM;          // Флаги разрешения прерываний от контроллера SLAVE
    arm_reg_t SA;           // Функциональный адрес контроллера
    arm_reg_t SFN_L;        // Номер фрейма (мл. часть)
    arm_reg_t SFN_H;        // Номер фрейма (ст. часть)
    unsigned gap6 [9];

    EndPointFifoRegs_t SEPF [4];

    arm_reg_t HSCR;         // Общее управление для контроллера USB интерфейса
    arm_reg_t HSVR;         // Версия аппаратного контроллера USB интерфейса
} USB_t;

#define ARM_USB         ((USB_t*) ARM_USB_BASE)

/*
 * HSCR
 */
#define ARM_USB_HOST_MODE           (1 << 0)
#define ARM_USB_RESET_CORE          (1 << 1)
#define ARM_USB_EN_TX               (1 << 2)
#define ARM_USB_EN_RX               (1 << 3)
#define ARM_USB_D_PLUS_PULLUP       (1 << 4)
#define ARM_USB_D_PLUS_PULLDOWN     (1 << 5)
#define ARM_USB_D_MINUS_PULLUP      (1 << 6)
#define ARM_USB_D_MINUS_PULLDOWN    (1 << 7)

/*
 * HXTC
 */
#define ARM_USB_TREQ                (1 << 0)
#define ARM_USB_SOFSYNC             (1 << 1)
#define ARM_USB_PREEN               (1 << 2)
#define ARM_USB_ISOEN               (1 << 3)

/*
 * HTXT, Slave TS
 */
#define ARM_USB_SETUP_TRANS         0
#define ARM_USB_IN_TRANS            1
#define ARM_USB_OUTDATA_TRANS       2 // for TS
#define ARM_USB_OUTDATA0_TRANS      2
#define ARM_USB_OUTDATA1_TRANS      3

/*
 * HTXLC
 */
#define ARM_USB_TXLC(x)             ((x) & 0x3)
#define ARM_USB_DC                  (1 << 2)
#define ARM_USB_FSPL                (1 << 3)
#define ARM_USB_FSLR                (1 << 4)

/*
 * HIS, HIM
 */
#define ARM_USB_TDONE               (1 << 0)
#define ARM_USB_RESUME              (1 << 1)
#define ARM_USB_CONEV               (1 << 2)
#define ARM_USB_SOFS                (1 << 3)

/*
 * HRXS
 */
#define ARM_USB_CRCERR              (1 << 0)
#define ARM_USB_BSERR               (1 << 1)
#define ARM_USB_RXOF                (1 << 2)
#define ARM_USB_RXTO                (1 << 3)
#define ARM_USB_NAK_RXED            (1 << 4)
#define ARM_USB_STALL_RXED          (1 << 5)
#define ARM_USB_ACK_RXED            (1 << 6)
#define ARM_USB_DATASEQ             (1 << 7)

/*
 * Slave CTRL
 */
#define ARM_USB_EPEN                (1 << 0)
#define ARM_USB_EPRDY               (1 << 1)
#define ARM_USB_EPDATASEQ           (1 << 2)
#define ARM_USB_EPSSTALL            (1 << 3)
#define ARM_USB_EPISOEN             (1 << 4)

/*
 * Slave STS
 */
#define ARM_USB_SC_CRC_ERR          (1 << 0)
#define ARM_USB_SC_BS_ERR           (1 << 1)
#define ARM_USB_SC_RXOF             (1 << 2)
#define ARM_USB_SC_RXTO             (1 << 3)
#define ARM_USB_NAK_SENT            (1 << 4)
#define ARM_USB_SC_STALL_SENT       (1 << 5)
#define ARM_USB_SC_ACK_RXED         (1 << 6)
#define ARM_USB_SC_DATA_SEQ         (1 << 7)

/*
 * Slave TS, NTS
 */
#define ARM_USB_SC_SETUP_TRANS      0
#define ARM_USB_SC_IN_TRANS         1
#define ARM_USB_SC_OUTDATA_TRANS    2

/*
 * SC
 */
#define ARM_USB_SCGEN               (1 << 0)
#define ARM_USB_SCTXL_D_MINUS       (1 << 1)
#define ARM_USB_SCTXL_D_PLUS        (1 << 2)
#define ARM_USB_SCDC                (1 << 3)
#define ARM_USB_SCFSP               (1 << 4)
#define ARM_USB_SCFSR               (1 << 5)

/*
 * SLS
 */
#define ARM_USB_SCRXLS_RESET        (1 << 0)
#define ARM_USB_SCRXLS_LS           (1 << 1)
#define ARM_USB_SCRXLS_FS           (1 << 2)

/*
 * SIS, SIM
 */
#define ARM_USB_SC_TDONE             (1 << 0)
#define ARM_USB_SC_RESUME           (1 << 1)
#define ARM_USB_SC_RESET_EV         (1 << 2)
#define ARM_USB_SC_SOF_REC          (1 << 3)
#define ARM_USB_SC_NAK_SENT         (1 << 4)


/*------------------------------------------------------
 * DMA Controller
 */
typedef struct
{
    arm_reg_t STATUS;               // DMA status
    arm_reg_t CONFIG;               // DMA configuration
    arm_reg_t CTRL_BASE_PTR;        // Channel control data base pointer
    arm_reg_t ALT_CTRL_BASE_PTR;    // Channel alternate control data base pointer
    arm_reg_t WAITONREQ_STATUS;     // Channel wait on request status
    arm_reg_t CHNL_SW_REQUEST;      // Channel software request
    arm_reg_t CHNL_USEBURST_SET;    // Channel useburst set
    arm_reg_t CHNL_USEBURST_CLR;    // Channel useburst clear
    arm_reg_t CHNL_REQ_MASK_SET;    // Channel request mask set
    arm_reg_t CHNL_REQ_MASK_CLR;    // Channel request mask clear
    arm_reg_t CHNL_ENABLE_SET;      // Channel enable set
    arm_reg_t CHNL_ENABLE_CLR;      // Channel enable clear
    arm_reg_t CHNL_PRI_ALT_SET;     // Channel primary-alternate set
    arm_reg_t CHNL_PRI_ALT_CLR;     // Channel primary-alternate clear
    arm_reg_t CHNL_PRIORITY_SET;    // Channel priority set
    arm_reg_t CHNL_PRIORITY_CLR;    // Channel priority clear
    unsigned reserved0 [3];
    arm_reg_t ERR_CLR;              // Bus error clear
} DMA_Controller_t;

typedef struct
{
    arm_reg_t INTEGRATION_CFG;
    unsigned reserved0;
    arm_reg_t STALL_STATUS;
    unsigned reserved1;
    arm_reg_t REQ_STATUS;
    unsigned reserved2;
    arm_reg_t SREQ_STATUS;
    unsigned reserved3;
    arm_reg_t DONE_SET;
    arm_reg_t DONE_CLR;
    arm_reg_t ACTIVE_SET;
    arm_reg_t ACTIVE_CLR;
    unsigned reserved4 [5];
    arm_reg_t ERR_SET;
} DMA_Test_t;

typedef struct
{
    arm_reg_t PERIPH_ID4;   // Peripheral identification 4
    unsigned reserved0[3];
    arm_reg_t PERIPH_ID0;
    arm_reg_t PERIPH_ID1;
    arm_reg_t PERIPH_ID2;
    arm_reg_t PERIPH_ID3;
} DMA_Periph_Identification_t;

typedef struct
{
    arm_reg_t PCELL_ID0;    // PrimeCell identification 0
    arm_reg_t PCELL_ID1;
    arm_reg_t PCELL_ID2;
    arm_reg_t PCELL_ID3;
} DMA_PrimeCell_Identification_t;

// Channel control data structure
typedef struct
{
    arm_reg_t SOURCE_END_POINTER;
    arm_reg_t DEST_END_POINTER;
    arm_reg_t CONTROL;
    arm_reg_t UNUSED;
} DMA_Data_t;

typedef struct
{
    DMA_Data_t PRIMARY_CH [32];
} DMA_PrimaryData_t;

typedef struct
{
    DMA_Data_t ALT_CH [32];
} DMA_AltData_t;

#define ARM_DMA                 ((DMA_Controller_t*) ARM_DMA_BASE)
#define ARM_DMA_Test            ((DMA_Test_t*) (ARM_DMA_BASE + 0x0E00))
#define ARM_DMA_PeriphID        ((DMA_Periph_Identification_t*) (ARM_DMA_BASE + 0x0FD0))
#define ARM_DMA_PrimeCellID     ((DMA_PrimeCell_Identification_t*) (ARM_DMA_BASE + 0x0FF0))

//======================================================================================
// DMA state register masks
//======================================================================================
#define ARM_DMA_CHANNEL_MASK    0x001F0000
#define ARM_DMA_CHANNEL1        0x00000000
#define ARM_DMA_CHANNEL2        0x00010000
#define ARM_DMA_CHANNEL3        0x00020000

#define ARM_DMA_STATE_MASK                          0x000000F0
#define ARM_DMA_STATE_IDLE                          0x00000000
#define ARM_DMA_STATE_READ_CH_CONTROLLER_DATA       0x00000010
#define ARM_DMA_STATE_READ_SRC_DATA_END_POINTER     0x00000020
#define ARM_DMA_STATE_READ_DEST_DATA_END_POINTER    0x00000030
#define ARM_DMA_STATE_READ_SRC_DATA                 0x00000040
#define ARM_DMA_STATE_WRITE_DEST_DATA               0x00000050
#define ARM_DMA_STATE_WAIT_REQUEST_CLR              0x00000060
#define ARM_DMA_STATE_WRITE_CH_CONTROLLER_DATA      0x00000070
#define ARM_DMA_STATE_STALLED                       0x00000080
#define ARM_DMA_STATE_DONE                          0x00000090
#define ARM_DMA_STATE_PERIPH_SCT_GATH_TRANS         0x000000A0
#define ARM_DMA_STATE_ENABLE                        0x00000001
//======================================================================================
// DMA configuration register masks
//======================================================================================
#define ARM_DMA_CFG_CH_PROT1    0x00000020
#define ARM_DMA_CFG_CH_PROT2    0x00000040
#define ARM_DMA_CFG_CH_PROT3    0x00000080
#define ARM_DMA_ENABLE          0x00000001
#define ARM_DMA_DISABLE_ALL     0xFFFFFFFF
#define ARM_DMA_SELECT(CHANNEL) (0x00000001<<(CHANNEL))
#define ARM_DMA_ERR_CLR         0x00000001
//======================================================================================
// DMA Control data configuration masks
//======================================================================================
#define ARM_DMA_DST_INC(x)  ((x) << 30)
#define ARM_DMA_DST_SIZE(x) ((x) << 28)
#define ARM_DMA_SRC_INC(x)  ((x) << 26)
#define ARM_DMA_SRC_SIZE(x) ((x) << 24)
#define ARM_DMA_DST_PROT(x) ((x) << 21)
#define ARM_DMA_SRC_PROT(x) ((x) << 18)
#define ARM_DMA_BYTE        0
#define ARM_DMA_HALFWORD    1
#define ARM_DMA_WORD        2
#define ARM_DMA_ADDR_NOINC  3
//======================================================================================
// R_power bits, DMA_PrimaryControlData->xxx_CONTROL |= xxx<<15 */
//======================================================================================
#define ARM_DMA_ALWAYS_ARBITR   0x00000000
#define ARM_DMA_AFTER2_ARBITR   0x00000001
#define ARM_DMA_AFTER4_ARBITR   0x00000002
#define ARM_DMA_AFTER8_ARBITR   0x00000003
#define ARM_DMA_AFTER16_ARBITR  0x00000004
#define ARM_DMA_AFTER32_ARBITR  0x00000005
#define ARM_DMA_AFTER64_ARBITR  0x00000006
#define ARM_DMA_AFTER128_ARBITR 0x00000007
#define ARM_DMA_AFTER256_ARBITR 0x00000008
#define ARM_DMA_AFTER512_ARBITR 0x00000009
#define ARM_DMA_NOT_ARBITR      0x0000000F
//======================================================================================
// n_minus_1 bits, DMA_PrimaryControlData->xxx_CONTROL |= xxx<<5
//======================================================================================
#define ARM_DMA_TRANSFERS(QUANTITY) ((QUANTITY-1)<<4)
#define ARM_DMA_RPOWER(QUANTITY)    ((QUANTITY-1)<<14)

#define ARM_DMA_NEXT_USEBURST   0x00000008
//======================================================================================
// DMA cycle bits
//======================================================================================
#define ARM_DMA_STOP            0x00000000
#define ARM_DMA_BASIC           1
#define ARM_DMA_AUTOREQ         2
#define ARM_DMA_PINGPONG        3
#define ARM_DMA_MEMSG_PDS       4
#define ARM_DMA_MEMSG_ADS       5
#define ARM_DMA_PERIPHSG_PDS    6
#define ARM_DMA_PERIPHSG_ADS    7

/*------------------------------------------------------
 * Описание регистров контроллера Flash памяти программ.
 */
typedef struct
{
    arm_reg_t CMD;      /* Управление Flash-памятью */
    arm_reg_t ADR;      /* Адрес (словный) */
    arm_reg_t DI;       /* Данные для записи */
    arm_reg_t DO;       /* Считанные данные */
    arm_reg_t KEY;      /* Ключ */
} EEPROM_t;

#define ARM_EEPROM      ((EEPROM_t*) ARM_EEPROM_BASE)

/*
 * Регистр EEPROM CMD
 */
/*
 * Переключение контроллера памяти EEPROM на
 * регистровое управление. Не может производиться
 * при исполнении программы из области EERPOM.
 * 0 – управление EERPOM от ядра, рабочий режим
 * 1 – управление от регистров, режим программирования
 */
#define ARM_EEPROM_CMD_CON  0x00000001
/*
 * Запись в память EERPOM (в режиме программирования)
 * 0 – нет записи
 * 1 – есть запись
 */
#define ARM_EEPROM_CMD_WR   0x00000002
/*
 * Чтение из памяти EERPOM (в режиме программирования)
 * 0 – нет чтения
 * 1 – есть чтение
 */
#define ARM_EEPROM_CMD_RD   0x00000004
/*
 * Задержка памяти программ при чтении
 */
#define ARM_EEPROM_CMD_DELAY_MASK   0x00000038

#define ARM_EEPROM_CMD_DELAY_0  0x00000000      /* 0 тактов - до 25 МГц */
#define ARM_EEPROM_CMD_DELAY_1  0x00000008      /* 1 такт - до 50 МГц */
#define ARM_EEPROM_CMD_DELAY_2  0x00000010      /* 2 такта - до 75 МГц */
#define ARM_EEPROM_CMD_DELAY_3  0x00000018      /* 3 такта - до 100 МГц */
#define ARM_EEPROM_CMD_DELAY_4  0x00000020      /* 4 такта - до 125 МГц */
#define ARM_EEPROM_CMD_DELAY_5  0x00000028      /* 5 тактов - до 150 МГц */
#define ARM_EEPROM_CMD_DELAY_6  0x00000030      /* 6 тактов - до 175 МГц */
#define ARM_EEPROM_CMD_DELAY_7  0x00000038      /* 7 тактов - до 200 МГц */

/*
 * Выдача адреса ADR[16:9]
 * 0 – не разрешено
 * 1 - разрешено
 */
#define ARM_EEPROM_CMD_XE   0x00000040
/*
 * Выдача адреса ADR[8:2]
 * 0 – не разрешено
 * 1 – разрешено
 */
#define ARM_EEPROM_CMD_YE   0x00000080
/*
 * Усилитель считывания
 * 0 – не включен
 * 1 – включен
 */
#define ARM_EEPROM_CMD_SE   0x00000100
/*
 * Работа с блоком информации
 * 0 – основная память
 * 1 – информационный блок
 */
#define ARM_EEPROM_CMD_IFREN    0x00000200
/*
 * Стереть строку с адресом ADR[16:9].
 * ADR[8:0] значения не имеет.
 * 0 – нет стирания
 * 1 – стирание
 */
#define ARM_EEPROM_CMD_ERASE    0x00000400
/*
 * Стереть весь блок, при ERASE=1
 * 0 – нет стирания
 * 1 – стирание
 */
#define ARM_EEPROM_CMD_MAS1     0x00000800
/*
 * Записать данные по ADR[16:2] из регистра EERPOM_DI
 * 0 – нет записи
 * 1 – есть запись
 */
#define ARM_EEPROM_CMD_PROG     0x00001000
/*
 * Операции записи или стирания
 * 0 – при чтении
 * 1 - при записи или стирании
 */
#define ARM_EEPROM_CMD_NVSTR    0x00002000

/*------------------------------------------------------
 * Контроллер CAN.
 */
typedef struct
{
    arm_reg_t ID;       /* Идентификатор сообщения */
    arm_reg_t DLC;      /* Длина и формат сообщения */
    arm_reg_t DATAL;    /* Младшая часть данных */
    arm_reg_t DATAH;    /* Старшая часть данных */
} CAN_BUF_t;

typedef struct
{
    arm_reg_t MASK;     /* Маска для приема сообщения */
    arm_reg_t FILTER;   /* Фильтр для приема сообщения */
} CAN_MASK_t;

typedef struct
{
    arm_reg_t CONTROL;      /* Управление */
    arm_reg_t STATUS;       /* Состояние */
    arm_reg_t BITTMNG;      /* Задание скорости работы */
    arm_reg_t reserved0;
    arm_reg_t INT_EN;       /* Разрешение прерываний */
    arm_reg_t reserved1;
    arm_reg_t reserved2;
    arm_reg_t OVER;         /* Границы счетчика ошибок */
    arm_reg_t RXID;         /* Идентификатор принятого сообщения */
    arm_reg_t RXDLC;        /* Длина и формат принятого сообщения */
    arm_reg_t RXDATAL;      /* Младшая часть принятых данных */
    arm_reg_t RXDATAH;      /* Старшая часть принятых данных */
    arm_reg_t TXID;         /* Идентификатор передаваемого сообщения */
    arm_reg_t TXDLC;        /* Длина и формат передаваемого сообщения */
    arm_reg_t DATAL;        /* Младшая часть передаваемых данных */
    arm_reg_t DATAH;        /* Старшая часть передаваемых данных */
    arm_reg_t BUF_CON [32]; /* Управление буферами 1..32 */
    arm_reg_t INT_RX;       /* Разрешение прерываний от приемных буферов */
    arm_reg_t RX;           /* Флаги RX_FULL от приемных буферов */
    arm_reg_t INT_TX;       /* Разрешение прерываний от передающих буферов */
    arm_reg_t TX;           /* Флаги ~TX_REQ от передающих буферов */
    arm_reg_t reserved3 [76];
    CAN_BUF_t BUF [32];     /* Буферы 1..32: ID, DLC, DATA */
    arm_reg_t reserved4 [64];
    CAN_MASK_t MASK [32];   /* Маски и фильтры для приёма в буферы 1..32 */
} CAN_t;

#define ARM_CAN1        ((CAN_t*) ARM_CAN1_BASE)
#define ARM_CAN2        ((CAN_t*) ARM_CAN2_BASE)

/*
 * Регистр CAN CONTROL - управление контроллером
 */
#define CAN_CONTROL_EN      (1 << 0)    /* Разрешение работы */
#define CAN_CONTROL_ROM     (1 << 1)    /* Только на приём */
#define CAN_CONTROL_STM     (1 << 2)    /* Режим самотестирования */
#define CAN_CONTROL_SAP     (1 << 3)    /* Подтверждаем приём собственных пакетов */
#define CAN_CONTROL_ROP     (1 << 4)    /* Принимаем собственные пакеты */

/*
 * Регистр CAN STATUS - состояние контроллера
 */
#define CAN_STATUS_TEC(s)       ((s) >> 24) /* Счетчик ошибок передатчика */
#define CAN_STATUS_REC(s)       ((s) >> 16 & 0xFF) /* Счетчик ошибок приемника */
#define CAN_STATUS_TEC8         (1 << 12)   /* TEC более 255 */
#define CAN_STATUS_REC8         (1 << 11)   /* REC более 255 */
#define CAN_STATUS_BUS_OFF      (1 << 10)   /* Ожидается восстановление шины */
#define CAN_STATUS_ERR_PASSIVE  (1 << 9)    /* Отсылается флаг пассивной ошибки */
#define CAN_STATUS_ID_LOWER     (1 << 8)    /* При передаче был проигран арбитраж */
#define CAN_STATUS_ERR_ACK      (1 << 7)    /* Ошибка подтверждения приема */
#define CAN_STATUS_ERR_FRAME    (1 << 6)    /* Ошибка формата пакета */
#define CAN_STATUS_ERR_CRC      (1 << 5)    /* Ошибка контрольной суммы пакета */
#define CAN_STATUS_ERR_BITSTUFF (1 << 4)    /* Ошибка вставленных битов пакета */
#define CAN_STATUS_ERR_BIT      (1 << 3)    /* Ошибка передаваемых битов пакета */
#define CAN_STATUS_ERR_OVER     (1 << 2)    /* Превышение заданного уровня TEC и REC */
#define CAN_STATUS_TX_READY     (1 << 1)    /* Есть буфер готовый для отправки сообщений */
#define CAN_STATUS_RX_READY     (1 << 0)    /* Есть буфер с принятым сообщением */

/*
 * Регистр CAN BITTMNG - задание скорости работы
 */
#define CAN_BITTMNG_SB      (1 << 27)       /* Трехкратное семплирование */
#define CAN_BITTMNG_SJW(n)  ((n-1) << 25)   /* Размер фазы SJW */
#define CAN_BITTMNG_SEG2(n) ((n-1) << 22)   /* Размер фазы SEG2 */
#define CAN_BITTMNG_SEG1(n) ((n-1) << 19)   /* Размер фазы SEG1 */
#define CAN_BITTMNG_PSEG(n) ((n-1) << 16)   /* Размер фазы PSEG */
#define CAN_BITTMNG_BRP(n)  (n)             /* Предделитель системной частоты */

/*
 * Регистр CAN INT_EN - разрешение прерываний
 */
#define CAN_INT_EN_ERR_OVER (1 << 4)    /* Счётчик TEC или REC переполнился */
#define CAN_INT_EN_ERR      (1 << 3)    /* Возникла ошибка */
#define CAN_INT_EN_TX       (1 << 2)    /* Возможна передача */
#define CAN_INT_EN_RX       (1 << 1)    /* Принято сообщение */
#define CAN_INT_EN_GLB      (1 << 0)    /* Общее разрешение прерывания */

/*
 * Регистры CAN BUF_CON[0..31] - управление буферами
 */
#define CAN_BUF_CON_EN      (1 << 0)    /* Разрешение работы буфера */
#define CAN_BUF_CON_RX_ON   (1 << 1)    /* Режим работы буфера: на приём */
#define CAN_BUF_CON_OVER_EN (1 << 2)    /* Разрешение перезаписи принятого сообщения */
#define CAN_BUF_CON_RTR_EN  (1 << 3)    /* Ответить при приеме RTR в буфер */
#define CAN_BUF_CON_PRIOR_0 (1 << 4)    /* Приоритет при отправке */
#define CAN_BUF_CON_TX_REQ  (1 << 5)    /* Запрос на отправку сообщения */
#define CAN_BUF_CON_RX_FULL (1 << 6)    /* Флаг готовности приема */
#define CAN_BUF_CON_OVER_WR (1 << 7)    /* Флаг перезаписи принятого сообщения */

/*
 * Регистры CAN BUF[0..31].ID - идентификатор сообщения
 */
#define CAN_ID_EID_SHIFT    0               /* Сдвиг поля EID */
#define CAN_ID_EID_MASK     0x3FFFF         /* Маска поля EID */
#define CAN_ID_SID_SHIFT    18              /* Сдвиг поля SID */
#define CAN_ID_SID_MASK     (0x7FF << 18)   /* Маска поля SID */

/*
 * Регистры CAN BUF[0..31].DLC - длина и формат сообщения
 */
#define CAN_DLC_LEN(n)      ((n) & 15)      /* Длина данных в байтах */
#define CAN_DLC_RTR         (1 << 8)        /* Запрос обратного ответа */
#define CAN_DLC_R1          (1 << 9)        /* Всегда должно быть “1” */
#define CAN_DLC_R0          (1 << 10)       /* Всегда должно быть “0” */
#define CAN_DLC_SSR         (1 << 11)       /* Всегда должно быть “1” */
#define CAN_DLC_IDE         (1 << 12)       /* Расширенный формат */

/*-----------------------------------
 * Описание регистров контроллера ЦАП
 */
typedef struct
{
    arm_reg_t DAC_CFG;
    arm_reg_t DAC1_DATA;
    arm_reg_t DAC2_DATA;
} DAC_t;

#define ARM_DAC         ((DAC_t*) ARM_DAC_BASE)

/*
 * Регистр DAC_CFG
 */

#define DAC1_REF            (1<<0)
#define DAC2_REF            (1<<1)
#define DAC1_ON             (1<<2)
#define DAC2_ON             (1<<3)
#define DAC1_DAC2_ASYNC     (1<<4);

/*-----------------------------------
 * Описание регистров контроллера АЦП
 */
typedef struct
{
    arm_reg_t ADC1_CFG;
    arm_reg_t ADC2_CFG;
    arm_reg_t ADC1_H_LEVEL;
    arm_reg_t ADC2_H_LEVEL;
    arm_reg_t ADC1_L_LEVEL;
    arm_reg_t ADC2_L_LEVEL;
    arm_reg_t ADC1_RESULT;
    arm_reg_t ADC2_RESULT;
    arm_reg_t ADC1_STATUS;
    arm_reg_t ADC2_STATUS;
    arm_reg_t ADC1_CHSEL;
    arm_reg_t ADC2_CHSEL;
} ADC_t;

#define ARM_ADC         ((ADC_t*) ARM_ADC_BASE)

/*
 * Регистр ADC1_CFG
 */
/* Включение АЦП */
#define ARM_CFG_REG_ADON    (1 << 0)
/* Начало преобразования. Запись “1” начинает
 * процесс преобразования, сбрасывается автоматически */
#define ARM_CFG_REG_GO      (1 << 1)
/* Выбор источника синхросигнала CLK работы ADC. Возможные значения см. ниже */
#define ARM_CFG_REG_CLKS    (1 << 2)
/* Выбор способа запуска АЦП. Возможные значения см. ниже */
#define ARM_CFG_REG_SAMPLE  (1 << 3)
/* Номер аналогового канала, по которому поступает сигнал для преобразования */
#define ARM_CFG_REG_CHS(x)  ((x) << 4)
/* Включение переключения каналов */
#define ARM_CFG_REG_CHCH    (1 << 9)
/* Разрешение автоматического контролирования уровней */
#define ARM_CFG_REG_RNGC    (1 << 10)
/* Выбор источника опорных напряжений. Возможные значения см. ниже */
#define ARM_CFG_M_REF       (1 << 11)
/* Выбор коэффициента деления входной частоты.
 * 0000 - CLK, 0001 - CLK/2, ..., 1111 - CLK/32768 */
#define ARM_CFG_REG_DIVCLK(x)   ((x) << 12)
/* Запускает работу двух АЦП одновременно, при этом биты
 * конфигурации второго АЦП, такие как Cfg_REG_DIVCLK,
 * Cfg_REG_ADON, Cfg_M_REF и Cfg_REG_CHS берутся из
 * регистра конфигурации первого */
#define ARM_CFG_SYNC_CONVER (1 << 16)
/* Включение датчика температуры и источника опорного напряжения */
#define ARM_TS_EN           (1 << 17)
/* Включение выходного усилителя для датчика температуры и
 * источника опорного напряжения */
#define ARM_TS_BUF_EN       (1 << 18)
/* Включение оцифровки датчика температуры
 * Должен использоваться совместно с выбором канала Cfg_REG_CHS = 31 */
#define ARM_SEL_TS          (1 << 19)
/* Включение оцифровки источника опорного напряжения на 1.23В */
#define ARM_SEL_VREF        (1 << 20)
/* Подстройка опорного напряжения */
#define ARM_TR(x)           ((x) << 21)
/* Задержка в тактах перед началом следующего преобразования после завершения
 * предыдущего при последовательном переборе каналов */
#define ARM_DELAY_GO(x)     ((x) << 25)
/* Задержка в тактах между началом преобразования ADC1 и ADC2 при
 * ADC последовательном переборе, либо работе на один канал */
#define ARM_DELAY_ADC(x)    ((x) << 28)

/*
 * Регистр ADC2_CFG - приведены только те поля, которые отличаются от регистра ADC1_CFG
 */
/* Выбор источника опорного напряжения 1.23В */
#define ARM_ADC1_OP         (1 << 17)
/* Выбор источника опорного напряжения 1.23В */
#define ARM_ADC2_OP         (1 << 18)

/* Источник синхросигнала CLK */
#define ARM_ADC_CPU_CKL         0
#define ARM_ADC_ADC_CKL         1
/* Способы запуска АЦП */
#define ARM_ADC_START_SINGLE    0   /* одиночный запуск */
#define ARM_ADC_START_SEQ       1   /* последовательный, автоматический запуск после
                                     * завершения предыдущего преобразования */

/* Выбор источника опорных напряжений */
#define ARM_ADC_REF_INT         0   /* внутреннее опорное напряжение (от AUcc и AGND) */
#define ARM_ADC_REF_EXT         1   /* внешнее опорное напряжение
                                     * (от ADC0_REF+ и ADC1_REF- для ADC1_CFG и
                                     * от датчика температуры для ADC2_CFG) */

/*
 * Регистры ADCx_STATUS
 */
/* Данные в регистре результата были перезаписаны, данный флаг
 * сбрасывается только при записи в регистр флагов */
#define ARM_FLG_REG_OVERWRITE   (1 << 0)
/* Флаг выставляется, когда результат преобразования выше верней
 * или ниже нижней границы автоматического контролирования
 * уровней. Очищается считыванием результата из регистра */
#define ARM_FLG_REG_AWOIFEN     (1 << 1)
/* Флаг выставляется, когда закончено преобразования и данные еще
 * не считаны. Очищается считыванием результата из регистра*/
#define ARM_FLG_REG_EOCIF       (1 << 2)
/* Флаг разрешения генерирования прерывания по событию Flg_REG_AWOIFEN */
#define ARM_AWOIF_IE            (1 << 3)
/* Флаг разрешения генерирования прерывания по событию Flg_REG_ECOIF */
#define ARM_ECOIF_IE            (1 << 4)

/*
 * Регистры ADCx_CHSEL: выбор канала АЦП
 */
#define ARM_ADC_CHANNEL(n)  (1 << (n))  /* Номер канала */


/*-----------------------------------------------------
 * Описание регистров интервального сторожевого таймера
 */
typedef struct
{
    arm_reg_t KR;
    arm_reg_t PR;
    arm_reg_t RLR;
    arm_reg_t SR;
} IWDG_t;

#define ARM_IWDG                ((IWDG_t *) ARM_IWDT_BASE)
/* Это значение должна записывать программа через определенные интервалы
 * времени. Иначе сторожевой таймер сгенерирует сброс */
#define ARM_IWDG_KEY_ALIVE      0xAAAA
/* Это значение должно быть записано в регистр KR для разрешения доступа
 * по записи к регистрам PR и RLR */
#define ARM_IWDG_KEY_UNBLOCK    0x5555
/* Это значение разрешает работу сторожевого таймера */
#define ARM_IWDG_KEY_START      0xCCCC

/* Делитель частоты сторожевого таймера */
#define ARM_IWDG_PR(x)          ((x) & 7)
/* Основание счета сторожевого таймера */
#define ARM_IWDG_RLR(x)         ((x) & 0xFFF)
/* Флаг обновления делителя частоты сторожевого таймера */
#define ARM_IWDG_PVU            (1 << 0)
/* Флаг обновления значения сторожевого таймера */
#define ARM_IWDG_RVU            (1 << 1)

/*------------------------------------------------
 * Описание регистров оконного сторожевого таймера
 */
typedef struct
{
    arm_reg_t CR;
    arm_reg_t CFR;
    arm_reg_t SR;
} WWDG_t;

#define ARM_WWDG                ((WWDG_t *) ARM_WWDT_BASE)

/* Значение счётчика */
#define ARM_WWDG_T(x)           ((x) & 0x7F)
/* Бит активации */
#define ARM_WWDG_WDGA           (1 << 7)

/* Значение окна */
#define ARM_WWDG_W(x)           ((x) & 0x7F)
/* Делитель частоты */
#define ARM_WWDG_WGTB(x)        ((x) << 7)
/* Раннее предупреждение прерывания */
#define ARM_WWDG_EWI            (1 << 9)

/* Флаг раннего предупреждения */
#define ARM_WWDG_EWIF           (1 << 0)

/*------------------------------------------------------------------------
 * Макроопределения для возможности указания привязки сигналов к контактам
 * из target.cfg
 */

/*
 * Каждому контакту микроконтроллера ставится в соответствие уникальное
 * число, имеющее также и символическое имя. Например, PA0, PC12 и т.д.
 * Пользователь драйвера должен установить значения констант, обозначающих
 * сигналы, в файле target.cfg своего проекта, в переменной CFLAGS.
 * Например,
 *     CFLAGS += -DSSP1RXD=PD11
 * В коде драйвера можно препроцессором разобрать значение константы для
 * сигнала и получить, к какому контакту пользователь привязал сигнал, и
 * правильно проинициализировать контакт.
 * Для примера см. драйвер SPI
 */
#define PORT_A  0
#define PORT_B  1
#define PORT_C  2
#define PORT_D  3
#define PORT_E  4
#define PORT_F  8

#define M1986VE91T_PIN(port,pin)    (((port) << 4) | (pin))
#define PORT(x)             (((x) >> 4) & 0xF)
#define PIN(x)              ((x) & 0xF)

#define PA0     M1986VE91T_PIN(PORT_A, 0)
#define PA1     M1986VE91T_PIN(PORT_A, 1)
#define PA2     M1986VE91T_PIN(PORT_A, 2)
#define PA3     M1986VE91T_PIN(PORT_A, 3)
#define PA4     M1986VE91T_PIN(PORT_A, 4)
#define PA5     M1986VE91T_PIN(PORT_A, 5)
#define PA6     M1986VE91T_PIN(PORT_A, 6)
#define PA7     M1986VE91T_PIN(PORT_A, 7)
#define PA8     M1986VE91T_PIN(PORT_A, 8)
#define PA9     M1986VE91T_PIN(PORT_A, 9)
#define PA10    M1986VE91T_PIN(PORT_A, 10)
#define PA11    M1986VE91T_PIN(PORT_A, 11)
#define PA12    M1986VE91T_PIN(PORT_A, 12)
#define PA13    M1986VE91T_PIN(PORT_A, 13)
#define PA14    M1986VE91T_PIN(PORT_A, 14)
#define PA15    M1986VE91T_PIN(PORT_A, 15)

#define PB0     M1986VE91T_PIN(PORT_B, 0)
#define PB1     M1986VE91T_PIN(PORT_B, 1)
#define PB2     M1986VE91T_PIN(PORT_B, 2)
#define PB3     M1986VE91T_PIN(PORT_B, 3)
#define PB4     M1986VE91T_PIN(PORT_B, 4)
#define PB5     M1986VE91T_PIN(PORT_B, 5)
#define PB6     M1986VE91T_PIN(PORT_B, 6)
#define PB7     M1986VE91T_PIN(PORT_B, 7)
#define PB8     M1986VE91T_PIN(PORT_B, 8)
#define PB9     M1986VE91T_PIN(PORT_B, 9)
#define PB10    M1986VE91T_PIN(PORT_B, 10)
#define PB11    M1986VE91T_PIN(PORT_B, 11)
#define PB12    M1986VE91T_PIN(PORT_B, 12)
#define PB13    M1986VE91T_PIN(PORT_B, 13)
#define PB14    M1986VE91T_PIN(PORT_B, 14)
#define PB15    M1986VE91T_PIN(PORT_B, 15)

#define PC0     M1986VE91T_PIN(PORT_C, 0)
#define PC1     M1986VE91T_PIN(PORT_C, 1)
#define PC2     M1986VE91T_PIN(PORT_C, 2)
#define PC3     M1986VE91T_PIN(PORT_C, 3)
#define PC4     M1986VE91T_PIN(PORT_C, 4)
#define PC5     M1986VE91T_PIN(PORT_C, 5)
#define PC6     M1986VE91T_PIN(PORT_C, 6)
#define PC7     M1986VE91T_PIN(PORT_C, 7)
#define PC8     M1986VE91T_PIN(PORT_C, 8)
#define PC9     M1986VE91T_PIN(PORT_C, 9)
#define PC10    M1986VE91T_PIN(PORT_C, 10)
#define PC11    M1986VE91T_PIN(PORT_C, 11)
#define PC12    M1986VE91T_PIN(PORT_C, 12)
#define PC13    M1986VE91T_PIN(PORT_C, 13)
#define PC14    M1986VE91T_PIN(PORT_C, 14)
#define PC15    M1986VE91T_PIN(PORT_C, 15)

#define PD0     M1986VE91T_PIN(PORT_D, 0)
#define PD1     M1986VE91T_PIN(PORT_D, 1)
#define PD2     M1986VE91T_PIN(PORT_D, 2)
#define PD3     M1986VE91T_PIN(PORT_D, 3)
#define PD4     M1986VE91T_PIN(PORT_D, 4)
#define PD5     M1986VE91T_PIN(PORT_D, 5)
#define PD6     M1986VE91T_PIN(PORT_D, 6)
#define PD7     M1986VE91T_PIN(PORT_D, 7)
#define PD8     M1986VE91T_PIN(PORT_D, 8)
#define PD9     M1986VE91T_PIN(PORT_D, 9)
#define PD10    M1986VE91T_PIN(PORT_D, 10)
#define PD11    M1986VE91T_PIN(PORT_D, 11)
#define PD12    M1986VE91T_PIN(PORT_D, 12)
#define PD13    M1986VE91T_PIN(PORT_D, 13)
#define PD14    M1986VE91T_PIN(PORT_D, 14)
#define PD15    M1986VE91T_PIN(PORT_D, 15)

#define PE0     M1986VE91T_PIN(PORT_E, 0)
#define PE1     M1986VE91T_PIN(PORT_E, 1)
#define PE2     M1986VE91T_PIN(PORT_E, 2)
#define PE3     M1986VE91T_PIN(PORT_E, 3)
#define PE4     M1986VE91T_PIN(PORT_E, 4)
#define PE5     M1986VE91T_PIN(PORT_E, 5)
#define PE6     M1986VE91T_PIN(PORT_E, 6)
#define PE7     M1986VE91T_PIN(PORT_E, 7)
#define PE8     M1986VE91T_PIN(PORT_E, 8)
#define PE9     M1986VE91T_PIN(PORT_E, 9)
#define PE10    M1986VE91T_PIN(PORT_E, 10)
#define PE11    M1986VE91T_PIN(PORT_E, 11)
#define PE12    M1986VE91T_PIN(PORT_E, 12)
#define PE13    M1986VE91T_PIN(PORT_E, 13)
#define PE14    M1986VE91T_PIN(PORT_E, 14)
#define PE15    M1986VE91T_PIN(PORT_E, 15)

#define PF0     M1986VE91T_PIN(PORT_F, 0)
#define PF1     M1986VE91T_PIN(PORT_F, 1)
#define PF2     M1986VE91T_PIN(PORT_F, 2)
#define PF3     M1986VE91T_PIN(PORT_F, 3)
#define PF4     M1986VE91T_PIN(PORT_F, 4)
#define PF5     M1986VE91T_PIN(PORT_F, 5)
#define PF6     M1986VE91T_PIN(PORT_F, 6)
#define PF7     M1986VE91T_PIN(PORT_F, 7)
#define PF8     M1986VE91T_PIN(PORT_F, 8)
#define PF9     M1986VE91T_PIN(PORT_F, 9)
#define PF10    M1986VE91T_PIN(PORT_F, 10)
#define PF11    M1986VE91T_PIN(PORT_F, 11)
#define PF12    M1986VE91T_PIN(PORT_F, 12)
#define PF13    M1986VE91T_PIN(PORT_F, 13)
#define PF14    M1986VE91T_PIN(PORT_F, 14)
#define PF15    M1986VE91T_PIN(PORT_F, 15)

#define FUNC_PORT   0
#define FUNC_MAIN   1
#define FUNC_ALT    2
#define FUNC_REDEF  3

static inline void milandr_init_pin (GPIO_t *gpio, unsigned port, unsigned pin, unsigned func)
{
    /* Подача синхроимпульсов */
    ARM_RSTCLK->PER_CLOCK |= (ARM_PER_CLOCK_GPIOA << port);
    /* Установка функции */
    gpio->FUNC = (gpio->FUNC & ~ARM_FUNC_MASK(pin)) | ARM_FUNC(pin, func);
    /* Цифровой вывод */
    gpio->ANALOG |= (1 << pin);
    /* Быстрый фронт */
    gpio->PWR = (gpio->PWR & ~ARM_PWR_MASK(pin)) | ARM_PWR_FASTEST(pin);
}


#ifndef UART1_RX
#define UART1_RX PA6
#endif

#if (UART1_RX!=PA6) && (UART1_RX!=PB6) && (UART1_RX!=PD7) && (UART1_RX!=PE12)
#error "Impossible assignment of UART1_RX pin in CFLAGS of target.cfg"
#endif

#if (PORT(UART1_RX)==PORT_A)
#   define UART1_RX_GPIO ARM_GPIOA
#   define UART1_RX_FUNC FUNC_REDEF
#elif (PORT(UART1_RX)==PORT_B)
#   define UART1_RX_GPIO ARM_GPIOB
#   define UART1_RX_FUNC FUNC_ALT
#elif (PORT(UART1_RX)==PORT_D)
#   define UART1_RX_GPIO ARM_GPIOD
#   define UART1_RX_FUNC FUNC_REDEF
#elif (PORT(UART1_RX)==PORT_E)
#   define UART1_RX_GPIO ARM_GPIOE
#   define UART1_RX_FUNC FUNC_REDEF
#else
#   error "UART1_RX pin is not assigned in CFLAGS of target.cfg"
#endif


#ifndef UART1_TX
#define UART1_TX PA7
#endif

#if (UART1_TX!=PA7) && (UART1_TX!=PB0) && (UART1_TX!=PB5) && (UART1_TX!=PD8) && (UART1_TX!=PE13)
#error "Impossible assignment of UART1_TX pin in CFLAGS of target.cfg"
#endif

#if (PORT(UART1_TX)==PORT_A)
#   define UART1_TX_GPIO ARM_GPIOA
#   define UART1_TX_FUNC FUNC_REDEF
#elif (PORT(UART1_TX)==PORT_B)
#   define UART1_TX_GPIO ARM_GPIOB
#   if (PIN(UART1_TX)==0)
#       define UART1_TX_FUNC FUNC_REDEF
#   elif (PIN(UART1_TX)==5)
#       define UART1_TX_FUNC FUNC_ALT
#   endif
#elif (PORT(UART1_TX)==PORT_D)
#   define UART1_TX_GPIO ARM_GPIOD
#   define UART1_TX_FUNC FUNC_REDEF
#elif (PORT(UART1_TX)==PORT_E)
#   define UART1_TX_GPIO ARM_GPIOE
#   define UART1_TX_FUNC FUNC_REDEF
#else
#   error "UART1_TX pin is not assigned in CFLAGS of target.cfg"
#endif


#ifndef UART2_RX
#define UART2_RX PF0
#endif

#if (UART2_RX!=PB1) && (UART2_RX!=PD0) && (UART2_RX!=PF0)
#error "Impossible assignment of UART2_RX pin in CFLAGS of target.cfg"
#endif

#if (PORT(UART2_RX)==PORT_B)
#   define UART2_RX_GPIO ARM_GPIOB
#   define UART2_RX_FUNC FUNC_REDEF
#elif (PORT(UART2_RX)==PORT_D)
#   define UART2_RX_GPIO ARM_GPIOD
#   define UART2_RX_FUNC FUNC_ALT
#elif (PORT(UART2_RX)==PORT_F)
#   define UART2_RX_GPIO ARM_GPIOF
#   define UART2_RX_FUNC FUNC_REDEF
#else
#   error "UART2_RX pin is not assigned in CFLAGS of target.cfg"
#endif


#ifndef UART2_TX
#define UART2_TX PF1
#endif

#if (UART2_TX!=PD1) && (UART2_TX!=PF1)
#error "Impossible assignment of UART2_TX pin in CFLAGS of target.cfg"
#endif

#if (PORT(UART2_TX)==PORT_D)
#   define UART2_TX_GPIO ARM_GPIOD
#   define UART2_TX_FUNC FUNC_ALT
#elif (PORT(UART2_TX)==PORT_F)
#   define UART2_TX_GPIO ARM_GPIOF
#   define UART2_TX_FUNC FUNC_REDEF
#else
#   error "UART2_TX pin is not assigned in CFLAGS of target.cfg"
#endif

/* Сигнал SSP1_RXD */
#ifndef SSP1_RXD
#define SSP1_RXD    PD11
#endif

#if (PORT(SSP1_RXD)==PORT_B)
#   define SSP1_RXD_GPIO ARM_GPIOB
#   define SSP1_RXD_FUNC FUNC_ALT
#elif (PORT(SSP1_RXD)==PORT_D)
#   define SSP1_RXD_GPIO ARM_GPIOD
#   define SSP1_RXD_FUNC FUNC_REDEF
#elif (PORT(SSP1_RXD)==PORT_E)
#   define SSP1_RXD_GPIO ARM_GPIOE
#   define SSP1_RXD_FUNC FUNC_ALT
#elif (PORT(SSP1_RXD)==PORT_F)
#   define SSP1_RXD_GPIO ARM_GPIOF
#   define SSP1_RXD_FUNC FUNC_ALT
#else
#   error "SSP1_RXD pin is not assigned in CFLAGS of target.cfg"
#endif

/* Сигнал SSP1_TXD */
#ifndef SSP1_TXD
#define SSP1_TXD    PD12
#endif

#if (PORT(SSP1_TXD)==PORT_B)
#   define SSP1_TXD_GPIO ARM_GPIOB
#   define SSP1_TXD_FUNC FUNC_ALT
#elif (PORT(SSP1_TXD)==PORT_D)
#   define SSP1_TXD_GPIO ARM_GPIOD
#   define SSP1_TXD_FUNC FUNC_REDEF
#elif (PORT(SSP1_TXD)==PORT_F)
#   define SSP1_TXD_GPIO ARM_GPIOF
#   define SSP1_TXD_FUNC FUNC_ALT
#else
#   error "SSP1_TXD pin is not assigned in CFLAGS of target.cfg"
#endif

/* Сигнал SSP1_FSS */
#ifndef SSP1_FSS
#define SSP1_FSS    PD9
#endif

#if (PORT(SSP1_FSS)==PORT_B)
#   define SSP1_FSS_GPIO ARM_GPIOB
#   define SSP1_FSS_FUNC FUNC_ALT
#elif (PORT(SSP1_FSS)==PORT_D)
#   define SSP1_FSS_GPIO ARM_GPIOD
#   define SSP1_FSS_FUNC FUNC_REDEF
#elif (PORT(SSP1_FSS)==PORT_E)
#   define SSP1_FSS_GPIO ARM_GPIOE
#   define SSP1_FSS_FUNC FUNC_ALT
#elif (PORT(SSP1_FSS)==PORT_F)
#   define SSP1_FSS_GPIO ARM_GPIOF
#   define SSP1_FSS_FUNC FUNC_ALT
#else
#   error "SSP1_FSS pin is not assigned in CFLAGS of target.cfg"
#endif

/* Сигнал SSP1_CLK */
#ifndef SSP1_CLK
#define SSP1_CLK    PD10
#endif

#if (PORT(SSP1_CLK)==PORT_B)
#   define SSP1_CLK_GPIO ARM_GPIOB
#   define SSP1_CLK_FUNC FUNC_ALT
#elif (PORT(SSP1_CLK)==PORT_D)
#   define SSP1_CLK_GPIO ARM_GPIOD
#   define SSP1_CLK_FUNC FUNC_REDEF
#elif (PORT(SSP1_CLK)==PORT_F)
#   define SSP1_CLK_GPIO ARM_GPIOF
#   define SSP1_CLK_FUNC FUNC_ALT
#else
#   error "SSP1_CLK pin is not assigned in CFLAGS of target.cfg"
#endif

/* Сигнал SSP2_RXD */
#ifndef SSP2_RXD
#define SSP2_RXD    PD2
#endif

#if (PORT(SSP2_RXD)==PORT_B)
#   define SSP2_RXD_GPIO ARM_GPIOB
#   define SSP2_RXD_FUNC FUNC_REDEF
#elif (PORT(SSP2_RXD)==PORT_C)
#   define SSP2_RXD_GPIO ARM_GPIOC
#   if (PIN(SSP2_RXD)==2)
#       define SSP2_RXD_FUNC FUNC_REDEF
#   elif (PIN(SSP2_RXD)==15)
#       define SSP2_RXD_FUNC FUNC_ALT
#   endif
#elif (PORT(SSP2_RXD)==PORT_D)
#   define SSP2_RXD_GPIO ARM_GPIOD
#   define SSP2_RXD_FUNC FUNC_ALT
#elif (PORT(SSP2_RXD)==PORT_F)
#   define SSP2_RXD_GPIO ARM_GPIOF
#   define SSP2_RXD_FUNC FUNC_REDEF
#else
#   error "SSP2_RXD pin is not assigned in CFLAGS of target.cfg"
#endif

/* Сигнал SSP2_TXD */
#ifndef SSP2_TXD
#define SSP2_TXD    PD6
#endif

#if (PORT(SSP2_TXD)==PORT_B)
#   define SSP2_TXD_GPIO ARM_GPIOB
#   define SSP2_TXD_FUNC FUNC_REDEF
#elif (PORT(SSP2_TXD)==PORT_C)
#   define SSP2_TXD_GPIO ARM_GPIOC
#   define SSP2_TXD_FUNC FUNC_REDEF
#elif (PORT(SSP2_TXD)==PORT_D)
#   define SSP2_TXD_GPIO ARM_GPIOD
#   define SSP2_TXD_FUNC FUNC_ALT
#elif (PORT(SSP2_TXD)==PORT_F)
#   define SSP2_TXD_GPIO ARM_GPIOF
#   define SSP2_TXD_FUNC FUNC_REDEF
#else
#   error "SSP2_TXD pin is not assigned in CFLAGS of target.cfg"
#endif

/* Сигнал SSP2_FSS */
#ifndef SSP2_FSS
#define SSP2_FSS    PD3
#endif

#if (PORT(SSP2_FSS)==PORT_B)
#   define SSP2_FSS_GPIO ARM_GPIOB
#   define SSP2_FSS_FUNC FUNC_REDEF
#elif (PORT(SSP2_FSS)==PORT_C)
#   define SSP2_FSS_GPIO ARM_GPIOC
#   if (PIN(SSP2_FSS)==0)
#       define SSP2_FSS_FUNC FUNC_REDEF
#   elif (PIN(SSP2_FSS)==14)
#       define SSP2_FSS_FUNC FUNC_ALT
#   endif
#elif (PORT(SSP2_FSS)==PORT_D)
#   define SSP2_FSS_GPIO ARM_GPIOD
#   define SSP2_FSS_FUNC FUNC_ALT
#elif (PORT(SSP2_FSS)==PORT_F)
#   define SSP2_FSS_GPIO ARM_GPIOF
#   define SSP2_FSS_FUNC FUNC_REDEF
#else
#   error "SSP2_FSS pin is not assigned in CFLAGS of target.cfg"
#endif

/* Сигнал SSP2_CLK */
#ifndef SSP2_CLK
#define SSP2_CLK    PD5
#endif

#if (PORT(SSP2_CLK)==PORT_B)
#   define SSP2_CLK_GPIO ARM_GPIOB
#   define SSP2_CLK_FUNC FUNC_REDEF
#elif (PORT(SSP2_CLK)==PORT_C)
#   define SSP2_CLK_GPIO ARM_GPIOC
#   define SSP2_CLK_FUNC FUNC_REDEF
#elif (PORT(SSP2_CLK)==PORT_D)
#   define SSP2_CLK_GPIO ARM_GPIOD
#   define SSP2_CLK_FUNC FUNC_ALT
#elif (PORT(SSP2_CLK)==PORT_F)
#   define SSP2_CLK_GPIO ARM_GPIOF
#   define SSP2_CLK_FUNC FUNC_REDEF
#else
#   error "SSP2_CLK pin is not assigned in CFLAGS of target.cfg"
#endif

/* Сигнал CAN1_RX */
#ifndef CAN1_RX
#define CAN1_RX PA7
#endif

#if (CAN1_RX!=PA7) && (CAN1_RX!=PB3) && (CAN1_RX!=PC9) && (CAN1_RX!=PD14) && (CAN1_RX!=PE0)
#error "Impossible assignment of CAN1_RX pin in CFLAGS of target.cfg"
#endif

#if (PORT(CAN1_RX)==PORT_A)
#       define CAN1_RX_GPIO ARM_GPIOA
#       define CAN1_RX_FUNC FUNC_ALT
#elif (PORT(CAN1_RX)==PORT_B)
#       define CAN1_RX_GPIO ARM_GPIOB
#       define CAN1_RX_FUNC FUNC_REDEF
#elif (PORT(CAN1_RX)==PORT_C)
#       define CAN1_RX_GPIO ARM_GPIOC
#       define CAN1_RX_FUNC FUNC_MAIN
#elif (PORT(CAN1_RX)==PORT_D)
#       define CAN1_RX_GPIO ARM_GPIOD
#       define CAN1_RX_FUNC FUNC_REDEF
#elif (PORT(CAN1_RX)==PORT_E)
#       define CAN1_RX_GPIO ARM_GPIOE
#       define CAN1_RX_FUNC FUNC_REDEF
#else
#       error "CAN1_RX pin is not assigned in CFLAGS of target.cfg"
#endif

/* Сигнал CAN1_TX */
#ifndef CAN1_TX
#define CAN1_TX PA6
#endif

#if (CAN1_TX!=PA6) && (CAN1_TX!=PB2) && (CAN1_TX!=PC8) && (CAN1_TX!=PD13) && (CAN1_TX!=PE1)
#error "Impossible assignment of CAN1_TX pin in CFLAGS of target.cfg"
#endif

#if (PORT(CAN1_TX)==PORT_A)
#       define CAN1_TX_GPIO ARM_GPIOA
#       define CAN1_TX_FUNC FUNC_ALT
#elif (PORT(CAN1_TX)==PORT_B)
#       define CAN1_TX_GPIO ARM_GPIOB
#       define CAN1_TX_FUNC FUNC_REDEF
#elif (PORT(CAN1_TX)==PORT_C)
#       define CAN1_TX_GPIO ARM_GPIOC
#       define CAN1_TX_FUNC FUNC_MAIN
#elif (PORT(CAN1_TX)==PORT_D)
#       define CAN1_TX_GPIO ARM_GPIOD
#       define CAN1_TX_FUNC FUNC_REDEF
#elif (PORT(CAN1_TX)==PORT_E)
#       define CAN1_TX_GPIO ARM_GPIOE
#       define CAN1_TX_FUNC FUNC_REDEF
#else
#       error "CAN1_TX pin is not assigned in CFLAGS of target.cfg"
#endif

/* Сигнал CAN2_RX */
#ifndef CAN2_RX
#define CAN2_RX PC14
#endif

#if (CAN2_RX!=PC14) && (CAN2_RX!=PD15) && (CAN2_RX!=PE6) && (CAN2_RX!=PF2)
#error "Impossible assignment of CAN2_RX pin in CFLAGS of target.cfg"
#endif

#if (PORT(CAN2_RX)==PORT_C)
#       define CAN2_RX_GPIO ARM_GPIOC
#       define CAN2_RX_FUNC FUNC_REDEF
#elif (PORT(CAN2_RX)==PORT_D)
#       define CAN2_RX_GPIO ARM_GPIOD
#       define CAN2_RX_FUNC FUNC_MAIN
#elif (PORT(CAN2_RX)==PORT_E)
#       define CAN2_RX_GPIO ARM_GPIOE
#       define CAN2_RX_FUNC FUNC_ALT
#elif (PORT(CAN2_RX)==PORT_F)
#       define CAN2_RX_GPIO ARM_GPIOF
#       define CAN2_RX_FUNC FUNC_REDEF
#else
#       error "CAN2_RX pin is not assigned in CFLAGS of target.cfg"
#endif

/* Сигнал CAN2_TX */
#ifndef CAN2_TX
#define CAN2_TX PC15
#endif

#if (CAN2_TX!=PC15) && (CAN2_TX!=PD9) && (CAN2_TX!=PE7) && (CAN2_TX!=PF3)
#error "Impossible assignment of CAN2_TX pin in CFLAGS of target.cfg"
#endif

#if (PORT(CAN2_TX)==PORT_C)
#       define CAN2_TX_GPIO ARM_GPIOC
#       define CAN2_TX_FUNC FUNC_REDEF
#elif (PORT(CAN2_TX)==PORT_D)
#       define CAN2_TX_GPIO ARM_GPIOD
#       define CAN2_TX_FUNC FUNC_MAIN
#elif (PORT(CAN2_TX)==PORT_E)
#       define CAN2_TX_GPIO ARM_GPIOE
#       define CAN2_TX_FUNC FUNC_ALT
#elif (PORT(CAN2_TX)==PORT_F)
#       define CAN2_TX_GPIO ARM_GPIOF
#       define CAN2_TX_FUNC FUNC_REDEF
#else
#       error "CAN2_TX pin is not assigned in CFLAGS of target.cfg"
#endif


/* End of Milandr 1986BE9x register definitions.
 *----------------------------------------------*/
