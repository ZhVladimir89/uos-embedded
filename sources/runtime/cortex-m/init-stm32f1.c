/*
 * Startup initialization and exception handlers for STM32L1xx family.
 *
 * Copyright (C) 2019 Vladimir Zhuravlev, <vladimir.rgrtu@mail.ru>
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

extern void main (void);

// Select 72MHz PLL core clock source based on 8MHz HSE
static void stm32f1_core_clk_PLL72_on_HSE8(void)
{
    volatile uint32_t timeoutCounter = 0;

    SET_BIT(RCC->CR, RCC_CR_HSEON);
    do
    {
        timeoutCounter++;
    } while(((RCC->CR & RCC_CR_HSERDY) == 0) && (timeoutCounter != HSE_STARTUP_TIMEOUT));

    if (RCC->CR & RCC_CR_HSERDY)
    {
        CLEAR_BIT(FLASH->ACR, FLASH_ACR_LATENCY);
        SET_BIT(FLASH->ACR, FLASH_ACR_LATENCY_2|FLASH_ACR_PRFTBE);
        SET_BIT(RCC->CFGR, RCC_CFGR_HPRE_DIV1|RCC_CFGR_PPRE2_DIV1|RCC_CFGR_PPRE1_DIV2);
#ifdef STM32F10X_CL
        CLEAR_BIT(RCC->CFGR2, RCC_CFGR2_PREDIV2|RCC_CFGR2_PLL2MUL|RCC_CFGR2_PREDIV1|RCC_CFGR2_PREDIV1SRC);
        SET_BIT(RCC->CFGR2, RCC_CFGR2_PREDIV2_DIV5|RCC_CFGR2_PLL2MUL8|RCC_CFGR2_PREDIV1SRC_PLL2|RCC_CFGR2_PREDIV1_DIV5);
        SET_BIT(RCC->CR, RCC_CR_PLL2ON);
        while(!(RCC->CR & RCC_CR_PLL2RDY)){}
        CLEAR_BIT(RCC->CFGR, RCC_CFGR_PLLXTPRE|RCC_CFGR_PLLSRC|RCC_CFGR_PLLMULL);
        SET_BIT(RCC->CFGR, RCC_CFGR_PLLXTPRE_PREDIV1|RCC_CFGR_PLLSRC_PREDIV1|RCC_CFGR_PLLMULL9);
#else    
        CLEAR_BIT(RCC->CFGR, RCC_CFGR_PLLSRC|RCC_CFGR_PLLXTPRE|RCC_CFGR_PLLMULL);
        SET_BIT(RCC->CFGR, RCC_CFGR_PLLSRC_HSE|RCC_CFGR_PLLMULL9);
#endif /* STM32F10X_CL */
        SET_BIT(RCC->CR, RCC_CR_PLLON);
        while(!(RCC->CR & RCC_CR_PLLRDY)){}
        CLEAR_BIT(RCC->CFGR, RCC_CFGR_SW);
        SET_BIT(RCC->CFGR, RCC_CFGR_SW_PLL);
        while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08){}
    }
    else
    {  // ERROR ON START HSE
        while(1);
    }
}

void init_sysclk()
{
#if defined(CLK_SOURCE_HSE)
#if KHZ != 72000 
#   error "Unsupported clock frequency for this version of runtime module (HSE 72000 only)"
#endif
#if KHZ_HSE != 8000
#   error "Unsupported OSC frequency for this version of runtime module (OSC 8000 only)"
#endif
    stm32f1_core_clk_PLL72_on_HSE8();
#else
#   error "Unsupported clock source for this version of runtime module (HSE 72000 only)"
#endif /* CLK_SOURCE_HSE */
}

/*
 * Initialize the system configuration, cache, intermal SRAM,
 * and set up the stack. Then call main().
 * _init_ is called from gcrt1.S.
 * Attribute "naked" skips function prologue.
 */
