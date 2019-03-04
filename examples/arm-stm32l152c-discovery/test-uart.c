/*
 * Testing uart driver.
 */
#include <runtime/lib.h>
#include <kernel/uos.h>
#include <stm32l1/uart.h>
#include <stm32l1/gpio.h>

#define PORT    3

#define TX_MESSAGE  (const unsigned char *)"Hello using UART driver!\n\r"

ARRAY (task_space, 0x400);	/* Memory for task stack */

stm32l1_uart_t uart;
stm32l1_gpio_t pin_uart_rx;
stm32l1_gpio_t pin_uart_tx;

void task (void *arg)
{
    unsigned char rxbuf[8];
    
    uart_set_param(to_uartif(&uart), UART_BITS(8) | UART_STOP_1, 115200);
    
	for (;;) {
	    /*
	    memset(rxbuf, 0, sizeof(rxbuf));
	    uart_rx(to_uartif(&uart), rxbuf, sizeof(rxbuf), 0);
	    debug_printf("Received: %s\n", rxbuf);
	    uart_tx(to_uartif(&uart), rxbuf, strlen(rxbuf));
	    */
	    uart_tx(to_uartif(&uart), TX_MESSAGE, strlen(TX_MESSAGE));
	}
}

void uos_init (void)
{
    debug_printf("Testing UART interface\n");
    stm32l1_uart_init(&uart, PORT);
    
    stm32l1_gpio_init(&pin_uart_rx,         GPIO_PORT_C, 11, GPIO_FLAGS_ALT_USART3);
    stm32l1_gpio_init(&pin_uart_tx,         GPIO_PORT_C, 10, GPIO_FLAGS_ALT_USART3);
    
	task_create (task, "task", "task", 1, task_space, sizeof (task_space));
}
