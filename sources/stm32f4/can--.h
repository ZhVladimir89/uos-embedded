#ifndef __STM32F3_CAN_H__
#define __STM32F3_CAN_H__

#include <can/can-interface.h>
#include <buffers/byte-queue.h>

#ifndef CAN_INQ_MAX_FRAMES
#define CAN_INQ_MAX_FRAMES  32
#endif

typedef struct _stm32f3_can_t
{
    canif_t             canif;
    
    mutex_t             rx0_lock;
    
    unsigned            timing;
    unsigned            baud_rate;
    
    bq_t                inq;
    uint8_t             inq_buffer[CAN_INQ_MAX_FRAMES * sizeof(can_frame_t)];
    
    unsigned            in_rec;
    unsigned            in_lost;
} stm32f3_can_t;

void stm32f3_can_init(stm32f3_can_t *can);

#endif // __STM32F3_CAN_H__
