#include <runtime/lib.h>
#include <kernel/uos.h>
#include <thermo/mic184.h>

enum {
    REG_TEMP,
    REG_CONFIG,
    REG_T_HYST,
    REG_T_SET
};

enum
{
    LAST_OP_UNDEFINED,
    LAST_OP_TEMP,
    LAST_OP_CONFIG,
    LAST_OP_T_SET,
    LAST_OP_T_HYST,
};

#define MIC184_CONFIG_ZONE_EXT  (1 << 5)
#define MIC184_CONFIG_IM        (1 << 6)
#define MIC184_CONFIG_STS       (1 << 7)

static int do_set_mode(mic184_t *mic184, uint8_t mode)
{
    i2c_transaction_t trans1;
    uint8_t config[2];
    
    config[0] = REG_CONFIG;
    config[1] = mode;
    
    mic184->msg.first = &trans1;
    trans1.data = config;
    trans1.size = I2C_TX_SIZE(2);
    trans1.next = 0;
    
    int res = i2c_trx(mic184->i2c, &mic184->msg);
    if (res == I2C_ERR_OK) {
        mic184->last_op = LAST_OP_CONFIG;
        mic184->mode = mode;
        return THERMO_ERR_OK;
    } else {
        mic184->last_op = LAST_OP_UNDEFINED;
        return 0x7FFFFFFF + THERMO_ERR_IO;
    }
}

static int mic184_read(thermoif_t *thermo)
{
    mic184_t *mic184 = (mic184_t *) thermo;
    i2c_transaction_t trans1;       // Переключение внутреннего указателя на регистр температуры
    i2c_transaction_t trans2;       // Чтение текущей температуры
    uint8_t pointer_to_temp = REG_TEMP;
    uint8_t temp[2];
    int res;

    trans2.data = temp;
    if (mic184->precision == MIC184_PRECISION_0_5_GRAD)
        trans2.size = I2C_RX_SIZE(2);
    else
        trans2.size = I2C_RX_SIZE(1);
    trans2.next = 0;
    
    if (mic184->last_op == LAST_OP_TEMP) {
        mic184->msg.first = &trans2;
    } else {
        trans1.data = &pointer_to_temp;
        trans1.size = I2C_TX_SIZE(1);
        trans1.next = &trans2;
        mic184->msg.first = &trans1;
    }
    
    res = i2c_trx(mic184->i2c, &mic184->msg);
    if (res != I2C_ERR_OK) {
        mic184->last_op = LAST_OP_UNDEFINED;
        return 0x7FFFFFFF + THERMO_ERR_IO;
    }

    mic184->last_op = LAST_OP_TEMP;
    
    if (mic184->precision == MIC184_PRECISION_0_5_GRAD) {
        short res_temp = (temp[0] << 8) | temp[1];
        res = res_temp * 1000 / 256;
    } else {
        res = temp[0] * 1000;
    }
    
    if (res <= 125000)
        return res;
    else
        return 0x7FFFFFFF + THERMO_ERR_BAD_VALUE;
}

int mic184_switch_sensor(mic184_t *mic184, int sensor)
{
    if (sensor == MIC184_EXTERNAL)
        return do_set_mode(mic184, mic184->mode | MIC184_CONFIG_ZONE_EXT);
    else
        return do_set_mode(mic184, mic184->mode & ~MIC184_CONFIG_ZONE_EXT);
}

int mic184_set_mode(mic184_t *mic184, uint8_t mode)
{
    i2c_transaction_t trans1;       // Запись регистра конфигурации
    uint8_t conf[2];
    
    conf[0] = REG_CONFIG;           // Переключение внутреннего указателя на регистр конфигурации
    conf[1] = mode & 0x1F;
    
    trans1.data = conf;
    trans1.size = I2C_TX_SIZE(2);
    trans1.next = 0;

    mic184->msg.first = &trans1;

    int res = i2c_trx(mic184->i2c, &mic184->msg);
    if (res != I2C_ERR_OK) {
        mic184->last_op = LAST_OP_UNDEFINED;
        return THERMO_ERR_IO;
    }
        
    mic184->last_op = LAST_OP_CONFIG;
    mic184->mode = (mic184->mode & 0xE0) | (mode & 0x1F);
    
    return THERMO_ERR_OK;
}

void mic184_set_precision(mic184_t *mic184, int precision)
{
    mic184->precision = precision;
}

int mic184_set_thermostat(mic184_t *mic184, int t_set, int t_hyst)
{
    i2c_transaction_t trans1;       // Запись Thyst
    i2c_transaction_t trans2;       // Запись Tos
    uint8_t pointer_to_thyst = REG_T_HYST;
    uint8_t pointer_to_tset  = REG_T_SET;
    
    uint8_t t_hyst_data[3];
    t_hyst_data[0] = pointer_to_thyst;
    t_hyst_data[1] = t_hyst / 1000;
    t_hyst_data[2] = t_hyst * 256 / 1000;

    uint8_t t_set_data[3];
    t_set_data[0] = pointer_to_tset;
    t_set_data[1] = t_set / 1000;
    t_set_data[2] = t_set * 256 / 1000;
    
    trans1.data = t_hyst_data;
    trans1.size = I2C_TX_SIZE(3);
    trans1.next = &trans2;
    trans2.data = t_set_data;
    trans2.size = I2C_TX_SIZE(3);
    trans2.next = 0;
    
    mic184->msg.first = &trans1;
    
    int res = i2c_trx(mic184->i2c, &mic184->msg);
    if (res != I2C_ERR_OK) {
        mic184->last_op = LAST_OP_UNDEFINED;
        return THERMO_ERR_IO;
    }
    
    mic184->last_op = LAST_OP_T_SET;
        
    return THERMO_ERR_OK;    
}

