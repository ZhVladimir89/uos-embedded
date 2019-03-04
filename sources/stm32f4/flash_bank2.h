#ifndef __STM32F4_FLASH_BANK2_H__
#define __STM32F4_FLASH_BANK2_H__

//
// This is driver for STM32F42xx and STM32F43xx program flash bank 2 only!
//

#include <flash/flash-interface.h>

#if !defined(ARM_STM32F42xxx) && !defined(ARM_STM32F43xxx)
#error This flash driver only supported for ARM_STM32F42xxx and ARM_STM32F42xxx!
#endif

enum {
    FLASH_BANK2_SIZE_512KBYTE,
    FLASH_BANK2_SIZE_1MBYTE
};

struct _stm32f4_flash_bank2_t
{
    flashif_t       flashif;
    
    int             size;
};
typedef struct _stm32f4_flash_bank2_t stm32f4_flash_bank2_t;

void stm32f4_flash_bank2_init(stm32f4_flash_bank2_t *m, int size_flag);

#endif // __STM32F4_FLASH_BANK2_H__
