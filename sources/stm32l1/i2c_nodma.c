#include <runtime/lib.h>
#include <kernel/uos.h>
#include <kernel/internal.h>
#include <stm32l1/i2c_nodma.h>

#define INTERRUPT_MASK  (I2C_ITEVTEN | I2C_ITERREN)

static inline int calc_ccr_sm(unsigned i2c_khz)
{
    return KHZ_PCLK1 / i2c_khz / 3;
}

static inline int calc_ccr_fm(unsigned i2c_khz)
{
    return KHZ_PCLK1 / i2c_khz / 25;
}

static inline int set_timings(I2C_t *reg, unsigned mode, unsigned khz)
{
	switch (mode) {
	case I2C_MODE_SM:
	case I2C_MODE_SMBUS:
	    if (khz > 100)
    	    return I2C_ERR_BAD_FREQ;
    	reg->CCR = I2C_CCR(calc_ccr_sm(khz));
    	reg->TRISE = KHZ_PCLK1 / 1000 + 1;
	    break;
	case I2C_MODE_FM:
	    if (khz > 100)
	        reg->CCR = I2C_CCR(calc_ccr_sm(khz)) | I2C_DUTY | I2C_FS;
	    else
	        reg->CCR = I2C_CCR(calc_ccr_sm(khz)) | I2C_FS;
    	reg->TRISE = 300 * KHZ_PCLK1 / 1000000 + 1;
	    break;
	case I2C_MODE_FM_PLUS:
        return I2C_ERR_MODE_NOT_SUPP;
	default:
	    return I2C_ERR_MODE_NOT_SUPP;
	}
	
	return I2C_ERR_OK;
}

static int start_trx(stm32l1_i2c_nodma_t *stm32l1_i2c)
{
    while (stm32l1_i2c->cur_trans->size == 0) {
        stm32l1_i2c->cur_trans = stm32l1_i2c->cur_trans->next;
        if (! stm32l1_i2c->cur_trans)
            return I2C_ERR_BAD_PARAM;
    }
    
    stm32l1_i2c->trx_bytes = stm32l1_i2c->cur_trans->data;

    if (stm32l1_i2c->cur_trans->size > 0)
        stm32l1_i2c->trx_size = stm32l1_i2c->cur_trans->size;
    else if (stm32l1_i2c->cur_trans->size < 0)
        stm32l1_i2c->trx_size = -stm32l1_i2c->cur_trans->size;
    
    stm32l1_i2c->reg->CR1 |= I2C_START;

/*
debug_printf("CR1   %08X\n", stm32l1_i2c->reg->CR1);
debug_printf("CR2   %08X\n", stm32l1_i2c->reg->CR2);
debug_printf("SR1   %08X\n", stm32l1_i2c->reg->SR1);
debug_printf("SR2   %08X\n", stm32l1_i2c->reg->SR2);
debug_printf("CCR   %08X\n", stm32l1_i2c->reg->CCR);
debug_printf("TRISE %08X\n", stm32l1_i2c->reg->TRISE);
*/

    return I2C_ERR_OK;
}

static int stm32l1_i2c_trx(i2cif_t *i2c, i2c_message_t *msg)
{
    stm32l1_i2c_nodma_t *stm32l1_i2c = (stm32l1_i2c_nodma_t *) i2c;
    I2C_t *ireg = stm32l1_i2c->reg;
    int res = I2C_ERR_OK;
    
    if (! msg->first)
        return I2C_ERR_BAD_PARAM;
    
    mutex_lock(&i2c->lock);
    while (stm32l1_i2c->busy)
        mutex_wait(&i2c->lock);
    stm32l1_i2c->busy = 1;
	
	res = set_timings(ireg, msg->mode & I2C_MODE_MASK, I2C_MODE_GET_FREQ_KHZ(msg->mode));
	if (res != I2C_ERR_OK)
	    goto trx_exit;

	stm32l1_i2c->cur_mode = msg->mode;
	stm32l1_i2c->cur_trans = msg->first;
	ireg->CR1 = I2C_PE;
	res = start_trx(stm32l1_i2c);
	if (res != I2C_ERR_OK)
	    goto trx_exit;
	    
	res = (int)mutex_wait(&i2c->lock);
	
trx_exit:
	ireg->CR1 = 0;
	stm32l1_i2c->busy = 0;
  	mutex_unlock(&i2c->lock);
    return res;
}

static void process_last_byte(stm32l1_i2c_nodma_t *stm32l1_i2c)
{
    I2C_t *ireg = stm32l1_i2c->reg;
    const int is_last_transaction = (stm32l1_i2c->cur_trans->next == 0);
//debug_printf("is_last_transaction %d\n", is_last_transaction);

    while (! (ireg->SR1 & I2C_BTF));

    if (is_last_transaction) {
        ireg->CR1 |= I2C_STOP;
    } else {
        stm32l1_i2c->cur_trans = stm32l1_i2c->cur_trans->next;
        int res = start_trx(stm32l1_i2c);
        if (res != I2C_ERR_OK) {
            ireg->CR1 |= I2C_STOP;
            mutex_activate(&stm32l1_i2c->i2cif.lock, (void *) res);
        }
    }
}