int mic184_read_t_set(mic184_t *mic184)
{
    i2c_transaction_t trans1;       // Переключение внутреннего указателя на Tset
    i2c_transaction_t trans2;       // Чтение Tset
    uint8_t pointer_to_tset = REG_T_SET;
    uint8_t temp[2];

    trans1.data = &pointer_to_tset;
    trans1.size = I2C_TX_SIZE(1);
    trans1.next = &trans2;
    trans2.data = temp;
    trans2.size = I2C_RX_SIZE(2);
    trans2.next = 0;
    
    mic184->msg.first = &trans1;
    
    int res = i2c_trx(mic184->i2c, &mic184->msg);
    if (res != I2C_ERR_OK) {
        mic184->last_op = LAST_OP_UNDEFINED;
        return 0x7FFFFFFF + THERMO_ERR_IO;
    }
    
    mic184->last_op = LAST_OP_T_SET;
     
    short res_temp = (temp[0] << 8) | temp[1];
    return res_temp * 1000 / 256;
}

int mic184_read_t_hyst(mic184_t *mic184)
{
    i2c_transaction_t trans1;       // Переключение внутреннего указателя на Thyst
    i2c_transaction_t trans2;       // Чтение Thyst
    uint8_t pointer_to_thyst = REG_T_HYST;
    uint8_t temp[2];

    trans1.data = &pointer_to_thyst;
    trans1.size = I2C_TX_SIZE(1);
    trans1.next = &trans2;
    trans2.data = temp;
    trans2.size = I2C_RX_SIZE(2);
    trans2.next = 0;
    
    mic184->msg.first = &trans1;
    
    int res = i2c_trx(mic184->i2c, &mic184->msg);
    if (res != I2C_ERR_OK) {
        mic184->last_op = LAST_OP_UNDEFINED;
        return 0x7FFFFFFF + THERMO_ERR_IO;
    }
    
    mic184->last_op = LAST_OP_T_HYST;
     
    short res_temp = (temp[0] << 8) | temp[1];
    return res_temp * 1000 / 256;
}

int mic184_interrupt_enable(mic184_t *mic184)
{
    int res = do_set_mode(mic184, mic184->mode & ~MIC184_CONFIG_IM);
    if (res != I2C_ERR_OK) {
        mic184->last_op = LAST_OP_UNDEFINED;
        return 0x7FFFFFFF + THERMO_ERR_IO;
    }
    
    mic184->last_op = LAST_OP_CONFIG;
    return THERMO_ERR_OK;
}

int mic184_interrupt_disable(mic184_t *mic184)
{
    int res = do_set_mode(mic184, mic184->mode | MIC184_CONFIG_IM);
    if (res != I2C_ERR_OK) {
        mic184->last_op = LAST_OP_UNDEFINED;
        return 0x7FFFFFFF + THERMO_ERR_IO;
    }
    
    mic184->last_op = LAST_OP_CONFIG;
    return THERMO_ERR_OK;
}

int mic184_interrupt_status(mic184_t *mic184)
{
    i2c_transaction_t trans1;       // Переключение внутреннего указателя на регистр конфигурации
    i2c_transaction_t trans2;       // Запись новой конфигурации
    uint8_t pointer_to_config = REG_CONFIG;
    uint8_t config;
    
    trans2.data = &config;
    trans2.size = I2C_RX_SIZE(1);
    trans2.next = 0;
    
    if (mic184->last_op == LAST_OP_CONFIG) {
        mic184->msg.first = &trans2;
    } else {
        trans1.data = &pointer_to_config;
        trans1.size = I2C_TX_SIZE(1);
        trans1.next = &trans2;
        mic184->msg.first = &trans1;
    }
    
    int res = i2c_trx(mic184->i2c, &mic184->msg);
    if (res == I2C_ERR_OK) {
        mic184->last_op = LAST_OP_CONFIG;
        return (config & MIC184_CONFIG_STS) ? 1 : 0;
    } else {
        mic184->last_op = LAST_OP_UNDEFINED;
        return 0x7FFFFFFF + THERMO_ERR_IO;
    }
}

void mic184_init(mic184_t *mic184, i2cif_t *i2c, uint8_t i2c_addr, unsigned i2c_timings, unsigned i2c_freq_khz)
{
    mic184->i2c = i2c;
    mic184->msg.mode = I2C_MODE_SLAVE_ADDR(i2c_addr) | I2C_MODE_7BIT_ADDR | 
        i2c_timings | I2C_MODE_FREQ_KHZ(i2c_freq_khz);
    
    mic184->thermoif.read = mic184_read;
}
