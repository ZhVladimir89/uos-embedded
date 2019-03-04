#include <runtime/lib.h>
#include <kernel/uos.h>
#include <kernel/internal.h>
#include <stm32f3/i2c.h>

// Требования спецификации для режима SM
#define SM_THDDAT_MIN               0
#define SM_TVDDAT_MAX               3450
#define SM_TSUDAT_MIN               250
#define SM_TR_MAX                   1000
#define SM_TF_MAX                   300

// Требования спецификации для режима SMBUS
#define SMBUS_THDDAT_MIN            300
#define SMBUS_TVDDAT_MAX            5000 // Не определено в спецификации!
#define SMBUS_TSUDAT_MIN            250
#define SMBUS_TR_MAX                1000
#define SMBUS_TF_MAX                300

// Требования спецификации для режима FM
#define FM_THDDAT_MIN               0
#define FM_TVDDAT_MAX               900
#define FM_TSUDAT_MIN               100
#define FM_TR_MAX                   300
#define FM_TF_MAX                   300

// Требования спецификации для режима FM+
#define FMP_THDDAT_MIN              0
#define FMP_TVDDAT_MAX              450
#define FMP_TSUDAT_MIN              50
#define FMP_TR_MAX                  120
#define FMP_TF_MAX                  120

// Расчёт констант, общих для всех режимов
#if KHZ >= 48000
#   define I2C_PRESCALER    16
#else
#   define I2C_PRESCALER    (KHZ / 4000)
#endif

#define T_I2CCLK                    (1000000.0 / KHZ)
#define DIG_FILT_STEPS_CALC         (I2C_DIGITAL_FILTER_WIDTH / T_I2CCLK)
#define DIG_FILT_STEPS              ((DIG_FILT_STEPS_CALC <= 15) ? (int)DIG_FILT_STEPS_CALC : 15)
#define T_SYNC1                     (int)(I2C_SCL_FALLING_SLOPE +\
                                    I2C_ANALOG_FILTER_ENABLED * (T_AF_MIN + T_AF_MAX) / 2 +\
                        	        (DIG_FILT_STEPS + 3) * T_I2CCLK)
#define T_SYNC2                     (int)(I2C_SCL_RISING_SLOPE +\
                                    I2C_ANALOG_FILTER_ENABLED * (T_AF_MIN + T_AF_MAX) / 2 +\
                        	        (DIG_FILT_STEPS + 3) * T_I2CCLK)
#define T_I2CCLK_PRESCALED          (int)(I2C_PRESCALER * T_I2CCLK)

// Расчёт констант для режима SM
#define SM_SDADEL_MIN_CALC          (SM_TF_MAX + SM_THDDAT_MIN - I2C_ANALOG_FILTER_ENABLED * T_AF_MIN -\
                        	        (DIG_FILT_STEPS + 3) * T_I2CCLK) / (I2C_PRESCALER * T_I2CCLK)
#define SM_SDADEL_MIN               (int)((SM_SDADEL_MIN_CALC <= 15) ? SM_SDADEL_MIN_CALC : 15)
#define SM_SDADEL_MAX_CALC          (SM_TVDDAT_MAX + SM_TR_MAX - I2C_ANALOG_FILTER_ENABLED * T_AF_MAX -\
                        	        (DIG_FILT_STEPS + 4) * T_I2CCLK) / (I2C_PRESCALER * T_I2CCLK)
#define SM_SDADEL_MAX               (int)((SM_SDADEL_MAX_CALC <= 15) ? SM_SDADEL_MAX_CALC : 15)
#define SM_SDADEL                   ((SM_SDADEL_MIN + SM_SDADEL_MAX) / 2)
#define SM_SCLDEL_MIN               (int)((SM_TR_MAX + SM_TSUDAT_MIN) / (I2C_PRESCALER * T_I2CCLK)) // не вычитаем 1, чтобы не было 
	                                                                                                // меньше минимального из-за округления

// Расчёт констант для режима SMBUS
#define SMBUS_SDADEL_MIN_CALC       (SMBUS_TF_MAX + SMBUS_THDDAT_MIN - I2C_ANALOG_FILTER_ENABLED * T_AF_MIN -\
                        	        (DIG_FILT_STEPS + 3) * T_I2CCLK) / (I2C_PRESCALER * T_I2CCLK)
