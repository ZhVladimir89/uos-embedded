#include <runtime/lib.h>
#include <kernel/uos.h>
#include <kernel/internal.h>
#include <stm32f3/can.h>

static inline void do_reset()
{
    CAN->MCR = CAN_RESET;
    while (CAN->MCR & CAN_RESET);
}

static void do_set_timing(stm32f3_can_t *stm32f3_can)
{
    CAN->MCR |= CAN_INRQ;
    while (! (CAN->MSR & CAN_INAK));
    
    unsigned sjw = CAN_TIMING_GET_SJW(stm32f3_can->timing);
    unsigned ts1 = CAN_TIMING_GET_PRS(stm32f3_can->timing) + CAN_TIMING_GET_PS1(stm32f3_can->timing);
    unsigned ts2 = CAN_TIMING_GET_PS2(stm32f3_can->timing);
    
    CAN->BTR = CAN_SJW(sjw-1) | CAN_TS1(ts1-1) | CAN_TS2(ts2-1) |
               CAN_BRP(KHZ * 1000 / (stm32f3_can->baud_rate * (1 + ts1 + ts2)) - 1);
               
    CAN->MCR &= ~CAN_INRQ;
}

static int stm32f3_can_output(canif_t *can, const can_frame_t *buffer, int nb_of_frames)
{
    const int NB_OF_TX_MAILBOXES = 3;
    int mb_n;
    int frame_n = 0;
    
    if (nb_of_frames <= 0)
        return CAN_ERR_OK;
    
    mutex_lock(&can->lock);

    for (;;) {
        mb_n = 0;    
        do {
            if (! (CAN->TMB[mb_n].IR & CAN_TXRQ)) {
                CAN->TMB[mb_n].DTR = buffer[frame_n].dlc;
                CAN->TMB[mb_n].LR = buffer[frame_n].data32[0];
                CAN->TMB[mb_n].HR = buffer[frame_n].data32[1];
                uint32_t id = buffer[frame_n].id & CAN_ID_MASK;
                CAN->TMB[mb_n].IR = 
                    ((buffer[frame_n].id & CAN_ID_EXT) ? CAN_EXID(id) | CAN_IDE : CAN_STID(id))
                    | ((buffer[frame_n].id & CAN_ID_RTR) ? CAN_RTR : 0)
                    | CAN_TXRQ;
                    
                ++frame_n;
            }
            ++mb_n;
        } while ((mb_n < NB_OF_TX_MAILBOXES) && (frame_n < nb_of_frames));
        
        if (frame_n >= nb_of_frames)
            break;
            
        mutex_wait(&can->lock);
    }
    
    mutex_unlock(&can->lock);
    return frame_n;
}

static bool_t rx_handler (void *arg)
{
    stm32f3_can_t *stm32f3_can = arg;
    
    while (CAN_GET_FMP(CAN->RFxR[0])) {
        uint32_t rec_id = CAN->RMB[0].IR;
        can_frame_t frame;
        if (rec_id & CAN_IDE)
            frame.id = CAN_GET_EXID(rec_id) | CAN_ID_EXT;
        else
            frame.id = CAN_GET_STID(rec_id);
        if (rec_id & CAN_RTR)
            frame.id |= CAN_ID_RTR;
        frame.dlc = CAN_GET_DLC(CAN->RMB[0].DTR);
        frame.data32[0] = CAN->RMB[0].LR;
        frame.data32[1] = CAN->RMB[0].HR;
        CAN->RFxR[0] = CAN_RFOM;
        
        if (bq_avail_put(&stm32f3_can->inq) < sizeof(frame))
            ++stm32f3_can->in_lost;
        else {
            bq_put_array(&stm32f3_can->inq, &frame, sizeof(frame));
            ++stm32f3_can->in_rec;
        }
    }
    
    arch_intr_allow (IRQ_USB_LP_CAN1_RX0);
    return 0;
}

static int stm32f3_can_input(canif_t *can, can_frame_t *buffer, int nb_of_frames, int non_block)
{
    stm32f3_can_t *stm32f3_can = (stm32f3_can_t *)can;
    
    if (nb_of_frames <= 0)
        return 0;
        
    mutex_lock(&can->lock);
    mutex_lock(&stm32f3_can->rx0_lock);

    if (!non_block)
        while (bq_is_empty(&stm32f3_can->inq))
            mutex_wait(&stm32f3_can->rx0_lock);
        
    const int avail_nb_frames = bq_avail_get(&stm32f3_can->inq) / sizeof(can_frame_t);
    if (avail_nb_frames < nb_of_frames)
        nb_of_frames = avail_nb_frames;
    if (nb_of_frames != 0)
        bq_get_array(&stm32f3_can->inq, buffer, nb_of_frames * sizeof(can_frame_t));
        
    mutex_unlock(&stm32f3_can->rx0_lock);
    mutex_unlock(&can->lock);
    return nb_of_frames;
}

