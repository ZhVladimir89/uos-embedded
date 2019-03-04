/*
 * Проверка драйвера таймера.
 */
#include <runtime/lib.h>
#include <kernel/uos.h>
#include <timer/timer.h>

ARRAY (task, 1000);
timer_t timer;

void hello (void *arg)
{
	for (;;) {
		debug_printf ("%s: msec = %d\n",
			arg, timer_milliseconds (&timer));
		mutex_wait (&timer.decisec);
	}
}

void uos_init (void)
{
	debug_puts ("\nTesting timer.\n");
	timer_init (&timer, KHZ, 10);
	task_create (hello, "Timer", "hello", 1, task, sizeof (task));
}
