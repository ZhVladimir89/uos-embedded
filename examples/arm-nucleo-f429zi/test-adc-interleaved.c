//
// Тренировка работы с АЦП.
// Используем 2 АЦП в режиме interleaved.
//
#include <runtime/lib.h>
#include <kernel/uos.h>
#include <kernel/internal.h>
#include <timer/timer.h>
#include <stm32f4/gpio.h>

#define ADC_CHANNEL_N       3
#define WATCHDOG_THRESHOLD  0xC00
#define ADC_DATA_WINDOW     (128 * 1024)

ARRAY (stack, 1000);
timer_t timer;
mutex_t adc_mutex;

stm32f4_gpio_t adc_pin;

uint8_t adc_data[ADC_DATA_WINDOW];

bool_t adc_interrupt(void *arg)
{
    if (ADC_COM->CSR & ADC_AWD1) {
        debug_printf("Analog watchdog!\n");
        ADC1->CR1 &= ~(ADC_AWDEN | ADC_AWDIE);
        ADC1->SR &= ~ADC_AWD;
    }
    
    if (ADC_COM->CSR & ADC_OVR1) {
        debug_printf("ADC1 overrun!\n");
        ADC1->CR1 &= ~ADC_OVRIE;
        ADC1->SR &= ~ADC_OVR;
    }

    if (ADC_COM->CSR & ADC_OVR2) {
        debug_printf("ADC2 overrun!\n");
        ADC2->CR1 &= ~ADC_OVRIE;
        ADC2->SR &= ~ADC_OVR;
    }
    
    arch_intr_allow(IRQ_ADC);
    return 0;
}

void adc_init()
{
    // DMA on ADC1 (master ADC for multi-mode)
    RCC->AHB1ENR |= RCC_DMA2EN;
    asm volatile ("dmb");
    DMA2_STR(0)->PAR = (unsigned)&ADC_COM->CDR;
    DMA2_STR(0)->M0AR = (unsigned)adc_data;
    DMA2_STR(0)->NDTR = ADC_DATA_WINDOW / 4;
    DMA2_STR(0)->CR = DMA_CHSEL(0) | DMA_MSIZE_32 | DMA_PSIZE_32 | DMA_MINC | 
        DMA_DIR_P2M | DMA_CIRC | DMA_EN;

    RCC->APB2ENR |= RCC_ADC1EN | RCC_ADC2EN;
    asm volatile ("dmb");
    
    ADC1->CR1 = ADC_RES_12BIT | ADC_OVRIE;
    ADC1->CR2 = ADC_CONT | ADC_ADON | ADC_EOCS;
    ADC1->SQR1 = ADC_L(1);
    ADC1->SQR3 = ADC_SQ1(ADC_CHANNEL_N);
    
    ADC2->CR1 = ADC_RES_12BIT | ADC_OVRIE;
    ADC2->CR2 = ADC_CONT | ADC_ADON | ADC_EOCS;
    ADC2->SQR1 = ADC_L(1);
    ADC2->SQR3 = ADC_SQ1(ADC_CHANNEL_N);

    // Analog watchdog on ADC1
    ADC1->HTR = WATCHDOG_THRESHOLD;
    ADC1->CR1 |= ADC_AWDEN | ADC_AWDIE | ADC_AWDCH(ADC_CHANNEL_N);
    
    // Multi-mode with DMA
    ADC_COM->CCR = ADC_ADCPRE_DIV2 | ADC_MULTI_DUAL_INTERLEAVED | ADC_DELAY(8) |
        ADC_DMA_MODE2 | ADC_COMMON_DDS;

    mutex_attach_irq(&adc_mutex, IRQ_ADC, adc_interrupt, 0);
}

void task (void *arg)
{
    ADC1->CR2 |= ADC_SWSTART;
    
	for (;;) {
	    debug_printf("DMA NDTR %05d, M0AR %08X, PAR %08X, CR %08X\n", DMA2_STR(0)->NDTR, DMA2_STR(0)->M0AR, DMA2_STR(0)->PAR, DMA2_STR(0)->CR);
	}
}

void uos_init (void)
{
	debug_puts ("\nTesting ADC interleaved\n");
	timer_init (&timer, KHZ, 10);
	
	stm32f4_gpio_init(&adc_pin, GPIO_PORT_A, 3, GPIO_FLAGS_ANALOG);
	
	adc_init();
	
	task_create (task, 0, "task", 1, stack, sizeof (stack));
}
