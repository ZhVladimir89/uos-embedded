/*
 * Measuring task switch time.
 */
#include "runtime/lib.h"
#include "kernel/uos.h"
#include <kernel/internal.h>

ARRAY (task_console, 0x400);
ARRAY (task_receiver, 0x400);
mutex_t mailbox;

volatile unsigned nmessages;
volatile unsigned latency_min = ~0;
volatile unsigned latency_max;

/*
 * Печать рационального числа a/b с двумя знаками после запятой.
 */
void print_rational (char *title, unsigned a, unsigned b)
{
	unsigned val = a * 1000 / b;

	debug_printf ("%s%u.%03u usec  \n", title, val/1000, val%1000);
}

/*
 * Задача выдачи статистики на консоль.
 */
void console (void *arg)
{
	unsigned t0;

	for (;;) {
		t0 = ARM_SYSTICK->VAL;
		mutex_signal (&mailbox, (void*) t0);

		debug_puts ("\33[H");
		debug_puts ("Measuring task switch time.\n\n");

		debug_printf ("CPU frequency: %u MHz\n", KHZ / 1000);
		debug_printf ("Task switches: %u  \n\n", nmessages);

		print_rational (" Latency, min: ", latency_min * 1000, KHZ);
		print_rational ("          max: ", latency_max * 1000, KHZ);
	}
}

/*
 * Задача приёма сообщений.
 */
void receiver (void *arg)
{
	unsigned t0, t1, latency;

	for (;;) {
		t0 = (unsigned) mutex_wait (&mailbox);
		t1 = ARM_SYSTICK->VAL;

		/* Вычисляем количество тактов, затраченных на вход в прерывание. */
		latency = t0 - t1;

		/*debug_printf ("<%u, %u, %u> ", latency, t0, t1);*/
		if (++nmessages > 10) {
			if (latency_min > latency)
				latency_min = latency;
			if (latency_max < latency)
				latency_max = latency;
		}
	}
}

void uos_init (void)
{
	/* Стираем экран. */
	debug_puts ("\33[H\33[2J");

    ARM_SYSTICK->LOAD = 0xFFFFFF;

	ARM_SYSTICK->CTRL = ARM_SYSTICK_CTRL_ENABLE |
			    ARM_SYSTICK_CTRL_TICKINT |
			    ARM_SYSTICK_CTRL_HCLK;

	/* Запускаем задачу выдачи статистики на консоль. */
	task_create (console, 0, "console", 1, task_console, sizeof (task_console));

	/* Запускаем задачу приёма сообщений. */
	task_create (receiver, 0, "rcv", 2, task_receiver, sizeof (task_receiver));
}
