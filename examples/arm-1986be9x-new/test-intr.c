/*
 * Testing GPIO interrupt on PB9.
 */
#include <runtime/lib.h>
#include <kernel/uos.h>
#include <milandr_new/gpio.h>

ARRAY (task_space, 1000);	/* Memory for task stack */

milandr_gpio_t input;

void input_irq_handler(gpioif_t *pin, void *arg)
{
    debug_printf("button_irq_handler, arg = %d\n", (int) arg);
}

void task (void *arg)
{
    gpioif_t *inp = arg;
    int cnt = 0;
    
    gpio_attach_interrupt(inp, 0, input_irq_handler, (void *) 5);
	for (;;) {
	    gpio_wait_irq(inp);
		debug_printf ("Message from task after interrupt\n");
		if (++cnt == 3) {
		    gpio_detach_interrupt(inp);
		    debug_printf ("Interrupt detached\n");
		}
	}
}

void uos_init (void)
{
    debug_printf("\n\nTesting interrupts from PB9\n");
    milandr_gpio_init(&input, GPIO_PORT_B, 9, GPIO_FLAGS_FUNC_ALT | GPIO_FLAGS_PULL_DOWN); // ALT FUNC - see errata.
	task_create (task, &input, "task", 1, task_space, sizeof (task_space));
}
