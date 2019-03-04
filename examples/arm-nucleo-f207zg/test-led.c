/*
 * Проверка светодиодов на отладочной плате
 */
#include <runtime/lib.h>

// PB0, PB7, PB14
const uint8_t led_pins[] = {0, 7, 14};

#define LED_ON(n) (GPIOB->BSRR = GPIO_SET(led_pins[(n)]))
#define LED_OFF(n) (GPIOB->BSRR = GPIO_RESET(led_pins[(n)]))

void init_leds ()
{
    RCC->AHB1ENR |= RCC_GPIOBEN;
    GPIOB->MODER = GPIO_OUT(led_pins[0]) | GPIO_OUT(led_pins[1]) | GPIO_OUT(led_pins[2]);
}

int main (void)
{
	init_leds ();

	int i = 0;
	for (;;) {
		LED_OFF(i);
		i = (i + 1) % sizeof(led_pins);
		LED_ON(i);
		mdelay (500);
	}
}
