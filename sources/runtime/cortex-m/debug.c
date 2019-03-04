/*
 * Debug console input/output for Cortex-M3 architecture.
 *
 * Copyright (C) 2010 Serge Vakulenko, <serge@vak.ru>
 *               2013 Dmitry Podkhvatilin <vatilin@gmail.com>
 *
 * This file is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You can redistribute this file and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software Foundation;
 * either version 2 of the License, or (at your discretion) any later version.
 * See the accompanying file "COPYING.txt" for more details.
 *
 * As a special exception to the GPL, permission is granted for additional
 * uses of the text contained in this file.  See the accompanying file
 * "COPY-UOS.txt" for details.
 */
#include <runtime/lib.h>
#include <kernel/uos.h>
#include <kernel/internal.h>

bool_t debug_onlcr = 1;

static int debug_char = -1;

static void (*hook) (void *arg, short c);
static void *hook_arg;

/*
 * Redirect debug output.
 */
void
debug_redirect (void (*func) (void*, short), void *arg)
{
	hook = func;
	hook_arg = arg;
}

#if defined (ARM_1986BE1) || defined (ARM_1986BE9)

#   ifdef ARM_UART1_DEBUG
#      define DBG_UART  ARM_UART1
#   else
#      define DBG_UART  ARM_UART2
#   endif

static inline int arch_transmitter_busy()
{
    return DBG_UART->FR & ARM_UART_FR_TXFF;
}

static inline void arch_putchar(short c)
{
    DBG_UART->DR = c;
}

static inline int arch_receiver_empty()
{
    return DBG_UART->FR & ARM_UART_FR_RXFE;
}

static inline short arch_getchar()
{
    return DBG_UART->DR & ARM_UART_DR_DATA;
}

#endif // defined (ARM_1986BE1) || defined (ARM_1986BE9)


#if defined(ARM_STM32F2) || defined(ARM_STM32F4) || defined(ARM_STM32L1)

#if defined(USE_USART1)
#	define DBG_USART	USART1
#elif defined(USE_USART6)
#	define DBG_USART	USART6
#else
#	define DBG_USART	USART3
#endif

static inline int arch_transmitter_busy()
{
    return ! (DBG_USART->SR & USART_TC);
}

static inline void arch_putchar(short c)
{
    DBG_USART->DR = c;
}

static inline int arch_receiver_empty()
{
    return ! (DBG_USART->SR & USART_RXNE);
}

static inline short arch_getchar()
{
    return DBG_USART->DR;
}

#endif // defined(ARM_STM32F4) || defined(ARM_STM32L1)


#if defined(ARM_STM32F3)

#	define DBG_USART	USART1

static inline int arch_transmitter_busy()
{
    return ! (DBG_USART->ISR & USART_TC);
}

static inline void arch_putchar(short c)
{
    DBG_USART->TDR = c;
}

static inline int arch_receiver_empty()
{
    return ! (DBG_USART->ISR & USART_RXNE);
}

static inline short arch_getchar()
{
    return DBG_USART->RDR;
}

#endif // defined(ARM_STM32F3)


/*
 * Send a byte to the UART transmitter, with interrupts disabled.
 */
void
debug_putchar (void *arg, short c)
{
	arch_state_t x;

	arm_intr_disable (&x);

	if (hook) {
		hook (hook_arg, c);
		arm_intr_restore (x);
		return;
	}
	/* Wait for transmitter holding register empty. */
	while (arch_transmitter_busy())
		continue;
		
again:
	/* Send byte. */
	/* TODO: unicode to utf8 conversion. */
	arch_putchar(c);

    /* Wait for transmitter holding register empty. */
	while (arch_transmitter_busy())
		continue;

	watchdog_alive ();
	if (debug_onlcr && c == '\n') {
		c = '\r';
		goto again;
	}
	arm_intr_restore (x);
}

/*
 * Wait for the byte to be received and return it.
 */
unsigned short
debug_getchar (void)
{
	unsigned c;
	arch_state_t x;

	if (debug_char >= 0) {
		c = debug_char;
		debug_char = -1;
/*debug_printf ("getchar -> 0x%02x\n", c);*/
		return c;
	}
	arm_intr_disable (&x);

	/* Wait until receive data available. */
	while (arch_receiver_empty()) {
		watchdog_alive ();
		arm_intr_restore (x);
		arm_intr_disable (&x);
	}
	/* TODO: utf8 to unicode conversion. */
	c = arch_getchar();

	arm_intr_restore (x);
	return c;
}

/*
 * Get the received byte without waiting.
 */
int
debug_peekchar (void)
{
	unsigned char c = 0;
	arch_state_t x;

	if (debug_char >= 0)
		return debug_char;

	arm_intr_disable (&x);
    
	if (hook) {
		hook (hook_arg, c);
		arm_intr_restore (x);
		return 0;
	}

	/* Check if receive data available. */
	if (arch_receiver_empty()) {
		arm_intr_restore (x);
		return -1;
	}
	/* TODO: utf8 to unicode conversion. */
	c = arch_getchar();
	arm_intr_restore (x);
	debug_char = c;
	return c;
}

void
debug_puts (const char *p)
{
	arch_state_t x;

	arm_intr_disable (&x);
	for (; *p; ++p)
		debug_putchar (0, *p);
	arm_intr_restore (x);
}
