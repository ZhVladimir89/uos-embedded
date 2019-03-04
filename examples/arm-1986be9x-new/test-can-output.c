//
// Выдача кадров CAN с постоянно увеличивающимся идентификатором
//
#include <runtime/lib.h>
#include <kernel/uos.h>
#include <milandr_new/can.h>
#include <milandr_new/gpio.h>

ARRAY (task_space, 0x400);

milandr_can_t can;
milandr_gpio_t can_tx;
milandr_gpio_t can_rx;

#define MAX_FRAMES_CNT  32
#define FRAMES_CNT      16
can_frame_t frame[MAX_FRAMES_CNT];

#define CAN_SPEED       1000000
#define USE_EXT_ID

void task (void *arg)
{
    unsigned counter = 0;
    int i;
    
    canif_t *canif = to_canif(&can);
    
    can_reset( canif );
    can_set_timing( canif, CAN_SPEED, 0 );
    can_switch_mode( canif, IFACE_MODE_OPERATION );
    
    for ( i = 0; i < FRAMES_CNT; ++i )
        frame[i].dlc = 4;
    
    for (;;) {
        debug_printf("Current counter: %d\n", counter);
        
        for ( i = 0; i < FRAMES_CNT; ++i ) {
            memcpy( frame[i].data, &counter, 4 );
            frame[i].id = counter;
#ifdef USE_EXT_ID
            frame[i].id |= CAN_ID_EXT;
#endif
            ++counter;
        }
        
        if (can_output( canif, frame, FRAMES_CNT ) != FRAMES_CNT)
            debug_printf("ERROR!\n");
    }
}

void uos_init (void)
{
    debug_printf("\n\nMilandr CAN Output\n");
    
    milandr_gpio_init(&can_tx, GPIO_PORT_C, 8, GPIO_FLAGS_FUNC_MAIN | GPIO_FLAGS_HIGH_SPEED);
    milandr_gpio_init(&can_rx, GPIO_PORT_C, 9, GPIO_FLAGS_FUNC_MAIN);
    milandr_can_init(&can, 1);
    
	task_create(task, 0, "task", 2, task_space, sizeof(task_space));
}
