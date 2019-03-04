#include <runtime/lib.h>
#include <kernel/uos.h>
#include <kernel/internal.h>
#include <milandr_new/i2c.h>

static int start_trx(milandr_i2c_t *milandr_i2c)
{
    while (milandr_i2c->cur_trans->size == 0) {
        milandr_i2c->cur_trans = milandr_i2c->cur_trans->next;
        if (! milandr_i2c->cur_trans)
            return I2C_ERR_BAD_PARAM;
    }

    milandr_i2c->trx_bytes = milandr_i2c->cur_trans->data;
    milandr_i2c->first_data = 1;

    arm_reg_t txd = I2C_MODE_GET_SLAVE_ADDR(milandr_i2c->cur_mode);
    milandr_i2c->trx_size = milandr_i2c->cur_trans->size;
    if (milandr_i2c->cur_trans->size < 0)
        txd |= 1;

    ARM_I2C->TXD = txd;
    ARM_I2C->CMD = I2C_START | I2C_WR | I2C_CLR_INT;
    
    return I2C_ERR_OK;
}

static int milandr_i2c_trx(i2cif_t *i2c, i2c_message_t *msg)
{
    milandr_i2c_t *milandr_i2c = (milandr_i2c_t *) i2c;
    int res = I2C_ERR_OK;
    
    if (! msg->first)
        return I2C_ERR_BAD_PARAM;
        
    if (msg->mode & I2C_MODE_10BIT_ADDR)
        return I2C_ERR_MODE_NOT_SUPP;
        
    arm_reg_t ctr = I2C_EN_I2C | I2C_EN_INT;
    switch (msg->mode & I2C_MODE_MASK) {
    case I2C_MODE_SM:
    case I2C_MODE_SMBUS:
    case I2C_MODE_FM:
        break;
    case I2C_MODE_FM_PLUS:
        ctr |= I2C_S_I2C;
        break;
    default:
        return I2C_ERR_MODE_NOT_SUPP;
    }
    
    mutex_lock(&i2c->lock);
    
    int div = KHZ / (5 * I2C_MODE_FREQ_KHZ(msg->mode)) - 1;

    ARM_I2C->PRL = div & 0xFF;
    ARM_I2C->PRH = div >> 8;
    ARM_I2C->CTR = ctr;
	    
	milandr_i2c->cur_trans = msg->first;
	milandr_i2c->cur_mode = msg->mode;
	res = start_trx(milandr_i2c);
	if (res != I2C_ERR_OK)
	    goto trx_exit;

	res = (int)mutex_wait(&i2c->lock);
	
trx_exit:
  	mutex_unlock(&i2c->lock);
    return res;
}

static bool_t i2c_handler(void *arg)
{
    milandr_i2c_t *milandr_i2c = arg;
    const uint32_t sta = ARM_I2C->STA;
    const int is_last_transaction = (milandr_i2c->cur_trans->next == 0);
    if (sta & I2C_LOST_ARB) {
        ARM_I2C->CMD = I2C_CLR_INT;
        mutex_activate(&milandr_i2c->i2cif.lock, (void *) I2C_ERR_ARBITR_LOST);
    } else if (sta & I2C_RX_NACK) {
        ARM_I2C->CMD = I2C_CLR_INT;
        if ((milandr_i2c->trx_size == 0) && is_last_transaction)
            mutex_activate(&milandr_i2c->i2cif.lock, (void *) I2C_ERR_OK);
        else
            mutex_activate(&milandr_i2c->i2cif.lock, (void *) I2C_ERR_NOT_ACKED);
    } else {
        if (milandr_i2c->trx_size == 0) {
            if (is_last_transaction) {
                ARM_I2C->CMD = I2C_CLR_INT;
                mutex_activate(&milandr_i2c->i2cif.lock, (void *) I2C_ERR_OK);
            } else {
                milandr_i2c->cur_trans = milandr_i2c->cur_trans->next;
                int res = start_trx(milandr_i2c);
                if (res != I2C_ERR_OK) {
                    ARM_I2C->CMD = I2C_CLR_INT | I2C_STOP;
                    mutex_activate(&milandr_i2c->i2cif.lock, (void *) res);
                }                
            }
        } else {
            arm_reg_t cmd = I2C_CLR_INT;
            
            if (milandr_i2c->trx_size > 0) {
                ARM_I2C->TXD = *milandr_i2c->trx_bytes++;
                --milandr_i2c->trx_size;
                cmd |= I2C_WR;
            } else {
                if (!milandr_i2c->first_data) {
                    *milandr_i2c->trx_bytes++ = ARM_I2C->RXD;
                    ++milandr_i2c->trx_size;
                }
                milandr_i2c->first_data = 0;
                cmd |= I2C_RD;
            }
        
            if (milandr_i2c->trx_size == 0) {
                cmd |= I2C_NACK;
                if (is_last_transaction)
                    cmd |= I2C_STOP;
            }
            
            ARM_I2C->CMD = cmd;
        }
    }
    
    arch_intr_allow(MDR_I2C_IRQn);
    return 0;
}

int milandr_i2c_init (milandr_i2c_t *i2c)
{
    i2cif_t *i2cif = to_i2cif(i2c);
    i2cif->trx = milandr_i2c_trx;
    
    ARM_RSTCLK->PER_CLOCK |= ARM_PER_CLOCK_I2C1;
    
    mutex_attach_irq(&i2c->irq_mutex, MDR_I2C_IRQn, i2c_handler, i2c);
        
    return I2C_ERR_OK;
}