#define SMBUS_SDADEL_MIN            (int)((SMBUS_SDADEL_MIN_CALC <= 15) ? SMBUS_SDADEL_MIN_CALC : 15)
#define SMBUS_SDADEL_MAX_CALC       (SMBUS_TVDDAT_MAX + SMBUS_TR_MAX - I2C_ANALOG_FILTER_ENABLED * T_AF_MAX -\
                        	        (DIG_FILT_STEPS + 4) * T_I2CCLK) / (I2C_PRESCALER * T_I2CCLK)
#define SMBUS_SDADEL_MAX            (int)((SMBUS_SDADEL_MAX_CALC <= 15) ? SMBUS_SDADEL_MAX_CALC : 15)
#define SMBUS_SDADEL                ((SMBUS_SDADEL_MIN + SMBUS_SDADEL_MAX) / 2)
#define SMBUS_SCLDEL_MIN            (int)((SMBUS_TR_MAX + SMBUS_TSUDAT_MIN) / (I2C_PRESCALER * T_I2CCLK)) // не вычитаем 1, чтобы не было 
	                                                                                                // меньше минимального из-за округления

// Расчёт констант для режима FM
#define FM_SDADEL_MIN_CALC          (FM_TF_MAX + FM_THDDAT_MIN - I2C_ANALOG_FILTER_ENABLED * T_AF_MIN -\
                        	        (DIG_FILT_STEPS + 3) * T_I2CCLK) / (I2C_PRESCALER * T_I2CCLK)
#define FM_SDADEL_MIN               (int)((FM_SDADEL_MIN_CALC <= 15) ? FM_SDADEL_MIN_CALC : 15)
#define FM_SDADEL_MAX_CALC          (FM_TVDDAT_MAX + FM_TR_MAX - I2C_ANALOG_FILTER_ENABLED * T_AF_MAX -\
                        	        (DIG_FILT_STEPS + 4) * T_I2CCLK) / (I2C_PRESCALER * T_I2CCLK)
#define FM_SDADEL_MAX               (int)((FM_SDADEL_MAX_CALC <= 15) ? FM_SDADEL_MAX_CALC : 15)
#define FM_SDADEL                   ((FM_SDADEL_MIN + FM_SDADEL_MAX) / 2)
#define FM_SCLDEL_MIN               (int)((FM_TR_MAX + FM_TSUDAT_MIN) / (I2C_PRESCALER * T_I2CCLK)) // не вычитаем 1, чтобы не было 
	                                                                                                // меньше минимального из-за округления

// Расчёт констант для режима FM+
#define FMP_SDADEL_MIN_CALC         (FMP_TF_MAX + FMP_THDDAT_MIN - I2C_ANALOG_FILTER_ENABLED * T_AF_MIN -\
                        	        (DIG_FILT_STEPS + 3) * T_I2CCLK) / (I2C_PRESCALER * T_I2CCLK)
#define FMP_SDADEL_MIN              (int)((FMP_SDADEL_MIN_CALC <= 15) ? FMP_SDADEL_MIN_CALC : 15)
#define FMP_SDADEL_MAX_CALC         (FMP_TVDDAT_MAX + FMP_TR_MAX - I2C_ANALOG_FILTER_ENABLED * T_AF_MAX -\
                        	        (DIG_FILT_STEPS + 4) * T_I2CCLK) / (I2C_PRESCALER * T_I2CCLK)
#define FMP_SDADEL_MAX              (int)((FMP_SDADEL_MAX_CALC <= 15) ? FMP_SDADEL_MAX_CALC : 15)
#define FMP_SDADEL                  ((FMP_SDADEL_MIN + FMP_SDADEL_MAX) / 2)
#define FMP_SCLDEL_MIN              (int)((FMP_TR_MAX + FMP_TSUDAT_MIN) / (I2C_PRESCALER * T_I2CCLK)) // не вычитаем 1, чтобы не было 
	                                                                                                // меньше минимального из-за округления
	                                                                                                


