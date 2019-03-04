#include <runtime/lib.h>
#include <kernel/uos.h>
#include <kernel/internal.h>
#include <milandr_new/gpio.h>

enum {
    HANDLER_EXTI1,
    HANDLER_EXTI2,
    HANDLER_EXTI3,
    HANDLER_EXTI4,
    HANDLER_MAX_NB
};

#define EXTI_HNDL_INIT  (1 << 30)

static int handler_state;
static list_t exti_hndl[HANDLER_MAX_NB];
static mutex_t exti_mutex[HANDLER_MAX_NB];
static const int irq_n[HANDLER_MAX_NB] = {
    EXT_INT1_IRQn, EXT_INT2_IRQn, EXT_INT3_IRQn, EXT_INT4_IRQn
};

static void milandr_to_input(gpioif_t *pin)
{
    milandr_gpio_t *mil_pin = (milandr_gpio_t *)pin;
    mil_pin->reg->OE &= ~(1 << mil_pin->pin_n);
}
    
static void milandr_to_output(gpioif_t *pin)
{
    milandr_gpio_t *mil_pin = (milandr_gpio_t *)pin;
    mil_pin->reg->OE |= (1 << mil_pin->pin_n);
}
    
static void milandr_set_value(gpioif_t *pin, int v)
{
    milandr_gpio_t *mil_pin = (milandr_gpio_t *)pin;
    
    if (v)
        mil_pin->reg->DATA |= (1 << mil_pin->pin_n);
    else
        mil_pin->reg->DATA &= ~(1 << mil_pin->pin_n);
}
    
static int milandr_value(gpioif_t *pin)
{
    milandr_gpio_t *mil_pin = (milandr_gpio_t *)pin;
    
    return (mil_pin->reg->DATA >> mil_pin->pin_n) & 1;
}

static bool_t milandr_gpio_irq_handler(void *arg)
{
    int handler_idx = (int)arg;
    milandr_gpio_hndl_list_item_t *item;
    
    list_iterate(item, &exti_hndl[handler_idx]) {
        item->pin->int_pending = 1;
        if (item->handler)
            item->handler(&item->pin->gpioif, item->handler_arg);
    }
    
    arch_intr_allow(irq_n[handler_idx]);
    
    return 0;
}
    
static void milandr_attach_interrupt(gpioif_t *pin, unsigned event, 
        gpio_handler_t handler, void *arg)
{
    milandr_gpio_t *mil_pin = (milandr_gpio_t *)pin;
    int handler_idx = HANDLER_MAX_NB;
    
    switch (mil_pin->port) {
    case GPIO_PORT_A:
        if (mil_pin->pin_n == 0)
            handler_idx = HANDLER_EXTI1;
        else
            return;
        break;
    case GPIO_PORT_B:
        if (mil_pin->pin_n == 9)
            handler_idx = HANDLER_EXTI4;
        else if (mil_pin->pin_n == 10)
            handler_idx = HANDLER_EXTI2;
        else if (mil_pin->pin_n == 11)
            handler_idx = HANDLER_EXTI1;
        else
            return;
        break;
    case GPIO_PORT_C:
        if (mil_pin->pin_n == 12)
            handler_idx = HANDLER_EXTI2;
        else if (mil_pin->pin_n == 13)
            handler_idx = HANDLER_EXTI4;
        else
            return;
        break;
    case GPIO_PORT_E:
        if (mil_pin->pin_n == 15)
            handler_idx = HANDLER_EXTI3;
        else
            return;
        break;
    default:
        return;
    }
    
    assert(handler_idx < HANDLER_MAX_NB);
    
    list_init(&mil_pin->hndl_item.item);
    mil_pin->hndl_item.pin = mil_pin;
    mil_pin->hndl_item.handler = handler;
    mil_pin->hndl_item.handler_arg = arg;
    mil_pin->irq_handler_idx = handler_idx;
        
    arch_state_t x;
    arch_intr_disable(&x);
    
    list_append(&exti_hndl[handler_idx], &mil_pin->hndl_item.item);
    
    if (! (handler_state & (1 << handler_idx))) {
        mutex_attach_irq(&exti_mutex[handler_idx], irq_n[handler_idx], 
            milandr_gpio_irq_handler, (void *)handler_idx);
        handler_state |= (1 << handler_idx);
    }
    
    arch_intr_restore(x);
}

static void milandr_detach_interrupt(gpioif_t *pin)
{
    milandr_gpio_t *mil_pin = (milandr_gpio_t *)pin;
    
    arch_state_t x;
    arch_intr_disable(&x);
    
    list_unlink(&mil_pin->hndl_item.item);
    
    if (list_is_empty(&exti_hndl[mil_pin->irq_handler_idx])) {
        mutex_lock(&exti_mutex[mil_pin->irq_handler_idx]);
        mutex_unlock_irq(&exti_mutex[mil_pin->irq_handler_idx]);
    }
    
    arch_intr_restore(x);
}

static int milandr_interrupt_pending(gpioif_t *pin)
{
    milandr_gpio_t *mil_pin = (milandr_gpio_t *)pin;
    return mil_pin->int_pending;
}

static void milandr_clear_interrupt(gpioif_t *pin)
{
    milandr_gpio_t *mil_pin = (milandr_gpio_t *)pin;
    mil_pin->int_pending = 0;
}

static mutex_t *milandr_get_mutex(gpioif_t *pin)
{
    milandr_gpio_t *mil_pin = (milandr_gpio_t *)pin;
    return &exti_mutex[mil_pin->irq_handler_idx];
}

