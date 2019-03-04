#ifndef __PCA9698_H__
#define __PCA9698_H__

#include <i2c/i2c-interface.h>
#include <gpio/gpio-interface.h>

#define PCA9698_USE_INTERRUPT   0

#ifndef PCA9698_USE_INTERRUPT
#define PCA9698_USE_INTERRUPT   1
#endif

typedef struct _pca9698_t pca9698_t;
typedef struct _pca9698_gpio_t pca9698_gpio_t;

typedef struct _pca9698_t
{
    mutex_t         lock;
    
    i2cif_t         *i2c;
    i2c_message_t   msg;
    
    uint8_t         ip[5];
    uint8_t         prev_ip[5];
    uint8_t         op[5];
    uint8_t         pi[5];
    uint8_t         ioc[5];
    uint8_t         msk[5];
    uint8_t         outconf;

#if PCA9698_USE_INTERRUPT    
    gpioif_t        *int_pin;
    ARRAY(int_stack, 800);
    
    pca9698_gpio_t  *pins[5 * 8];
#endif
} pca9698_t;

typedef struct _pca9698_gpio_t
{
    gpioif_t            gpioif;
    
    pca9698_t           *owner;
    unsigned            port;
    unsigned            pin;
    
#if PCA9698_USE_INTERRUPT
    gpio_handler_t      handler;
    void                *handler_arg;
#endif
} pca9698_gpio_t;


// Функция инициализации драйвера pca9698. Можно вызывать из uos_init().
// i2c              - указатель на драйвер шины i2c.
// i2c_addr         - адрес микросхемы на шине i2c (старшие 7 битов).
// i2c_timings      - режим времянки (таймингов) шины i2c; возможные значения:
//                      I2C_MODE_SM         - времянка Sm (Standard-mode);
//                      I2C_MODE_SMBUS      - времянка SMBUS;
//                      I2C_MODE_FM         - времянка Fm (Fast-mode);
//                      I2C_MODE_FM_PLUS    - времянка Fm+ (Fast-mode+);
// i2c_freq_khz     - бодовая частота передачи на шине i2c в кГц;
// interrupt_pin    - вывод, используемый в качестве входа прерывания (используется,
//                      только если PCA9698_USE_INTERRUPT = 1);
// interrupt_prio   - приоритет задачи-обработчика прерывания.
void pca9698_init(pca9698_t *pca9698, i2cif_t *i2c, uint8_t i2c_addr, unsigned i2c_timings, 
    unsigned i2c_freq_khz, gpioif_t *interrupt_pin, int interrupt_prio);

// Функция записи настроек в микросхему PCA9698.
// Данная функция должна быть вызвана для применения любых изменений настроек.
// Сюда входит: 
//  - добавление нового вывода (с помощью pca9698_gpio_init);
//  - изменение параметров вывода (направление, инверсия);
//  - добавление обработчика прерывания для входа PCA9698 и его удаление.
int pca9698_apply_configuration(pca9698_t *pca9698);

// Функция чтения состояния входов из микросхемы PCA9698.
// Значения входов GPIO PCA9698 не обновляются сами по себе, то есть при вызове
// gpio_val() не происходит реального чтения входа, а возвращается его значение
// из кэша драйвера. Чтобы обновить значения в кэше необходимо вызвать данную функцию.
// Данную функцию имеет смысл использовать, только если используется режим работы
// без прерываний (PCA9698_USE_INTERRUPT = 0). В режиме работы с прерываниями она
// вызывается в нужный момент в теле драйвера.
int pca9698_update_inputs(pca9698_t *pca9698);

// Функция записи значений выходов в микросхему PCA9698.
// Значения выходов GPIO PCA9698 не обновляются сами по себе, то есть при вызове
// gpio_set_val() не происходит реальной установки состояния выхода, а лишь значение
// записывается в кэш драйвера. Чтобы перенести значения из кэша в микросхему, тем
// самым вызвав установку всех выходов в требуемые значения, необходимо вызвать данную функцию.
int pca9698_update_outputs(pca9698_t *pca9698);

// Ножка настраивается в режим цифрового выхода
#define PCA9698_GPIO_FLAGS_OUTPUT       0
// Ножка настраивается в режим цифрового входа
#define PCA9698_GPIO_FLAGS_INPUT        1
// Режим работы цифрового выхода - открытый коллектор (если флаг не указан, то push-pull)
// С установкой режима "открытый коллектор" в PCA9698 есть особенности:
// - в портах 1, 2, 3, 4 этот режим устанавливается сразу для всех выходов;
// - в порту 0 этот режим устанавливается одновременно для двух выходов, попарно: (0, 1), (2, 3), (4, 5), (6, 7).
#define PCA9698_GPIO_FLAGS_OPEN_DRAIN   2
// Инвертирование полярности при чтении
#define PCA9698_GPIO_FLAGS_INV_INPUT    4

// Функция инициализации вывода PCA9698.
void pca9698_gpio_init(pca9698_gpio_t *gpio, pca9698_t *pca9698, unsigned port, unsigned pin, unsigned flags);

#endif // __PCA9698_H__
