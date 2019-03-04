//
// Тестирование датчика температуры с термостатом MIC184.
//
// Вывод INT MIC184 подключается к выводу микроконтроллера, который
// настраивается как вход с обработкой прерываний. Термостат должен давать
// прерывание при превышении Tset и при снижении температуры ниже Thyst
// (задаются в параметрах функции mic184_set_thermostat).
#include <runtime/lib.h>
#include <kernel/uos.h>
#include <stm32f4/i2c.h>
#include <stm32f4/gpio.h>
#include <thermo/mic184.h>
#include <timer/timer.h>

#define SLA     0x9E
#define FREQ    100000

ARRAY (task_space, 0x400);
ARRAY (mic184_intr_space, 0x400);

stm32f4_i2c_t i2c;
stm32f4_gpio_t sda;
stm32f4_gpio_t scl;
stm32f4_gpio_t int_pin;
mic184_t mic184;
timer_t timer;


void task (void *arg)
{
    int res;
    
    mic184_init(&mic184, to_i2cif(&i2c), SLA, I2C_MODE_SM, FREQ/1000);
    
    res = mic184_set_mode(&mic184, MIC184_MODE_INTERRUPT | MIC184_INT_ACTIVE_LOW |
        MIC184_INT_DEBOUNCE_4);
    debug_printf("mic184_set_mode result: %d\n", res);
    
    res = mic184_set_thermostat(&mic184, 31500, 31000);
    debug_printf("mic184_set_thermostat result: %d\n", res);
    
    res = mic184_read_t_set(&mic184);
    debug_printf("Tset = %d\n", res);
    res = mic184_read_t_hyst(&mic184);
    debug_printf("Thyst = %d\n", res);
    
    //mic184_set_precision(&mic184, MIC184_PRECISION_1_GRAD);

    for (;;) {
        res = mic184_switch_sensor(&mic184, MIC184_INTERNAL);
        if (res != THERMO_ERR_OK)
            debug_printf("Error switching sensor: %d\n", res);
        timer_delay(&timer, 160);
            
        res = thermo_read(to_thermoif(&mic184));
        if (res > 0x7FFFFFF0)
            debug_printf("ERROR: %d!\n", res - 0x7FFFFFFF);
        else
            debug_printf("Local temperature: %d\n", res);
            
        res = mic184_switch_sensor(&mic184, MIC184_EXTERNAL);
        if (res != THERMO_ERR_OK)   
            debug_printf("Error switching sensor: %d\n", res);
        timer_delay(&timer, 320);
            
        res = thermo_read(to_thermoif(&mic184));
        if (res > 0x7FFFFFF0)
            debug_printf("ERROR: %d!\n", res - 0x7FFFFFFF);
        else
            debug_printf("External temperature: %d\n", res);

        debug_printf("-------------------------------------------------------\n");
    }
}

void mic184_interrupt_task (void *arg)
{
    gpio_attach_interrupt(to_gpioif(&int_pin),
        GPIO_EVENT_RISING_EDGE | GPIO_EVENT_FALLING_EDGE, 0, 0);

    for (;;) {
        mutex_wait(gpio_get_mutex(to_gpioif(&int_pin)));
        debug_printf("mic184 interrupt status %d\n", mic184_interrupt_status(&mic184));
    }
}

void uos_init (void)
{
    debug_printf("\n\nTesting MIC184\n");
    
    timer_init(&timer, KHZ, 10);

    stm32f4_gpio_init(&scl, GPIO_PORT_B, 8, GPIO_FLAGS_ALT_I2C1);
    stm32f4_gpio_init(&sda, GPIO_PORT_B, 9, GPIO_FLAGS_ALT_I2C1 | GPIO_FLAGS_OPEN_DRAIN | GPIO_FLAGS_PULL_UP);
    //stm32f4_gpio_init(&os,  GPIO_PORT_B, 12, GPIO_FLAGS_INPUT | GPIO_FLAGS_PULL_UP);
    stm32f4_i2c_init(&i2c, 1);
    
	task_create(task, 0, "task", 2, task_space, sizeof(task_space));
	//task_create(mic184_interrupt_task, 0, "intr", 1, mic184_intr_space, sizeof(mic184_intr_space));
}
