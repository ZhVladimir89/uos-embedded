#include <runtime/lib.h>
#include <kernel/uos.h>
#include <gpio-expander/pca9698.h>

#define BIT_AUTO_INCREMENT      0x80
#define REG_IP0                 0x00
#define REG_OP0                 0x08
#define REG_PI0                 0x10
#define REG_IOC0                0x18
#define REG_MSK0                0x20
#define REG_OUTCONF             0x28

int pca9698_apply_configuration(pca9698_t *pca9698)
{
    i2c_transaction_t trans;
    uint8_t buf[6];
    int res;
    
    mutex_lock(&pca9698->lock);
    
    pca9698->msg.first = &trans;
    trans.data = buf;
    trans.next = 0;
    
    buf[0] = REG_OUTCONF;
    buf[1] = pca9698->outconf;
    trans.size = I2C_TX_SIZE(2);
    
    res = i2c_trx(pca9698->i2c, &pca9698->msg);
    if (res != I2C_ERR_OK)
        goto apply_conf_exit;

    buf[0] = BIT_AUTO_INCREMENT | REG_PI0;
    memcpy(buf + 1, pca9698->pi, sizeof(pca9698->pi));
    trans.size = I2C_TX_SIZE(6);
    
    res = i2c_trx(pca9698->i2c, &pca9698->msg);
    if (res != I2C_ERR_OK)
         goto apply_conf_exit;

    buf[0] = BIT_AUTO_INCREMENT | REG_IOC0;
    memcpy(buf + 1, pca9698->ioc, sizeof(pca9698->ioc));
    trans.size = I2C_TX_SIZE(6);
    
    res = i2c_trx(pca9698->i2c, &pca9698->msg);
    if (res != I2C_ERR_OK)
        goto apply_conf_exit;

    buf[0] = BIT_AUTO_INCREMENT | REG_MSK0;
    memcpy(buf + 1, pca9698->msk, sizeof(pca9698->msk));
    trans.size = I2C_TX_SIZE(6);
    
    res = i2c_trx(pca9698->i2c, &pca9698->msg);
    if (res != I2C_ERR_OK)
        goto apply_conf_exit;
    
apply_conf_exit:
    mutex_unlock(&pca9698->lock);
    return (res == I2C_ERR_OK);
}

int pca9698_update_inputs(pca9698_t *pca9698)
{
    i2c_transaction_t trans1;
    i2c_transaction_t trans2;
    uint8_t cmd = BIT_AUTO_INCREMENT | REG_IP0;
    int res;
    
    mutex_lock(&pca9698->lock);
    
    pca9698->msg.first = &trans1;
    trans1.data = &cmd;
    trans1.size = I2C_TX_SIZE(1);
    trans1.next = &trans2;
    
    trans2.data = pca9698->ip;
    trans2.size = I2C_RX_SIZE(5);
    trans2.next = 0;
    
    res = i2c_trx(pca9698->i2c, &pca9698->msg);

    mutex_unlock(&pca9698->lock);
    
    return (res == I2C_ERR_OK);
}

int pca9698_update_outputs(pca9698_t *pca9698)
{
    i2c_transaction_t trans;
    uint8_t buf[6];
    int res;
    
    mutex_lock(&pca9698->lock);
    
    pca9698->msg.first = &trans;
    trans.data = buf;
    trans.next = 0;
    
    buf[0] = BIT_AUTO_INCREMENT | REG_OP0;
    memcpy(buf + 1, pca9698->op, sizeof(pca9698->op));
    trans.size = I2C_TX_SIZE(6);
    
    res = i2c_trx(pca9698->i2c, &pca9698->msg);
    
    mutex_unlock(&pca9698->lock);
    
    return (res == I2C_ERR_OK);
}

#if PCA9698_USE_INTERRUPT
static void interrupt_task(void *arg)
{
    pca9698_t *pca9698 = (pca9698_t *)arg;
    
    gpio_attach_interrupt(pca9698->int_pin, GPIO_EVENT_RISING_EDGE | GPIO_EVENT_FALLING_EDGE, 0, 0);
    
    mutex_lock(gpio_get_mutex(pca9698->int_pin));
    
    for (;;) {
        gpio_wait_irq(pca9698->int_pin);
        mutex_unlock(gpio_get_mutex(pca9698->int_pin));
   
        memcpy(pca9698->prev_ip, pca9698->ip, sizeof(pca9698->ip));
        
        pca9698_update_inputs(pca9698);
        
        int i, j;
        for (i = 0; i < sizeof(pca9698->ip); ++i) {
            if (pca9698->ip[i] != pca9698->prev_ip[i]) {
                for (j = 0; j < 8; ++j) {
                    if ((pca9698->ip[i] ^ pca9698->prev_ip[i]) & ~pca9698->msk[i] & (1 << j)) {
                        pca9698_gpio_t *pin = pca9698->pins[i * 8 + j];
                        if (pin && pin->handler)
                            pin->handler(to_gpioif(pin), pin->handler_arg);
                    }
                }
            }
        }
        
        mutex_lock(gpio_get_mutex(pca9698->int_pin));
    }
}
#endif

void pca9698_init(pca9698_t *pca9698, i2cif_t *i2c, uint8_t i2c_addr, unsigned i2c_timings, 
    unsigned i2c_freq_khz, gpioif_t *interrupt_pin, int interrupt_prio)
{
    pca9698->i2c = i2c;
    pca9698->msg.mode = I2C_MODE_SLAVE_ADDR(i2c_addr) | I2C_MODE_7BIT_ADDR | 
        i2c_timings | I2C_MODE_FREQ_KHZ(i2c_freq_khz);
        
    memset(pca9698->ioc, 0xFF, sizeof(pca9698->ioc));
    memset(pca9698->msk, 0xFF, sizeof(pca9698->msk));
    pca9698->outconf = 0xFF;

#if PCA9698_USE_INTERRUPT
    pca9698->int_pin = interrupt_pin;
    task_create(interrupt_task, pca9698, "pca9698_int", interrupt_prio, pca9698->int_stack, sizeof(pca9698->int_stack));
#endif
}


