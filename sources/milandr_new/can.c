#include <runtime/lib.h>
#include <kernel/uos.h>
#include <kernel/internal.h>
#include <milandr_new/can.h>

#define NBUF        32                  // Общее число буферов CAN
#define NRBUF       16                  // Количество приёмных буферов
#define NTBUF       (NBUF - NRBUF)      // Количество передающих буферов
#define RBUF_MASK   ((1 << NRBUF) - 1)  // Маска прерываний от приёмных буферов
#define TBUF_MASK   ~RBUF_MASK          // Маска прерываний от передающих буферов

static inline void do_reset(milandr_can_t *milandr_can)
{
    milandr_can->reg->CONTROL = 0;
}

static void do_set_timing(milandr_can_t *milandr_can)
{
    unsigned sjw = CAN_TIMING_GET_SJW(milandr_can->timing);
    unsigned prs = CAN_TIMING_GET_PRS(milandr_can->timing);
    unsigned ps1 = CAN_TIMING_GET_PS1(milandr_can->timing);
    unsigned ps2 = CAN_TIMING_GET_PS2(milandr_can->timing);
    
    milandr_can->reg->BITTMNG = CAN_BITTMNG_SJW(sjw) | CAN_BITTMNG_SEG1(ps1) | 
               CAN_BITTMNG_SEG2(ps2) | CAN_BITTMNG_PSEG(prs) |
               CAN_BITTMNG_BRP(KHZ * 1000 / (milandr_can->baud_rate * (1 + prs + ps1 + ps2)) - 1);
}


static int milandr_can_output(canif_t *can, const can_frame_t *buffer, int nb_of_frames)
{
    milandr_can_t *milandr_can = (milandr_can_t *)can;
    int frame_n = 0;
    int mb_n;
    
    if (nb_of_frames <= 0)
        return 0;
    
    mutex_lock(&can->lock);

    while (frame_n < nb_of_frames) {
	    // Дожидаемся наличия свободного буфера. Буферы загружаются только в 
	    // порядке возрастания номера, чтобы соблюдать очередность выдачи кадров.
	    mb_n = NBUF - arm_count_leading_zeroes (~(milandr_can->reg->TX & TBUF_MASK));
	    if (mb_n < NRBUF || mb_n >= NBUF) {
		    mutex_wait(&can->lock);
		    continue;
        }

        // Загружаем буфер
        CAN_BUF_t *mesbox = &milandr_can->reg->BUF[mb_n];
        uint32_t id = buffer->id & CAN_ID_MASK;
	    if (buffer->id & CAN_ID_EXT) {
		    // Расширенный формат кадра
		    mesbox->ID = id;
		    mesbox->DLC = CAN_DLC_LEN(buffer->dlc) |
			    CAN_DLC_IDE | CAN_DLC_SSR | CAN_DLC_R1;
	    } else {
		    // Стандартный формат кадра
		    mesbox->ID = id << CAN_ID_SID_SHIFT;
		    mesbox->DLC = CAN_DLC_LEN(buffer->dlc);
	    }
        if (buffer->dlc & CAN_ID_RTR)
            mesbox->DLC |= CAN_DLC_RTR;
	    mesbox->DATAL = buffer->data32[0];
	    mesbox->DATAH = buffer->data32[1];
	    if (mb_n == NBUF - 1)
	        milandr_can->reg->INT_EN |= CAN_INT_EN_TX;
	    milandr_can->reg->BUF_CON[mb_n] |= CAN_BUF_CON_TX_REQ;

	    // Разрешение прерывания от передающего буфера.
	    //milandr_can->reg->INT_TX |= 1 << mb_n;

	    ++buffer;
	    ++frame_n;
    }
    
    mutex_unlock(&can->lock);
    return frame_n;
}

static bool_t intr_handler (void *arg)
{
    milandr_can_t *milandr_can = arg;
    
    if (milandr_can->reg->STATUS & CAN_STATUS_TX_READY)
        milandr_can->reg->INT_EN &= ~CAN_INT_EN_TX;

    if (milandr_can->reg->STATUS & CAN_STATUS_RX_READY) {
        can_frame_t frame;
		unsigned i;
		milandr_can->reg->STATUS = 0;
		for (i = 0; i < NRBUF; i++) {
			unsigned bufcon = milandr_can->reg->BUF_CON[i];
			if (! (bufcon & CAN_BUF_CON_RX_FULL))
				continue;
			if (bufcon & CAN_BUF_CON_OVER_WR) {
				/* Сообщение перезаписано */
				++milandr_can->in_lost;
				milandr_can->reg->BUF_CON[i] = bufcon & ~CAN_BUF_CON_OVER_WR;
			}
			CAN_BUF_t *buf = &milandr_can->reg->BUF[i];
			if (buf->DLC & CAN_DLC_IDE) {
				frame.id = buf->ID;
				frame.id |= CAN_ID_EXT;
			} else {
				frame.id = buf->ID >> CAN_ID_SID_SHIFT;
			}
			if (buf->DLC & CAN_DLC_RTR)
			    frame.id |= CAN_ID_RTR;
			frame.dlc = CAN_DLC_LEN(buf->DLC);
			frame.data32[0] = buf->DATAL;
			frame.data32[1] = buf->DATAH;
			milandr_can->reg->BUF_CON[i] = bufcon & ~CAN_BUF_CON_RX_FULL;

            if (bq_avail_put(&milandr_can->inq) < sizeof(frame)) {
                ++milandr_can->in_lost;
            } else {
                bq_put_array(&milandr_can->inq, &frame, sizeof(frame));
                ++milandr_can->in_rec;
            }
        }
    }
    
    arch_intr_allow(milandr_can->port - 1);
    return 0;
}