static int stm32f3_can_add_filter(canif_t *can, uint32_t mask, uint32_t pattern)
{
    mutex_lock(&can->lock);
    
    CAN->FMR = CAN_FINIT;
    int filter_n = 32 - arm_count_leading_zeroes(CAN->FA1R);
    if (filter_n == 1 && CAN->FILT[0].R1 == 0 && CAN->FILT[0].R2 == 0)
        filter_n = 0;
    
    const unsigned mask_ext = mask & CAN_ID_EXT;
    const unsigned mask_rtr = mask & CAN_ID_RTR;
    const unsigned mask_id = mask & CAN_ID_MASK;
    unsigned f_mask = (mask_ext) ? CAN_EXID(mask_id) | CAN_IDE : CAN_STID(mask_id);
    if (mask_rtr)
        f_mask |= CAN_RTR;
        
    const unsigned pattern_ext = pattern & CAN_ID_EXT;
    const unsigned pattern_rtr = pattern & CAN_ID_RTR;        
    const unsigned pattern_id = pattern & CAN_ID_MASK;
    // В условии проверяется mask_ext, это не ошибка! Мы определяем, какой тип
    // идентификатора требуется по маске, а не по самому идентификатору.
    unsigned f_id = (mask_ext) ? CAN_EXID(pattern_id) : CAN_STID(pattern_id);
    if (pattern_ext)
        f_id |= CAN_IDE;
    if (pattern_rtr)
        f_id |= CAN_RTR;
        
    CAN->FILT[filter_n].R1 = f_id;
    CAN->FILT[filter_n].R2 = f_mask;
    CAN->FA1R |= CAN_FACT(filter_n);
    CAN->FMR = 0;
   
    mutex_unlock(&can->lock);
    return CAN_ERR_OK;
}

static void do_clear_filters()
{
    CAN->FMR = CAN_FINIT;
    CAN->FS1R = 0x3FFF;
    CAN->FILT[0].R1 = 0;
    CAN->FILT[0].R2 = 0;
    CAN->FA1R = CAN_FACT(0);
    CAN->FMR = 0;
}

static int stm32f3_can_clear_filters(canif_t *can)
{
    mutex_lock(&can->lock);
    do_clear_filters();
    mutex_unlock(&can->lock);
    return CAN_ERR_OK;
}

static int stm32f3_can_get_mode(canif_t *can)
{
    if (! (RCC->APB1ENR & RCC_CANEN))
        return IFACE_MODE_SHUTDOWN;
    else if (CAN->MSR & CAN_SLAK)
        return IFACE_MODE_SLEEP;
    else
        return IFACE_MODE_OPERATION;
}

static int stm32f3_can_get_state(canif_t *can)
{
    if (CAN->ESR & CAN_BOFF)
        return CAN_BUS_OFF;
    else if (CAN->ESR & CAN_EPVF)
        return CAN_ERROR_PASSIVE;
    else
        return CAN_ERROR_ACTIVE;
}

static int stm32f3_can_reset(canif_t *can)
{
    mutex_lock(&can->lock);
    if (stm32f3_can_get_mode(can) != IFACE_MODE_SHUTDOWN)
        do_reset();
    mutex_unlock(&can->lock);
    return CAN_ERR_OK;
}

static int stm32f3_can_set_timing(canif_t *can, unsigned baud_rate, unsigned timing)
{
    stm32f3_can_t *stm32f3_can = (stm32f3_can_t *)can;
    
    mutex_lock(&can->lock);
    
    stm32f3_can->baud_rate = baud_rate;
    
    if (timing == 0)
        stm32f3_can->timing = CAN_TIMING_SET_PRS(2) | CAN_TIMING_SET_PS1(2) | 
            CAN_TIMING_SET_PS2(3) | CAN_TIMING_SET_SJW(1);
    else
        stm32f3_can->timing = timing;

    if (stm32f3_can_get_mode(can) != IFACE_MODE_SHUTDOWN)
        do_set_timing(stm32f3_can);
    
    mutex_unlock(&can->lock);
    return CAN_ERR_OK;
}

static void enable_controller(canif_t *can)
{
    RCC->APB1ENR |= RCC_CANEN;
    do_reset();
    
    CAN->MCR |= CAN_ABOM | CAN_TXFP;
    CAN->IER |= CAN_TMEIE | CAN_FMPIE(0);

    stm32f3_can_t *stm32f3_can = (stm32f3_can_t *)can;    
    do_set_timing(stm32f3_can);
    
    do_clear_filters();
}

static int stm32f3_can_switch_mode(canif_t *can, int mode)
{
    mutex_lock(&can->lock);
    
    int cur_mode = stm32f3_can_get_mode(can);
    if (cur_mode == mode) {
        mutex_unlock(&can->lock);
        return CAN_ERR_OK;
    }
        
    switch (mode) {
    case IFACE_MODE_SHUTDOWN:
        RCC->APB1ENR &= ~RCC_CANEN;
        break;
    case IFACE_MODE_SLEEP:
        if (cur_mode == IFACE_MODE_SHUTDOWN)
            enable_controller(can);
        CAN->MCR |= CAN_SLEEP;
        while (! (CAN->MSR & CAN_SLAK))
        break;
    case IFACE_MODE_OPERATION:
        if (cur_mode == IFACE_MODE_SHUTDOWN)
            enable_controller(can);
        CAN->MCR &= ~CAN_SLEEP;
        break;
    default:
        mutex_unlock(&can->lock);
        return CAN_ERR_BAD_PARAM;
    }
    
    mutex_unlock(&can->lock);
    return CAN_ERR_OK;
}

void stm32f3_can_init(stm32f3_can_t *can)
{
    canif_t *canif = to_canif(can);
    canif->set_timing = stm32f3_can_set_timing;
    canif->output = stm32f3_can_output;
    canif->input = stm32f3_can_input;
    canif->add_filter = stm32f3_can_add_filter;
    canif->clear_filters = stm32f3_can_clear_filters;
    canif->get_state = stm32f3_can_get_state;
    canif->switch_mode = stm32f3_can_switch_mode;
    canif->get_mode = stm32f3_can_get_mode;
    canif->reset = stm32f3_can_reset;
    
    bq_init(&can->inq, can->inq_buffer, sizeof(can->inq_buffer));
    
    mutex_attach_irq(&canif->lock, IRQ_USB_HP_CAN1_TX, 0, 0);
    mutex_attach_irq(&can->rx0_lock, IRQ_USB_LP_CAN1_RX0, rx_handler, can);
}

