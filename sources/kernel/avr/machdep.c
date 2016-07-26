/*
 * Machine-dependent part of uOS for: Atmel AVR, GCC.
 *
 * Copyright (C) 2000-2005 Serge Vakulenko, <vak@cronyx.ru>
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
#include "runtime/lib.h"
#include "kernel/uos.h"
#include "kernel/internal.h"

/*
 * Clear bit in register, using only r30.
 */
#define clearb_r30(bit, port) do {			\
	if (_SFR_IO_ADDR (port) < 0x20)			\
		asm volatile (				\
			"cbi %0, %1"			\
			: /* no outputs */		\
			: "I" (_SFR_IO_ADDR (port)),	\
			  "I" (bit));			\
	else if (_SFR_IO_REG_P (port))			\
		asm volatile (				\
			"in r30, %0" "\n"		\
			"andi r30, %1" "\n"		\
			"out %0, r30"			\
			: /* no outputs */		\
			: "I" (_SFR_IO_ADDR (port)),	\
			  "M" (0xff & ~(1 << (bit))) 	\
			: "cc", "r30");			\
	else						\
		asm volatile (				\
			"lds r30, %0" "\n"		\
			"andi r30, %1" "\n"		\
			"sts %0, r30"			\
			: /* no outputs */		\
			: "M" (_SFR_MEM_ADDR(port)),	\
			  "M" (0xff & ~(1 << (bit)))	\
			 : "cc", "r30");		\
	} while (0)

#define SAVE_REGS() asm volatile (\
	"push	r29\n	push	r28\n"\
	"push	r27\n	push	r26\n	push	r25\n	push	r24\n"\
	"push	r23\n	push	r22\n	push	r21\n	push	r20\n"\
	"push	r19\n	push	r18\n	push	r17\n	push	r16\n"\
	"push	r15\n	push	r14\n	push	r13\n	push	r12\n"\
	"push	r11\n	push	r10\n	push	r9\n	push	r8\n"\
	"push	r7\n	push	r6\n	push	r5\n	push	r4\n"\
	"push	r3\n	push	r2\n	push	r1\n	push	r0\n"\
	"clr	__zero_reg__\n") // Последня строчка вроде лишняя
/*
 * Build the initial task's stack frame.
 * Arguments:
 * t	- the task object
 * func	- the task function to call
 * arg	- the function argument
 * sp	- the pointer to (end of) stack space
 */
void
arch_build_stack_frame (task_t *t, void (*func) (void*), void *arg, unsigned stacksz)
{
	char *sp = (char*) t + stacksz; // but not including???

	*--sp = 0;			/* caller address low... */ /* для возврата из основной функции задачи */
	*--sp = 0;			/* ...high */
#ifdef __AVR_ATmega2561__
	*--sp = 0;                      /* ...additional */
#endif
	*--sp = (unsigned) func;	/* callee address low... */ /* для вызова основной функции задачи */
	*--sp = (unsigned) func >> 8;	/* ...high */
#ifdef __AVR_ATmega2561__
	*--sp = 0;                      /* ...additional */
#endif
    *--sp = 0;			/* r31 */
	*--sp = 0x80;		/* SREG - enable interrupts */ /* задача стартует с включенными прерываниями */
	*--sp = 0;			/* r30 */
	*--sp = 0;			/* r29 */
	*--sp = 0;			/* r28 */
	*--sp = 0;			/* r27 */
	*--sp = 0;			/* r26 */
	*--sp = (unsigned) arg >> 8;	/* r25 - task argument high... */
	*--sp = (unsigned) arg;			/* r24 ...low */
	*--sp = 0;			/* r23 */
	*--sp = 0;			/* r22 */
	*--sp = 0;			/* r21 */
	*--sp = 0;			/* r20 */
	*--sp = 0;			/* r19 */
	*--sp = 0;			/* r18 */
	*--sp = 0;			/* r17 */
	*--sp = 0;			/* r16 */
	*--sp = 0;			/* r15 */
	*--sp = 0;			/* r14 */
	*--sp = 0;			/* r13 */
	*--sp = 0;			/* r12 */
	*--sp = 0;			/* r11 */
	*--sp = 0;			/* r10 */
	*--sp = 0;			/* r9 */
	*--sp = 0;			/* r8 */
	*--sp = 0;			/* r7 */
	*--sp = 0;			/* r6 */
	*--sp = 0;			/* r5 */
	*--sp = 0;			/* r4 */
	*--sp = 0;			/* r3 */
	*--sp = 0;			/* r2 */
	*--sp = 0;			/* r1 */
	*--sp = 0;			/* r0 */
	t->stack_context = (void*) --sp;

	/*
	 * Define assembler constant task_stack_context_offset,
	 * which is a byte offset of stack_context field in task_t structure.
	 * Needed for _init_ for saving task_current.
	 */

#if 0
	ASSIGN_VIRTUAL_ADDRESS (task_stack_context_offset,
		__builtin_offsetof (task_t, stack_context));
#endif
}

/*
 * Check that the memory address is correct.
 */
