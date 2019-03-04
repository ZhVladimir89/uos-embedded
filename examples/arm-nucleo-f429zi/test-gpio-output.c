/*
 * Testing GPIO output.
 */
#include <runtime/lib.h>
#include <kernel/uos.h>
#include <timer/timer.h>
#include <stm32f4/gpio.h>

ARRAY (task_space, 0x400);	/* Memory for task stack */

timer_t timer;
stm32f4_gpio_t outpin;

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
    timer_init(&timer, KHZ, 1);
    stm32f4_gpio_init(&outpin, GPIO_PORT_A, 3, GPIO_FLAGS_OUTPUT);
	task_create (task, "task", "task", 1, task_space, sizeof (task_space));
}
