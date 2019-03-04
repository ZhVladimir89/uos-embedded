/*
 * Register definitions for STM32F4xx.
 *
 * Copyright (C) 2015 Dmitry Podkhvatilin, <vatilin@gmail.com>
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


#ifndef KHZ_HCLK
#define KHZ_HCLK    KHZ
#endif

#ifndef KHZ_PCLK1
#define KHZ_PCLK1   KHZ_HCLK
#endif

#ifndef KHZ_PCLK2
#define KHZ_PCLK2   KHZ_HCLK
#endif


#define ARM_SRAM_BASE       0x20000000  // Internal static memory
#define ARM_PERIPH_BASE     0x40000000  // Peripheral registers
#define ARM_SYSTEM_BASE     0xE0000000  // Core registers

#define ARM_SRAM_SIZE       (192*1024)  // Internal SRAM size

#define STM_EEPROM_BASE     0x08080000  // EEPROM base address

#include <runtime/cortex-m/io-cortex-m.h>

//
// Peripheral memory map
//
#define STM_OB_BASE             0x1FF80000
#define STM_TIM2_BASE           0x40000000
#define STM_TIM3_BASE           0x40000400
#define STM_TIM4_BASE           0x40000800
#define STM_TIM5_BASE           0x40000C00
#define STM_TIM6_BASE           0x40001000
#define STM_TIM7_BASE           0x40001400
#define STM_TIM12_BASE          0x40001800
#define STM_TIM13_BASE          0x40001C00
#define STM_TIM14_BASE          0x40002000
#define STM_LCD_BASE            0x40002400
#define STM_RTC_BKP_BASE        0x40002800
#define STM_WWDG_BASE           0x40002C00
#define STM_IWDG_BASE           0x40003000
#define STM_I2S2EXT_BASE        0x40003400
#define STM_SPI2_I2S2_BASE      0x40003800
#define STM_SPI3_I2S3_BASE      0x40003C00
#define STM_I2S3EXT_BASE        0x40004000
#define STM_USART2_BASE         0x40004400
#define STM_USART3_BASE         0x40004800
#define STM_UART4_BASE          0x40004C00
#define STM_UART5_BASE          0x40005000
#define STM_I2C1_BASE           0x40005400
#define STM_I2C2_BASE           0x40005800
#define STM_USB_DEV_FS_BASE     0x40005C00
#define STM_USB_DEV_FS_SRAM_BASE 0x40006000
#define STM_PWR_BASE            0x40007000
#define STM_DAC_BASE            0x40007400
#define STM_COMP_BASE           0x40007C00
#define STM_RI_BASE             0x40007C04
#define STM_OPAMP_BASE          0x40007C5C
#define STM_SYSCFG_BASE         0x40010000
#define STM_EXTI_BASE           0x40010400
#define STM_TIM9_BASE           0x40010800
#define STM_TIM10_BASE          0x40010C00
#define STM_TIM11_BASE          0x40011000
#define STM_ADC_BASE            0x40012400
#define STM_SDIO_BASE           0x40012C00
#define STM_SPI1_BASE           0x40013000
#define STM_USART1_BASE         0x40013800
#define STM_GPIOA_BASE          0x40020000
#define STM_GPIOB_BASE          0x40020400
#define STM_GPIOC_BASE          0x40020800
#define STM_GPIOD_BASE          0x40020C00
#define STM_GPIOE_BASE          0x40021000
#define STM_GPIOH_BASE          0x40021400 // Перепутан порядок букв
#define STM_GPIOF_BASE          0x40021800 // Сделано, как написано в RM0038
#define STM_GPIOG_BASE          0x40021C00
#define STM_CRC_BASE            0x40023000
#define STM_RCC_BASE            0x40023800
#define STM_FLASH_IFACE_BASE    0x40023C00
#define STM_BKPSRAM_BASE        0x40024000
#define STM_DMA1_BASE           0x40026000
#define STM_DMA2_BASE           0x40026400
#define STM_CRYP_BASE           0x50060000
#define STM_FSMC_BASE           0xA0000000


typedef enum
{
    IRQ_WWDG,
    IRQ_PVD,
    IRQ_TAMPER_STAMP,
    IRQ_RTC_WKUP,
    IRQ_FLASH,
    IRQ_RCC,
    IRQ_EXTI0,
    IRQ_EXTI1,
    IRQ_EXTI2,
    IRQ_EXTI3,
    IRQ_EXTI4,
    IRQ_DMA1_CHANNEL1,
    IRQ_DMA1_CHANNEL2,
    IRQ_DMA1_CHANNEL3,
    IRQ_DMA1_CHANNEL4,
    IRQ_DMA1_CHANNEL5,
    IRQ_DMA1_CHANNEL6,
    IRQ_DMA1_CHANNEL7,
    IRQ_ADC1,
    IRQ_USB_HP,
    IRQ_USB_LP,
    IRQ_DAC,
    IRQ_COMP_CA,
    IRQ_EXTI9_5,
    IRQ_LCD,
    IRQ_TIM9,
    IRQ_TIM10,
    IRQ_TIM11,
    IRQ_TIM2,
    IRQ_TIM3,
    IRQ_TIM4,
    IRQ_I2C1_EV,
    IRQ_I2C1_ER,
    IRQ_I2C2_EV,
    IRQ_I2C2_ER,
    IRQ_SPI1,
    IRQ_SPI2,
    IRQ_USART1,
    IRQ_USART2,
    IRQ_USART3,
    IRQ_EXTI15_10,
    IRQ_RTC_ALARM,
    IRQ_USB_FS_WKUP,
    IRQ_TIM6,
    IRQ_TIM7,
#if defined(STM32L1_CAT3)
    IRQ_TIM5,
    IRQ_SPI3,
    IRQ_DMA2_CHANNEL1,
    IRQ_DMA2_CHANNEL2,
    IRQ_DMA2_CHANNEL3,
    IRQ_DMA2_CHANNEL4,
    IRQ_DMA2_CHANNEL5,
    IRQ_AES,
    IRQ_COMP_ACQ,
#endif
#if defined(STM32L1_CAT4) || defined(STM32L1_CAT5) || defined(STM32L1_CAT6)
    IRQ_SDIO,
    IRQ_TIM5,
    IRQ_SPI3,
    IRQ_UART4,
    IRQ_UART5,
    IRQ_DMA2_CHANNEL1,
    IRQ_DMA2_CHANNEL2,
    IRQ_DMA2_CHANNEL3,
    IRQ_DMA2_CHANNEL4,
    IRQ_DMA2_CHANNEL5,
    IRQ_AES,
    IRQ_COMP_ACQ,
#endif
    ARCH_TIMER_IRQ,
    ARCH_INTERRUPTS
} IRQn_t;


/////////////////////////////////////////
// External interrupt/event controller
/////////////////////////////////////////
typedef struct
{
    arm_reg_t IMR;      // Interrupt mask
    arm_reg_t EMR;      // Event mask
    arm_reg_t RTSR;     // Rising edge trigger selection
    arm_reg_t FTSR;     // Falling edge trigger selection
    arm_reg_t SWIER;    // Software interrupt event
    arm_reg_t PR;       // Pending
} EXTI_t;

#define EXTI     ((EXTI_t *) STM_EXTI_BASE)

// EXTI lines
#define EXTI_GPIO(n)        (1 << (n))
#define EXTI_PVD            (1 << 16)
#define EXTI_RTC_ALARM      (1 << 17)
#define EXTI_USB_DEV_FS     (1 << 18)
#define EXTI_RTC_TAMP_TS    (1 << 19)
#define EXTI_RTC_WKUP       (1 << 20)
#define EXTI_COMP1_WKUP     (1 << 21)
#define EXTI_COMP2_WKUP     (1 << 22)
#define EXTI_CHAN_ACQ       (1 << 23)


/////////////////////////////////////////
// Power control
/////////////////////////////////////////
typedef struct
{
    arm_reg_t CR;       // Power control
    arm_reg_t CSR;      // Power control/status
} PWR_t;

#define PWR     ((PWR_t *) STM_PWR_BASE)

// CR bits
#define PWR_LPRUN           (1 << 14)
#define PWR_VOS_1_8         (1 << 11)
#define PWR_VOS_1_5         (2 << 11)
#define PWR_VOS_1_2         (3 << 11)
#define PWR_VOS_MASK        (3 << 11)
#define PWR_FWU             (1 << 10)
#define PWR_ULP             (1 << 9)
#define PWR_DBP             (1 << 8)
#define PWR_PLS_1_9         (0 << 5)
#define PWR_PLS_2_1         (1 << 5)
#define PWR_PLS_2_3         (2 << 5)
#define PWR_PLS_2_5         (3 << 5)
#define PWR_PLS_2_7         (4 << 5)
#define PWR_PLS_2_9         (5 << 5)
#define PWR_PLS_3_1         (6 << 5)
#define PWR_PLS_EXT         (7 << 5)
#define PWR_PVDE            (1 << 4)
#define PWR_CSBF            (1 << 3)
#define PWR_CWUF            (1 << 2)
#define PWR_PDDS            (1 << 1)
#define PWR_LPSDSR          (1 << 0)

// CSR bits
#define PWR_EWUP3           (1 << 10)
#define PWR_EWUP2           (1 << 9)
#define PWR_EWUP1           (1 << 8)
#define PWR_REGLPF          (1 << 5)
#define PWR_VOSF            (1 << 4)
#define PWR_VREFINTRDYF     (1 << 3)
#define PWR_PVDO            (1 << 2)
#define PWR_SBF             (1 << 1)
#define PWR_WUF             (1 << 0)


/////////////////////////////////////////
// Reset and clock control
/////////////////////////////////////////
typedef struct
{
    arm_reg_t CR;       // Clock control
    arm_reg_t ICSCR;    // Internal clock sources calibration
    arm_reg_t CFGR;     // Clock configuration
    arm_reg_t CIR;      // Clock interrupt
    arm_reg_t AHBRSTR;  // AHB peripheral reset
    arm_reg_t APB2RSTR; // APB2 peripheral reset
    arm_reg_t APB1RSTR; // APB1 peripheral reset
    arm_reg_t AHBENR;   // AHB peripheral clock enable
    arm_reg_t APB2ENR;  // APB2 peripheral clock enable
    arm_reg_t APB1ENR;  // APB1 peripheral clock enable
    arm_reg_t AHBLPENR; // AHB peripheral clock enable in low power mode
    arm_reg_t APB2LPENR;// APB2 peripheral clock enable in low power mode
    arm_reg_t APB1LPENR;// APB1 peripheral clock enable in low power mode
    arm_reg_t CSR;      // Clock control & status
} RCC_t;

#define RCC     ((RCC_t *) STM_RCC_BASE)

// CR bits
#define RCC_RTCPRE_DIV2         (0 << 30)
#define RCC_RTCPRE_DIV4         (1 << 30)
#define RCC_RTCPRE_DIV8         (2 << 30)
#define RCC_RTCPRE_DIV16        (3 << 30)
#define RCC_CSSON               (1 << 28)
#define RCC_PLLRDY              (1 << 25)
#define RCC_PLLON               (1 << 24)
#define RCC_HSEBYP              (1 << 18)
#define RCC_HSERDY              (1 << 17)
#define RCC_HSEON               (1 << 16)
#define RCC_MSIRDY              (1 << 9)
#define RCC_MSION               (1 << 8)
#define RCC_HSIRDY              (1 << 1)
#define RCC_HSION               (1 << 0)

// ICSCR
#define RCC_MSITRIM(n)          ((n) << 24)
#define RCC_MSICAL(n)           ((n) << 16)
#define RCC_MSIRANGE_65536      (0 << 13)
#define RCC_MSIRANGE_131072     (1 << 13)
#define RCC_MSIRANGE_262144     (2 << 13)
#define RCC_MSIRANGE_524288     (3 << 13)
#define RCC_MSIRANGE_1048K      (4 << 13)
#define RCC_MSIRANGE_2097K      (5 << 13)
#define RCC_MSIRANGE_4194K      (6 << 13)
#define RCC_MSIRANGE_MASK       (7 << 13)
#define RCC_HSITRIM(n)          ((n) << 8)
#define RCC_HSICAL(n)           ((n))

// CFGR bits
#define RCC_MCOPRE_NODIV        (0 << 28)
#define RCC_MCOPRE_DIV2         (1 << 28)
#define RCC_MCOPRE_DIV4         (2 << 28)
#define RCC_MCOPRE_DIV8         (3 << 28)
#define RCC_MCOPRE_DIV16        (4 << 28)
#define RCC_MCOSEL_NO_OUT       (0 << 24)
#define RCC_MCOSEL_SYSCLK       (1 << 24)
#define RCC_MCOSEL_HSI          (2 << 24)
#define RCC_MCOSEL_MSI          (3 << 24)
#define RCC_MCOSEL_HSE          (4 << 24)
#define RCC_MCOSEL_PLL          (5 << 24)
#define RCC_MCOSEL_LSI          (6 << 24)
#define RCC_MCOSEL_LSE          (7 << 24)
#define RCC_PLLDIV2             (1 << 22)
#define RCC_PLLDIV3             (2 << 22)
#define RCC_PLLDIV4             (3 << 22)
#define RCC_PLLMUL3             (0 << 18)
#define RCC_PLLMUL4             (1 << 18)
#define RCC_PLLMUL6             (2 << 18)
#define RCC_PLLMUL8             (3 << 18)
#define RCC_PLLMUL12            (4 << 18)
#define RCC_PLLMUL16            (5 << 18)
#define RCC_PLLMUL24            (6 << 18)
#define RCC_PLLMUL32            (7 << 18)
#define RCC_PLLMUL48            (8 << 18)
#define RCC_PLLSRC_HSI          (0 << 16)
#define RCC_PLLSRC_HSE          (1 << 16)
#define RCC_PPRE2_DIV(x)        ((((x) >> 1) + 3) << 11)
#define RCC_PPRE1_DIV(x)        ((((x) >> 1) + 3) << 8)
#define RCC_HPRE_DIV(x)         ((((x) >> 1) + 7) << 4)
#define RCC_SWS_MSI             (0 << 2)
#define RCC_SWS_HSI             (1 << 2)
#define RCC_SWS_HSE             (2 << 2)
#define RCC_SWS_PLL             (3 << 2)
#define RCC_SWS_MASK            (3 << 2)
#define RCC_SW_MSI              (0 << 0)
#define RCC_SW_HSI              (1 << 0)
#define RCC_SW_HSE              (2 << 0)
#define RCC_SW_PLL              (3 << 0)
#define RCC_SW_MASK             (3 << 0)

// CIR bits
#define RCC_CSSC                (1 << 23)
#define RCC_LSECSSC             (1 << 22)
#define RCC_MSIRDYC             (1 << 21)
#define RCC_PLLRDYC             (1 << 20)
#define RCC_HSERDYC             (1 << 19)
#define RCC_HSIRDYC             (1 << 18)
#define RCC_LSERDYC             (1 << 17)
#define RCC_LSIRDYC             (1 << 16)
#define RCC_LSECSSIE            (1 << 14)
#define RCC_MSIRDYIE            (1 << 13)
#define RCC_PLLRDYIE            (1 << 12)
#define RCC_HSERDYIE            (1 << 11)
#define RCC_HSIRDYIE            (1 << 10)
#define RCC_LSERDYIE            (1 << 9)
#define RCC_LSIRDYIE            (1 << 8)
#define RCC_CSSF                (1 << 7)
#define RCC_LSECSSF             (1 << 6)
#define RCC_PLLI2SSDRYF         (1 << 5)
#define RCC_PLLRDYF             (1 << 4)
#define RCC_HSERDYF             (1 << 3)
#define RCC_HSIRDYF             (1 << 2)
#define RCC_LSERDYF             (1 << 1)
#define RCC_LSIRDYF             (1 << 0)

// AHBRSTR bits
#define RCC_FSMCRST             (1 << 30)
#define RCC_AESRST              (1 << 27)
#define RCC_DMA2RST             (1 << 25)
#define RCC_DMA1RST             (1 << 24)
#define RCC_FLITFRST            (1 << 15)
#define RCC_CRCRST              (1 << 12)
#define RCC_GPIOGRST            (1 << 7)
#define RCC_GPIOFRST            (1 << 6)
#define RCC_GPIOHRST            (1 << 5)
#define RCC_GPIOERST            (1 << 4)
#define RCC_GPIODRST            (1 << 3)
#define RCC_GPIOCRST            (1 << 2)
#define RCC_GPIOBRST            (1 << 1)
#define RCC_GPIOARST            (1 << 0)

// APB2RSTR bits
#define RCC_USART1RST           (1 << 14)
#define RCC_SPI1RST             (1 << 12)
#define RCC_SDIORST             (1 << 11)
#define RCC_ADC1RST             (1 << 9)
#define RCC_TIM11RST            (1 << 4)
#define RCC_TIM10RST            (1 << 3)
#define RCC_TIM9RST             (1 << 2)
#define RCC_SYSCFGRST           (1 << 0)

// APB1RSTR bits
#define RCC_COMPRST             (1 << 31)
#define RCC_DACRST              (1 << 29)
#define RCC_PWRRST              (1 << 28)
#define RCC_USBRST              (1 << 23)
#define RCC_I2C2RST             (1 << 22)
#define RCC_I2C1RST             (1 << 21)
#define RCC_UART5RST            (1 << 20)
#define RCC_UART4RST            (1 << 19)
#define RCC_USART3RST           (1 << 18)
#define RCC_USART2RST           (1 << 17)
#define RCC_SPI3RST             (1 << 15)
#define RCC_SPI2RST             (1 << 14)
#define RCC_WWDGRST             (1 << 11)
#define RCC_LCDRST              (1 << 9)
#define RCC_TIM7RST             (1 << 5)
#define RCC_TIM6RST             (1 << 4)
#define RCC_TIM5RST             (1 << 3)
#define RCC_TIM4RST             (1 << 2)
#define RCC_TIM3RST             (1 << 1)
#define RCC_TIM2RST             (1 << 0)

// AHBENR bits
#define RCC_FSMCEN              (1 << 30)
#define RCC_AESEN               (1 << 27)
#define RCC_DMA2EN              (1 << 25)
#define RCC_DMA1EN              (1 << 24)
#define RCC_FLITFEN             (1 << 15)
#define RCC_CRCEN               (1 << 12)
#define RCC_GPIOGEN             (1 << 7)
#define RCC_GPIOFEN             (1 << 6)
#define RCC_GPIOHEN             (1 << 5)
#define RCC_GPIOEEN             (1 << 4)
#define RCC_GPIODEN             (1 << 3)
#define RCC_GPIOCEN             (1 << 2)
#define RCC_GPIOBEN             (1 << 1)
#define RCC_GPIOAEN             (1 << 0)

// APB2ENR bits
#define RCC_USART1EN           (1 << 14)
#define RCC_SPI1EN             (1 << 12)
#define RCC_SDIOEN             (1 << 11)
#define RCC_ADC1EN             (1 << 9)
#define RCC_TIM11EN            (1 << 4)
#define RCC_TIM10EN            (1 << 3)
#define RCC_TIM9EN             (1 << 2)
#define RCC_SYSCFGEN           (1 << 0)

// APB1ENR bits
#define RCC_COMPEN             (1 << 31)
#define RCC_DACEN              (1 << 29)
#define RCC_PWREN              (1 << 28)
#define RCC_USBEN              (1 << 23)
#define RCC_I2C2EN             (1 << 22)
#define RCC_I2C1EN             (1 << 21)
#define RCC_UART5EN            (1 << 20)
#define RCC_UART4EN            (1 << 19)
#define RCC_USART3EN           (1 << 18)
#define RCC_USART2EN           (1 << 17)
#define RCC_SPI3EN             (1 << 15)
#define RCC_SPI2EN             (1 << 14)
#define RCC_WWDGEN             (1 << 11)
#define RCC_LCDEN              (1 << 9)
#define RCC_TIM7EN             (1 << 5)
#define RCC_TIM6EN             (1 << 4)
#define RCC_TIM5EN             (1 << 3)
#define RCC_TIM4EN             (1 << 2)
#define RCC_TIM3EN             (1 << 1)
#define RCC_TIM2EN             (1 << 0)

// AHBLPENR bits
#define RCC_FSMCLPEN           (1 << 30)
#define RCC_AESLPEN            (1 << 27)
#define RCC_DMA2LPEN           (1 << 25)
#define RCC_DMA1LPEN           (1 << 24)
#define RCC_SRAMLPEN           (1 << 16)
#define RCC_FLITFLPEN          (1 << 15)
#define RCC_CRCLPEN            (1 << 12)
#define RCC_GPIOGLPEN          (1 << 7)
#define RCC_GPIOFLPEN          (1 << 6)
#define RCC_GPIOHLPEN          (1 << 5)
#define RCC_GPIOELPEN          (1 << 4)
#define RCC_GPIODLPEN          (1 << 3)
#define RCC_GPIOCLPEN          (1 << 2)
#define RCC_GPIOBLPEN          (1 << 1)
#define RCC_GPIOALPEN          (1 << 0)

// APB2LPENR bits
#define RCC_USART1LPEN         (1 << 14)
#define RCC_SPI1LPEN           (1 << 12)
#define RCC_SDIOLPEN           (1 << 11)
#define RCC_ADC1LPEN           (1 << 9)
#define RCC_TIM11LPEN          (1 << 4)
#define RCC_TIM10LPEN          (1 << 3)
#define RCC_TIM9LPEN           (1 << 2)
#define RCC_SYSCFGLPEN         (1 << 0)

// APB1LPENR bits
#define RCC_COMPLPEN           (1 << 31)
#define RCC_DACLPEN            (1 << 29)
#define RCC_PWRLPEN            (1 << 28)
#define RCC_USBLPEN            (1 << 23)
#define RCC_I2C2LPEN           (1 << 22)
#define RCC_I2C1LPEN           (1 << 21)
#define RCC_UART5LPEN          (1 << 20)
#define RCC_UART4LPEN          (1 << 19)
#define RCC_USART3LPEN         (1 << 18)
#define RCC_USART2LPEN         (1 << 17)
#define RCC_SPI3LPEN           (1 << 15)
#define RCC_SPI2LPEN           (1 << 14)
#define RCC_WWDGLPEN           (1 << 11)
#define RCC_LCDLPEN            (1 << 9)
#define RCC_TIM7LPEN           (1 << 5)
#define RCC_TIM6LPEN           (1 << 4)
#define RCC_TIM5LPEN           (1 << 3)
#define RCC_TIM4LPEN           (1 << 2)
#define RCC_TIM3LPEN           (1 << 1)
#define RCC_TIM2LPEN           (1 << 0)

// CSR bits
#define RCC_LPWRRSTF            (1 << 31)
#define RCC_WWDGRSTF            (1 << 30)
#define RCC_IWDGRSTF            (1 << 29)
#define RCC_SFTRSTF             (1 << 28)
#define RCC_PORRSTF             (1 << 27)
#define RCC_PINRSTF             (1 << 26)
#define RCC_OBLRSTF             (1 << 25)
#define RCC_RMVF                (1 << 24)
#define RCC_RTCRST              (1 << 23)
#define RCC_RTCEN               (1 << 22)
#define RCC_RTCSEL_NO_CLK       (0 << 16)
#define RCC_RTCSEL_LSE          (1 << 16)
#define RCC_RTCSEL_LSI          (2 << 16)
#define RCC_RTCSEL_HSE          (3 << 16)
#define RCC_LSECSSD             (1 << 12)
#define RCC_LSECSSON            (1 << 11)
#define RCC_LSEBYP              (1 << 10)
#define RCC_LSERDY              (1 << 9)
#define RCC_LSEON               (1 << 8)
#define RCC_LSIRDY              (1 << 1)
#define RCC_LSION               (1 << 0)


/////////////////////////////////////////
// Flash interface
/////////////////////////////////////////
typedef struct
{
    arm_reg_t ACR;      // Access control
    arm_reg_t PECR;     // Program/erase control
    arm_reg_t PDKEYR;   // Power down key
    arm_reg_t PEKEYR;   // Program/erase key
    arm_reg_t PRGKEYR;  // Program memory key
    arm_reg_t OPTKEYR;  // Option byte key
    arm_reg_t SR;       // Status
    arm_reg_t OBR;      // Option byte
    arm_reg_t WRPR1;    // Write protection
    arm_reg_t res0[23]; // Reserved
    arm_reg_t WRPR2;    // Write protection
    arm_reg_t WRPR3;    // Write protection
    arm_reg_t WRPR4;    // Write protection
} FLASH_t;

#define FLASH     ((FLASH_t*) STM_FLASH_IFACE_BASE)

// ACR bits
#define FLASH_RUN_PD            (1 << 4)
#define FLASH_SLEEP_PD          (1 << 3)
#define FLASH_ACC64             (1 << 2)
#define FLASH_PRFTEN            (1 << 1)
#define FLASH_LATENCY(n)        (n)

// PECR bits
#define FLASH_OBL_LAUNCH        (1 << 18)
#define FLASH_ERRIE             (1 << 17)
#define FLASH_EOPIE             (1 << 16)
#define FLASH_PARALLBANK        (1 << 15)
#define FLASH_FPRG              (1 << 10)
#define FLASH_ERASE             (1 << 9)
#define FLASH_FTDW              (1 << 8)
#define FLASH_DATA              (1 << 4)
#define FLASH_PROG              (1 << 3)
#define FLASH_OPTLOCK           (1 << 2)
#define FLASH_PRGLOCK           (1 << 1)
#define FLASH_PELOCK            (1 << 0)

// PDKEYR bits
#define FLASH_PDKEY1            0x04152637
#define FLASH_PDKEY2            0xFAFBFCFD

// PEKEYR bits
#define FLASH_PEKEY1            0x89ABCDEF
#define FLASH_PEKEY2            0x02030405

// PRGKEYR bits
#define FLASH_PRGKEY1           0x8C9DAEBF
#define FLASH_PRGKEY2           0x13141516

// OPTKEYR bits
#define FLASH_OPTKEY1           0xFBEAD9C8
#define FLASH_OPTKEY2           0x24252627

// SR bits
#define FLASH_RDERR             (1 << 13)
#define FLASH_OPTVERRUSR        (1 << 12)
#define FLASH_OPTVERR           (1 << 11)
#define FLASH_SIZERR            (1 << 10)
#define FLASH_PGAERR            (1 << 9)
#define FLASH_WRPERR            (1 << 8)
#define FLASH_ERR_MASK          (0x1F << 8)
#define FLASH_READY             (1 << 3)
#define FLASH_ENDHV             (1 << 2)
#define FLASH_EOP               (1 << 1)
#define FLASH_BSY               (1 << 0)

// OBR bits
#define FLASH_nBFB2             (1 << 23)
#define FLASH_nRST_STDBY        (1 << 22)
#define FLASH_nRST_STOP         (1 << 21)
#define FLASH_IWDG_SW           (1 << 20)
#define FLASH_BOR_OFF           (0 << 19)
#define FLASH_BOR_OFF_MASK      (1 << 19)
#define FLASH_BOR_LEVEL_MASK    (0xF << 16)
#define FLASH_BOR_LEVEL1        (0x8 << 16)
#define FLASH_BOR_LEVEL2        (0x9 << 16)
#define FLASH_BOR_LEVEL3        (0xA << 16)
#define FLASH_BOR_LEVEL4        (0xB << 16)
#define FLASH_BOR_LEVEL5        (0xC << 16)
#define FLASH_SPRMOD            (1 << 8)
#define FLASH_RDPRT_LEVEL0      0xAA
#define FLASH_RDPRT_LEVEL2      0xCC


/////////////////////////////////////////
// Option bytes
/////////////////////////////////////////
typedef struct
{
    arm_reg_t RDP;
    arm_reg_t USER;
    arm_reg_t WRP1_LO;
    arm_reg_t WRP1_HI;
    arm_reg_t WRP2_LO;
    arm_reg_t WRP2_HI;
    arm_reg_t WRP3_LO;
    arm_reg_t WRP3_HI;
    arm_reg_t res0[24];
    arm_reg_t WRP4_LO;
    arm_reg_t WRP4_HI;
} OB_t;

#define OB     ((OB_t*) STM_OB_BASE)

// RDP bits
#define OB_RDP_ALWAYS_ONES      0xFE000000
#define OB_nSPRMOD              (1 << 24)
#define OB_nRDP(x)              ((x) << 16)
#define OB_RDP_ALWAYS_ZEROES    0x0000FE00
#define OB_SPRMOD               (1 << 8)
#define OB_RDP_LEVEL0           0xAA
#define OB_RDP_LEVEL2           0xCC

// USER bits
// nUSER field
#define OB_BFB2                 (1 << 23)
#define OB_RST_STDBY            (1 << 22)
#define OB_RST_STOP             (1 << 21)
#define OB_nIWDG_SW             (1 << 20)
#define OB_nBOR_OFF             (1 << 19)
#define OB_nBOR_OFF_MASK        (1 << 19)
#define OB_nBOR_LEVEL_MASK      (0xF << 16)
#define OB_nBOR_LEVEL1          (0x7 << 16)
#define OB_nBOR_LEVEL2          (0x6 << 16)
#define OB_nBOR_LEVEL3          (0x5 << 16)
#define OB_nBOR_LEVEL4          (0x4 << 16)
#define OB_nBOR_LEVEL5          (0x3 << 16)
// USER field
#define OB_nBFB2                (1 << 7)
#define OB_nRST_STDBY           (1 << 6)
#define OB_nRST_STOP            (1 << 5)
#define OB_IWDG_SW              (1 << 4)
#define OB_BOR_OFF              (0 << 3)
#define OB_BOR_OFF_MASK         (1 << 3)
#define OB_BOR_LEVEL_MASK       (0xF << 0)
#define OB_BOR_LEVEL1           (0x8 << 0)
#define OB_BOR_LEVEL2           (0x9 << 0)
#define OB_BOR_LEVEL3           (0xA << 0)
#define OB_BOR_LEVEL4           (0xB << 0)
#define OB_BOR_LEVEL5           (0xC << 0)


/////////////////////////////////////////
// CRC calculation
/////////////////////////////////////////
typedef struct
{
    arm_reg_t DR;    // Data
    arm_reg_t IDR;   // Independent data
    arm_reg_t CR;    // Control
} CRC_t;

#define CRC         ((CRC_t*) STM_CRC_BASE)

// CRC_CR bits
#define CRC_RESET       (1 << 0)


/////////////////////////////////////////
// General purpose I/O
/////////////////////////////////////////
typedef struct
{
    arm_reg_t MODER;    // Mode
    arm_reg_t OTYPER;   // Output type
    arm_reg_t OSPEEDR;  // Output speed
    arm_reg_t PUPDR;    // Pull-up/pull-down
    arm_reg_t IDR;      // Input data
    arm_reg_t ODR;      // Output data
    arm_reg_t BSRR;     // Bit set/reset
    arm_reg_t LCKR;     // Configuration lock
    arm_reg_t AFRL;     // Alternate function low register
    arm_reg_t AFRH;     // Alternate function high register
    arm_reg_t BRR;      // Bit reset (Cat. 3,4,5,6 only)
} GPIO_t;

#define GPIOA       ((GPIO_t*) STM_GPIOA_BASE)
#define GPIOB       ((GPIO_t*) STM_GPIOB_BASE)
#define GPIOC       ((GPIO_t*) STM_GPIOC_BASE)
#define GPIOD       ((GPIO_t*) STM_GPIOD_BASE)
#define GPIOE       ((GPIO_t*) STM_GPIOE_BASE)
#define GPIOF       ((GPIO_t*) STM_GPIOF_BASE)
#define GPIOG       ((GPIO_t*) STM_GPIOG_BASE)
#define GPIOH       ((GPIO_t*) STM_GPIOH_BASE)
#define GPIOI       ((GPIO_t*) STM_GPIOI_BASE)

// MODER bits
#define GPIO_IN(n)          (0 << (2 * n))  // Input mode
#define GPIO_OUT(n)         (1 << (2 * n))  // General purpose output mode
#define GPIO_ALT(n)         (2 << (2 * n))  // Alternate function mode
#define GPIO_ANA(n)         (3 << (2 * n))  // Analog mode
#define GPIO_MODE_MASK(n)   (3 << (2 * n))  // Mask

// OTYPER bits
#define GPIO_PP(n)      (0 << (n))      // Push-pull
#define GPIO_OD(n)      (1 << (n))      // Open drain

// OSPEEDR bits
#define GPIO_2MHz(n)        (0 << (2 * n))  // Low speed
#define GPIO_25MHz(n)       (1 << (2 * n))  // Medium speed
#define GPIO_50MHz(n)       (2 << (2 * n))  // Fast speed
#define GPIO_100MHz(n)      (3 << (2 * n))  // High speed
#define GPIO_OSPEED_MASK(n) (3 << (2 * n))  // Field mask

// PUPDR bits
#define GPIO_NO_PULL(n)   (0 << (2 * n))  // No pull-up, no pull-down
#define GPIO_PULL_UP(n)   (1 << (2 * n))  // Pull-up
#define GPIO_PULL_DOWN(n) (2 << (2 * n))  // Pull-down

// BSRR bits
#define GPIO_SET(n)     (1 << (n))      // Atomic set n-th bit of ODR
#define GPIO_RESET(n)   (1 << (n + 16)) // Atomic reset n-th bit of ODR

// LCKR bits
#define GPIO_LCK(n)     (1 << (n))      // Pin configuration lock
#define GPIO_LCKK(n)    (1 << 16)       // Port configuration lock key

// Alternate functions (AFRL and AFRH bits)
#define GPIO_AF_SYSTEM(n)   (0x0 << (4 * ((n) & 7)))
#define GPIO_AF_TIM1(n)     (0x1 << (4 * ((n) & 7)))
#define GPIO_AF_TIM2(n)     (0x1 << (4 * ((n) & 7)))
#define GPIO_AF_TIM3(n)     (0x2 << (4 * ((n) & 7)))
#define GPIO_AF_TIM4(n)     (0x2 << (4 * ((n) & 7)))
#define GPIO_AF_TIM5(n)     (0x2 << (4 * ((n) & 7)))
#define GPIO_AF_TIM8(n)     (0x3 << (4 * ((n) & 7)))
#define GPIO_AF_TIM9(n)     (0x3 << (4 * ((n) & 7)))
#define GPIO_AF_TIM10(n)    (0x3 << (4 * ((n) & 7)))
#define GPIO_AF_TIM11(n)    (0x3 << (4 * ((n) & 7)))
#define GPIO_AF_I2C1(n)     (0x4 << (4 * ((n) & 7)))
#define GPIO_AF_I2C2(n)     (0x4 << (4 * ((n) & 7)))
#define GPIO_AF_I2C3(n)     (0x4 << (4 * ((n) & 7)))
#define GPIO_AF_SPI1(n)     (0x5 << (4 * ((n) & 7)))
#define GPIO_AF_SPI2(n)     (0x5 << (4 * ((n) & 7)))
#define GPIO_AF_SPI3(n)     (0x6 << (4 * ((n) & 7)))
#define GPIO_AF_USART1(n)   (0x7 << (4 * ((n) & 7)))
#define GPIO_AF_USART2(n)   (0x7 << (4 * ((n) & 7)))
#define GPIO_AF_USART3(n)   (0x7 << (4 * ((n) & 7)))
#define GPIO_AF_USART4(n)   (0x8 << (4 * ((n) & 7)))
#define GPIO_AF_USART5(n)   (0x8 << (4 * ((n) & 7)))
#define GPIO_AF_USART6(n)   (0x8 << (4 * ((n) & 7)))
#define GPIO_AF_CAN1(n)     (0x9 << (4 * ((n) & 7)))
#define GPIO_AF_CAN2(n)     (0x9 << (4 * ((n) & 7)))
#define GPIO_AF_TIM12(n)    (0x9 << (4 * ((n) & 7)))
#define GPIO_AF_TIM13(n)    (0x9 << (4 * ((n) & 7)))
#define GPIO_AF_TIM14(n)    (0x9 << (4 * ((n) & 7)))
#define GPIO_AF_OTG_FS(n)   (0xA << (4 * ((n) & 7)))
#define GPIO_AF_OTG_HS(n)   (0xA << (4 * ((n) & 7)))
#define GPIO_AF_ETH(n)      (0xB << (4 * ((n) & 7)))
#define GPIO_AF_FSMC(n)     (0xC << (4 * ((n) & 7)))
#define GPIO_AF_SDIO(n)     (0xC << (4 * ((n) & 7)))
#define GPIO_AF_OTG_HS_FS(n) (0xC << (4 * ((n) & 7)))
#define GPIO_AF_DCMI(n)     (0xD << (4 * ((n) & 7)))
#define GPIO_AF_EVENTOUT(n) (0xF << (4 * ((n) & 7)))
#define GPIO_AF_MASK(n)     (0xF << (4 * ((n) & 7)))


/////////////////////////////////////////
// SYSCFG
/////////////////////////////////////////
typedef struct
{
    arm_reg_t MEMRMP;   // memory remap
    arm_reg_t PMC;      // peripheral mode configuration
    arm_reg_t EXTICR1;  // external interrupt configuration 1
    arm_reg_t EXTICR2;  // external interrupt configuration 2
    arm_reg_t EXTICR3;  // external interrupt configuration 3
    arm_reg_t EXTICR4;  // external interrupt configuration 4
} SYSCFG_t;

#define SYSCFG      ((SYSCFG_t*) STM_SYSCFG_BASE)

// SYSCFG_MEMRMP bits
#define SYSCFG_BOOT_MAIN_FLASH      (0 << 8)
#define SYSCFG_BOOT_SYSTEM_FLASH    (1 << 8)
#define SYSCFG_BOOT_EMB_SRAM        (3 << 8)
#define SYSCFG_MEM_MAIN_FLASH       (0 << 0)
#define SYSCFG_MEM_SYSTEM_FLASH     (1 << 0)
#define SYSCFG_MEM_FSMC             (2 << 0)
#define SYSCFG_MEM_SRAM             (3 << 0)

// SYSCFG_PMC bits
#define SYSCFG_LCD_CAPA_PE12        (1 << 5)
#define SYSCFG_LCD_CAPA_PE11        (1 << 4)
#define SYSCFG_LCD_CAPA_PB0         (1 << 3)
#define SYSCFG_LCD_CAPA_PB12        (1 << 2)
#define SYSCFG_LCD_CAPA_PB2         (1 << 1)
#define SYSCFG_USB_PU               (1 << 0)

// SYSCFG_EXTICRx bits
#define SYSCFG_PA(x)                (0 << (((x) & 3) * 4))
#define SYSCFG_PB(x)                (1 << (((x) & 3) * 4))
#define SYSCFG_PC(x)                (2 << (((x) & 3) * 4))
#define SYSCFG_PD(x)                (3 << (((x) & 3) * 4))
#define SYSCFG_PE(x)                (4 << (((x) & 3) * 4))
#define SYSCFG_PH(x)                (5 << (((x) & 3) * 4))
#define SYSCFG_PF(x)                (6 << (((x) & 3) * 4))
#define SYSCFG_PG(x)                (7 << (((x) & 3) * 4))
#define SYSCFG_PMASK(x)             (0xF << (((x) & 3) * 4))


/////////////////////////////////////////
// ADC
/////////////////////////////////////////
typedef struct
{
    arm_reg_t SR;       // status
    arm_reg_t CR1;      // control 1
    arm_reg_t CR2;      // control 2
    arm_reg_t SMPR1;    // sample time 1
    arm_reg_t SMPR2;    // sample time 2
    arm_reg_t SMPR3;    // sample time 3
    arm_reg_t JOFR1;    // injected channel data offset 1
    arm_reg_t JOFR2;    // injected channel data offset 2
    arm_reg_t JOFR3;    // injected channel data offset 3
    arm_reg_t JOFR4;    // injected channel data offset 4
    arm_reg_t HTR;      // watchdog higher threshold
    arm_reg_t LTR;      // watchdog lower threshold
    arm_reg_t SQR1;     // regular sequence 1
    arm_reg_t SQR2;     // regular sequence 2
    arm_reg_t SQR3;     // regular sequence 3
    arm_reg_t SQR4;     // regular sequence 4
    arm_reg_t SQR5;     // regular sequence 5
    arm_reg_t JSQR;     // injected sequence
    arm_reg_t JDR1;     // injected data 1
    arm_reg_t JDR2;     // injected data 2
    arm_reg_t JDR3;     // injected data 3
    arm_reg_t JDR4;     // injected data 4
    arm_reg_t DR;       // regular data
    arm_reg_t SMPR0;    // sample time 0
    arm_reg_t gap0[168];
    arm_reg_t CSR;      // common status
    arm_reg_t CCR;      // common control
} ADC_t;

#define ADC   ((ADC_t*) STM_ADC_BASE)

// ADC_SR
#define ADC_JCNR                (1 << 9)
#define ADC_RCNR                (1 << 8)
#define ADC_ADONS               (1 << 6)
#define ADC_OVR                 (1 << 5)
#define ADC_STRT                (1 << 4)
#define ADC_JSTRT               (1 << 3)
#define ADC_JEOC                (1 << 2)
#define ADC_EOC                 (1 << 1)
#define ADC_AWD                 (1 << 0)

// ADC_CR1
#define ADC_OVRIE               (1 << 26)
#define ADC_RES_12BIT           (0 << 24)
#define ADC_RES_10BIT           (1 << 24)
#define ADC_RES_8BIT            (2 << 24)
#define ADC_RES_6BIT            (3 << 24)
#define ADC_AWDEN               (1 << 23)
#define ADC_JAWDEN              (1 << 22)
#define ADC_PDI                 (1 << 17)
#define ADC_PDD                 (1 << 16)
#define ADC_DISCNUM(n)          ((n) << 13)
#define ADC_JDISCEN             (1 << 12)
#define ADC_DISCEN              (1 << 11)
#define ADC_JAUTO               (1 << 10)
#define ADC_AWDSGL              (1 << 9)
#define ADC_SCAN                (1 << 8)
#define ADC_JOECIE              (1 << 7)
#define ADC_AWDIE               (1 << 6)
#define ADC_OECIE               (1 << 5)
#define ADC_AWDCH(n)            ((n) & 0x1F)

// ADC_CR2
#define ADC_SWSTART             (1 << 30)
#define ADC_EXTEN_DISABLE       (0 << 28)
#define ADC_EXTEN_RISING        (1 << 28)
#define ADC_EXTEN_FALLING       (2 << 28)
#define ADC_EXTEN_BOTH          (3 << 28)
#define ADC_EXTSEL_TIM9_CC2     (0 << 24)
#define ADC_EXTSEL_TIM9_TRGO    (1 << 24)
#define ADC_EXTSEL_TIM2_CC3     (2 << 24)
#define ADC_EXTSEL_TIM2_CC2     (3 << 24)
#define ADC_EXTSEL_TIM3_TRGO    (4 << 24)
#define ADC_EXTSEL_TIM4_CC4     (5 << 24)
#define ADC_EXTSEL_TIM2_TRGO    (6 << 24)
#define ADC_EXTSEL_TIM3_CC1     (7 << 24)
#define ADC_EXTSEL_TIM3_CC3     (8 << 24)
#define ADC_EXTSEL_TIM4_TRGO    (9 << 24)
#define ADC_EXTSEL_TIM6_TRGO    (10 << 24)
#define ADC_EXTSEL_EXTI11       (15 << 24)
#define ADC_JSWSTART            (1 << 22)
#define ADC_JEXTEN_DISABLE      (0 << 20)
#define ADC_JEXTEN_RISING       (1 << 20)
#define ADC_JEXTEN_FALLING      (2 << 20)
#define ADC_JEXTEN_BOTH         (3 << 20)
#define ADC_JEXTSEL_TIM9_CC1    (0 << 16)
#define ADC_JEXTSEL_TIM9_TRGO   (1 << 16)
#define ADC_JEXTSEL_TIM2_TRGO   (2 << 16)
#define ADC_JEXTSEL_TIM2_CC1    (3 << 16)
#define ADC_JEXTSEL_TIM3_CC4    (4 << 16)
#define ADC_JEXTSEL_TIM4_TRGO   (5 << 16)
#define ADC_JEXTSEL_TIM4_CC1    (6 << 16)
#define ADC_JEXTSEL_TIM4_CC2    (7 << 16)
#define ADC_JEXTSEL_TIM4_CC3    (8 << 16)
#define ADC_JEXTSEL_TIM10_CC1   (9 << 16)
#define ADC_JEXTSEL_TIM7_TRGO   (10 << 16)
#define ADC_JEXTSEL_EXTI15      (15 << 16)
#define ADC_ALIGN               (1 << 11)
#define ADC_EOCS                (1 << 10)
#define ADC_DDS                 (1 << 9)
#define ADC_DMA                 (1 << 8)
#define ADC_DELS_NO_DELAY       (0 << 4)
#define ADC_DELS_UNTIL_READ     (1 << 4)
#define ADC_DELS_7              (2 << 4)
#define ADC_DELS_15             (3 << 4)
#define ADC_DELS_31             (4 << 4)
#define ADC_DELS_63             (5 << 4)
#define ADC_DELS_127            (6 << 4)
#define ADC_DELS_255            (7 << 4)
#define ADC_CFG                 (1 << 2)
#define ADC_CONT                (1 << 1)
#define ADC_ADON                (1 << 0)

// ADC_SMPR1, ADC_SMPR2, ADC_SMPR3, ADC_SMPR0
#define ADC_SMP_4CYCLES(n)      (0 << (3 * (n%10)))
#define ADC_SMP_9CYCLES(n)      (1 << (3 * (n%10)))
#define ADC_SMP_16CYCLES(n)     (2 << (3 * (n%10)))
#define ADC_SMP_24CYCLES(n)     (3 << (3 * (n%10)))
#define ADC_SMP_48CYCLES(n)     (4 << (3 * (n%10)))
#define ADC_SMP_96CYCLES(n)     (5 << (3 * (n%10)))
#define ADC_SMP_192CYCLES(n)    (6 << (3 * (n%10)))
#define ADC_SMP_384CYCLES(n)    (7 << (3 * (n%10)))

// ADC_SQR1
#define ADC_L(n)                ((n) << 20)
#define ADC_SQ28(n)             ((n) << 15)
#define ADC_SQ27(n)             ((n) << 10)
#define ADC_SQ26(n)             ((n) << 5)
#define ADC_SQ25(n)             ((n) << 0)

// ADC_SQR2
#define ADC_SQ24(n)             ((n) << 26)
#define ADC_SQ23(n)             ((n) << 20)
#define ADC_SQ22(n)             ((n) << 15)
#define ADC_SQ21(n)             ((n) << 10)
#define ADC_SQ20(n)             ((n) << 5)
#define ADC_SQ19(n)             ((n) << 0)

// ADC_SQR3
#define ADC_SQ18(n)             ((n) << 25)
#define ADC_SQ17(n)             ((n) << 20)
#define ADC_SQ16(n)             ((n) << 15)
#define ADC_SQ15(n)             ((n) << 10)
#define ADC_SQ14(n)             ((n) << 5)
#define ADC_SQ13(n)             ((n) << 0)

// ADC_SQR4
#define ADC_SQ12(n)             ((n) << 26)
#define ADC_SQ11(n)             ((n) << 20)
#define ADC_SQ10(n)             ((n) << 15)
#define ADC_SQ9(n)              ((n) << 10)
#define ADC_SQ8(n)              ((n) << 5)
#define ADC_SQ7(n)              ((n) << 0)

// ADC_SQR5
#define ADC_SQ6(n)              ((n) << 25)
#define ADC_SQ5(n)              ((n) << 20)
#define ADC_SQ4(n)              ((n) << 15)
#define ADC_SQ3(n)              ((n) << 10)
#define ADC_SQ2(n)              ((n) << 5)
#define ADC_SQ1(n)              ((n) << 0)

// ADC_JSQR
#define ADC_JL(n)               ((n) << 20)
#define ADC_JSQ4(n)             ((n) << 15)
#define ADC_JSQ3(n)             ((n) << 10)
#define ADC_JSQ2(n)             ((n) << 5)
#define ADC_JSQ1(n)             ((n) << 0)

// ADC_CSR
#define ADC_ADONS1              (1 << 6)
#define ADC_OVR1                (1 << 5)
#define ADC_STRT1               (1 << 4)
#define ADC_JSTRT1              (1 << 3)
#define ADC_JEOC1               (1 << 2)
#define ADC_EOC1                (1 << 1)
#define ADC_AWD1                (1 << 0)

// ADC_CSR
#define ADC_ADONS1              (1 << 6)
#define ADC_OVR1                (1 << 5)
#define ADC_STRT1               (1 << 4)
#define ADC_JSTRT1              (1 << 3)
#define ADC_JEOC1               (1 << 2)
#define ADC_EOC1                (1 << 1)
#define ADC_AWD1                (1 << 0)

// ADC_CCR
#define ADC_TSVREFE             (1 << 23)
#define ADC_HSI_DIV_1           (0 << 16)
#define ADC_HSI_DIV_2           (1 << 16)
#define ADC_HSI_DIV_4           (2 << 16)


/////////////////////////////////////////
// DAC
/////////////////////////////////////////
typedef struct
{
    arm_reg_t CR;       // control
    arm_reg_t SWTRIGR;  // software trigger
    arm_reg_t DHR12R1;  // channel1 12-bit right-aligned data
    arm_reg_t DHR12L1;  // channel1 12-bit left aligned data
    arm_reg_t DHR8R1;   // channel1 8-bit right aligned data
    arm_reg_t DHR12R2;  // channel2 12-bit right aligned data
    arm_reg_t DHR12L2;  // channel2 12-bit left aligned data
    arm_reg_t DHR8R2;   // channel2 8-bit right-aligned data
    arm_reg_t DHR12RD;  // dual DAC 12-bit right-aligned data
    arm_reg_t DHR12LD;  // dual DAC 12-bit left aligned data
    arm_reg_t DHR8RD;   // dual DAC 8-bit right aligned data
    arm_reg_t DOR1;     // channel1 data output
    arm_reg_t DOR2;     // channel2 data output
    arm_reg_t SR;       // status
} DAC_t;

#define DAC   ((DAC_t*) STM_DAC_BASE)

// DAC_CR bits
#define DMA_DMAUDRIE2           (1 << 29)
#define DMA_DMAEN2              (1 << 28)
#define DMA_MAMP2(n)            ((n) << 24)
#define DMA_WAVE2(n)            ((n) << 22)
#define DMA_TSEL2(n)            ((n) << 19)
#define DMA_TEN2                (1 << 18)
#define DMA_BOFF2               (1 << 17)
#define DMA_EN2                 (1 << 16)
#define DMA_DMAUDRIE1           (1 << 13)
#define DMA_DMAEN1              (1 << 12)
#define DMA_MAMP1(n)            ((n) << 8)
#define DMA_WAVE1(n)            ((n) << 6)
#define DMA_TSEL1(n)            ((n) << 3)
#define DMA_TEN1                (1 << 2)
#define DMA_BOFF1               (1 << 1)
#define DMA_EN1                 (1 << 0)

// DAC_SWTRIGR bits
#define DAC_SWTRIG2             (1 << 1)
#define DAC_SWTRIG1             (1 << 0)

// DAC_SR bits
#define DAC_DMAUDR2             (1 << 29)
#define DAC_DMAUDR1             (1 << 13)


/////////////////////////////////////////
// Comparators (COMP)
/////////////////////////////////////////
typedef struct
{
    arm_reg_t CSR;      // control and status
} COMP_t;

#define COMP   ((COMP_t*) STM_COMP_BASE)

// COMP_CSR bits
#define COMP_TSUSP                  (1 << 31)
#define COMP_CAIF                   (1 << 30)
#define COMP_CAIE                   (1 << 29)
#define COMP_RCH13                  (1 << 28)
#define COMP_FCH8                   (1 << 27)
#define COMP_FCH3                   (1 << 26)
#define COMP_OUTSEL_TIM2_INP_CAP4   (0 << 21)
#define COMP_OUTSEL_TIM2_OCREF_CLR  (1 << 21)
#define COMP_OUTSEL_TIM3_INP_CAP4   (2 << 21)
#define COMP_OUTSEL_TIM3_OCREF_CLR  (3 << 21)
#define COMP_OUTSEL_TIM4_INP_CAP4   (4 << 21)
#define COMP_OUTSEL_TIM4_OCREF_CLR  (5 << 21)
#define COMP_OUTSEL_TIM10_INP_CAP1  (6 << 21)
#define COMP_OUTSEL_NO_REDIR        (7 << 21)
#define COMP_INSEL_NO_SEL           (0 << 18)
#define COMP_INSEL_EXT_IO           (1 << 18)
#define COMP_INSEL_VREFINT          (2 << 18)
#define COMP_INSEL_3_4_VREFINT      (3 << 18)
#define COMP_INSEL_1_2_VREFINT      (4 << 18)
#define COMP_INSEL_1_4_VREFINT      (5 << 18)
#define COMP_INSEL_DAC_OUT1         (6 << 18)
#define COMP_INSEL_DAC_OUT2         (7 << 18)
#define COMP_INSEL_MASK			        (7 << 18)
#define COMP_WNDWE                  (1 << 17)
#define COMP_VREFOUTEN              (1 << 16)
#define COMP_CMP2OUT                (1 << 13)
#define COMP_SPEED                  (1 << 12)
#define COMP_CMP1OUT                (1 << 7)
#define COMP_SW1                    (1 << 5)
#define COMP_CMP1EN                 (1 << 4)
#define COMP_400KPD                 (1 << 3)
#define COMP_10KPD                  (1 << 2)
#define COMP_400KPU                 (1 << 1)
#define COMP_10KPU                  (1 << 0)


/////////////////////////////////////////
// Timers (TIMx)
/////////////////////////////////////////
typedef struct
{
    arm_reg_t CR1;      // control register 1
    arm_reg_t CR2;      // control register 2
    arm_reg_t SMCR;     // slave mode control register
    arm_reg_t DIER;     // DMA/Interrupt enable register
    arm_reg_t SR;       // status register
    arm_reg_t EGR;      // event generation register
    arm_reg_t CCMR1;    // capture/compare mode register 1
    arm_reg_t CCMR2;    // capture/compare mode register 2
    arm_reg_t CCER;     // capture/compare enable register
    arm_reg_t CNT;      // counter
    arm_reg_t PSC;      // prescaler
    arm_reg_t ARR;      // auto-reload register
    arm_reg_t gap0;
    arm_reg_t CCR1;     // capture/compare register 1
    arm_reg_t CCR2;     // capture/compare register 2
    arm_reg_t CCR3;     // capture/compare register 3
    arm_reg_t CCR4;     // capture/compare register 4
    arm_reg_t gap1;
    arm_reg_t DCR;      // DMA control register
    arm_reg_t DMAR;     // DMA address for full transfer
    arm_reg_t OR;       // option register
} TIM_t;

#define TIM2   ((TIM_t*) STM_TIM2_BASE)
#define TIM3   ((TIM_t*) STM_TIM3_BASE)
#define TIM4   ((TIM_t*) STM_TIM4_BASE)
#define TIM5   ((TIM_t*) STM_TIM5_BASE)
#define TIM6   ((TIM_t*) STM_TIM6_BASE)
#define TIM7   ((TIM_t*) STM_TIM7_BASE)
#define TIM9   ((TIM_t*) STM_TIM9_BASE)
#define TIM10  ((TIM_t*) STM_TIM10_BASE)
#define TIM11  ((TIM_t*) STM_TIM11_BASE)

// TIMx_CR1
#define TIM_CKD(x)      ((x) << 8)
#define TIM_ARPE        (1 << 7)
#define TIM_CMS         ((x) << 5)
#define TIM_DIR         (1 << 4)
#define TIM_OPM         (1 << 3)
#define TIM_URS         (1 << 2)
#define TIM_UDIS        (1 << 1)
#define TIM_CEN         (1 << 0)

// TIMx_CR2
#define TIM_TI1S        (1 << 7)
#define TIM_MMS(x)      ((x) << 4)
#define TIM_CCDS        (1 << 3)

// TIMx_CMSR
#define TIM_ETP         (1 << 15)
#define TIM_ECE         (1 << 14)
#define TIM_ETPS(x)     ((x) << 12)
#define TIM_ETF(x)      ((x) << 8)
#define TIM_MSM         (1 << 7)
#define TIM_TS(x)       ((x) << 4)
#define TIM_OCCS        (1 << 3)
#define TIM_SMS(x)      (x)

// TIMx_DIER
#define TIM_TDE         (1 << 14)
#define TIM_CC4DE       (1 << 12)
#define TIM_CC3DE       (1 << 11)
#define TIM_CC2DE       (1 << 10)
#define TIM_CC1DE       (1 << 9)
#define TIM_UDE         (1 << 8)
#define TIM_TIE         (1 << 6)
#define TIM_CC4IE       (1 << 4)
#define TIM_CC3IE       (1 << 3)
#define TIM_CC2IE       (1 << 2)
#define TIM_CC1IE       (1 << 1)
#define TIM_UIE         (1 << 0)

// TIMx_SR
#define TIM_CC4OF       (1 << 12)
#define TIM_CC3OF       (1 << 11)
#define TIM_CC2OF       (1 << 10)
#define TIM_CC1OF       (1 << 9)
#define TIM_TIF         (1 << 6)
#define TIM_CC4IF       (1 << 4)
#define TIM_CC3IF       (1 << 3)
#define TIM_CC2IF       (1 << 2)
#define TIM_CC1IF       (1 << 1)
#define TIM_UIF         (1 << 0)

// TIMx_EGR
#define TIM_TG          (1 << 6)
#define TIM_CC4G        (1 << 4)
#define TIM_CC3G        (1 << 3)
#define TIM_CC2G        (1 << 2)
#define TIM_CC1G        (1 << 1)
#define TIM_UG          (1 << 0)

// TIMx_CCMR1
// output compare mode
#define TIM_OC2CE       (1 << 15)
#define TIM_OC2M(x)     ((x) << 12)
#define TIM_OC2PE       (1 << 11)
#define TIM_OC2FE       (1 << 10)
#define TIM_CC2S(x)     ((x) << 8)
#define TIM_OC1CE       (1 << 7)
#define TIM_OC1M(x)     ((x) << 4)
#define TIM_OC1PE       (1 << 3)
#define TIM_OC1FE       (1 << 2)
#define TIM_CC1S(x)     (x)
// input capture mode
#define TIM_IC2F(x)     ((x) << 12)
#define TIM_IC2PSC(x)   ((x) << 10)
#define TIM_CC2S(x)     ((x) << 8)
#define TIM_IC1F        ((x) << 4)
#define TIM_IC1PSC(x)   ((x) << 2)
#define TIM_CC1S(x)     (x)

// TIMx_CCMR2
// output compare mode
#define TIM_OC4CE       (1 << 15)
#define TIM_OC4M(x)     ((x) << 12)
#define TIM_OC4PE       (1 << 11)
#define TIM_OC4FE       (1 << 10)
#define TIM_CC4S(x)     ((x) << 8)
#define TIM_OC3CE       (1 << 7)
#define TIM_OC3M(x)     ((x) << 4)
#define TIM_OC3PE       (1 << 3)
#define TIM_OC3FE       (1 << 2)
#define TIM_CC3S(x)     (x)
// input capture mode
#define TIM_IC4F(x)     ((x) << 12)
#define TIM_IC4PSC(x)   ((x) << 10)
#define TIM_CC4S(x)     ((x) << 8)
#define TIM_IC3F        ((x) << 4)
#define TIM_IC3PSC(x)   ((x) << 2)
#define TIM_CC3S(x)     (x)

// TIMx_CCER
#define TIM_CC4NP       (1 << 15)
#define TIM_CC4P        (1 << 13)
#define TIM_CC4E        (1 << 12)
#define TIM_CC3NP       (1 << 11)
#define TIM_CC3P        (1 << 9)
#define TIM_CC3E        (1 << 8)
#define TIM_CC2NP       (1 << 7)
#define TIM_CC2P        (1 << 5)
#define TIM_CC2E        (1 << 4)
#define TIM_CC1NP       (1 << 3
#define TIM_CC1P        (1 << 1)
#define TIM_CC1E        (1 << 0)

// TIMx_DCR
#define TIM_DBL(x)      ((x) << 8)
#define TIM_DBA(x)      (x)

// TIMx_OR
#define TIM2_ITR1_RMP    (1 << 0)
#define TIM3_ITR2_RMP    (1 << 0)
#define TIM9_ITR1_RMP    (1 << 2)
#define TIM9_TI1_RMP(x)  (x)
#define TIM10_TI1_RMP_RI (1 << 3)
#define TIM10_ETR_RMP    (1 << 2)
#define TIM10_TI1_RMP(x) (x)
#define TIM11_TI1_RMP_RI (1 << 3)
#define TIM11_ETR_RMP    (1 << 2)
#define TIM11_TI1_RMP(x) (x)



/////////////////////////////////////////
// Routing Interface (RI)
/////////////////////////////////////////
typedef struct
{
  arm_reg_t ICR;       // input capture register,                           Address offset: 0x04
  arm_reg_t ASCR1;     // analog switches control register,                 Address offset: 0x08
  arm_reg_t ASCR2;     // analog switch control register 2,                 Address offset: 0x0C
  arm_reg_t HYSCR1;    // hysteresis control register 1,                    Address offset: 0x10
  arm_reg_t HYSCR2;    // Hysteresis control register 2,                    Address offset: 0x14
  arm_reg_t HYSCR3;    // Hysteresis control register 3,                    Address offset: 0x18
  arm_reg_t HYSCR4;    // Hysteresis control register 4,                    Address offset: 0x1C
  arm_reg_t ASMR1;     // Analog switch mode register 1,                    Address offset: 0x20
  arm_reg_t CMR1;      // Channel mask register 1,                          Address offset: 0x24
  arm_reg_t CICR1;     // Channel identification for capture register 1,    Address offset: 0x28
  arm_reg_t ASMR2;     // Analog switch mode register 2,                    Address offset: 0x2C
  arm_reg_t CMR2;      // Channel mask register 2,                          Address offset: 0x30
  arm_reg_t CICR2;     // Channel identification for capture register 2,    Address offset: 0x34
  arm_reg_t ASMR3;     // Analog switch mode register 3,                    Address offset: 0x38
  arm_reg_t CMR3;      // Channel mask register 3,                          Address offset: 0x3C
  arm_reg_t CICR3;     // Channel identification for capture register3 ,    Address offset: 0x40
  arm_reg_t ASMR4;     // Analog switch mode register 4,                    Address offset: 0x44
  arm_reg_t CMR4;      // Channel mask register 4,                          Address offset: 0x48
  arm_reg_t CICR4;     // Channel identification for capture register 4,    Address offset: 0x4C
  arm_reg_t ASMR5;     // Analog switch mode register 5,                    Address offset: 0x50
  arm_reg_t CMR5;      // Channel mask register 5,                          Address offset: 0x54
  arm_reg_t CICR5;     // Channel identification for capture register 5,    Address offset: 0x58
} RI_t;

#define   RI      ((RI_t *) STM_RI_BASE)

//RI_ICR
#define   RI_IC4                            (1 << 21)
#define   RI_IC3                            (1 << 20)
#define   RI_IC2                            (1 << 19)
#define   RI_IC1                            (1 << 18)

#define   RI_TIM_NONE                       (0 << 16)
#define   RI_TIM2                           (1 << 16)
#define   RI_TIM3                           (2 << 16)
#define   RI_TIM4                           (3 << 16)
#define   RI_TIM_MASK                       (3 << 16)

#define   RI_IC4IOS_PA3                     (0 << 12)
#define   RI_IC4IOS_PA7                     (1 << 12)
#define   RI_IC4IOS_PA11                    (2 << 12)
#define   RI_IC4IOS_PA15                    (3 << 12)
#define   RI_IC4IOS_PC3                     (4 << 12)
#define   RI_IC4IOS_PC7                     (5 << 12)
#define   RI_IC4IOS_PC11                    (6 << 12)
#define   RI_IC4IOS_PC15                    (7 << 12)
#define   RI_IC4IOS_PD3                     (8 << 12)
#define   RI_IC4IOS_PD7                     (9 << 12)
#define   RI_IC4IOS_PD11                    (10 << 12)
#define   RI_IC4IOS_PD15                    (11 << 12)
#define   RI_IC4IOS_PE3                     (12 << 12)
#define   RI_IC4IOS_PE7                     (13 << 12)
#define   RI_IC4IOS_PE11                    (14 << 12)
#define   RI_IC4IOS_PE15                    (15 << 12)
#define   RI_IC4IOS_MASK                    (15 << 12)

#define   RI_IC3IOS_PA2                     (0 << 8)
#define   RI_IC3IOS_PA6                     (1 << 8)
#define   RI_IC3IOS_PA10                    (2 << 8)
#define   RI_IC3IOS_PA14                    (3 << 8)
#define   RI_IC3IOS_PC2                     (4 << 8)
#define   RI_IC3IOS_PC6                     (5 << 8)
#define   RI_IC3IOS_PC10                    (6 << 8)
#define   RI_IC3IOS_PC14                    (7 << 8)
#define   RI_IC3IOS_PD2                     (8 << 8)
#define   RI_IC3IOS_PD6                     (9 << 8)
#define   RI_IC3IOS_PD10                    (10 << 8)
#define   RI_IC3IOS_PD14                    (11 << 8)
#define   RI_IC3IOS_PE2                     (12 << 8)
#define   RI_IC3IOS_PE6                     (13 << 8)
#define   RI_IC3IOS_PE10                    (14 << 8)
#define   RI_IC3IOS_PE14                    (15 << 8)
#define   RI_IC3IOS_MASK                    (15 << 8)

#define   RI_IC2IOS_PA1                     (0 << 4)
#define   RI_IC2IOS_PA5                     (1 << 4)
#define   RI_IC2IOS_PA9                     (2 << 4)
#define   RI_IC2IOS_PA13                    (3 << 4)
#define   RI_IC2IOS_PC1                     (4 << 4)
#define   RI_IC2IOS_PC5                     (5 << 4)
#define   RI_IC2IOS_PC9                     (6 << 4)
#define   RI_IC2IOS_PC13                    (7 << 4)
#define   RI_IC2IOS_PD1                     (8 << 4)
#define   RI_IC2IOS_PD5                     (9 << 4)
#define   RI_IC2IOS_PD9                     (10 << 4)
#define   RI_IC2IOS_PD13                    (11 << 4)
#define   RI_IC2IOS_PE1                     (12 << 4)
#define   RI_IC2IOS_PE5                     (13 << 4)
#define   RI_IC2IOS_PE9                     (14 << 4)
#define   RI_IC2IOS_PE13                    (15 << 4)
#define   RI_IC2IOS_MASK                    (15 << 4)

#define   RI_IC1IOS_PA0                     (0 << 0)
#define   RI_IC1IOS_PA4                     (1 << 0)
#define   RI_IC1IOS_PA8                     (2 << 0)
#define   RI_IC1IOS_PA12                    (3 << 0)
#define   RI_IC1IOS_PC0                     (4 << 0)
#define   RI_IC1IOS_PC4                     (5 << 0)
#define   RI_IC1IOS_PC8                     (6 << 0)
#define   RI_IC1IOS_PC12                    (7 << 0)
#define   RI_IC1IOS_PD0                     (8 << 0)
#define   RI_IC1IOS_PD4                     (9 << 0)
#define   RI_IC1IOS_PD8                     (10 << 0)
#define   RI_IC1IOS_PD12                    (11 << 0)
#define   RI_IC1IOS_PE0                     (12 << 0)
#define   RI_IC1IOS_PE4                     (13 << 0)
#define   RI_IC1IOS_PE8                     (14 << 0)
#define   RI_IC1IOS_PE12                    (15 << 0)
#define   RI_IC1IOS_MASK                    (15 << 0)

//RI_ASCR1
#define   RI_SCM                            (1 << 31)
#define   RI_CH(n)                          (1 << n)
#define   RI_VCOMP                          (1 << 26)

//RI_ASCR2
#define   RI_GR64                           (1 << 28)
#define   RI_GR63                           (1 << 27)
#define   RI_GR77                           (1 << 26)
#define   RI_GR76                           (1 << 25)
#define   RI_GR75                           (1 << 24)
#define   RI_GR25                           (1 << 23)
#define   RI_GR24                           (1 << 22)
#define   RI_GR23                           (1 << 21)
#define   RI_GR94                           (1 << 20)
#define   RI_GR93                           (1 << 19)
#define   RI_GR35                           (1 << 18)
#define   RI_GR34                           (1 << 17)
#define   RI_GR33                           (1 << 16)
#define   RI_GR43                           (1 << 11)
#define   RI_GR42                           (1 << 10)
#define   RI_GR41                           (1 << 9)
#define   RI_GR53                           (1 << 8)
#define   RI_GR52                           (1 << 7)
#define   RI_GR51                           (1 << 6)
#define   RI_GR62                           (1 << 5)
#define   RI_GR61                           (1 << 4)
#define   RI_GR104                          (1 << 3)
#define   RI_GR103                          (1 << 2)
#define   RI_GR102                          (1 << 1)
#define   RI_GR101                          (1 << 0)

//RI_HYSCR1
#define   RI_HYST_PB(n)                     (1 << (16+n))
#define   RI_HYST_PA(n)                     (1 << n)

//RI_HYSCR2
#define   RI_HYST_PD(n)                     (1 << (16+n))
#define   RI_HYST_PC(n)                     (1 << n)

//RI_HYSCR3
#define   RI_HYST_PF(n)                     (1 << (16+n))
#define   RI_HYST_PE(n)                     (1 << n)

//RI_HYSCR4
#define   RI_HYST_PG(n)                     (1 << n)

//RI_ASMR1
#define   RI_ASM_PA(n)                      (1 << n)

//RI_CMR1
#define   RI_CMR_PA(n)                      (1 << n)

//RI_CICR1
#define   RI_CICR_PA(n)                     (1 << n)

//RI_ASMR2
#define   RI_ASM_PB(n)                      (1 << n)

//RI_CMR2
#define   RI_CMR_PB(n)                      (1 << n)

//RI_CICR2
#define   RI_CICR_PB(n)                     (1 << n)

//RI_ASMR3
#define   RI_ASM_PC(n)                      (1 << n)

//RI_CMR3
#define   RI_CMR_PC(n)                      (1 << n)

//RI_CICR3
#define   RI_CICR_PC(n)                     (1 << n)

//RI_ASMR4
#define   RI_ASM_PF(n)                      (1 << n)

//RI_CMR4
#define   RI_CMR_PF(n)                      (1 << n)

//RI_CICR4
#define   RI_CICR_PF(n)                     (1 << n)

//RI_ASMR5
#define   RI_ASM_PG(n)                      (1 << n)

//RI_CMR5
#define   RI_CMR_PG(n)                      (1 << n)

//RI_CICR5
#define   RI_CICR_PG(n)                     (1 << n)

/////////////////////////////////////////
// RTC
/////////////////////////////////////////
typedef struct
{
    arm_reg_t TR;       // Time
    arm_reg_t DR;       // Date
    arm_reg_t CR;       // Control
    arm_reg_t ISR;      // Initialization and status
    arm_reg_t PRER;     // Prescaler
    arm_reg_t WUTR;     // Wakeup timer
    arm_reg_t CALIBR;   // Calibration
    arm_reg_t ALRMAR;   // Alarm A
    arm_reg_t ALRMBR;   // Alarm B
    arm_reg_t WPR;      // Write protection
    arm_reg_t SSR;      // Sub second
    arm_reg_t SHIFTR;   // Shift control
    arm_reg_t TSTR;     // Time stamp time
    arm_reg_t TSDR;     // Time stamp date
    arm_reg_t TSSSR;    // Timestamp sub second
    arm_reg_t CALR;     // Calibration
    arm_reg_t TAFCR;    // Tamper and alternate function configuration
    arm_reg_t ALRMASSR; // Alarm A sub second
    arm_reg_t ALRMBSSR; // Alarm B sub second
    arm_reg_t gap0;
    arm_reg_t BKPR[32]; // Backup registers
} RTC_t;

#define RTC   ((RTC_t*) STM_RTC_BKP_BASE)

// TR, TSTR bits
#define RTC_PM          (1 << 22)
#define RTC_HT(x)       ((x) << 20)
#define RTC_GET_HT(r)   (((r) >> 20) & 0x3)
#define RTC_HU(x)       ((x) << 16)
#define RTC_GET_HU(r)   (((r) >> 16) & 0xF)
#define RTC_MNT(x)      ((x) << 12)
#define RTC_GET_MNT(r)  (((r) >> 12) & 0x7)
#define RTC_MNU(x)      ((x) << 8)
#define RTC_GET_MNU(r)  (((r) >> 8) & 0xF)
#define RTC_ST(x)       ((x) << 4)
#define RTC_GET_ST(r)   (((r) >> 4) & 0x7)
#define RTC_SU(x)       ((x))
#define RTC_GET_SU(r)   (((r)) & 0xF)

// DR, TSDR (low 16 bits) bits
#define RTC_YT(x)       ((x) << 20)
#define RTC_GET_YT(r)   (((r) >> 20) & 0xF)
#define RTC_YU(x)       ((x) << 16)
#define RTC_GET_YU(r)   (((r) >> 16) & 0xF)
#define RTC_WDU(x)      ((x) << 13)
#define RTC_GET_WDU(r)  (((r) >> 13) & 0x7)
#define RTC_WDU_MON     1
#define RTC_WDU_TUE     2
#define RTC_WDU_WED     3
#define RTC_WDU_THU     4
#define RTC_WDU_FRI     5
#define RTC_WDU_SAT     6
#define RTC_WDU_SUN     7
#define RTC_MT(x)       ((x) << 12)
#define RTC_GET_MT(r)   (((r) >> 12) & 0x1)
#define RTC_MU(x)       ((x) << 8)
#define RTC_GET_MU(r)   (((r) >> 8) & 0xF)
#define RTC_DT(x)       ((x) << 4)
#define RTC_GET_DT(r)   (((r) >> 4) & 0x3)
#define RTC_DU(x)       ((x))
#define RTC_GET_DU(r)   (((r)) & 0xF)

// CR bits
#define RTC_COE         (1 << 23)
#define RTC_OSEL(x)     ((x) << 21)
#define RTC_POL         (1 << 20)
#define RTC_COSEL       (1 << 19)
#define RTC_BKP         (1 << 18)
#define RTC_SUB1H       (1 << 17)
#define RTC_ADD1H       (1 << 16)
#define RTC_TSIE        (1 << 15)
#define RTC_WUTIE       (1 << 14)
#define RTC_ALRBIE      (1 << 13)
#define RTC_ALRAIE      (1 << 12)
#define RTC_TSE         (1 << 11)
#define RTC_WUTE        (1 << 10)
#define RTC_ALRBE       (1 << 9)
#define RTC_ALRAE       (1 << 8)
#define RTC_DCE         (1 << 7)
#define RTC_FMT         (1 << 6)
#define RTC_BYPSHAD     (1 << 5)
#define RTC_REFCKON     (1 << 4)
#define RTC_TSEDGE      (1 << 3)
#define RTC_WUCKSEL_DIV16           0
#define RTC_WUCKSEL_DIV8            1
#define RTC_WUCKSEL_DIV4            2
#define RTC_WUCKSEL_DIV2            3
#define RTC_WUCKSEL_CK_SPRE         4
#define RTC_WUCKSEL_CK_SPRE_ADDED   6

// ISR bits
#define RTC_RECALPF     (1 << 16)
#define RTC_TAMP3F      (1 << 15)
#define RTC_TAMP2F      (1 << 14)
#define RTC_TAMP1F      (1 << 13)
#define RTC_TSOVF       (1 << 12)
#define RTC_TSF         (1 << 11)
#define RTC_WUTF        (1 << 10)
#define RTC_ALRBF       (1 << 9)
#define RTC_ALRAF       (1 << 8)
#define RTC_INIT        (1 << 7)
#define RTC_INITF       (1 << 6)
#define RTC_RSF         (1 << 5)
#define RTC_INITS       (1 << 4)
#define RTC_SHPF        (1 << 3)
#define RTC_WUTWF       (1 << 2)
#define RTC_ALRBWF      (1 << 1)
#define RTC_ALRAWF      (1 << 0)

// PRER bits
#define RTC_PREDIV_A(x) ((x) << 16)
#define RTC_PREDIV_S(x) ((x))

// CALIBR bits
#define RTC_DCS         (1 << 7)
#define RTC_DC(x)       ((x))

// ALRMAR, ALRMBR bits
#define RTC_ALRM_MSK4   (1 << 31)
#define RTC_ALRM_WDSEL  (1 << 30)
#define RTC_ALRM_DT(x)  ((x) << 28)
#define RTC_ALRM_DU(x)  ((x) << 24)
#define RTC_ALRM_MSK3   (1 << 23)
#define RTC_ALRM_PM     (1 << 22)
#define RTC_ALRM_HT(x)  ((x) << 20)
#define RTC_ALRM_HU(x)  ((x) << 16)
#define RTC_ALRM_MSK2   (1 << 15)
#define RTC_ALRM_MNT(x) ((x) << 12)
#define RTC_ALRM_MNU(x) ((x) << 8)
#define RTC_ALRM_MSK1   (1 << 7)
#define RTC_ALRM_ST(x)  ((x) << 4)
#define RTC_ALRM_SU(x)  ((x) << 0)

// SHIFTR bits
#define RTC_ADD1S       (1 << 31)
#define RTC_SUBFS(x)    ((x) << 0)

// CALR bits
#define RTC_CALP        (1 << 15)
#define RTC_CALW8       (1 << 14)
#define RTC_CALW16      (1 << 13)
#define RTC_CALM(x)     (x)

// TAFCR bits
#define RTC_ALARMOUTTYPE    (1 << 18)
#define RTC_TAMPPUDIS       (1 << 15)
#define RTC_TAMPPRCH(x)     ((x) << 13)
#define RTC_TAMPFLT(x)      ((x) << 11)
#define RTC_TAMPFREQ(x)     ((x) << 8)
#define RTC_TAMPTS          (1 << 7)
#define RTC_TAMP3TRG        (1 << 6)
#define RTC_TAMP3E          (1 << 5)
#define RTC_TAMP2TRG        (1 << 4)
#define RTC_TAMP2E          (1 << 3)
#define RTC_TAMPIE          (1 << 2)
#define RTC_TAMP1TRG        (1 << 1)
#define RTC_TAMP1E          (1 << 0)

// ALRMASSR, ALRMASSR bits
#define RTC_MASKSS(x)       ((x) << 24)
#define RTC_SS(x)           ((x) & 0x7FFF)


/////////////////////////////////////////
// Independent watchdog (IWDG)
/////////////////////////////////////////
typedef struct
{
    arm_reg_t KR;       // Key
    arm_reg_t DR;       // Prescaler
    arm_reg_t RLR;      // Reload
    arm_reg_t SR;      // Status
} IWDG_t;

#define IWDG   ((IWDG_t*) STM_IWDG_BASE)

// IWDG_KR bits
#define IWDG_ALIVE_KEY      0xAAAA
#define IWDG_ACCESS_KEY     0x5555
#define IWDG_START_KEY      0xCCCC

// IWDG_PR bits
#define IWDG_PR_DIV_4       0
#define IWDG_PR_DIV_8       1
#define IWDG_PR_DIV_16      2
#define IWDG_PR_DIV_32      3
#define IWDG_PR_DIV_64      4
#define IWDG_PR_DIV_128     5
#define IWDG_PR_DIV_256     6

// IWDG_SR bits
#define IWDG_RVU            (1 << 1)
#define IWDG_PVU            (1 << 0)


/////////////////////////////////////////
// Window watchdog (WWDG)
/////////////////////////////////////////
typedef struct
{
    arm_reg_t CR;       // Control
    arm_reg_t CFR;      // Configuration
    arm_reg_t ISR;      // Status
} WWDG_t;

#define WWDG   ((WWDG_t*) STM_WWDG_BASE)

// WWDG_CR bits
#define WWDG_WDGA           (1 << 7)
#define WWDG_T(n)           (n)

// WWDG_CFR bits
#define WWDG_EWI            (1 << 9)
#define WWDG_WDGTB_DIV_1    (0 << 7)
#define WWDG_WDGTB_DIV_2    (1 << 7)
#define WWDG_WDGTB_DIV_4    (2 << 7)
#define WWDG_WDGTB_DIV_8    (3 << 7)
#define WWDG_W(n)           (n)

// WWDG_SR bits
#define WWDG_EWIF           (1 << 0)


/////////////////////////////////////////
// USB
/////////////////////////////////////////
typedef struct
{
    arm_reg_t EPR[8];       // Endpoint n registers
    arm_reg_t gap0[8];      // Reserved
    arm_reg_t CNTR;         // Control
    arm_reg_t ISTR;         // Interrupt status
    arm_reg_t FNR;          // Frame number
    arm_reg_t DADDR;        // Device address
    arm_reg_t BTABLE;       // Buffer table address (the value must be
                            // 8-byte aligned)
} USB_t;

// One item of USB buffer descriptor table. The table shall be an
// array of defined below structure placed somewhere in memory.
// Must be 8-byte aligned.
typedef struct
{
    arm_reg_t ADDR_TX;
    arm_reg_t COUNT_TX;
    arm_reg_t ADDR_RX;
    arm_reg_t COUNT_RX;
} USB_BTABLE_item_t;

#define USB                 ((USB_t*) STM_USB_DEV_FS_BASE)
#define USB_PACKET_BUF      ((volatile uint8_t *) STM_USB_DEV_FS_SRAM_BASE)
#define USB_PACKET_BUF_SZ   (512 * 2)

// USB_EPR[n] bits
#define USB_CTR_RX              (1 << 15)
#define USB_DTOG_RX             (1 << 14)
#define USB_GET_DTOG_RX(r)      (((r) >> 14) & 1)
#define USB_STAT_RX_DISABLED    (0 << 12)
#define USB_STAT_RX_STALL       (1 << 12)
#define USB_STAT_RX_NAK         (2 << 12)
#define USB_STAT_RX_VALID       (3 << 12)
#define USB_STAT_RX_MASK        (3 << 12)
#define USB_SETUP               (1 << 11)
#define USB_EP_TYPE_BULK        (0 << 9)
#define USB_EP_TYPE_CONTROL     (1 << 9)
#define USB_EP_TYPE_ISO         (2 << 9)
#define USB_EP_TYPE_INTERRUPT   (3 << 9)
#define USB_EP_TYPE_MASK        (3 << 9)
#define USB_EP_TYPE_CTRL_INTR_MASK   (1 << 9)
#define USB_DBL_BUF             (1 << 8) // EP_KIND for BULK EP
#define USB_STATUS_OUT          (1 << 8) // EP_KING for CONTROL EP
#define USB_CTR_TX              (1 << 7)
#define USB_DTOG_TX             (1 << 6)
#define USB_GET_DTOG_TX(r)      (((r) >> 6) & 1)
#define USB_STAT_TX_DISABLED    (0 << 4)
#define USB_STAT_TX_STALL       (1 << 4)
#define USB_STAT_TX_NAK         (2 << 4)
#define USB_STAT_TX_VALID       (3 << 4)
#define USB_STAT_TX_MASK        (3 << 4)
#define USB_EA(x)               ((x) & 0xF)

#define USB_EPR_RW_MASK         0x070F

// BTABLE COUNT_RX bits
#define USB_BL_SIZE             (1 << 15)
#define USB_NUM_BLOCK(x)        ((x) << 10)
#define USB_GET_COUNT_RX(x)     ((x) & 0x1FF)

// USB_CNTR bits
#define USB_CTRM                (1 << 15)
#define USB_PMAOVRM             (1 << 14)
#define USB_ERRM                (1 << 13)
#define USB_WKUPM               (1 << 12)
#define USB_SUSPM               (1 << 11)
#define USB_RESETM              (1 << 10)
#define USB_SOFM                (1 << 9)
#define USB_ESOFM               (1 << 8)
#define USB_RESUME              (1 << 4)
#define USB_FSUSP               (1 << 3)
#define USB_LP_MODE             (1 << 2)
#define USB_PDWN                (1 << 1)
#define USB_FRES                (1 << 0)

// USB_ISTR bits
#define USB_CTR                 (1 << 15)
#define USB_PMAOVR              (1 << 14)
#define USB_ERR                 (1 << 13)
#define USB_WKUP                (1 << 12)
#define USB_SUSP                (1 << 11)
#define USB_RESET               (1 << 10)
#define USB_SOF                 (1 << 9)
#define USB_ESOF                (1 << 8)
#define USB_DIR                 (1 << 4)
#define USB_GET_EP_ID(x)        ((x) & 0xF)

// USB_FNR bits
#define USB_RXDP                (1 << 15)
#define USB_RXDM                (1 << 14)
#define USB_LCK                 (1 << 13)
#define USB_GET_LSOF(r)         (((r) >> 11) & 3)
#define USB_GET_FN(r)           ((r) & 0x7FF)

// USB_DADDR bits
#define USB_EF                  (1 << 7)
#define USB_ADD(x)              (x)


/////////////////////////////////////////
// USART
/////////////////////////////////////////
typedef struct
{
    arm_reg_t SR;       // Status
    arm_reg_t DR;       // Data
    arm_reg_t BRR;      // Baud rate
    arm_reg_t CR1;      // Control 1
    arm_reg_t CR2;      // Control 2
    arm_reg_t CR3;      // Control 3
    arm_reg_t GTPR;     // Guard time and prescaler
} USART_t;

#define USART1   ((USART_t*) STM_USART1_BASE)
#define USART2   ((USART_t*) STM_USART2_BASE)
#define USART3   ((USART_t*) STM_USART3_BASE)
#define UART4    ((USART_t*) STM_UART4_BASE)
#define UART5    ((USART_t*) STM_UART5_BASE)
#define USART6   ((USART_t*) STM_USART6_BASE)

// SR bits
#define USART_CTS           (1 << 9)
#define USART_LBD           (1 << 8)
#define USART_TXE           (1 << 7)
#define USART_TC            (1 << 6)
#define USART_RXNE          (1 << 5)
#define USART_IDLE          (1 << 4)
#define USART_ORE           (1 << 3)
#define USART_NF            (1 << 2)
#define USART_FE            (1 << 1)
#define USART_PE            (1 << 0)

// BRR bits
#define USART_DIV_MANTISSA(n)   ((n) << 4)
#define USART_DIV_FRACTION(n)   (n)

// CR1 bits
#define USART_OVER8         (1 << 15)
#define USART_UE            (1 << 13)
#define USART_M             (1 << 12)
#define USART_WAKE          (1 << 11)
#define USART_PCE           (1 << 10)
#define USART_PS            (1 << 9)
#define USART_PEIE          (1 << 8)
#define USART_TXEIE         (1 << 7)
#define USART_TCIE          (1 << 6)
#define USART_RXNEIE        (1 << 5)
#define USART_IDLEIE        (1 << 4)
#define USART_TE            (1 << 3)
#define USART_RE            (1 << 2)
#define USART_RWU           (1 << 1)
#define USART_SBK           (1 << 0)

// CR2 bits
#define USART_LINEN         (1 << 14)
#define USART_STOP_1        (0 << 12)
#define USART_STOP_05       (1 << 12)
#define USART_STOP_2        (2 << 12)
#define USART_STOP_15       (3 << 12)
#define USART_STOP_MASK     (3 << 12)
#define USART_CLKEN         (1 << 11)
#define USART_CPOL          (1 << 10)
#define USART_CPHA          (1 << 9)
#define USART_LBCL          (1 << 8)
#define USART_LBDIE         (1 << 6)
#define USART_LBDL          (1 << 5)
#define USART_ADD(n)        (n)

// CR3 bits
#define USART_ONEBIT        (1 << 11)
#define USART_CTSIE         (1 << 10)
#define USART_CTSE          (1 << 9)
#define USART_RTSE          (1 << 8)
#define USART_DMAT          (1 << 7)
#define USART_DMAR          (1 << 6)
#define USART_SCEN          (1 << 5)
#define USART_NACK          (1 << 4)
#define USART_HDSEL         (1 << 3)
#define USART_IRLP          (1 << 2)
#define USART_IREN          (1 << 1)
#define USART_EIE           (1 << 0)

// GTPR bits
#define USART_GT(n)         ((n) << 8)
#define USART_PSC(n)        (n)


/////////////////////////////////////////
// FSMC
/////////////////////////////////////////
typedef struct
{
    arm_reg_t BCR1;     // chip-select control register 1
    arm_reg_t BTR1;     // chip-select timing register 1
    arm_reg_t BCR2;     // chip-select control register 2
    arm_reg_t BTR2;     // chip-select timing register 2
    arm_reg_t BCR3;     // chip-select control register 3
    arm_reg_t BTR3;     // chip-select timing register 3
    arm_reg_t BCR4;     // chip-select control register 4
    arm_reg_t BTR4;     // chip-select timing register 4
    arm_reg_t gap0[57];
    arm_reg_t BWTR1;    // write timing register 1
    arm_reg_t gap1;
    arm_reg_t BWTR2;    // write timing register 2
    arm_reg_t gap2;
    arm_reg_t BWTR3;    // write timing register 3
    arm_reg_t gap3;
    arm_reg_t BWTR4;    // write timing register 4
} FSMC_t;

#define FSMC    ((FSMC_t*) STM_FSMC_BASE)

// FSMC_BCR bits
#define FSMC_CBURSTRW       (1 << 19)
#define FSMC_ASYNCWAIT      (1 << 15)
#define FSMC_EXTMOD         (1 << 14)
#define FSMC_WAITEN         (1 << 13)
#define FSMC_WREN           (1 << 12)
#define FSMC_WAITCFG        (1 << 11)
#define FSMC_WRAPMOD        (1 << 10)
#define FSMC_WAITPOL        (1 << 9)
#define FSMC_BURSTEN        (1 << 8)
#define FSMC_FACCEN         (1 << 6)
#define FSMC_MWID_8BITS     (0 << 4)
#define FSMC_MWID_16BITS    (1 << 4)
#define FSMC_MTYP_SRAM      (0 << 2)
#define FSMC_MTYP_CRAM      (1 << 2)
#define FSMC_MTYP_FLASH     (2 << 2)
#define FSMC_MUXEN          (1 << 1)
#define FSMC_MBKEN          (1 << 0)

// FSMC_BTR, FSMC_BWTR bits
#define FSMC_ACCMOD_A       (0 << 28)
#define FSMC_ACCMOD_B       (1 << 28)
#define FSMC_ACCMOD_C       (2 << 28)
#define FSMC_ACCMOD_D       (3 << 28)
#define FSMC_DATLAT(x)      ((x) << 24) // Only BTR
#define FSMC_CLKDIV(x)      ((x) << 20) // Only BTR
#define FSMC_BUSTURN(x)     ((x) << 16)
#define FSMC_DATAST(x)      ((x) << 8)
#define FSMC_ADDHLD(x)      ((x) << 4)
#define FSMC_ADDSET(x)      ((x) << 0)


/////////////////////////////////////////
// I2C
/////////////////////////////////////////
typedef struct
{
    arm_reg_t CR1;      // Control 1
    arm_reg_t CR2;      // Control 2
    arm_reg_t OAR1;     // Own address 1
    arm_reg_t OAR2;     // Own address 2
    arm_reg_t DR;       // Data
    arm_reg_t SR1;      // Status 1
    arm_reg_t SR2;      // Status 2
    arm_reg_t CCR;      // Clock control
    arm_reg_t TRISE;    // TRISE
} I2C_t;

#define I2C1    ((I2C_t*) STM_I2C1_BASE)
#define I2C2    ((I2C_t*) STM_I2C2_BASE)

// I2C_CR1 bits
#define I2C_SWRST           (1 << 15)
#define I2C_ALERT           (1 << 13)
#define I2C_PEC             (1 << 12)
#define I2C_POS             (1 << 11)
#define I2C_ACK             (1 << 10)
#define I2C_STOP            (1 << 9)
#define I2C_START           (1 << 8)
#define I2C_NOSTRETCH       (1 << 7)
#define I2C_ENGC            (1 << 6)
#define I2C_ENPEC           (1 << 5)
#define I2C_ENARR           (1 << 4)
#define I2C_SMBTYPE         (1 << 3)
#define I2C_SMBUS           (1 << 1)
#define I2C_PE              (1 << 0)

// I2C_CR2 bits
#define I2C_LAST            (1 << 12)
#define I2C_DMAEN           (1 << 11)
#define I2C_ITBUFEN         (1 << 10)
#define I2C_ITEVTEN         (1 << 9)
#define I2C_ITERREN         (1 << 8)
#define I2C_FREQ(x)         ((x) << 0)

// I2C_OAR1 bits
#define I2C_ADDMODE         (1 << 15)
#define I2C_ALWAYS_ONE      (1 << 14)
#define I2C_ADD(x)          ((x) << 0)

// I2C_OAR2 bits
#define I2C_ADD2(x)         ((x) << 1)
#define I2C_ENDUAL          (1 << 0)

// I2C_SR1 bits
#define I2C_SMBALERT        (1 << 15)
#define I2C_TIMEOUT         (1 << 14)
#define I2C_PECERR          (1 << 12)
#define I2C_OVR             (1 << 11)
#define I2C_AF              (1 << 10)
#define I2C_ARLO            (1 << 9)
#define I2C_BERR            (1 << 8)
#define I2C_TXE             (1 << 7)
#define I2C_RXNE            (1 << 6)
#define I2C_STOPF           (1 << 4)
#define I2C_ADD10           (1 << 3)
#define I2C_BTF             (1 << 2)
#define I2C_ADDR            (1 << 1)
#define I2C_SB              (1 << 0)

// I2C_SR2 bits
#define I2C_PECR(x)         ((x) << 8)
#define I2C_DUALF           (1 << 7)
#define I2C_SMBHOST         (1 << 6)
#define I2C_SMBDEFAULT      (1 << 5)
#define I2C_GENCALL         (1 << 4)
#define I2C_TRA             (1 << 2)
#define I2C_BUSY            (1 << 1)
#define I2C_MSL             (1 << 0)

// I2C_CCR bits
#define I2C_FS              (1 << 15)
#define I2C_DUTY            (1 << 14)
#define I2C_CCR(x)          ((x) << 0)


/////////////////////////////////////////
// SPI
/////////////////////////////////////////
typedef struct
{
    arm_reg_t CR1;      // Control 1
    arm_reg_t CR2;      // Control 2
    arm_reg_t SR;       // Status
    arm_reg_t DR;       // Data
    arm_reg_t CRCPR;    // CRC polynomial
    arm_reg_t RXCRCR;   // RX CRC
    arm_reg_t TXCRCR;   // TX CRC
    arm_reg_t I2SCFGR;  // I2S configuration
    arm_reg_t I2SPR;    // I2S prescaler
} SPI_t;

#define SPI1    ((SPI_t*) STM_SPI1_BASE)
#define SPI2    ((SPI_t*) STM_SPI2_I2S2_BASE)
#define SPI3    ((SPI_t*) STM_SPI3_I2S3_BASE)


// SPI_CR1 bits
#define SPI_BIDIMODE        (1 << 15)
#define SPI_BIDIOE          (1 << 14)
#define SPI_CRCEN           (1 << 13)
#define SPI_CRCNEXT         (1 << 12)
#define SPI_DFF             (1 << 11)
#define SPI_RXONLY          (1 << 10)
#define SPI_SSM             (1 << 9)
#define SPI_SSI             (1 << 8)
#define SPI_LSBFIRST        (1 << 7)
#define SPI_SPE             (1 << 6)
#define SPI_BR(x)           ((x) << 3)
#define SPI_MSTR            (1 << 2)
#define SPI_CPOL            (1 << 1)
#define SPI_CPHA            (1 << 0)

// SPI_CR2 bits
#define SPI_TXEIE           (1 << 7)
#define SPI_RXNEIE          (1 << 6)
#define SPI_ERRIE           (1 << 5)
#define SPI_FRF             (1 << 4)
#define SPI_SSOE            (1 << 2)
#define SPI_TXDMAEN         (1 << 1)
#define SPI_RXDMAEN         (1 << 0)

// SPI_SR bits
#define SPI_FRE             (1 << 8)
#define SPI_BSY             (1 << 7)
#define SPI_OVR             (1 << 6)
#define SPI_MODF            (1 << 5)
#define SPI_CRCERR          (1 << 4)
#define SPI_UDR             (1 << 3)
#define SPI_CHSIDE          (1 << 2)
#define SPI_TXE             (1 << 1)
#define SPI_RXNE            (1 << 0)

// SPI_I2SCFGR bits
#define SPI_I2SMOD              (1 << 11)
#define SPI_I2SE                (1 << 10)
#define SPI_I2SCFG_SLAVE_TX     (0 << 8)
#define SPI_I2SCFG_SLAVE_RX     (1 << 8)
#define SPI_I2SCFG_MASTER_TX    (2 << 8)
#define SPI_I2SCFG_MASTER_RX    (3 << 8)
#define SPI_PCMSYNC             (1 << 7)
#define SPI_I2SSTD_PHILIPS      (0 << 4)
#define SPI_I2SSTD_MSB_JUST     (1 << 4)
#define SPI_I2SSTD_LSB_JUST     (2 << 4)
#define SPI_I2SSTD_PCM          (3 << 4)
#define SPI_CKPOL               (1 << 3)
#define SPI_DATLEN_16BIT        (0 << 1)
#define SPI_DATLEN_24BIT        (1 << 1)
#define SPI_DATLEN_32BIT        (2 << 1)
#define SPI_CHLEN_16BIT         (0 << 0)
#define SPI_CHLEN_32BIT         (1 << 0)

// SPI_I2SPR bits
#define SPI_MCKOE           (1 << 9)
#define SPI_ODD             (1 << 8)
#define SPI_I2SDIV(x)       (x)

/////////////////////////////////////////
// DMA
/////////////////////////////////////////
typedef struct
{
    arm_reg_t CCR;      // Configuration
    arm_reg_t CNDTR;    // Number of data
    arm_reg_t CPAR;     // Peripheral address
    arm_reg_t CMAR;     // Memory address
    arm_reg_t reserved;
} DMA_CHANNEL_t;

typedef struct
{
    arm_reg_t       ISR;        // Interrupt Status
    arm_reg_t       IFCR;       // Interrupt Flag Clear
    DMA_CHANNEL_t   chan[7];    // DMA channels registers
} DMA_t;

#define DMA1    ((DMA_t*) STM_DMA1_BASE)
#define DMA2    ((DMA_t*) STM_DMA2_BASE)

#define DMA1_CH(x)  ((DMA_CHANNEL_t *) &(DMA1->chan[(x)-1]))
#define DMA2_CH(x)  ((DMA_CHANNEL_t *) &(DMA2->chan[(x)-1]))

// ISR bits
#define DMA_TEIF(x)         (8 << (((x) - 1) * 4))
#define DMA_HTIF(x)         (4 << (((x) - 1) * 4))
#define DMA_TCIF(x)         (2 << (((x) - 1) * 4))
#define DMA_GIF(x)          (1 << (((x) - 1) * 4))

// IFCR bits
#define DMA_CTEIF(x)        (8 << (((x) - 1) * 4))
#define DMA_CHTIF(x)        (4 << (((x) - 1) * 4))
#define DMA_CTCIF(x)        (2 << (((x) - 1) * 4))
#define DMA_CGIF(x)         (1 << (((x) - 1) * 4))

// CCR bits
#define DMA_MEM2MEM         (1 << 14)
#define DMA_PL(x)           ((x) << 12)
#define DMA_MSIZE_8         (0 << 10)
#define DMA_MSIZE_16        (1 << 10)
#define DMA_MSIZE_32        (2 << 10)
#define DMA_PSIZE_8         (0 << 8)
#define DMA_PSIZE_16        (1 << 8)
#define DMA_PSIZE_32        (2 << 8)
#define DMA_MINC            (1 << 7)
#define DMA_PINC            (1 << 6)
#define DMA_CIRC            (1 << 5)
#define DMA_DIR_FROM_MEM    (1 << 4)
#define DMA_TEIE            (1 << 3)
#define DMA_HTIE            (1 << 2)
#define DMA_TCIE            (1 << 1)
#define DMA_EN              (1 << 0)

