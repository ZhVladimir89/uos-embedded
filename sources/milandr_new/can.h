#ifndef __MILANDR_CAN_H__
#define __MILANDR_CAN_H__

#include <can/can-interface.h>
#include <buffers/byte-queue.h>

#ifndef CAN_INQ_MAX_FRAMES
#define CAN_INQ_MAX_FRAMES  32
#endif

typedef struct _milandr_can_t
{
    canif_t             canif;
    
    int                 port;
    CAN_t               *reg;
    
    unsigned            timing;
    unsigned            baud_rate;
    
    bq_t                inq;
    uint8_t             inq_buffer[CAN_INQ_MAX_FRAMES * sizeof(can_frame_t)];
    unsigned            nb_filters;
    
    unsigned            in_rec;
    unsigned            in_lost;
} milandr_can_t;

int milandr_can_init(milandr_can_t *can, int port);

#endif // __MILANDR_CAN_H__