static int milandr_can_input(canif_t *can, can_frame_t *buffer, int nb_of_frames, int non_block)
{
    milandr_can_t *milandr_can = (milandr_can_t *)can;
    
    if (nb_of_frames <= 0)
        return 0;
        
    mutex_lock(&can->lock);

    if (!non_block)
        while (bq_is_empty(&milandr_can->inq))
            mutex_wait(&can->lock);
        
    const int avail_nb_frames = bq_avail_get(&milandr_can->inq) / sizeof(can_frame_t);
    if (avail_nb_frames < nb_of_frames)
        nb_of_frames = avail_nb_frames;
    if (nb_of_frames != 0)
        bq_get_array(&milandr_can->inq, buffer, nb_of_frames * sizeof(can_frame_t));
        
    mutex_unlock(&can->lock);
    return nb_of_frames;
}

static int milandr_can_add_filter(canif_t *can, uint32_t mask, uint32_t pattern)
{
    milandr_can_t *milandr_can = (milandr_can_t *)can;
    
    if (milandr_can->nb_filters >= NRBUF)
        return CAN_TOO_MANY_FILTERS;

    mutex_lock(&can->lock);

    if (milandr_can->nb_filters == 0) {
        unsigned i;
        for (i = 0; i < NRBUF; ++i) {
            if (mask & CAN_ID_EXT) {
                milandr_can->reg->MASK[i].MASK = mask;
                milandr_can->reg->MASK[i].FILTER = pattern;
            } else {
                milandr_can->reg->MASK[i].MASK = mask << CAN_ID_SID_SHIFT;
                milandr_can->reg->MASK[i].FILTER = pattern << CAN_ID_SID_SHIFT;
            }
        }
    } else {
        if (mask & CAN_ID_EXT) {
            milandr_can->reg->MASK[milandr_can->nb_filters].MASK = mask;
            milandr_can->reg->MASK[milandr_can->nb_filters].FILTER = pattern;
        } else {
            milandr_can->reg->MASK[milandr_can->nb_filters].MASK = mask << CAN_ID_SID_SHIFT;
            milandr_can->reg->MASK[milandr_can->nb_filters].FILTER = pattern << CAN_ID_SID_SHIFT;
        }
    }
    
    ++milandr_can->nb_filters;
   
    mutex_unlock(&can->lock);
    return CAN_ERR_OK;
}

static int milandr_can_clear_filters(canif_t *can)
{
    milandr_can_t *milandr_can = (milandr_can_t *)can;
    
    mutex_lock(&can->lock);
    
	unsigned i;
	for (i = 0; i < NRBUF; i++) {
		milandr_can->reg->MASK[i].MASK = 0;
		milandr_can->reg->MASK[i].FILTER = 0;
	}
	
	milandr_can->nb_filters = 0;
	
    mutex_unlock(&can->lock);
    return CAN_ERR_OK;
}

static int milandr_can_get_mode(canif_t *can)
{
    milandr_can_t *milandr_can = (milandr_can_t *)can;
    
    if (((milandr_can->port == 1) && !(ARM_RSTCLK->CAN_CLOCK & ARM_CAN_CLOCK_EN1)) ||
        ((milandr_can->port == 2) && !(ARM_RSTCLK->CAN_CLOCK & ARM_CAN_CLOCK_EN2)))
            return IFACE_MODE_SHUTDOWN;
    else
        return IFACE_MODE_OPERATION;
}

static int milandr_can_get_state(canif_t *can)
{
    milandr_can_t *milandr_can = (milandr_can_t *)can;
    
    if (milandr_can->reg->STATUS & CAN_STATUS_BUS_OFF)
        return CAN_BUS_OFF;
    else if (milandr_can->reg->STATUS & CAN_STATUS_ERR_PASSIVE)
        return CAN_ERROR_PASSIVE;
    else
        return CAN_ERROR_ACTIVE;
}