void __attribute ((naked))
_init_ (void)
{
    arm_set_basepri (64);
    init_sysclk();
    arch_init_ram();
    /* Initialize priority of exceptions.
     * Only faults and SVC are permitted when interrupts are disabled
     * (priority level = 0).  All other interrupts have level 64. */
    ARM_SCB->SHPR1 = ARM_SHPR1_UFAULT(0) |    /* usage fault */
                     ARM_SHPR1_BFAULT(0) |    /* bus fault */
                     ARM_SHPR1_MMFAULT(0);    /* memory management fault */
    ARM_SCB->SHPR2 = ARM_SHPR2_SVCALL(0);     /* SVCall */
    ARM_SCB->SHPR3 = ARM_SHPR3_SYSTICK(64) |  /* SysTick */
                     ARM_SHPR3_PENDSV(0);     /* PendSV */

    int i;
    for (i = 0; i < (ARCH_INTERRUPTS + 3) / 4; ++i)
        ARM_NVIC_IPR(i) = 0x40404040;

    main ();
}

/*
 * Check memory address.
 * Board-dependent function, should be replaced by user.
 */
bool_t __attribute ((weak))
uos_valid_memory_address (void *ptr)
{
    unsigned u = (unsigned) ptr;

    if (u >= ARM_SRAM_BASE && u < ARM_SRAM_BASE + ARM_SRAM_SIZE)
        return 1;

    return 0;
}

/*
 * This routine should be supplied by user.
 * Implementation of watchdog is different on different boards.
 */
void __attribute ((weak))
watchdog_alive ()
{
    /*TODO*/
}

static void dump_of_death (unsigned *frame, unsigned sp, unsigned ipsr)
{
    debug_printf ("r0 = %08x     r5 = %08x     r10 = %08x     pc   = %08x\n"
                  "r1 = %08x     r6 = %08x     r11 = %08x     xpsr = %08x\n"
                  "r2 = %08x     r7 = %08x     r12 = %08x     ipsr = %08x\n"
                  "r3 = %08x     r8 = %08x     sp  = %08x  basepri = %08x\n"
                  "r4 = %08x     r9 = %08x     lr  = %08x\n"
                  "\nReset...\n\n",
                  frame[9],      frame[1],     frame[6],      frame[15],
                  frame[10],     frame[2],     frame[7],      frame[16],
                  frame[11],     frame[3],     frame[13],     ipsr,
                  frame[12],     frame[4],     sp,            frame[8],
                  frame[0],      frame[5],     frame[14]);
    mdelay (1000);
    for (;;)
        asm volatile ("dmb");
}

void __attribute__ ((naked))
_fault_ ()
{
    /* Save registers in stack. */
    asm volatile (
                "mrs    r12, basepri \n\t"
                "push    {r4-r12}");

    unsigned *frame = arm_get_stack_pointer ();
    unsigned ipsr = arm_get_ipsr ();
    char *message;
    switch (ipsr) {
    case 2:  message = "Non Maskable"; break;
    case 3:  message = "Hard Fault"; break;
    case 4:  message = "Memory Management"; break;
    case 5:  message = "Bus Fault"; break;
    case 6:  message = "Usage Fault"; break;
    case 11: message = "SV Call"; break;
    case 12: message = "Debug Monitor"; break;
    case 14: message = "Pend SV"; break;
    case 15: message = "System Tick"; break;
    default: message = "UNKNOWN"; break;
    }

    debug_printf ("\n\n*** 0x%08x: %s Interrupt\n\n", frame[15], message);
    dump_of_death (frame, (unsigned)arm_get_stack_pointer(), ipsr);
}

void __attribute__ ((naked))
_unexpected_interrupt_ ()
{
    asm volatile ("push    {r4-r11}");

    unsigned *frame = arm_get_stack_pointer ();
    unsigned ipsr = arm_get_ipsr ();

    debug_printf ("\n\n*** 0x%08x: unexpected interrupt #%d\n\n", frame[14], ipsr);
    dump_of_death (frame, (unsigned)arm_get_stack_pointer(), ipsr);
}

void __attribute__ ((naked, weak))
_svc_ ()
{
    /* This is needed when no kernel is present. */
}

void __attribute__ ((naked, weak))
_irq_handler_ ()
{
    /* This is needed when no kernel is present. */
}
