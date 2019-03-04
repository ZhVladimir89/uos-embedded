#include <runtime/lib.h>
#include <kernel/uos.h>
#include "rtc.h"

static void stm32f4_get_time(rtcif_t *rtc, datetime_t *dt)
{
    while (! (RTC->ISR & RTC_RSF));
    
    uint32_t ssr = RTC->SSR;
    uint32_t tr  = RTC->TR;
    uint32_t dr  = RTC->DR;
    
    RTC->ISR &= ~RTC_RSF;
    
    dt->year = 2000 + RTC_GET_YT(dr) * 10 + RTC_GET_YU(dr);
    dt->month = RTC_GET_MT(dr) * 10 + RTC_GET_MU(dr);
    dt->day = RTC_GET_DT(dr) * 10 + RTC_GET_DU(dr);
    dt->hour = RTC_GET_HT(tr) * 10 + RTC_GET_HU(tr);
    dt->minute = RTC_GET_MNT(tr) * 10 + RTC_GET_MNU(tr);
    dt->second = RTC_GET_ST(tr) * 10 + RTC_GET_SU(tr);
    dt->nsecond = 1000000000ull * ssr / 65536;
}

static void stm32f4_set_time(rtcif_t *rtc, const datetime_t *dt)
{
    uint32_t ssr = 65536ull * dt->nsecond / 1000000000ull;
    uint32_t tr  = RTC_ST(dt->second / 10) | RTC_SU(dt->second % 10) |
        RTC_MNT(dt->minute / 10) | RTC_MNU(dt->minute % 10) |
        RTC_HT(dt->hour / 10) | RTC_HU(dt->hour % 10);
    uint32_t dr  = RTC_DT(dt->day / 10) | RTC_DU(dt->day % 10) |
        RTC_MT(dt->month / 10) | RTC_MU(dt->month % 10) |
        RTC_YT((dt->year - 2000) / 10) | RTC_YU((dt->year - 2000) % 10);
        
    RTC->ISR |= RTC_INIT;
    while (! (RTC->ISR & RTC_INITF));
    
    RTC->TR = tr;
    RTC->DR = dr;
    RTC->SSR = ssr;
    
    RTC->ISR &= ~RTC_INIT;
}

void stm32f4_rtc_init(stm32f4_rtc_t *stm_rtc)
{
	rtcif_t *rtc = &stm_rtc->rtcif;
	rtc->get_time = stm32f4_get_time;
	rtc->set_time = stm32f4_set_time;
	
    /* Clear write protection for RTC registers */
    RCC->APB1ENR |= RCC_PWREN;
    PWR->CR |= PWR_DBP;
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;
    /* Enable LSE and set it as clock source for RTC */
    RCC->BDCR |= RCC_LSEON;
    while (! (RCC->BDCR & RCC_LSERDY));
    RCC->BDCR |= RCC_RTCEN | RCC_RTCSEL_LSE;
}