bool_t __attribute__((weak))
uos_valid_memory_address (void *ptr)
{
	unsigned u = (unsigned) ptr;
	extern char __stack;

	return u > 0x7FFF || (u > 0xFF && u <= (unsigned) &__stack);
}

/*
 * Perform the task switch.
 * The call is performed via the assembler label,
 * to skip the function prologue, generated by the compiler.
 */
void __attribute__((naked))
arch_task_switch (task_t *target)
{
	/* Skip function prologue. */
	asm volatile (
	"push	r31\n"    /* r31 */
	"in	r31, __SREG__\n"
	"cli\n"
	"push	r31\n"   /* SREC */
	"push	r30\n"); /* r30 */
	SAVE_REGS();     /* r29 - r0 */

	/* Save current task stack. */
	task_current->stack_context = arch_get_stack_pointer ();

	task_current = target;

	/* Switch to the new task. */
	arch_set_stack_pointer (task_current->stack_context);

	asm volatile (
"restore_and_ret:" );

	/* Restore registers. */
	asm volatile (
	"pop	r0\n	pop	r1\n	pop	r2\n	pop	r3\n"
	"pop	r4\n	pop	r5\n	pop	r6\n	pop	r7\n"
	"pop	r8\n	pop	r9\n	pop	r10\n	pop	r11\n"
	"pop	r12\n	pop	r13\n	pop	r14\n	pop	r15\n"
	"pop	r16\n	pop	r17\n	pop	r18\n	pop	r19\n"
	"pop	r20\n	pop	r21\n	pop	r22\n	pop	r23\n"
	"pop	r24\n	pop	r25\n	pop	r26\n	pop	r27\n"
	"pop	r28\n	pop	r29\n	pop	r30\n	pop	r31\n"
	"sbrc	r31, 7\n"		/* test I flag */
	"rjmp	enable_interrupts\n"
	"out	__SREG__, r31\n"
	"pop	r31\n"
	"ret\n"				/* exit with interrupts disabled */
"enable_interrupts:\n"
	"andi	r31, 0x7f\n"		/* clear I flag */
	"out	__SREG__, r31\n"
	"pop	r31\n"
	"reti\n");			/* exit with interrupts enabled */
}

/*
 * The common part of the interrupt handler,
 * to minimize the code size.
 * vch: "супербыстрый" обработчик может сюда прыгать.
 */
void __attribute__ ((naked))
handle_interrupt (void)
{
	register mutex_irq_t *h;
	register char a;	

	SAVE_REGS();
	
debug_puts ("<interrupt>\n");

	/* Assign h = r30:r31. */
	asm volatile ("" : "=z" (h));
	
	if (! h->lock) {
		/* Cannot happen. Временно */
debug_puts ("<unexpected interrupt>\n");
    a = h - mutex_irq;
    debug_putchar (0, a );
    
		asm volatile ("rjmp restore_and_ret");
	}

	if (h->handler) {
		/* If the lock is free -- call fast handler. */
		if (h->lock->master) {
			/* Remember pending irq.
			 * Call fast handler later, in mutex_unlock(). */
			h->pending = 1;
			asm volatile ("rjmp restore_and_ret");
		}
		if ((h->handler) (h->arg) != 0) {
			/* The fast handler returns 1 when it fully serviced
			 * an interrupt. In this case there is no need to
			 * wake up the interrupt handling task, stopped on
			 * mutex_wait. Task switching is not performed. */
			asm volatile ("rjmp restore_and_ret");
		}
	}

	/* Signal the interrupt handler, if any. */
	mutex_activate (h->lock, 0);

	/* LY: copy few lines of code from task_schedule() here. */
	if (task_need_schedule)	{
		task_t *new;

		task_need_schedule = 0;
		new = task_policy ();
		if (new != task_current) {
			task_current->stack_context = arch_get_stack_pointer ();
			task_current = new;
			new->ticks++;
			arch_set_stack_pointer (task_current->stack_context);
		}
	}
	asm volatile ("rjmp restore_and_ret");
}

/*
 * The interrupt handler pattern.
 */
#define HANDLE(n,mask)\
void __attribute__ ((naked)) \
_intr##n (void) \
{\
	asm volatile (\
	"push	r31\n"\
	"in	r31, __SREG__\n"\
	"ori	r31, 0x80\n"		/* set I flag on exit */\
	"push	r31\n"\
	"push	r30\n");\
	mask;				/* disable the irq, avoiding loops */\
	asm volatile ("rjmp handle_interrupt"\
		: : "z" (&mutex_irq[n]));\
}

#ifdef __AVR_ATmega103__
#  include "ATmega103.c"
#endif
#ifdef __AVR_ATmega128__
#  include "ATmega128.c"
#endif
#ifdef __AVR_AT90USB647__
#  include "AT90USB647.c"
#endif
#ifdef __AVR_ATmega161__
#  include "ATmega161.c"
#endif
#ifdef __AVR_ATmega168__
#  include "ATmega168.c"
#endif
#ifdef __AVR_ATmega2561__
#  include "ATmega2561.c"
#endif
