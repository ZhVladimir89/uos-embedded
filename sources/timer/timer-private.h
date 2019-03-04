#ifndef _TIMER_PRIVATE_H_
#define _TIMER_PRIVATE_H_

#include <runtime/lib.h>

#if I386
#   include <runtime/i386/i8253.h>
#   define TIMER_IRQ        0   /* IRQ0 */
#endif

#if __AVR__             /* Timer 1 compare A */
#   if defined(__AVR_ATmega2561__) || defined(__AVR_ATmega2560__)
#      define TIMER_IRQ     16
#   endif
#   if defined (__AVR_ATmega103__) || defined (__AVR_ATmega128__)
#      define TIMER_IRQ     11
#   endif
#   ifdef __AVR_ATmega161__
#      define TIMER_IRQ     6
#   endif
#   ifdef __AVR_ATmega168__
#      define TIMER_IRQ     10
#   endif
#endif  /*__AVR__*/

#if ARM_S3C4530
#   define TIMER_IRQ        10  /* Timer 0 interrupt */
#endif

#if ARM_AT91SAM
#   define TIMER_IRQ    AT91C_ID_SYS
#endif

#if ARM_OMAP44XX
#   define TIMER_IRQ        29  /* Private Timer interrupt */
#endif

#if ELVEES_MC24
#   define TIMER_IRQ        29  /* Interval Timer interrupt */
#endif

#if ELVEES_MC0226
#   define TIMER_IRQ        29  /* Interval Timer interrupt */
#endif

#if ELVEES_MC24R2
#   define TIMER_IRQ        22  /* Interval Timer interrupt */
#endif

#if ELVEES_NVCOM01
#   define TIMER_IRQ        22  /* Interval Timer interrupt */
#endif

#if ELVEES_NVCOM02
#   define TIMER_IRQ        22  /* TODO: Interval Timer interrupt */
#endif

#if ELVEES_MCT02
#   define TIMER_IRQ        22  /* Interval Timer interrupt */
#endif

#if ELVEES_MCT03P
#   define TIMER_IRQ        22  /* Interval Timer interrupt */
#endif

#if ELVEES_MC0428
#   define TIMER_IRQ        22  /* Interval Timer interrupt */
#endif

#if ELVEES_MC30SF6
#   define TIMER_IRQ		22	/* Interval Timer interrupt */
#endif

#if PIC32MX
#   define TIMER_IRQ    PIC32_VECT_CT   /* Core Timer Interrupt */
#endif

#if ARM_STM32F2 || ARM_STM32F3 || ARM_STM32F4
#   define TIMER_IRQ        ARCH_TIMER_IRQ  /* Systick */
#endif

#if ARM_STM32L1
#   if defined(RTC_TIMER)
#       define TIMER_IRQ        IRQ_RTC_WKUP    /* RTC */
#   else
#       define TIMER_IRQ        ARCH_TIMER_IRQ  /* Systick */
#   endif
#endif

#if ARM_1986BE9
#   define TIMER_IRQ        32  /* Systick */
#endif

#if ARM_1986BE1
#   if (ARM_SYS_TIMER==1)
#       define TIMER_IRQ    TIMER1_IRQn
#       define SYS_TIMER    ARM_TIMER1
#       define PER_CLOCK_EN ARM_PER_CLOCK_TIMER1
#       define TIM_CLK_EN   ARM_TIM_CLOCK_EN1
#   elif (ARM_SYS_TIMER==2)
#       define TIMER_IRQ    TIMER2_IRQn
#       define SYS_TIMER    ARM_TIMER2
#       define PER_CLOCK_EN ARM_PER_CLOCK_TIMER2
#       define TIM_CLK_EN   ARM_TIM_CLOCK_EN2
#   elif (ARM_SYS_TIMER==3)
#       define TIMER_IRQ    TIMER3_IRQn
#       define SYS_TIMER    ARM_TIMER3
#       define PER_CLOCK_EN ARM_PER_CLOCK_TIMER3
#       define TIM_CLK_EN   ARM_TIM_CLOCK_EN3
#   elif (ARM_SYS_TIMER==4)
#       define TIMER_IRQ    TIMER4_IRQn
#       define SYS_TIMER    ARM_TIMER4
#       define PER_CLOCK_EN ARM_PER_CLOCK_TIMER4
#       define TIM_CLK_EN   ARM_UART_CLOCK_TIM4_EN
#   else
#       warning "ARM_SYS_TIMER is not defined in CFLAGS (target.cfg). Using TIMER1 for system timer."
#       define TIMER_IRQ    TIMER1_IRQn
#       define SYS_TIMER    ARM_TIMER1
#       define PER_CLOCK_EN ARM_PER_CLOCK_TIMER1
#       define TIM_CLK_EN   ARM_TIM_CLOCK_EN1
#   endif
#endif

#if MSP430
#   ifdef TIMERA0_VECTOR
#      define TIMER_IRQ     (TIMERA0_VECTOR / 2)
#   else
#      define TIMER_IRQ     (TIMER0_A0_VECTOR / 2)
#      define TACTL         TA0CTL
#      define TACCR0        TA0CCR0
#      define TAEX0         TA0EX0
#   endif
#endif

#if LINUX386
#   include <sys/time.h>
#   include <signal.h>
#   define TIMER_IRQ        SIGALRM
#endif


void hw_set_period(unsigned long khz, unsigned long new_period);
void hw_clear_interrupt();

#endif // _TIMER_PRIVATE_H_
