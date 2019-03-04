#ifndef __STM32F3_I2C_H__
#define __STM32F3_I2C_H__

#include <i2c/i2c-interface.h>

// Использовать ли аналоговый фильтр
#ifndef I2C_ANALOG_FILTER_ENABLED
#define I2C_ANALOG_FILTER_ENABLED   0
#endif

//
// Далее все настроечные макроопределения в наносекундах!
//
// Минимальная задержка аналогового фильтра (по даташиту на микроконтроллер)
#ifndef T_AF_MIN
#define T_AF_MIN                    50
#endif

// Максимальная задержка аналогового фильтра (по даташиту на микроконтроллер)
#ifndef T_AF_MAX
#define T_AF_MAX                    260
#endif

// Время (ширина) фильтрации с помощью цифрового фильтра
#ifndef I2C_DIGITAL_FILTER_WIDTH
#define I2C_DIGITAL_FILTER_WIDTH    200
#endif

// Время спада SCL (можно замерить осциллографом)
#ifndef I2C_SCL_FALLING_SLOPE
#define I2C_SCL_FALLING_SLOPE       100
#endif

// Время фронта SCL (можно замерить осциллографом)
#ifndef I2C_SCL_RISING_SLOPE
#define I2C_SCL_RISING_SLOPE        100
#endif


typedef struct _stm32f3_i2c_t
{
	i2cif_t             i2cif;
	
	I2C_t               *reg;
	mutex_t             error_mutex;
	mutex_t             trx_mutex;
    int                 trx_irq;
	uint8_t             *trx_bytes;
	int                 trx_size;
	unsigned            cur_mode;
	i2c_transaction_t   *cur_trans;
} stm32f3_i2c_t;

// Нумерация портов начиная с 1.
int stm32f3_i2c_init(stm32f3_i2c_t *i2c, int port);

#endif /* __STM32F3_I2C_H__ */
