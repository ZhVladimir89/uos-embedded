#include <runtime/lib.h>

void iwdg_init (unsigned timeout_ms)
{
    RCC->CR |= RCC_LSION;
    while (! (RCC->CR & RCC_LSIRDY));
    
	IWDG->KR = IWDG_ACCESS_KEY;
	while (IWDG->SR & IWDG_PVU);

    // Используется входная частота 37 кГц от генератора LSI.
    // Частота делится на 4 (по умолчанию).
	IWDG->RLR = timeout_ms * 37 / 4 + 1;
	IWDG->KR = IWDG_START_KEY;
	IWDG->KR = IWDG_ALIVE_KEY;
}

void iwdg_ack ()
{
    IWDG->KR = IWDG_ALIVE_KEY;
}

