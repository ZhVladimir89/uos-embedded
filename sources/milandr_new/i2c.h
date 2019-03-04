#ifndef __MILANDR_I2C_H__
#define __MILANDR_I2C_H__

#include <i2c/i2c-interface.h>

typedef struct _milandr_i2c_t
{
	i2cif_t             i2cif;
	
	mutex_t             irq_mutex;
	
	uint8_t             *trx_bytes;
	int                 trx_size;
	int                 first_data;
	unsigned            cur_mode;
	i2c_transaction_t   *cur_trans;
} milandr_i2c_t;

int milandr_i2c_init(milandr_i2c_t *i2c);

#endif /* __MILANDR_I2C_H__ */
