/*
 * Testing GPIO interrupt.
 */
#include <runtime/lib.h>
#include <kernel/uos.h>
#include <stm32f4/gpio.h>

ARRAY (task_space, 1000);	/* Memory for task stack */

stm32f4_gpio_t user_button;

void button_irq_handler(gpioif_t *pin, void *arg)
{
    debug_printf("button_irq_handler, arg = %d\n", (int) arg);
}

void task (void *arg)
{
    gpioif_t *btn = arg;
    int cnt = 0;
    
    gpio_attach_interrupt(btn, GPIO_EVENT_RISING_EDGE, 
            button_irq_handler, (void *) 5);
	for (;;) {
	    gpio_wait_irq(btn);
		debug_printf ("Message from task after interrupt\n");
		if (++cnt == 3) {
		    gpio_detach_interrupt(btn);
		    debug_printf ("Interrupt detached\n");
		}
	}
}

void uos_init (void)
{
    debug_printf("Testing interrupts from button\n");
    stm32f4_gpio_init(&user_button, GPIO_PORT_C, 13, GPIO_FLAGS_INPUT);
	task_create (task, &user_button, "task", 1, task_space, sizeof (task_space));
}