static inline int set_timings(I2C_t *reg, unsigned mode, unsigned khz)
{
    const int scll = 1000000 / khz / 2 / T_I2CCLK_PRESCALED - 1;
    const int sclh = (1000000 / khz / 2 - T_SYNC1 - T_SYNC2) / T_I2CCLK_PRESCALED - 1;
    const unsigned CR1_IE_MASK = I2C_ERRIE | I2C_TCIE | I2C_NACKIE | I2C_RXIE | I2C_TXIE | I2C_STOPIE;
    
    if ((scll > 255) || (scll < 0) || (sclh > 255) || (sclh < 0))
        return I2C_ERR_BAD_FREQ;
	
    switch (mode) {
    case I2C_MODE_SM:
        reg->TIMINGR = I2C_PRESC(I2C_PRESCALER - 1) |
            I2C_SDADEL(SM_SDADEL) | I2C_SCLDEL(SM_SCLDEL_MIN) |
            I2C_SCLH(sclh) | I2C_SCLL(scll);
        break;
    case I2C_MODE_SMBUS:
        reg->TIMINGR = I2C_PRESC(I2C_PRESCALER - 1) |
            I2C_SDADEL(SMBUS_SDADEL) | I2C_SCLDEL(SMBUS_SCLDEL_MIN) |
            I2C_SCLH(sclh) | I2C_SCLL(scll);
        break;
    case I2C_MODE_FM:
        reg->TIMINGR = I2C_PRESC(I2C_PRESCALER - 1) |
            I2C_SDADEL(FM_SDADEL) | I2C_SCLDEL(FM_SCLDEL_MIN) |
            I2C_SCLH(sclh) | I2C_SCLL(scll);
        break;
    case I2C_MODE_FM_PLUS:
        reg->TIMINGR = I2C_PRESC(I2C_PRESCALER - 1) |
            I2C_SDADEL(FMP_SDADEL) | I2C_SCLDEL(FMP_SCLDEL_MIN) |
            I2C_SCLH(sclh) | I2C_SCLL(scll);
        break;
    default:
        return I2C_ERR_MODE_NOT_SUPP;
    }
	
    reg->CR1 = I2C_DNF(DIG_FILT_STEPS) | CR1_IE_MASK
#if !I2C_ANALOG_FILTER_ENABLED
        | I2C_ANFOFF
#endif
        | I2C_PE;
	
	return I2C_ERR_OK;
}

static int start_trx(stm32f3_i2c_t *stm32f3_i2c)
{
    while (stm32f3_i2c->cur_trans->size == 0) {
        stm32f3_i2c->cur_trans = stm32f3_i2c->cur_trans->next;
        if (! stm32f3_i2c->cur_trans)
            return I2C_ERR_BAD_PARAM;
    }

    uint32_t cr2 = I2C_SADD(I2C_MODE_GET_SLAVE_ADDR(stm32f3_i2c->cur_mode)) |
        ((stm32f3_i2c->cur_mode & I2C_MODE_10BIT_ADDR) ? I2C_ADD10 : 0) | I2C_START;
    stm32f3_i2c->trx_bytes = stm32f3_i2c->cur_trans->data;
        
    if (stm32f3_i2c->cur_trans->size > 0) {
        stm32f3_i2c->trx_size = stm32f3_i2c->cur_trans->size;
    } else if (stm32f3_i2c->cur_trans->size < 0) {
        stm32f3_i2c->trx_size = -stm32f3_i2c->cur_trans->size;
        cr2 |= I2C_RD_WRN;
    }
    if (stm32f3_i2c->trx_size > 255)
        return I2C_ERR_TOO_LONG;
    cr2 |= I2C_NBYTES(stm32f3_i2c->trx_size);
    stm32f3_i2c->reg->CR2 = cr2;
    
    return I2C_ERR_OK;
}

static int stm32f3_i2c_trx(i2cif_t *i2c, i2c_message_t *msg)
{
    stm32f3_i2c_t *stm32f3_i2c = (stm32f3_i2c_t *) i2c;
    I2C_t *ireg = stm32f3_i2c->reg;
    int res = I2C_ERR_OK;
    
    if (! msg->first)
        return I2C_ERR_BAD_PARAM;
    
    mutex_lock(&i2c->lock);
	
	res = set_timings(ireg, msg->mode & I2C_MODE_MASK, I2C_MODE_GET_FREQ_KHZ(msg->mode));
	if (res != I2C_ERR_OK)
	    goto trx_exit;

	stm32f3_i2c->cur_mode = msg->mode;
	stm32f3_i2c->cur_trans = msg->first;
	res = start_trx(stm32f3_i2c);
	if (res != I2C_ERR_OK)
	    goto trx_exit;

	res = (int)mutex_wait(&i2c->lock);
	
trx_exit:
	ireg->CR1 = 0;
  	mutex_unlock(&i2c->lock);
    return res;
}

