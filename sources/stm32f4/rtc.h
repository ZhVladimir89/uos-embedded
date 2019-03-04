#ifndef __STM32F4_RTC__
#define __STM32F4_RTC__

#include <rtc/rtc-interface.h>

typedef struct _stm32f4_rtc_t stm32f4_rtc_t;
struct _stm32f4_rtc_t
{
    rtcif_t             rtcif;
};

void stm32f4_rtc_init(stm32f4_rtc_t *stm_rtc);

#endif // __STM32F4_RTC__
