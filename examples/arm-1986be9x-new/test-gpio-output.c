/*
 * Testing GPIO output.
 */
#include <runtime/lib.h>
#include <kernel/uos.h>
#include <timer/timer.h>
#include <milandr_new/gpio.h>

ARRAY (task_space, 0x400);	/* Memory for task stack */

timer_t timer;
milandr_gpio_t outpin;

void task (void *arg)
{
    int val = 0;
	for (;;) {
	    mutex_wait(&timer.lock);
		gpio_set_val(&outpin.gpioif, val);
		val = !val;
	}
}

void uos_init (void)
{
    debug_printf("\n\nTesting GPIO output (look at oscilloscope!)\n");
    timer_init(&timer, KHZ, 1);
    milandr_gpio_init(&outpin, GPIO_PORT_C, 4, GPIO_FLAGS_OUTPUT | GPIO_FLAGS_LOW_SPEED);
	task_create (task, "task", "task", 1, task_space, sizeof (task_space));
}
