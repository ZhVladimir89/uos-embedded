@
@ Startup code for Cortex-M3 microcontrollers.
@
@ Copyright (C) 2019 Vladimir Zhuravlev, <vladimir.rgrtu@mail.ru>
@
@ This file is distributed in the hope that it will be useful, but WITHOUT
@ ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
@ FITNESS FOR A PARTICULAR PURPOSE.
@
@ You can redistribute this file and/or modify it under the terms of the GNU
@ General Public License (GPL) as published by the Free Software Foundation;
@ either version 2 of the License, or (at your discretion) any later version.
@ See the accompanying file "COPYING.txt" for more details.
@
@ As a special exception to the GPL, permission is granted for additional
@ uses of the text contained in this file.  See the accompanying file
@ "COPY-UOS.txt" for details.
@
		.syntax unified

		@ Vector table for Cortex-M.

		.section .init
		.align	2
		.extern	_init_			@ startup
		.extern	_fault_			@ handle faults and software interrupt
		.extern	_svc_			@ handle supervisor call
		.extern	_irq_handler_		@ handle interrupts and systick

		.thumb
_start_:	.global	_start_
		@ End of user mode stack
		.word  _estack			@ 0

		@ Exception handlers.
		@ offset 0x00000004
		.word	_init_			@ 1 - reset
		.word	_fault_			@ 2 - non-maskable interrupt
		.word	_fault_			@ 3 - hard fault
		.word	_fault_			@ 4 - memory management fault
		.word	_fault_			@ 5 - bus fault
		.word	_fault_			@ 6 - usage fault
		.word	_fault_			@ 7 - reserved
		.word	_fault_			@ 8 - reserved
		.word	_fault_			@ 9 - reserved
		.word	_fault_			@ 10 - reserved
		.word	_svc_			@ 11 - supervisor call (SVCall)
		.word	_fault_			@ 12 - debug monitor
		.word	_fault_			@ 13 - reserved
		.word	_fault_			@ 14 - software interrupt (PendSV)
		.word	_irq_handler_		@ 15 - system timer tick

		@ 68 IRQs. (see RM0008, s.197)
		@ offset 0x00000040 - position 0, global #16
		.word	_irq_handler_		@ 0
		.word	_irq_handler_		@ 1
		.word	_irq_handler_		@ 2
		.word	_irq_handler_		@ 3
		.word	_irq_handler_		@ 4
		.word	_irq_handler_		@ 5
		.word	_irq_handler_		@ 6
		.word	_irq_handler_		@ 7
		.word	_irq_handler_		@ 8
		.word	_irq_handler_		@ 9
		.word	_irq_handler_		@ 10
		.word	_irq_handler_		@ 11
		.word	_irq_handler_		@ 12
		.word	_irq_handler_		@ 13
		.word	_irq_handler_		@ 14
		.word	_irq_handler_		@ 15
		.word	_irq_handler_		@ 16
		.word	_irq_handler_		@ 17
		.word	_irq_handler_		@ 18
		.word	_irq_handler_		@ 19
		.word	_irq_handler_		@ 20
		.word	_irq_handler_		@ 21
		.word	_irq_handler_		@ 22
		.word	_irq_handler_		@ 23
		.word	_irq_handler_		@ 24
		.word	_irq_handler_		@ 25
		.word	_irq_handler_		@ 26
		.word	_irq_handler_		@ 27
		.word	_irq_handler_		@ 28
		.word	_irq_handler_		@ 29
		.word	_irq_handler_		@ 30
		.word	_irq_handler_		@ 31
		.word	_irq_handler_		@ 32
		.word	_irq_handler_		@ 33
		.word	_irq_handler_		@ 34
		.word	_irq_handler_		@ 35
		.word	_irq_handler_		@ 36
		.word	_irq_handler_		@ 37
		.word	_irq_handler_		@ 38
		.word	_irq_handler_		@ 39
		.word	_irq_handler_		@ 40
		.word	_irq_handler_		@ 41
		.word	_irq_handler_		@ 42
		.word	_irq_handler_		@ 43
		.word	_irq_handler_		@ 44
		.word	_irq_handler_		@ 45
		.word	_irq_handler_		@ 46
		.word	_irq_handler_		@ 47
		.word	_irq_handler_		@ 48
		.word	_irq_handler_		@ 49
		.word	_irq_handler_		@ 50
		.word	_irq_handler_		@ 51
		.word	_irq_handler_		@ 52
		.word	_irq_handler_		@ 53
		.word	_irq_handler_		@ 54
		.word	_irq_handler_		@ 55
		.word	_irq_handler_		@ 56
		.word	_irq_handler_		@ 57
		.word	_irq_handler_		@ 58
		.word	_irq_handler_		@ 59
		.word	_irq_handler_		@ 60
		.word	_irq_handler_		@ 61
		.word	_irq_handler_		@ 62
		.word	_irq_handler_		@ 63
		.word	_irq_handler_		@ 64
		.word	_irq_handler_		@ 65
		.word	_irq_handler_		@ 66
		.word	_irq_handler_		@ 67 - USB On The Go FS global interrupt
