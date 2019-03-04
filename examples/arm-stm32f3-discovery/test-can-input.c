//
// Приём кадров CAN
//
#include <runtime/lib.h>
#include <kernel/uos.h>
#include <stm32f3/can.h>
#include <stm32f3/gpio.h>

ARRAY (task_space, 0x400);

stm32f3_can_t can;
stm32f3_gpio_t can_tx;
stm32f3_gpio_t can_rx;

#define MAX_FRAMES_CNT  32
#define FRAMES_CNT      16
can_frame_t frame[MAX_FRAMES_CNT];

#define CAN_SPEED       1000000

#define USE_FILTER
#define CLEAR_FILTER

void task (void *arg)
{
    int i;
    int j;
    
    canif_t *canif = to_canif(&can);
    
    can_reset( canif );
    can_set_timing( canif, CAN_SPEED, 0 );
    can_switch_mode( canif, IFACE_MODE_OPERATION );
    
#ifdef USE_FILTER
    can_add_filter( canif, 0x7FF, 0x4 );
    can_add_filter( canif, 0x7FF, 0x5 );
#endif

    int cycle = 0;
    for (;;) {
        int nb_rec = can_input( canif, frame, FRAMES_CNT, 0 );
        for (i = 0; i < nb_rec; ++i) {
            debug_printf("Received frame, id 0x%X%s%s, data: [", frame[i].id & CAN_ID_MASK,
                (frame[i].id & CAN_ID_RTR) ? " RTR":"", (frame[i].id & CAN_ID_EXT) ? " IDE":"");
            for (j = 0; j < frame[i].dlc; ++j)
                debug_printf(" %02X", frame[i].data[j]);
            debug_printf(" ], nb_lost: %d\n", can.in_lost);
        }
        //debug_printf("%d\n", can.in_lost);
        
#ifdef CLEAR_FILTER
        if (cycle > 9)
            can_clear_filters( canif );
#endif
        cycle++;
    }
}

void uos_init (void)
{
    debug_printf("\n\nSTM32F3 CAN Input\n");
    
    stm32f3_gpio_init( &can_rx, GPIO_PORT_B, 8, GPIO_FLAGS_ALT_FUNC(9));
    stm32f3_gpio_init( &can_tx, GPIO_PORT_B, 9, GPIO_FLAGS_ALT_FUNC(9) | GPIO_FLAGS_HIGH_SPEED);
    stm32f3_can_init( &can );
    
	task_create( task, "task", "task", 1, task_space, sizeof(task_space) );
}
