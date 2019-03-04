/*
 * Testing GPIO input.
 */
#include <runtime/lib.h>
#include <kernel/uos.h>
#include <stm32f4/gpio.h>

ARRAY (task_space, 0x400);	/* Memory for task stack */

stm32f4_gpio_t user_button;

void task (void *arg)
{
	for (;;) {
		debug_printf ("Button value: %d\n", gpio_val(&user_button.gpioif));
	}
}

void uos_init (void)
{
    stm32f4_gpio_init(&user_button, GPIO_PORT_C, 13, GPIO_FLAGS_INPUT);
	task_create (task, "task", "task", 1, task_space, sizeof (task_space));
}
