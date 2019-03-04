//
// Тестирование датчика температуры с термостатом ADT7461.
//
#include <runtime/lib.h>
#include <kernel/uos.h>
#include <stm32f4/i2c.h>
#include <stm32f4/gpio.h>
#include <thermo/adt7461.h>

#define SLA     0x98
#define FREQ    100000

ARRAY (task_space, 0x400);

stm32f4_i2c_t i2c;
stm32f4_gpio_t sda;
stm32f4_gpio_t scl;
adt7461_t adt7461;

void task (void *arg)
{
    int res;
    
    // Запускаем сразу в режиме Standby, в этом режиме датчик считывает температуру
    // по запросу
    res = adt7461_init(&adt7461, to_i2cif(&i2c), SLA, FREQ/1000, 1);
    if (res != THERMO_ERR_OK) {
        debug_printf("FAILED TO INIT ADT7461!\n");
        for (;;);
    }

    for (;;) {
        res = thermo_read(adt7461_loc_sensor(&adt7461));
        if (res > 0x7FFFFFF0)
            debug_printf("ERROR: %d!\n", res - 0x7FFFFFFF);
        else
            debug_printf("Local temperature: %d\n", res);
       
        res = thermo_read(adt7461_ext_sensor(&adt7461));
        if (res > 0x7FFFFFF0)
            debug_printf("ERROR: %d!\n", res - 0x7FFFFFFF);
        else
            debug_printf("External temperature: %d\n", res);
            
        debug_printf("-------------------------------------------------------\n");
            
        mdelay(500);
    }
}

void uos_init (void)
{
    debug_printf("\n\nTesting ADT7461\n");

    stm32f4_gpio_init(&scl, GPIO_PORT_B, 8, GPIO_FLAGS_ALT_I2C1);
    stm32f4_gpio_init(&sda, GPIO_PORT_B, 9, GPIO_FLAGS_ALT_I2C1 | GPIO_FLAGS_OPEN_DRAIN | GPIO_FLAGS_PULL_UP);
    stm32f4_i2c_init(&i2c, 1);
    
	task_create( task, "task", "task", 1, task_space, sizeof(task_space) );
}