static bool_t trx_handler(void *arg)
{
    stm32l1_i2c_nodma_t *stm32l1_i2c = arg;
    I2C_t *ireg = stm32l1_i2c->reg;
    const uint32_t sr1 = ireg->SR1;
    const int is_last_byte = (stm32l1_i2c->trx_size == 1);
    
//debug_printf("trx_handler, SR1 = %08X, trx_size %d, is_last %d\n", sr1, stm32l1_i2c->trx_size, is_last_byte);

    if (sr1 & I2C_SB) {
        const int rx_mode = (stm32l1_i2c->cur_trans->size < 0);
//debug_printf("SB, rx_mode %d\n", rx_mode);
        if (stm32l1_i2c->cur_mode & I2C_MODE_10BIT_ADDR)
            ireg->DR = 0xF | ((I2C_MODE_GET_SLAVE_ADDR(stm32l1_i2c->cur_mode) >> 7) & 0x6) | rx_mode;
        else {
            ireg->DR = I2C_MODE_GET_SLAVE_ADDR(stm32l1_i2c->cur_mode) | rx_mode;
            ireg->CR1 |= I2C_ACK;
        }
    } else if (sr1 & I2C_ADD10) {
//debug_printf("ADD10\n");
        ireg->DR = I2C_MODE_GET_SLAVE_ADDR(stm32l1_i2c->cur_mode);
    } else if (sr1 & I2C_ADDR) {
//debug_printf("ADDR\n");
        const uint32_t sr2 = ireg->SR2;
        if (sr2 & I2C_TRA) {
//debug_printf("TRA\n");
            ireg->DR = *stm32l1_i2c->trx_bytes++;
            --stm32l1_i2c->trx_size;
            if (is_last_byte)
                process_last_byte(stm32l1_i2c);
        } else {
//debug_printf("!TRA\n");
            if (is_last_byte)
                ireg->CR1 &= ~I2C_ACK;
        }
    } else if (sr1 & I2C_BTF) {
//debug_printf("BTF\n");
        if (sr1 & I2C_TXE) {
            ireg->DR = *stm32l1_i2c->trx_bytes++;
            --stm32l1_i2c->trx_size;
        } else {
            if (stm32l1_i2c->trx_size > 0) {
                *stm32l1_i2c->trx_bytes++ = ireg->DR;
                --stm32l1_i2c->trx_size;
                if (is_last_byte)
                    ireg->CR1 &= ~I2C_ACK;
            } else {
//debug_printf("STOP\n");
                (void) ireg->DR;
                mutex_activate(&stm32l1_i2c->i2cif.lock, (void *) I2C_ERR_OK);
            }
        }
        if (is_last_byte) {
            process_last_byte(stm32l1_i2c);
        }
    //} else if (sr1 & I2C_STOPF) {
    } else if ((sr1 == 0) && (stm32l1_i2c->trx_size == 0)) {
//debug_printf("STOP\n");
        mutex_activate(&stm32l1_i2c->i2cif.lock, (void *) I2C_ERR_OK);
    }
    
    arch_intr_allow(stm32l1_i2c->trx_irq);
    return 0;
}

static bool_t error_handler(void *arg)
{
//debug_printf("error_handler\n");
    stm32l1_i2c_nodma_t *stm32l1_i2c = arg;
    I2C_t *ireg = stm32l1_i2c->reg;
    const uint32_t sr1 = ireg->SR1;
    int res = I2C_ERR_OK;
    
    if (sr1 & I2C_ARLO)
        res = I2C_ERR_ARBITR_LOST;
	else if (sr1 & I2C_BERR)
	    res = I2C_ERR_BUS;
	else if (sr1 & I2C_AF)
	    res = I2C_ERR_NOT_ACKED;
	    
	ireg->SR1 = 0;
	mutex_activate(&stm32l1_i2c->i2cif.lock, (void *) res);
	    
    arch_intr_allow(stm32l1_i2c->trx_irq + 1);
    return 0;
}

int stm32l1_i2c_nodma_init(stm32l1_i2c_nodma_t *i2c, int port)
{
    i2cif_t *i2cif = to_i2cif(i2c);
    i2cif->trx = stm32l1_i2c_trx;
    
    if (port == 1) {
        i2c->reg = I2C1;
        RCC->APB1ENR |= RCC_I2C1EN;
        RCC->APB1LPENR |= RCC_I2C1LPEN;
        i2c->trx_irq = IRQ_I2C1_EV;
    } else if (port == 2) {
        i2c->reg = I2C2;
        RCC->APB1ENR |= RCC_I2C2EN;
        RCC->APB1LPENR |= RCC_I2C2LPEN;
        i2c->trx_irq = IRQ_I2C2_EV;
    } else {
        return I2C_ERR_BAD_PORT;
    }

    mutex_attach_irq(&i2c->trx_mutex, i2c->trx_irq, trx_handler, i2c);
    mutex_attach_irq(&i2c->error_mutex, i2c->trx_irq + 1, error_handler, i2c);

    i2c->reg->CR1 = 0;
    i2c->reg->CR2 = I2C_FREQ(KHZ_PCLK1 / 1000) | INTERRUPT_MASK;
        
    return I2C_ERR_OK;
}

