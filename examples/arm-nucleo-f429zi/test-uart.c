/*
 * Testing uart driver.
 */
#include <runtime/lib.h>
#include <kernel/uos.h>
#include <stm32f4/uart.h>
#include <stm32f4/gpio.h>

#define PORT    6

#define TX_MESSAGE  (const unsigned char *)"Hello using UART driver!\n\r"

ARRAY (task_space, 0x400);	/* Memory for task stack */

stm32f4_uart_t uart;
stm32f4_gpio_t pin_uart_rx;
stm32f4_gpio_t pin_uart_tx;

void task (void *arg)
{
    unsigned char rxbuf[8];
    
    uart_set_param(to_uartif(&uart), UART_BITS(8) | UART_STOP_1, 115200);
    
	for (;;) {
	    uart_tx(to_uartif(&uart), TX_MESSAGE, strlen(TX_MESSAGE));
	    uart_rx(to_uartif(&uart), rxbuf, sizeof(rxbuf), 0);
	}
}

void uos_init (void)
{
    debug_printf("Testing UART interface\n");
    stm32f4_uart_init(&uart, PORT);
    
    stm32f4_gpio_init(&pin_uart_rx, GPIO_PORT_G,  9, GPIO_FLAGS_ALT_USART6);
    stm32f4_gpio_init(&pin_uart_tx, GPIO_PORT_G, 14, GPIO_FLAGS_ALT_USART6);
    
	task_create (task, "task", "task", 1, task_space, sizeof (task_space));
}
