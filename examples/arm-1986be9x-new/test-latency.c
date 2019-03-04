/*
 * Measuring interrupt latency.
 */
#include "runtime/lib.h"
#include "kernel/uos.h"
#include <kernel/internal.h>

#define MSEC_PER_TICK	100	/* Период таймера в миллисекундах */

ARRAY (task, 0x400);
mutex_t timer_lock;

volatile unsigned nirqs;
volatile unsigned latency_min = ~0;
volatile unsigned latency_max;

/*
 * Печать рационального числа a/b с двумя знаками после запятой.
 */
void print_rational (char *title, unsigned a, unsigned b)
{
	unsigned val = a * 100 / b;

	debug_printf ("%s%u.%02u usec  \n", title, val/100, val%100);
}

/*
 * Задача выдачи статистики на консоль.
 */
void console (void *arg)
{
	for (;;) {
		mutex_wait (&timer_lock);
		debug_puts ("\33[H");
		debug_puts ("Measuring interrupt latency.\n\n");

		debug_printf ("CPU frequency: %u MHz\n", KHZ / 1000);
		debug_printf ("\n");
		debug_printf ("   Interrupts: %u  \n\n", nirqs);

		print_rational (" Latency, min: ", latency_min * 1000, KHZ);
		print_rational ("          max: ", latency_max * 1000, KHZ);
	}
}

/*
 * Быстрый обработчик прерывания от интервального таймера.
 * Аргумент вызова не используется.
 */
static bool_t timer_handler (void *arg)
{
	/* Вычисляем количество тактов, затраченных на вход в прерывание. */
	unsigned latency = ARM_SYSTICK->LOAD - ARM_SYSTICK->VAL;

	/*debug_printf ("<%u> ", latency);*/
	if (++nirqs > 10) {
		if (latency_min > latency)
			latency_min = latency;
		if (latency_max < latency)
			latency_max = latency;
	}

	/* Возвращаем 0, чтобы послать сигнал для mutex_wait(). */
	return 0;
}

void uos_init (void)
{
	debug_printf ("\n\nTesting latency\n");

	/* Стираем экран. */
	debug_puts ("\33[H\33[2J");

	/* Устанавливаем быстрый обработчик прерывания. */
	mutex_attach_irq (&timer_lock, ARCH_TIMER_IRQ, timer_handler, 0);

	/* Используем SysTick. */
    ARM_SYSTICK->CTRL = 0;
    ARM_SYSTICK->VAL = 0;
    ARM_SYSTICK->LOAD = KHZ * MSEC_PER_TICK - 1;
    ARM_SYSTICK->CTRL = ARM_SYSTICK_CTRL_ENABLE |
                        ARM_SYSTICK_CTRL_TICKINT |
                        ARM_SYSTICK_CTRL_HCLK;

	/* Запускаем задачу выдачи статистики на консоль. */
	task_create (console, "task", "console", 1, task, sizeof (task));
}