static void pin_to_input(gpioif_t *pin)
{
    pca9698_gpio_t *pca_pin = (pca9698_gpio_t *)pin;
    mutex_lock(&pca_pin->owner->lock);
    pca_pin->owner->ioc[pca_pin->port] |= (1 << pca_pin->pin);
    mutex_unlock(&pca_pin->owner->lock);
}
    
static void pin_to_output(gpioif_t *pin)
{
    pca9698_gpio_t *pca_pin = (pca9698_gpio_t *)pin;
    mutex_lock(&pca_pin->owner->lock);
    pca_pin->owner->ioc[pca_pin->port] &= ~(1 << pca_pin->pin);
    mutex_unlock(&pca_pin->owner->lock);
}
    
static void pin_set_value(gpioif_t *pin, int v)
{
    pca9698_gpio_t *pca_pin = (pca9698_gpio_t *)pin;
    mutex_lock(&pca_pin->owner->lock);
    if (v)
        pca_pin->owner->op[pca_pin->port] |= (1 << pca_pin->pin);
    else
        pca_pin->owner->op[pca_pin->port] &= ~(1 << pca_pin->pin);
    mutex_unlock(&pca_pin->owner->lock);
}
    
static int pin_value(gpioif_t *pin)
{
    pca9698_gpio_t *pca_pin = (pca9698_gpio_t *)pin;
    return (pca_pin->owner->ip[pca_pin->port] >> pca_pin->pin) & 1;
}

static void pin_attach_interrupt(gpioif_t *pin, unsigned event, gpio_handler_t handler, void *arg)
{
#if PCA9698_USE_INTERRUPT
    pca9698_gpio_t *pca_pin = (pca9698_gpio_t *)pin;
    pca_pin->handler = handler;
    pca_pin->handler_arg = arg;
    
    mutex_lock(&pca_pin->owner->lock);
    pca_pin->owner->pins[pca_pin->port * 8 + pca_pin->pin] = pca_pin;
    pca_pin->owner->msk[pca_pin->port] &= ~(1 << pca_pin->pin);
    mutex_unlock(&pca_pin->owner->lock);
#endif
}

static void pin_detach_interrupt(gpioif_t *pin)
{
#if PCA9698_USE_INTERRUPT
    pca9698_gpio_t *pca_pin = (pca9698_gpio_t *)pin;
    
    mutex_lock(&pca_pin->owner->lock);
    pca_pin->owner->pins[pca_pin->port * 8 + pca_pin->pin] = 0;
    pca_pin->owner->msk[pca_pin->port] |= (1 << pca_pin->pin);
    mutex_unlock(&pca_pin->owner->lock);
#endif
}

static int pin_interrupt_pending(gpioif_t *pin)
{
#if PCA9698_USE_INTERRUPT
    pca9698_gpio_t *pca_pin = (pca9698_gpio_t *)pin;
    return (pca_pin->owner->ip[pca_pin->port] ^ pca_pin->owner->prev_ip[pca_pin->port]) & (1 << pca_pin->pin);
#else
    return 0;
#endif
}

static void pin_clear_interrupt(gpioif_t *pin)
{
#if PCA9698_USE_INTERRUPT
    pca9698_gpio_t *pca_pin = (pca9698_gpio_t *)pin;
    mutex_lock(&pca_pin->owner->lock);
    if (pca_pin->owner->ip[pca_pin->port] & (1 << pca_pin->pin))
        pca_pin->owner->prev_ip[pca_pin->port] |= (1 << pca_pin->pin);
    else
        pca_pin->owner->prev_ip[pca_pin->port] &= ~(1 << pca_pin->pin);
    mutex_unlock(&pca_pin->owner->lock);
#endif
}

static mutex_t *pin_get_mutex(gpioif_t *pin)
{
    pca9698_gpio_t *pca_pin = (pca9698_gpio_t *)pin;
    return &pca_pin->owner->lock;
}


void pca9698_gpio_init(pca9698_gpio_t *gpio, pca9698_t *pca9698, unsigned port, unsigned pin, unsigned flags)
{
    gpio->owner = pca9698;
    gpio->port = port;
    gpio->pin = pin;
    
    if (flags & PCA9698_GPIO_FLAGS_INPUT) {
        pca9698->ioc[port] |= (1 << pin);
        if (flags & PCA9698_GPIO_FLAGS_INV_INPUT)
            pca9698->pi[port] |= (1 << pin);
    } else {
        pca9698->ioc[port] &= ~(1 << pin);
        if (flags & PCA9698_GPIO_FLAGS_OPEN_DRAIN) {
            if (port == 0)
                pca9698->outconf |= (1 << (pin >> 1));
            else
                pca9698->outconf |= (0x08 << port);
        }
    }
    
    gpioif_t *gpioif = to_gpioif(gpio);
    gpioif->to_input = pin_to_input;
    gpioif->to_output = pin_to_output;
    gpioif->set_value = pin_set_value;
    gpioif->value = pin_value;
    gpioif->attach_interrupt = pin_attach_interrupt;
    gpioif->detach_interrupt = pin_detach_interrupt;
    gpioif->interrupt_pending = pin_interrupt_pending;
    gpioif->clear_interrupt = pin_clear_interrupt;
    gpioif->get_mutex = pin_get_mutex;
}