void milandr_gpio_init(milandr_gpio_t *gpio, unsigned port, unsigned pin_n, 
        unsigned flags)
{
    gpio->pin_n  = pin_n;
    gpio->port   = port;
    
    switch (port) {
    case GPIO_PORT_A:
        ARM_RSTCLK->PER_CLOCK |= ARM_PER_CLOCK_GPIOA;
        gpio->reg = ARM_GPIOA;
        break;
    case GPIO_PORT_B:
        ARM_RSTCLK->PER_CLOCK |= ARM_PER_CLOCK_GPIOB;
        gpio->reg = ARM_GPIOB;
        break;
    case GPIO_PORT_C:
        ARM_RSTCLK->PER_CLOCK |= ARM_PER_CLOCK_GPIOC;
        gpio->reg = ARM_GPIOC;
        break;
    case GPIO_PORT_D:
        ARM_RSTCLK->PER_CLOCK |= ARM_PER_CLOCK_GPIOD;
        gpio->reg = ARM_GPIOD;
        break;
    case GPIO_PORT_E:
        ARM_RSTCLK->PER_CLOCK |= ARM_PER_CLOCK_GPIOE;
        gpio->reg = ARM_GPIOE;
        break;
    case GPIO_PORT_F:
        ARM_RSTCLK->PER_CLOCK |= ARM_PER_CLOCK_GPIOF;
        gpio->reg = ARM_GPIOF;
        break;
    default:
        gpio->reg = 0;
    }
    
    switch (flags & _IO_MODE_MASK) {
    case GPIO_FLAGS_FUNC_PORT:
        gpio->reg->ANALOG |= ARM_DIGITAL(pin_n);
        gpio->reg->FUNC = (gpio->reg->FUNC & ~ARM_FUNC_MASK(pin_n)) | ARM_FUNC_PORT(pin_n);
        if (flags & GPIO_FLAGS_OUTPUT)
            gpio->reg->OE |= ARM_GPIO_OUT(pin_n);
        else
            gpio->reg->OE &= ~ARM_GPIO_OUT(pin_n);
        break;
    case GPIO_FLAGS_FUNC_MAIN:
        gpio->reg->ANALOG |= ARM_DIGITAL(pin_n);
        gpio->reg->FUNC = (gpio->reg->FUNC & ~ARM_FUNC_MASK(pin_n)) | ARM_FUNC_MAIN(pin_n);
        break;
    case GPIO_FLAGS_FUNC_ALT:
        gpio->reg->ANALOG |= ARM_DIGITAL(pin_n);
        gpio->reg->FUNC = (gpio->reg->FUNC & ~ARM_FUNC_MASK(pin_n)) | ARM_FUNC_ALT(pin_n);
        break;
    case GPIO_FLAGS_FUNC_REDEF:
        gpio->reg->ANALOG |= ARM_DIGITAL(pin_n);
        gpio->reg->FUNC = (gpio->reg->FUNC & ~ARM_FUNC_MASK(pin_n)) | ARM_FUNC_REDEF(pin_n);
        break;
    case GPIO_FLAGS_FUNC_ANALOG:
        gpio->reg->ANALOG &= ~ARM_DIGITAL(pin_n);
        break;
    }
        
    if (flags & GPIO_FLAGS_PULL_UP)
        gpio->reg->PULL |= ARM_PULL_UP(pin_n);
    else
        gpio->reg->PULL &= ~ARM_PULL_UP(pin_n);
        
    if (flags & GPIO_FLAGS_PULL_DOWN)
        gpio->reg->PULL |= ARM_PULL_DOWN(pin_n);
    else
        gpio->reg->PULL &= ~ARM_PULL_DOWN(pin_n);
        
    if (flags & GPIO_FLAGS_OPEN_DRAIN)
        gpio->reg->PD |= ARM_PD_OD(pin_n);
    else
        gpio->reg->PD &= ~ARM_PD_OD(pin_n);

    if (flags & GPIO_FLAGS_SCHMITT)
        gpio->reg->PD |= ARM_PD_SCHMITT(pin_n);
    else
        gpio->reg->PD &= ~ARM_PD_SCHMITT(pin_n);

    if (flags & GPIO_FLAGS_IN_FILTER)
        gpio->reg->PD |= ARM_GFEN(pin_n);
    else
        gpio->reg->PD &= ~ARM_GFEN(pin_n);
       
    gpio->reg->PWR = (gpio->reg->PWR & ~ARM_PWR_MASK(pin_n)) | (_GET_POWER(flags) << (2 * pin_n));
       
    gpioif_t *gpioif = &gpio->gpioif;
    
    gpioif->to_input = milandr_to_input;
    gpioif->to_output = milandr_to_output;
    gpioif->set_value = milandr_set_value;
    gpioif->value = milandr_value;
    gpioif->attach_interrupt = milandr_attach_interrupt;
    gpioif->detach_interrupt = milandr_detach_interrupt;
    gpioif->interrupt_pending = milandr_interrupt_pending;
    gpioif->clear_interrupt = milandr_clear_interrupt;
    gpioif->get_mutex = milandr_get_mutex;
    
    if (! (handler_state & EXTI_HNDL_INIT)) {
        int i;
        for (i = 0; i < HANDLER_MAX_NB; ++i)
            list_init(&exti_hndl[i]);
        handler_state |= EXTI_HNDL_INIT;
    }
}