static bool_t trx_handler(void *arg)
{
    stm32f3_i2c_t *stm32f3_i2c = arg;
    I2C_t *ireg = stm32f3_i2c->reg;
    const uint32_t isr = ireg->ISR;

    if (isr & I2C_TXIS) {
        ireg->TXDR = *stm32f3_i2c->trx_bytes++;
        --stm32f3_i2c->trx_size;
    } else if (isr & I2C_RXNE) {
        *stm32f3_i2c->trx_bytes++ = ireg->RXDR;
        --stm32f3_i2c->trx_size;
    } else if (isr & I2C_TC) {
        const int is_last_transaction = (stm32f3_i2c->cur_trans->next == 0);
        if (is_last_transaction) {
            ireg->CR2 |= I2C_STOP;
        } else {
            stm32f3_i2c->cur_trans = stm32f3_i2c->cur_trans->next;
            int res = start_trx(stm32f3_i2c);
            if (res != I2C_ERR_OK) {
                ireg->CR2 |= I2C_STOP;
                mutex_activate(&stm32f3_i2c->i2cif.lock, (void *) res);
            }
        }
    } else if (isr & I2C_STOPF) {
        mutex_activate(&stm32f3_i2c->i2cif.lock, (void *) I2C_ERR_OK);
        ireg->ICR = I2C_STOPCF;
    } else if (isr & I2C_NACKF) {
        mutex_activate(&stm32f3_i2c->i2cif.lock, (void *) I2C_ERR_NOT_ACKED);
        ireg->ICR = I2C_NACKCF;
    }
    
    arch_intr_allow(stm32f3_i2c->trx_irq);
    return 0;
}

static bool_t error_handler(void *arg)
{
    stm32f3_i2c_t *stm32f3_i2c = arg;
    I2C_t *ireg = stm32f3_i2c->reg;
    const uint32_t isr = ireg->ISR;
    int res = I2C_ERR_OK;
    
    if (isr & I2C_ARLO) {
        ireg->ICR = I2C_ARLO;
        res = I2C_ERR_ARBITR_LOST;
	} else if (isr & I2C_BERR) {
	    ireg->ICR = I2C_BERR;
	    res = I2C_ERR_BUS;
	}
	mutex_activate(&stm32f3_i2c->i2cif.lock, (void *) res);
	    
    arch_intr_allow(stm32f3_i2c->trx_irq + 1);
    return 0;
}

int stm32f3_i2c_init (stm32f3_i2c_t *i2c, int port)
{
    i2cif_t *i2cif = to_i2cif(i2c);
    i2cif->trx = stm32f3_i2c_trx;
    
    if (port == 1) {
        i2c->reg = I2C1;
        RCC->CFGR3 |= RCC_I2C1SW_SYSCLK;
        RCC->APB1ENR |= RCC_I2C1EN;
        i2c->trx_irq = IRQ_I2C1_EV;
    } else if (port == 2) {
        i2c->reg = I2C2;
        RCC->CFGR3 |= RCC_I2C2SW_SYSCLK;
        RCC->APB1ENR |= RCC_I2C2EN;
        i2c->trx_irq = IRQ_I2C2_EV;
    } else if (port == 3) {
        i2c->reg = I2C3;
        RCC->CFGR3 |= RCC_I2C3SW_SYSCLK;
        RCC->APB1ENR |= RCC_I2C3EN;
        i2c->trx_irq = IRQ_I2C3_EV;
    } else {
        return I2C_ERR_BAD_PORT;
    }

    mutex_attach_irq(&i2c->trx_mutex, i2c->trx_irq, trx_handler, i2c);
    mutex_attach_irq(&i2c->error_mutex, i2c->trx_irq + 1, error_handler, i2c);
    
    i2c->reg->CR1 = 0;
        
    return I2C_ERR_OK;
}

