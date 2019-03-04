@
@ Startup code for Cortex-M3 microcontrollers.
@
@ Copyright (C) 2010 Serge Vakulenko, <serge@vak.ru>
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

@---------------------------------------
@ Vector table for Cortex-M.
@
		.section .init
		.align	2
		.extern	_init_			@ startup
		.extern	_fault_			@ handle faults and software interrupt
		.extern	_svc_			@ handle supervisor call
		.extern	_irq_handler_		@ handle interrupts and systick

		.thumb
_start_:	.global	_start_
		@
		@ End of user mode stack
		@
		.word  _estack			@ 0

		@
		@ Exception handlers.
		@
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
		.word	_svc_			@ 11 - supervisor call
		.word	_fault_			@ 12 - reserved
		.word	_fault_			@ 13 - reserved
		.word	_fault_			@ 14 - software interrupt
		.word	_irq_handler_	@ 15 - system timer tick

		@
		@ IRQs.
		@
		.word	_irq_handler_   @ 16
		.word	_irq_handler_   @ 17
		.word	_irq_handler_   @ 18
		.word	_irq_handler_   @ 19
		.word	_irq_handler_   @ 20
		.word	_irq_handler_   @ 21
		.word	_irq_handler_   @ 22
		.word	_irq_handler_   @ 23
		.word	_irq_handler_   @ 24
		.word	_irq_handler_   @ 25
		.word	_irq_handler_   @ 26
		.word	_irq_handler_   @ 27
		.word	_irq_handler_   @ 28
		.word	_irq_handler_   @ 29
		.word	_irq_handler_   @ 30
		.word	_irq_handler_   @ 31
		.word	_irq_handler_   @ 32
		.word	_irq_handler_   @ 33
		.word	_irq_handler_   @ 34
		.word	_irq_handler_   @ 35
		.word	_irq_handler_   @ 36
		.word	_irq_handler_   @ 37
		.word	_irq_handler_   @ 38
		.word	_irq_handler_   @ 39
		.word	_irq_handler_   @ 40
		.word	_irq_handler_   @ 41
		.word	_irq_handler_   @ 42
		.word	_irq_handler_   @ 43
		.word	_irq_handler_   @ 44
		.word	_irq_handler_   @ 45
		.word	_irq_handler_   @ 46
		.word	_irq_handler_   @ 47
		.word	_irq_handler_   @ 48
		.word	_irq_handler_   @ 49
		.word	_irq_handler_   @ 50
		.word	_irq_handler_   @ 51
		.word	_irq_handler_   @ 52
		.word	_irq_handler_   @ 53
		.word	_irq_handler_   @ 54
		.word	_irq_handler_   @ 55
		.word	_irq_handler_   @ 56
		.word	_irq_handler_   @ 57
		.word	_irq_handler_   @ 58
		.word	_irq_handler_   @ 59
		.word	_irq_handler_   @ 60
		.word	_irq_handler_   @ 61
		.word	_irq_handler_   @ 62
		.word	_irq_handler_   @ 63
		.word	_irq_handler_   @ 64
		.word	_irq_handler_   @ 65
		.word	_irq_handler_   @ 66
		.word	_irq_handler_   @ 67
		.word	_irq_handler_   @ 68
		.word	_irq_handler_   @ 69
		.word	_irq_handler_   @ 70
		.word	_irq_handler_   @ 71
		.word	_irq_handler_   @ 72
		.word	_irq_handler_   @ 73
		.word	_irq_handler_   @ 74
		.word	_irq_handler_   @ 75
		.word	_irq_handler_   @ 76
		.word	_irq_handler_   @ 77
		.word	_irq_handler_   @ 78
		.word	_irq_handler_   @ 79
		.word	_irq_handler_   @ 80
		.word	_irq_handler_   @ 81
		.word	_irq_handler_   @ 82
		.word	_irq_handler_   @ 83
		.word	_irq_handler_   @ 84
		.word	_irq_handler_   @ 85
		.word	_irq_handler_   @ 85
		.word	_irq_handler_   @ 87
		.word	_irq_handler_   @ 88
		.word	_irq_handler_   @ 89
		.word	_irq_handler_   @ 90
		.word	_irq_handler_   @ 91
		.word	_irq_handler_   @ 92
		.word	_irq_handler_   @ 93
		.word	_irq_handler_   @ 94
		.word	_irq_handler_   @ 95
		.word	_irq_handler_   @ 96
		.word	_irq_handler_   @ 97