static int milandr_can_reset(canif_t *can)
{
    milandr_can_t *milandr_can = (milandr_can_t *)can;
    
    mutex_lock(&can->lock);
    if (milandr_can_get_mode(can) != IFACE_MODE_SHUTDOWN)
        do_reset(milandr_can);
    mutex_unlock(&can->lock);
    return CAN_ERR_OK;
}

static int milandr_can_set_timing(canif_t *can, unsigned baud_rate, unsigned timing)
{
    milandr_can_t *milandr_can = (milandr_can_t *)can;
    
    mutex_lock(&can->lock);
    
    milandr_can->baud_rate = baud_rate;
    
    if (timing == 0)
        milandr_can->timing = CAN_TIMING_SET_PRS(2) | CAN_TIMING_SET_PS1(2) | 
            CAN_TIMING_SET_PS2(3) | CAN_TIMING_SET_SJW(1);
    else
        milandr_can->timing = timing;

    if (milandr_can_get_mode(can) != IFACE_MODE_SHUTDOWN)
        do_set_timing(milandr_can);
    
    mutex_unlock(&can->lock);
    return CAN_ERR_OK;
}

static void enable_controller(canif_t *can)
{
    milandr_can_t *milandr_can = (milandr_can_t *)can;
    
    if (milandr_can->port == 1)
        ARM_RSTCLK->CAN_CLOCK |= ARM_CAN_CLOCK_EN1;
    else
        ARM_RSTCLK->CAN_CLOCK |= ARM_CAN_CLOCK_EN2;

    do_reset(milandr_can);
    do_set_timing(milandr_can);
    
	unsigned i;
	for (i = 0; i < 32; i++) {
		milandr_can->reg->BUF_CON[i] = CAN_BUF_CON_EN;
		milandr_can->reg->MASK[i].MASK = 0;
		milandr_can->reg->MASK[i].FILTER = 0;

		if (i < NRBUF)
			milandr_can->reg->BUF_CON[i] |= CAN_BUF_CON_RX_ON;
	}
    
    milandr_can->reg->INT_RX = RBUF_MASK;
    milandr_can->reg->INT_TX = 1 << (NBUF - 1);
    milandr_can->reg->OVER = 255;
    milandr_can->reg->STATUS = 0;
    milandr_can->reg->INT_EN = CAN_INT_EN_RX | CAN_INT_EN_GLB;
    milandr_can->reg->CONTROL = CAN_CONTROL_EN | CAN_CONTROL_SAP;
}

static int milandr_can_switch_mode(canif_t *can, int mode)
{
    milandr_can_t *milandr_can = (milandr_can_t *)can;
    
    mutex_lock(&can->lock);
    
    int cur_mode = milandr_can_get_mode(can);
    if (cur_mode == mode) {
        mutex_unlock(&can->lock);
        return CAN_ERR_OK;
    }
        
    switch (mode) {
    case IFACE_MODE_SHUTDOWN:
    case IFACE_MODE_SLEEP:
        if (milandr_can->port == 1)
            ARM_RSTCLK->CAN_CLOCK &= ~ARM_CAN_CLOCK_EN1;
        else
            ARM_RSTCLK->CAN_CLOCK &= ~ARM_CAN_CLOCK_EN2;
        break;
    case IFACE_MODE_OPERATION:
        if (cur_mode == IFACE_MODE_SHUTDOWN)
            enable_controller(can);
        break;
    default:
        mutex_unlock(&can->lock);
        return CAN_ERR_BAD_PARAM;
    }
    
    mutex_unlock(&can->lock);
    return CAN_ERR_OK;
}

int milandr_can_init(milandr_can_t *can, int port)
{
    switch (port) {
    case 1:
        ARM_RSTCLK->PER_CLOCK |= ARM_PER_CLOCK_CAN1;
        can->reg = ARM_CAN1;
        break;
    case 2:
        ARM_RSTCLK->PER_CLOCK |= ARM_PER_CLOCK_CAN2;
        can->reg = ARM_CAN2;
        break;
    default:
        return CAN_ERR_BAD_PORT;
    }
    
    can->port = port;
    
    canif_t *canif = to_canif(can);
    canif->set_timing = milandr_can_set_timing;
    canif->output = milandr_can_output;
    canif->input = milandr_can_input;
    canif->add_filter = milandr_can_add_filter;
    canif->clear_filters = milandr_can_clear_filters;
    canif->get_state = milandr_can_get_state;
    canif->switch_mode = milandr_can_switch_mode;
    canif->get_mode = milandr_can_get_mode;
    canif->reset = milandr_can_reset;
    
    bq_init(&can->inq, can->inq_buffer, sizeof(can->inq_buffer));
    
    // Номер прерывания совпадает с номером порта, уменьшенным на 1.
    mutex_attach_irq(&canif->lock, port - 1, intr_handler, can);
    
    return CAN_ERR_OK;
}

