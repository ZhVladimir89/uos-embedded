/*
 * Register definitions for STM32F4xx.
 *
 * Copyright (C) 2013 Dmitry Podkhvatilin, <vatilin@gmail.com>
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


#define ARM_SRAM_BASE       0x20000000  // Internal static memory
#define ARM_PERIPH_BASE     0x40000000  // Peripheral registers
#define ARM_SYSTEM_BASE     0xE0000000  // Core registers

#define ARM_SRAM_SIZE       (192*1024)   // Internal SRAM size

#include <runtime/cortex-m/io-cortex-m.h>

//
// Peripheral memory map
//
#define STM_TIM2_BASE           0x40000000
#define STM_TIM3_BASE           0x40000400
#define STM_TIM4_BASE           0x40000800
#define STM_TIM6_BASE           0x40001000
#define STM_TIM7_BASE           0x40001400
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
#define STM_USB_DEV_BASE        0x40005C00
#define STM_USB_SRAM_BASE       0x40006000
#define STM_BXCAN_BASE          0x40006400
#define STM_PWR_BASE            0x40007000
#define STM_DAC1_BASE           0x40007400
#define STM_I2C3_BASE           0x40007800
#define STM_DAC2_BASE           0x40009800
#define STM_SYSCFG_BASE         0x40010000
#define STM_EXTI_BASE           0x40010400
#define STM_TIM1_BASE           0x40012C00
#define STM_SPI1_BASE           0x40013000
#define STM_TIM8_BASE           0x40013400
#define STM_USART1_BASE         0x40013800
#define STM_SPI4_BASE           0x40013C00
#define STM_TIM15_BASE          0x40014000
#define STM_TIM16_BASE          0x40014400
#define STM_TIM17_BASE          0x40014800
#define STM_TIM20_BASE          0x40015000
#define STM_DMA1_BASE           0x40020000
#define STM_DMA2_BASE           0x40020400
#define STM_RCC_BASE            0x40021000
#define STM_FLASH_BASE          0x40022000
#define STM_CRC_BASE            0x40023000
#define STM_TSC_BASE            0x40024000
#define STM_GPIOA_BASE          0x48000000
#define STM_GPIOB_BASE          0x48000400
#define STM_GPIOC_BASE          0x48000800
#define STM_GPIOD_BASE          0x48000C00
#define STM_GPIOE_BASE          0x48001000
#define STM_GPIOF_BASE          0x48001400
#define STM_GPIOG_BASE          0x48001800
#define STM_GPIOH_BASE          0x48001C00
#define STM_ADC1_ADC2_BASE      0x50000000
#define STM_ADC3_ADC4_BASE      0x50000400
#define STM_FMC_BANK1_2_BASE    0x60000000
#define STM_FMC_BANK3_4_BASE    0x80000400
#define STM_FMC_BASE            0xA0000400


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
    IRQ_EXTI2_TS,
    IRQ_EXTI3,
    IRQ_EXTI4,
    IRQ_DMA1_CHANNEL1,
    IRQ_DMA1_CHANNEL2,
    IRQ_DMA1_CHANNEL3,
    IRQ_DMA1_CHANNEL4,
    IRQ_DMA1_CHANNEL5,
    IRQ_DMA1_CHANNEL6,
    IRQ_DMA1_CHANNEL7,
    IRQ_ADC1_2,
    IRQ_USB_HP_CAN1_TX,
    IRQ_USB_LP_CAN1_RX0,
    IRQ_CAN1_RX1,
    IRQ_CAN1_SCE,
    IRQ_EXTI9_5,
    IRQ_TIM1_BRK_TIM15,
    IRQ_TIM1_UP_TIM16,
    IRQ_TIM1_TRG_COM_TIM17,
    IRQ_TIM1_CC,
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
    IRQ_USB_WKUP,
    IRQ_TIM8_BRK,
    IRQ_TIM8_UP,
    IRQ_TIM8_TRG_COM,
    IRQ_TIM8_CC,
    IRQ_ADC3,
    IRQ_FMC,
    IRQ_RESERVED0,
    IRQ_RESERVED1,
    IRQ_SPI3,
    IRQ_UART4,
    IRQ_UART5,
    IRQ_TIM6_DAC,
    IRQ_TIM7,
    IRQ_DMA2_CHANNEL1,
    IRQ_DMA2_CHANNEL2,
    IRQ_DMA2_CHANNEL3,
    IRQ_DMA2_CHANNEL4,
    IRQ_DMA2_CHANNEL5,
    IRQ_ADC4,
    IRQ_RESERVED2,
    IRQ_RESERVED3,
    IRQ_COMP1_2_3,
    IRQ_COMP4_5_6,
    IRQ_COMP7,
    IRQ_I2C3_EV,
    IRQ_I2C3_ER,
    IRQ_USB_HP,
    IRQ_USB_LP,
    IRQ_USB_WKUP_RMP,
    IRQ_TIM20_BRK,
    IRQ_TIM20_UP,
    IRQ_TIM20_TRG_COM,
    IRQ_TIM20_CC,
    IRQ_FPU,
    IRQ_RESERVED4,
    IRQ_RESERVED5,
    IRQ_SPI4,
    ARCH_TIMER_IRQ,
    ARCH_INTERRUPTS
} IRQn_t;


/////////////////////////////////////////
// Reset and clock control
/////////////////////////////////////////
typedef struct
{
    arm_reg_t CR;       // Clock control
    arm_reg_t CFGR;     // Clock configuration
    arm_reg_t CIR;      // Clock interrupt
    arm_reg_t AHB2RSTR; // AHB2 peripheral reset
    arm_reg_t AHB1RSTR; // AHB1 peripheral reset
    arm_reg_t AHBENR;   // AHB1 peripheral clock enable
    arm_reg_t APB2ENR;  // APB2 peripheral clock enable
    arm_reg_t APB1ENR;  // APB1 peripheral clock enable
    arm_reg_t BDCR;     // Backup domain control
    arm_reg_t CSR;      // Clock control & status
    arm_reg_t AHBRSTR;  // AHB peripheral reset
    arm_reg_t CFGR2;    // Clock configuration 2
    arm_reg_t CFGR3;    // Clock configuration 3
} RCC_t;

#define RCC     ((RCC_t *) STM_RCC_BASE)

// CR bits
#define RCC_PLLRDY              (1 << 25)
#define RCC_PLLON               (1 << 24)
#define RCC_CSSON               (1 << 19)
#define RCC_HSEBYP              (1 << 18)
#define RCC_HSERDY              (1 << 17)
#define RCC_HSEON               (1 << 16)
#define RCC_HSICAL(n)           ((n) << 8)
#define RCC_HSITRIM(n)          ((n) << 3)
#define RCC_HSIRDY              (1 << 1)
#define RCC_HSION               (1 << 0)

// CFGR bits
#define RCC_PLLNODIV            (1 << 31)
#define RCC_MCOPRE_DIV1         (0 << 28)
#define RCC_MCOPRE_DIV2         (1 << 28)
#define RCC_MCOPRE_DIV4         (2 << 28)
#define RCC_MCOPRE_DIV8         (3 << 28)
#define RCC_MCOPRE_DIV16        (4 << 28)
#define RCC_MCOPRE_DIV32        (5 << 28)
#define RCC_MCOPRE_DIV64        (6 << 28)
#define RCC_MCOPRE_DIV128       (7 << 28)
#define RCC_MCOF                (1 << 28)
#define RCC_MCO_DISABLED        (0 << 24)
#define RCC_MCO_LSI             (2 << 24)
#define RCC_MCO_LSE             (3 << 24)
#define RCC_MCO_SYSCLK          (4 << 24)
#define RCC_MCO_HSI             (5 << 24)
#define RCC_MCO_HSE             (6 << 24)
#define RCC_MCO_PLL             (7 << 24)
#define RCC_I2SSRC              (1 << 23)
#define RCC_USBPRE              (1 << 22)
#define RCC_PLLMUL(x)           (((x) - 2) << 18)
#define RCC_PLLXTPRE            (1 << 17)
#define RCC_PLLSRC_HSI_DIV2     (0 << 15)
#define RCC_PLLSRC_HSI          (1 << 15)
#define RCC_PLLSRC_HSE          (2 << 15)
#define RCC_PPRE2_NODIV         (0 << 11)
#define RCC_PPRE2_DIV2          (4 << 11)
#define RCC_PPRE2_DIV4          (5 << 11)
#define RCC_PPRE2_DIV8          (6 << 11)
#define RCC_PPRE2_DIV16         (7 << 11)
#define RCC_PPRE1_NODIV         (0 << 8)
#define RCC_PPRE1_DIV2          (4 << 8)
#define RCC_PPRE1_DIV4          (5 << 8)
#define RCC_PPRE1_DIV8          (6 << 8)
#define RCC_PPRE1_DIV16         (7 << 8)
#define RCC_HPRE_NODIV          (0x0 << 4)
#define RCC_HPRE_DIV2           (0x8 << 4)
#define RCC_HPRE_DIV4           (0x9 << 4)
#define RCC_HPRE_DIV8           (0xA << 4)
#define RCC_HPRE_DIV16          (0xB << 4)
#define RCC_HPRE_DIV64          (0xC << 4)
#define RCC_HPRE_DIV128         (0xD << 4)
#define RCC_HPRE_DIV256         (0xE << 4)
#define RCC_HPRE_DIV512         (0xF << 4)
#define RCC_SWS_HSI             (0 << 2)
#define RCC_SWS_HSE             (1 << 2)
#define RCC_SWS_PLL             (2 << 2)
#define RCC_SWS_MASK            (3 << 2)
#define RCC_SW_HSI              (0 << 0)
#define RCC_SW_HSE              (1 << 0)
#define RCC_SW_PLL              (2 << 0)
#define RCC_SW_MASK             (3 << 0)

// CIR bits
#define RCC_CSSC                (1 << 23)
#define RCC_PLLI2SSDRYC         (1 << 21)
#define RCC_PLLRDYC             (1 << 20)
#define RCC_HSERDYC             (1 << 19)
#define RCC_HSIRDYC             (1 << 18)
#define RCC_LSERDYC             (1 << 17)
#define RCC_LSIRDYC             (1 << 16)
#define RCC_PLLRDYIE            (1 << 12)
#define RCC_HSERDYIE            (1 << 11)
#define RCC_HSIRDYIE            (1 << 10)
#define RCC_LSERDYIE            (1 << 9)
#define RCC_LSIRDYIE            (1 << 8)
#define RCC_CSSF                (1 << 7)
#define RCC_PLLRDYF             (1 << 4)
#define RCC_HSERDYF             (1 << 3)
#define RCC_HSIRDYF             (1 << 2)
#define RCC_LSERDYF             (1 << 1)
#define RCC_LSIRDYF             (1 << 0)

// APB2RSTR bits
#define RCC_TIM20RST            (1 << 20)
#define RCC_TIM17RST            (1 << 18)
#define RCC_TIM16RST            (1 << 17)
#define RCC_TIM15RST            (1 << 16)
#define RCC_SPI4RST             (1 << 15)
#define RCC_USART1RST           (1 << 14)
#define RCC_TIM8RST             (1 << 13)
#define RCC_SPI1RST             (1 << 12)
#define RCC_TIM1RST             (1 << 11)
#define RCC_SYSCFGRST           (1 << 0)

// APB1RSTR bits
#define RCC_I2C3RST             (1 << 30)
#define RCC_DAC1RST             (1 << 29)
#define RCC_PWRRST              (1 << 28)
#define RCC_DAC2RST             (1 << 26)
#define RCC_CANRST              (1 << 25)
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
#define RCC_TIM7RST             (1 << 5)
#define RCC_TIM6RST             (1 << 4)
#define RCC_TIM4RST             (1 << 2)
#define RCC_TIM3RST             (1 << 1)
#define RCC_TIM2RST             (1 << 0)

// AHBENR bits
#define RCC_ADC34EN             (1 << 29)
#define RCC_ADC12EN             (1 << 28)
#define RCC_TSCEN               (1 << 24)
#define RCC_IOPGEN              (1 << 23)
#define RCC_IOPFEN              (1 << 22)
#define RCC_IOPEEN              (1 << 21)
#define RCC_IOPDEN              (1 << 20)
#define RCC_IOPCEN              (1 << 19)
#define RCC_IOPBEN              (1 << 18)
#define RCC_IOPAEN              (1 << 17)
#define RCC_IOPHEN              (1 << 16)
#define RCC_CRCEN               (1 << 6)
#define RCC_FMCEN               (1 << 5)
#define RCC_FLITFEN             (1 << 4)
#define RCC_SRAMEN              (1 << 2)
#define RCC_DMA2EN              (1 << 1)
#define RCC_DMA1EN              (1 << 0)

// APB2ENR bits
#define RCC_TIM20EN             (1 << 20)
#define RCC_TIM17EN             (1 << 18)
#define RCC_TIM16EN             (1 << 17)
#define RCC_TIM15EN             (1 << 16)
#define RCC_SPI4EN              (1 << 15)
#define RCC_USART1EN            (1 << 14)
#define RCC_TIM8EN              (1 << 13)
#define RCC_SPI1EN              (1 << 12)
#define RCC_TIM1EN              (1 << 11)
#define RCC_SYSCFGEN            (1 << 0)

// APB1ENR bits
#define RCC_I2C3EN              (1 << 30)
#define RCC_DAC1EN              (1 << 29)
#define RCC_PWREN               (1 << 28)
#define RCC_DAC2EN              (1 << 26)
#define RCC_CANEN               (1 << 25)
#define RCC_USBEN               (1 << 23)
#define RCC_I2C2EN              (1 << 22)
#define RCC_I2C1EN              (1 << 21)
#define RCC_UART5EN             (1 << 20)
#define RCC_UART4EN             (1 << 19)
#define RCC_USART3EN            (1 << 18)
#define RCC_USART2EN            (1 << 17)
#define RCC_SPI3EN              (1 << 15)
#define RCC_SPI2EN              (1 << 14)
#define RCC_WWDGEN              (1 << 11)
#define RCC_TIM7EN              (1 << 5)
#define RCC_TIM6EN              (1 << 4)
#define RCC_TIM4EN              (1 << 2)
#define RCC_TIM3EN              (1 << 1)
#define RCC_TIM2EN              (1 << 0)

// BDCR bits
#define RCC_BDRST               (1 << 16)
#define RCC_RTCEN               (1 << 15)
#define RCC_RTCSEL_NOCLK        (0 << 8)
#define RCC_RTCSEL_LSE          (1 << 8)
#define RCC_RTCSEL_LSI          (2 << 8)
#define RCC_RTCSEL_HSE          (3 << 8)
#define RCC_LSEDRV_LOWER        (0 << 3)
#define RCC_LSEDRV_MEDIUM_HIGH  (1 << 3)
#define RCC_LSEDRV_MEDIUM_LOW   (2 << 3)
#define RCC_LSEDRV_HIGHER       (3 << 3)
#define RCC_LSEBYP              (1 << 2)
#define RCC_LSERDY              (1 << 1)
#define RCC_LSEON               (1 << 0)

// CSR bits
#define RCC_LPWRRSTF            (1 << 31)
#define RCC_WWDGRSTF            (1 << 30)
#define RCC_IWDGRSTF            (1 << 29)
#define RCC_SFTRSTF             (1 << 28)
#define RCC_PORRSTF             (1 << 27)
#define RCC_PINRSTF             (1 << 26)
#define RCC_OBLRSTF             (1 << 25)
#define RCC_RMVF                (1 << 24)
#define RCC_V18PWRRSTF          (1 << 23)
#define RCC_LSIRDY              (1 << 1)
#define RCC_LSION               (1 << 0)

// AHBRSTR bits
#define RCC_ADC34RST            (1 << 29)
#define RCC_ADC12RST            (1 << 28)
#define RCC_TSCRST              (1 << 24)
#define RCC_IOPGRST             (1 << 23)
#define RCC_IOPFRST             (1 << 22)
#define RCC_IOPERST             (1 << 21)
#define RCC_IOPDRST             (1 << 20)
#define RCC_IOPCRST             (1 << 19)
#define RCC_IOPBRST             (1 << 18)
#define RCC_IOPARST             (1 << 17)
#define RCC_IOPHRST             (1 << 16)
#define RCC_FMCRST              (1 << 5)

// CFGR2 bits
#define RCC_ADC34PRES_DISABLE   (0x00 << 9)
#define RCC_ADC34PRES_DIV1      (0x10 << 9)
#define RCC_ADC34PRES_DIV2      (0x11 << 9)
#define RCC_ADC34PRES_DIV4      (0x12 << 9)
#define RCC_ADC34PRES_DIV6      (0x13 << 9)
#define RCC_ADC34PRES_DIV8      (0x14 << 9)
#define RCC_ADC34PRES_DIV10     (0x15 << 9)
#define RCC_ADC34PRES_DIV12     (0x16 << 9)
#define RCC_ADC34PRES_DIV16     (0x17 << 9)
#define RCC_ADC34PRES_DIV32     (0x18 << 9)
#define RCC_ADC34PRES_DIV64     (0x19 << 9)
#define RCC_ADC34PRES_DIV128    (0x1A << 9)
#define RCC_ADC34PRES_DIV256    (0x1B << 9)
#define RCC_ADC12PRES_DISABLE   (0x00 << 4)
#define RCC_ADC12PRES_DIV1      (0x10 << 4)
#define RCC_ADC12PRES_DIV2      (0x11 << 4)
#define RCC_ADC12PRES_DIV4      (0x12 << 4)
#define RCC_ADC12PRES_DIV6      (0x13 << 4)
#define RCC_ADC12PRES_DIV8      (0x14 << 4)
#define RCC_ADC12PRES_DIV10     (0x15 << 4)
#define RCC_ADC12PRES_DIV12     (0x16 << 4)
#define RCC_ADC12PRES_DIV16     (0x17 << 4)
#define RCC_ADC12PRES_DIV32     (0x18 << 4)
#define RCC_ADC12PRES_DIV64     (0x19 << 4)
#define RCC_ADC12PRES_DIV128    (0x1A << 4)
#define RCC_ADC12PRES_DIV256    (0x1B << 4)
#define RCC_PREDIV_DIV(n)       (((n) - 1))

// CFGR3 bits
#define RCC_TIM34SW_PCLK2       (0 << 25)
#define RCC_TIM34SW_PLLVCO      (1 << 25)
#define RCC_TIM2SW_PCLK2        (0 << 24)
#define RCC_TIM2SW_PLLVCO       (1 << 24)
#define RCC_UART5SW_PCLK        (0 << 22)
#define RCC_UART5SW_SYSCLK      (1 << 22)
#define RCC_UART5SW_LSE         (2 << 22)
#define RCC_UART5SW_HSI         (3 << 22)
#define RCC_UART4SW_PCLK        (0 << 20)
#define RCC_UART4SW_SYSCLK      (1 << 20)
#define RCC_UART4SW_LSE         (2 << 20)
#define RCC_UART4SW_HSI         (3 << 20)
#define RCC_UART3SW_PCLK        (0 << 18)
#define RCC_UART3SW_SYSCLK      (1 << 18)
#define RCC_UART3SW_LSE         (2 << 18)
#define RCC_UART3SW_HSI         (3 << 18)
#define RCC_UART2SW_PCLK        (0 << 16)
#define RCC_UART2SW_SYSCLK      (1 << 16)
#define RCC_UART2SW_LSE         (2 << 16)
#define RCC_UART2SW_HSI         (3 << 16)
#define RCC_TIM20SW_PCLK2       (0 << 15)
#define RCC_TIM20SW_PLLVCO      (1 << 15)
#define RCC_TIM17SW_PCLK2       (0 << 13)
#define RCC_TIM17SW_PLLVCO      (1 << 13)
#define RCC_TIM16SW_PCLK2       (0 << 11)
#define RCC_TIM16SW_PLLVCO      (1 << 11)
#define RCC_TIM15SW_PCLK2       (0 << 10)
#define RCC_TIM15SW_PLLVCO      (1 << 10)
#define RCC_TIM8SW_PCLK2        (0 << 9)
#define RCC_TIM8SW_PLLVCO       (1 << 9)
#define RCC_TIM1SW_PCLK2        (0 << 8)
#define RCC_TIM1SW_PLLVCO       (1 << 8)
#define RCC_I2C3SW_HSI          (0 << 6)
#define RCC_I2C3SW_SYSCLK       (1 << 6)
#define RCC_I2C2SW_HSI          (0 << 5)
#define RCC_I2C2SW_SYSCLK       (1 << 5)
#define RCC_I2C1SW_HSI          (0 << 4)
#define RCC_I2C1SW_SYSCLK       (1 << 4)
#define RCC_UART1SW_PCLK        (0 << 0)
#define RCC_UART1SW_SYSCLK      (1 << 0)
#define RCC_UART1SW_LSE         (2 << 0)
#define RCC_UART1SW_HSI         (3 << 0)


/////////////////////////////////////////
// Flash interface
/////////////////////////////////////////
typedef struct
{
    arm_reg_t ACR;
    arm_reg_t KEYR;
    arm_reg_t OPTKEYR;
    arm_reg_t SR;
    arm_reg_t CR;
    arm_reg_t AR;
    arm_reg_t OBR;
    arm_reg_t WRPR;
} FLASH_t;

#define FLASH     ((FLASH_t*) STM_FLASH_BASE)

// ACR bits
#define FLASH_PRFTBS            (1 << 5)
#define FLASH_PRFTBE            (1 << 4)
#define FLASH_HLFCYA            (1 << 3)
#define FLASH_LATENCY(n)        (n)

// KEYR bits
#define FLASH_KEY1              0x45670123
#define FLASH_KEY2              0xCDEF89AB

// OPTKEYR bits
#define FLASH_OPTKEY1           0x45670123
#define FLASH_OPTKEY2           0xCDEF89AB

// SR bits
#define FLASH_EOP               (1 << 5)
#define FLASH_WRPRTERR          (1 << 4)
#define FLASH_PGERR             (1 << 2)
#define FLASH_BSY               (1 << 0)

// CR bits
#define FLASH_OBL_LAUNCH        (1 << 13)
#define FLASH_EOPIE             (1 << 12)
#define FLASH_ERRIE             (1 << 10)
#define FLASH_OPTWRE            (1 << 9)
#define FLASH_LOCK              (1 << 7)
#define FLASH_STRT              (1 << 6)
#define FLASH_OPTER             (1 << 5)
#define FLASH_OPTPG             (1 << 4)
#define FLASH_MER               (1 << 2)
#define FLASH_PER               (1 << 1)
#define FLASH_PG                (1 << 0)

// OBR bits
#define FLASH_OBR_SRAM_PE           (1 << 14)
#define FLASH_OBR_SRAM_VDDA_MONITOR (1 << 13)
#define FLASH_OBR_NBOOT1            (1 << 12)
#define FLASH_OBR_NRST_STDBY        (1 << 10)
#define FLASH_OBR_NRST_STOP         (1 << 9)
#define FLASH_OBR_WDG_SW            (1 << 8)
#define FLASH_RDRPT(x)              ((x) << 1)
#define FLASH_OPTERR                (1 << 0)


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
    arm_reg_t BRR;      // Bit reset
} GPIO_t;

#define GPIOA       ((GPIO_t*) STM_GPIOA_BASE)
#define GPIOB       ((GPIO_t*) STM_GPIOB_BASE)
#define GPIOC       ((GPIO_t*) STM_GPIOC_BASE)
#define GPIOD       ((GPIO_t*) STM_GPIOD_BASE)
#define GPIOE       ((GPIO_t*) STM_GPIOE_BASE)
#define GPIOF       ((GPIO_t*) STM_GPIOF_BASE)
#define GPIOG       ((GPIO_t*) STM_GPIOG_BASE)
#define GPIOH       ((GPIO_t*) STM_GPIOH_BASE)

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

#define GPIO_AF_MASK(n)     (0xF << (4 * ((n) & 7)))


/////////////////////////////////////////
// SYSCFG
/////////////////////////////////////////
typedef struct
{
    arm_reg_t CFGR1;    // configuration 1
    arm_reg_t RCR;      // CCM SRAM protection
    arm_reg_t EXTICR1;  // external interrupt configuration 1
    arm_reg_t EXTICR2;  // external interrupt configuration 2
    arm_reg_t EXTICR3;  // external interrupt configuration 3
    arm_reg_t EXTICR4;  // external interrupt configuration 4
    arm_reg_t CFGR2;    // configuration 1
    arm_reg_t CFGR3;    // configuration 1
    arm_reg_t CFGR4;    // configuration 1
} SYSCFG_t;

#define SYSCFG      ((SYSCFG_t*) STM_SYSCFG_BASE)

// CFGR1 bits
#define SYSCFG_FPU_IE(x)                ((x) << 26)
#define SYSCFG_I2C3_FMP                 (1 << 24)
#define SYSCFG_ENCODER_MODE(x)          ((x) << 22)
#define SYSCFG_I2C2_FMP                 (1 << 21)
#define SYSCFG_I2C1_FMP                 (1 << 20)
#define SYSCFG_I2C_PBx_FMP(x)           ((x) << 16)
#define SYSCFG_DAC2_CH1_DMA_RMP         (1 << 15)
#define SYSCFG_TIM7_DAC1_CH2_DMA_RMP    (1 << 14)
#define SYSCFG_TIM6_DAC1_CH2_DMA_RMP    (1 << 13)
#define SYSCFG_TIM17_DMA_RMP            (1 << 12)
#define SYSCFG_TIM16_DMA_RMP            (1 << 11)
#define SYSCFG_ADC2_DMA_RMP             (1 << 8)
#define SYSCFG_DAC1_TRIG_RMP            (1 << 7)
#define SYSCFG_TIM1_ITR3_RMP            (1 << 6)
#define SYSCFG_USB_IT_RMP               (1 << 5)
#define SYSCFG_MEM_MODE(x)              (x)

// EXTICRx bits
#define SYSCFG_PA(x)                (0 << (((x) & 3) * 4))
#define SYSCFG_PB(x)                (1 << (((x) & 3) * 4))
#define SYSCFG_PC(x)                (2 << (((x) & 3) * 4))
#define SYSCFG_PD(x)                (3 << (((x) & 3) * 4))
#define SYSCFG_PE(x)                (4 << (((x) & 3) * 4))
#define SYSCFG_PH(x)                (5 << (((x) & 3) * 4))
#define SYSCFG_PF(x)                (6 << (((x) & 3) * 4))
#define SYSCFG_PG(x)                (7 << (((x) & 3) * 4))
#define SYSCFG_PMASK(x)             (0xF << (((x) & 3) * 4))

// CFGR2 bits
#define SYSCFG_SRAM_PEF             (1 << 8)
#define SYSCFG_BYP_ADDR_PAR         (1 << 4)
#define SYSCFG_PVD_LOCK             (1 << 2)
#define SYSCFG_SRAM_PARITY_LOCK     (1 << 1)
#define SYSCFG_LOCKUP_LOCK          (1 << 0)

// CFGR3 bits
#define SYSCFG_ADC2_DMA_RMP1        (1 << 9)
#define SYSCFG_ADC2_DMA_RMP0        (1 << 8)
#define SYSCFG_I2C1_TX_DMA_RMP(x)   ((x) << 6)
#define SYSCFG_I2C1_RX_DMA_RMP(x)   ((x) << 4)
#define SYSCFG_SPI1_TX_DMA_RMP(x)   ((x) << 2)
#define SYSCFG_SPI1_RX_DMA_RMP(x)   ((x) << 0)

// CFGR4 bits
#define SYSCFG_ADC34_JEXT14_RMP     (1 << 13)
#define SYSCFG_ADC34_JEXT11_RMP     (1 << 12)
#define SYSCFG_ADC34_JEXT5_RMP      (1 << 11)
#define SYSCFG_ADC34_EXT15_RMP      (1 << 10)
#define SYSCFG_ADC34_EXT6_RMP       (1 << 9)
#define SYSCFG_ADC34_EXT5_RMP       (1 << 8)
#define SYSCFG_ADC12_JEXT13_RMP     (1 << 7)
#define SYSCFG_ADC12_JEXT6_RMP      (1 << 6)
#define SYSCFG_ADC12_JEXT3_RMP      (1 << 5)
#define SYSCFG_ADC12_EXT15_RMP      (1 << 4)
#define SYSCFG_ADC12_EXT13_RMP      (1 << 3)
#define SYSCFG_ADC12_EXT5_RMP       (1 << 2)
#define SYSCFG_ADC12_EXT3_RMP       (1 << 1)
#define SYSCFG_ADC12_EXT2_RMP       (1 << 0)


/////////////////////////////////////////
// External interrupt/event controller
/////////////////////////////////////////
typedef struct
{
    arm_reg_t IMR1;     // Interrupt mask
    arm_reg_t EMR1;     // Event mask
    arm_reg_t RTSR1;    // Rising edge trigger selection
    arm_reg_t FTSR1;    // Falling edge trigger selection
    arm_reg_t SWIER1;   // Software interrupt event
    arm_reg_t PR1;      // Pending
    arm_reg_t IMR2;     // Interrupt mask
    arm_reg_t EMR2;     // Event mask
    arm_reg_t RTSR2;    // Rising edge trigger selection
    arm_reg_t FTSR2;    // Falling edge trigger selection
    arm_reg_t SWIER2;   // Software interrupt event
    arm_reg_t PR2;      // Pending
} EXTI_t;

#define EXTI     ((EXTI_t *) STM_EXTI_BASE)

// EXTI lines
#define EXTI_GPIO(n)        (1 << (n))
#define EXTI_PVD            (1 << 16)
#define EXTI_RTC_ALARM      (1 << 17)
#define EXTI_USB_DEV_FS     (1 << 18)
#define EXTI_RTC_TAMP_TS    (1 << 19)
#define EXTI_RTC_WKUP       (1 << 20)
#define EXTI_COMP1          (1 << 21)
#define EXTI_COMP2          (1 << 22)
#define EXTI_I2C1           (1 << 23)
#define EXTI_I2C2           (1 << 24)
#define EXTI_USART1         (1 << 25)
#define EXTI_USART2         (1 << 26)
#define EXTI_I2C3           (1 << 27)
#define EXTI_USART3         (1 << 28)
#define EXTI_COMP3          (1 << 29)
#define EXTI_COMP4          (1 << 30)
#define EXTI_COMP5          (1 << 31)
#define EXTI_COMP6          (1 << 0)    // Write to registers with index 2!
#define EXTI_COMP7          (1 << 1)    // Write to registers with index 2!
#define EXTI_UART4          (1 << 2)    // Write to registers with index 2!
#define EXTI_UART5          (1 << 3)    // Write to registers with index 2!


/////////////////////////////////////////
// USART
/////////////////////////////////////////
typedef struct
{
    arm_reg_t CR1;
    arm_reg_t CR2;
    arm_reg_t CR3;
    arm_reg_t BRR;
    arm_reg_t GTPR;
    arm_reg_t RTOR;
    arm_reg_t RQR;
    arm_reg_t ISR;
    arm_reg_t ICR;
    arm_reg_t RDR;
    arm_reg_t TDR;
} USART_t;

#define USART1   ((USART_t*) STM_USART1_BASE)
#define USART2   ((USART_t*) STM_USART2_BASE)
#define USART3   ((USART_t*) STM_USART3_BASE)
#define UART4    ((USART_t*) STM_UART4_BASE)
#define UART5    ((USART_t*) STM_UART5_BASE)

// CR1 bits
#define USART_M1            (1 << 28)
#define USART_EOBIE         (1 << 27)
#define USART_RTOIE         (1 << 26)
#define USART_DEAT(x)       ((x) << 21)
#define USART_DEDT(x)       ((x) << 16)
#define USART_OVER8         (1 << 15)
#define USART_CMIE          (1 << 14)
#define USART_MME           (1 << 13)
#define USART_M0            (1 << 12)
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
#define USART_UESM          (1 << 1)
#define USART_UE            (1 << 0)

// CR2 bits
#define USART_ADD(x)        ((x) << 24)
#define USART_RTOEN         (1 << 23)
#define USART_ABRMOD(x)     ((x) << 21)
#define USART_ABREN         (1 << 20)
#define USART_MSBFIRST      (1 << 19)
#define USART_DATAINV       (1 << 18)
#define USART_TXINV         (1 << 17)
#define USART_RXINV         (1 << 16)
#define USART_SWAP          (1 << 15)
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
#define USART_ADDM7(x)      (x)

// CR3 bits
#define USART_WUFIE         (1 << 22)
#define USART_WUS_ADD_MATCH (0 << 20)
#define USART_WUS_START_BIT (2 << 20)
#define USART_WUS_RXNE      (3 << 20)
#define USART_SCARCNT(x)    ((x) << 17)
#define USART_DEP           (1 << 15)
#define USART_DEM           (1 << 14)
#define USART_DDRE          (1 << 13)
#define USART_OVRDIS        (1 << 12)
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

// RTOR bits
#define USART_BLEN(x)       ((x) << 24)
#define USART_RTO(x)        (x)

// RQR bits
#define USART_TXFRQ         (1 << 4)
#define USART_RXFRQ         (1 << 3)
#define USART_MMRQ          (1 << 2)
#define USART_SBKRQ         (1 << 1)
#define USART_ABRRQ         (1 << 0)

// ISR bits
#define USART_REACK         (1 << 22)
#define USART_TEACK         (1 << 21)
#define USART_WUF           (1 << 20)
#define USART_RWU           (1 << 19)
#define USART_SBKF          (1 << 18)
#define USART_CMF           (1 << 17)
#define USART_BUSY          (1 << 16)
#define USART_ABRF          (1 << 15)
#define USART_ABRE          (1 << 14)
#define USART_EOBF          (1 << 12)
#define USART_RTOF          (1 << 11)
#define USART_CTS           (1 << 10)
#define USART_CTSIF         (1 << 9)
#define USART_LBDF          (1 << 8)
#define USART_TXE           (1 << 7)
#define USART_TC            (1 << 6)
#define USART_RXNE          (1 << 5)
#define USART_IDLE          (1 << 4)
#define USART_ORE           (1 << 3)
#define USART_NF            (1 << 2)
#define USART_FE            (1 << 1)
#define USART_PE            (1 << 0)

// ICR bits
#define USART_WUCF          (1 << 20)
#define USART_CMCF          (1 << 17)
#define USART_EOBCF         (1 << 12)
#define USART_RTOCF         (1 << 11)
#define USART_CTSCF         (1 << 9)
#define USART_LBDCF         (1 << 8)
#define USART_TCCF          (1 << 6)
#define USART_IDLECF        (1 << 4)
#define USART_ORECF         (1 << 3)
#define USART_NCF           (1 << 2)
#define USART_FECF          (1 << 1)
#define USART_PECF          (1 << 0)

/////////////////////////////////////////
// CAN
/////////////////////////////////////////
typedef struct
{
    arm_reg_t IR;
    arm_reg_t DTR;
    arm_reg_t LR;
    arm_reg_t HR;
} CAN_mailbox_t;

typedef struct
{
    arm_reg_t R1;
    arm_reg_t R2;
} CAN_filter_t;

typedef struct
{
    arm_reg_t MCR;
    arm_reg_t MSR;
    arm_reg_t TSR;
    arm_reg_t RFxR[2];
    arm_reg_t IER;
    arm_reg_t ESR;
    arm_reg_t BTR;
    
    arm_reg_t reserved0[88];
    
    CAN_mailbox_t   TMB[3];
    CAN_mailbox_t   RMB[2];
    
    arm_reg_t reserved1[12];
    
    arm_reg_t FMR;
    arm_reg_t FM1R;
    
    arm_reg_t reserved2;
    
    arm_reg_t FS1R;
    
    arm_reg_t reserved3;
    
    arm_reg_t FFA1R;
    
    arm_reg_t reserved4;
    
    arm_reg_t FA1R;
    
    arm_reg_t reserved5;
    
    arm_reg_t reserved6[7];

    CAN_filter_t    FILT[14];
} CAN_t;

#define CAN   ((CAN_t*) STM_BXCAN_BASE)

// MCR bits
#define CAN_DBF             (1 << 16)
#define CAN_RESET           (1 << 15)
#define CAN_TTCM            (1 << 7)
#define CAN_ABOM            (1 << 6)
#define CAN_AWUM            (1 << 5)
#define CAN_NART            (1 << 4)
#define CAN_RFLM            (1 << 3)
#define CAN_TXFP            (1 << 2)
#define CAN_SLEEP           (1 << 1)
#define CAN_INRQ            (1 << 0)

// MSR bits
#define CAN_RX              (1 << 11)
#define CAN_SAMP            (1 << 10)
#define CAN_RXM             (1 << 9)
#define CAN_TXM             (1 << 8)
#define CAN_SLAKI           (1 << 4)
#define CAN_WKUI            (1 << 3)
#define CAN_ERRI            (1 << 2)
#define CAN_SLAK            (1 << 1)
#define CAN_INAK            (1 << 0)

// TSR bits
#define CAN_LOW(n)          (1 << (29 + (n)))
#define CAN_TME(n)          (1 << (26 + (n)))
#define CAN_GET_CODE(r)     (((r) >> 24) & 0x3)
#define CAN_ABRQ(n)         (1 << (7 + 8 * (n)))
#define CAN_TERR(n)         (1 << (3 + 8 * (n)))
#define CAN_ALST(n)         (1 << (2 + 8 * (n)))
#define CAN_TXOK(n)         (1 << (1 + 8 * (n)))
#define CAN_RQCP(n)         (1 << (0 + 8 * (n)))

// RFxR bits
#define CAN_RFOM            (1 << 5)
#define CAN_FOVR            (1 << 4)
#define CAN_FULL            (1 << 3)
#define CAN_GET_FMP(r)      ((r) & 0x3)

// IER bits
#define CAN_SLKIE           (1 << 17)
#define CAN_WKUIE           (1 << 16)
#define CAN_ERRIE           (1 << 15)
#define CAN_LECIE           (1 << 11)
#define CAN_BOFIE           (1 << 10)
#define CAN_EPVIE           (1 << 9)
#define CAN_EWGIE           (1 << 8)
#define CAN_FOVIE(n)        (1 << (3 + 3 * (n)))
#define CAN_FFIE(n)         (1 << (2 + 3 * (n)))
#define CAN_FMPIE(n)        (1 << (1 + 3 * (n)))
#define CAN_TMEIE           (1 << 0)

// ESR bits
#define CAN_GET_REC(r)      (((r) >> 24) & 0xFF)
#define CAN_GET_TEC(r)      (((r) >> 16) & 0xFF)
#define CAN_GET_LEC(r)      (((r) >> 4) & 0x7)
#define CAN_BOFF            (1 << 2)
#define CAN_EPVF            (1 << 1)
#define CAN_EWGF            (1 << 0)
// LEC field codes
#define CAN_LEC_NO_ERROR                0
#define CAN_LEC_STUFF_ERROR             1
#define CAN_LEC_FORM_ERROR              2
#define CAN_LEC_ACKNOWLEDGMENT_ERROR    3
#define CAN_LEC_BIT_RECESSIVE_ERROR     4
#define CAN_LEC_BIT_DOMINANT_ERROR      5
#define CAN_LEC_CRC_ERROR               6
#define CAN_LEC_RESET                   7

// BTR bits
#define CAN_SILM            (1 << 31)
#define CAN_LBKM            (1 << 30)
#define CAN_SJW(x)          ((x) << 24)
#define CAN_TS2(x)          ((x) << 20)
#define CAN_TS1(x)          ((x) << 16)
#define CAN_BRP(x)          ((x) << 0)

// Mailbox IR bits
#define CAN_STID(x)         ((x) << 21)
#define CAN_GET_STID(r)     (((r) >> 21) & 0x7FF)
#define CAN_EXID(x)         ((x) << 3)
#define CAN_GET_EXID(r)     (((r) >> 3) & 0x1FFFFFFF)
#define CAN_IDE             (1 << 2)
#define CAN_RTR             (1 << 1)
#define CAN_TXRQ            (1 << 0)

// Mailbox DTR bits
#define CAN_TIME(x)         ((x) << 16)
#define CAN_GET_TIME(r)     ((r) >> 16)
#define CAN_TGT             (1 << 8)
#define CAN_GET_FMI(r)      (((r) >> 8) & 0xFF)
#define CAN_DLC(x)          (x)
#define CAN_GET_DLC(r)      ((r) & 0xF)

// FMR bits
#define CAN_FINIT           (1 << 0)

// FM1R bits
#define CAN_FBM(n)          (1 << (n))

// FS1R bits
#define CAN_FSC(n)          (1 << (n))

// FFA1R bits
#define CAN_FFA(n)          (1 << (n))

// FA1R bits
#define CAN_FACT(n)         (1 << (n))

/////////////////////////////////////////
// I2C
/////////////////////////////////////////
typedef struct
{
    arm_reg_t CR1;
    arm_reg_t CR2;
    arm_reg_t OAR1;
    arm_reg_t OAR2;
    arm_reg_t TIMINGR;
    arm_reg_t TIMEOUTR;
    arm_reg_t ISR;
    arm_reg_t ICR;
    arm_reg_t PECR;
    arm_reg_t RXDR;
    arm_reg_t TXDR;
} I2C_t;

#define I2C1   ((I2C_t*) STM_I2C1_BASE)
#define I2C2   ((I2C_t*) STM_I2C2_BASE)
#define I2C3   ((I2C_t*) STM_I2C3_BASE)

// CR1 bits
#define I2C_PECEN           (1 << 23)
#define I2C_ALERTEN         (1 << 22)
#define I2C_SMBDEN          (1 << 21)
#define I2C_SMBHEN          (1 << 20)
#define I2C_GCEN            (1 << 19)
#define I2C_WUPEN           (1 << 18)
#define I2C_NOSTRETCH       (1 << 17)
#define I2C_SBC             (1 << 16)
#define I2C_RXDMAEN         (1 << 15)
#define I2C_TXDMAEN         (1 << 14)
#define I2C_ANFOFF          (1 << 12)
#define I2C_DNF(x)          ((x) << 8)
#define I2C_ERRIE           (1 << 7)
#define I2C_TCIE            (1 << 6)
#define I2C_STOPIE          (1 << 5)
#define I2C_NACKIE          (1 << 4)
#define I2C_ADDRIE          (1 << 3)
#define I2C_RXIE            (1 << 2)
#define I2C_TXIE            (1 << 1)
#define I2C_PE              (1 << 0)

// CR2 bits
#define I2C_PECBYTE         (1 << 26)
#define I2C_AUTOEND         (1 << 25)
#define I2C_RELOAD          (1 << 24)
#define I2C_NBYTES(n)       ((n) << 16)
#define I2C_NACK            (1 << 15)
#define I2C_STOP            (1 << 14)
#define I2C_START           (1 << 13)
#define I2C_HEAD10R         (1 << 12)
#define I2C_ADD10           (1 << 11)
#define I2C_RD_WRN          (1 << 10)
#define I2C_SADD(x)         (x)

// OAR1 bits
#define I2C_OA1EN           (1 << 15)
#define I2C_OA1MODE         (1 << 10)
#define I2C_OA(x)           (x)

// OAR2 bits
#define I2C_OA2EN           (1 << 15)
#define I2C_OA2MSK(x)       ((x) << 10)
#define I2C_OA2             (x)

// TIMINGR bits
#define I2C_PRESC(x)        ((x) << 28)
#define I2C_SCLDEL(x)       ((x) << 20)
#define I2C_SDADEL(x)       ((x) << 16)
#define I2C_SCLH(x)         ((x) << 8)
#define I2C_SCLL(x)         ((x) << 0)

// TIMEOUTR bits
#define I2C_TEXTEN          (1 << 31)
#define I2C_TIMEOUTB(x)     ((x) << 16)
#define I2C_TIMEOUTEN       (1 << 15)
#define I2C_TIDLE           (1 << 12)
#define I2C_TIMEOUTA(x)     ((x) << 0)

// ISR bits
#define I2C_ADDCODE(x)      ((x) << 17)
#define I2C_GET_ADDCODE(r)  (((r) >> 17) & 0x7F)
#define I2C_DIR             (1 << 16)
#define I2C_BUSY            (1 << 15)
#define I2C_ALERT           (1 << 13)
#define I2C_TIMOUT          (1 << 12)
#define I2C_PECERR          (1 << 11)
#define I2C_OVR             (1 << 10)
#define I2C_ARLO            (1 << 9)
#define I2C_BERR            (1 << 8)
#define I2C_TCR             (1 << 7)
#define I2C_TC              (1 << 6)
#define I2C_STOPF           (1 << 5)
#define I2C_NACKF           (1 << 4)
#define I2C_ADDR            (1 << 3)
#define I2C_RXNE            (1 << 2)
#define I2C_TXIS            (1 << 1)
#define I2C_TXE             (1 << 0)

// ICR bits
#define I2C_ALERTCF         (1 << 13)
#define I2C_TIMOUTCF        (1 << 12)
#define I2C_PECCF           (1 << 11)
#define I2C_OVRCF           (1 << 10)
#define I2C_ARLOCF          (1 << 9)
#define I2C_BERRCF          (1 << 8)
#define I2C_STOPCF          (1 << 5)
#define I2C_NACKCF          (1 << 4)
#define I2C_ADDRCF          (1 << 3)


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
#define SPI4    ((SPI_t*) STM_SPI4_BASE)


// SPI_CR1 values
#define SPI_BIDIMODE        (1 << 15)
#define SPI_BIDIOE          (1 << 14)
#define SPI_CRCEN           (1 << 13)
#define SPI_CRCNEXT         (1 << 12)
#define SPI_CRCL            (1 << 11)
#define SPI_RXONLY          (1 << 10)
#define SPI_SSM             (1 << 9)
#define SPI_SSI             (1 << 8)
#define SPI_LSBFIRST        (1 << 7)
#define SPI_SPE             (1 << 6)
#define SPI_BR(x)           ((x) << 3)
#define SPI_MSTR            (1 << 2)
#define SPI_CPOL            (1 << 1)
#define SPI_CPHA            (1 << 0)

// SPI_CR2 values
#define SPI_LDMA_TX         (1 << 14)
#define SPI_LDMA_RX         (1 << 13)
#define SPI_FRXTH           (1 << 12)
#define SPI_DS(n)           (((n) - 1) << 8)
#define SPI_DS_MASK         (0xF << 8)
#define SPI_TXEIE           (1 << 7)
#define SPI_RXNEIE          (1 << 6)
#define SPI_ERRIE           (1 << 5)
#define SPI_FRF             (1 << 4)
#define SPI_NSSP            (1 << 3)
#define SPI_SSOE            (1 << 2)
#define SPI_TXDMAEN         (1 << 1)
#define SPI_RXDMAEN         (1 << 0)

// SPI_SR values
#define SPI_FTLVL_EMPTY     (0 << 11)
#define SPI_FTLVL_QUARTER   (1 << 11)
#define SPI_FTLVL_HALF      (2 << 11)
#define SPI_FTLVL_FULL      (3 << 11)
#define SPI_FTLVL_MASK      (3 << 11)
#define SPI_FRLVL_EMPTY     (0 << 9)
#define SPI_FRLVL_QUARTER   (1 << 9)
#define SPI_FRLVL_HALF      (2 << 9)
#define SPI_FRLVL_FULL      (3 << 9)
#define SPI_FRLVL_MASK      (3 << 9)
#define SPI_FRE             (1 << 8)
#define SPI_BSY             (1 << 7)
#define SPI_OVR             (1 << 6)
#define SPI_MODF            (1 << 5)
#define SPI_CRCERR          (1 << 4)
#define SPI_UDR             (1 << 3)
#define SPI_CHSIDE          (1 << 2)
#define SPI_TXE             (1 << 1)
#define SPI_RXNE            (1 << 0)

// SPI_I2SCFGR values
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

// SPI_I2SPR values
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

// ISR values
#define DMA_TEIF(x)         (8 << (((x) - 1) * 4))
#define DMA_HTIF(x)         (4 << (((x) - 1) * 4))
#define DMA_TCIF(x)         (2 << (((x) - 1) * 4))
#define DMA_GIF(x)          (1 << (((x) - 1) * 4))

// IFCR values
#define DMA_CTEIF(x)        (8 << (((x) - 1) * 4))
#define DMA_CHTIF(x)        (4 << (((x) - 1) * 4))
#define DMA_CTCIF(x)        (2 << (((x) - 1) * 4))
#define DMA_CGIF(x)         (1 << (((x) - 1) * 4))

// CCR values
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

