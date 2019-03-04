// ID: SPO-UOS-milandr-mil-std-1553_bc.c VER: 1.0.0
//
// История изменений:
//
// 1.0.0	Начальная версия
//
#include <runtime/lib.h>
#include <kernel/uos.h>
#include <kernel/internal.h>
#include <milandr/mil-std-1553_setup.h>
#include <milandr/mil-std-1553_bc.h>

//! Режимы передачи командных данных между узлами MIL-STD 1553.
typedef enum {
    MIL_REQUEST_WRITE,           //!< Передача данных КШ->ОУ
    MIL_REQUEST_READ             //!< Передача данных ОУ->КШ
} mil_request_mode_t;

static void copy_to_rxq(milandr_mil1553_t* mil, mil_slot_desc_t slot, uint8_t is_single)
{
    if (mem_queue_is_full(&mil->bc_rxq)) {
        mil->nb_lost++;
    } else {
        unsigned wrc = (slot.words_count == 0 ? 32 : slot.words_count);
#ifdef MIL_RX_PACKETS_WITH_TIMESTAMPS
        uint32_t *que_elem32 = mem_alloc_dirty(mil->pool, 2*wrc + 12);
#else
        uint32_t *que_elem32 = mem_alloc_dirty(mil->pool, 2*wrc + 8);
#endif
        if (!que_elem32) {
            mil->nb_lost++;
            return;
        }
        mem_queue_put(&mil->bc_rxq, que_elem32);
        // Выставляем признак передачи (однократная или циклическая).
        *que_elem32++ = is_single;
#ifdef MIL_RX_PACKETS_WITH_TIMESTAMPS
        // Копируем текущее время
        *que_elem32++ = mil->operation_time;
#endif
        // Копируем дескриптор слота
        *que_elem32++ = slot.raw;
        // Копируем данные слота
        arm_reg_t *preg = &mil->reg->DATA[slot.subaddr * MIL_SUBADDR_WORDS_COUNT];

#if BC_DEBUG
        uint16_t *que_elem_debug = (uint16_t *)que_elem32; // Область данных
        int wordscount = wrc;
#endif

        uint16_t *que_elem = (uint16_t *) que_elem32;  // Область данных
        while (wrc) {
            *que_elem++ = *preg++;
            wrc--;
        }

#if BC_DEBUG
        int i;
        debug_printf("\n");
        debug_printf("wc=%d\n", wordscount);
        for (i=0;i<wordscount;i++) {
            debug_printf("%04x\n", *que_elem_debug++);
        }
        debug_printf("\n");
#endif

    }
}

void start_slot(milandr_mil1553_t *mil, mil_slot_desc_t slot, uint16_t *pdata)
{
#ifdef MIL_DETAILED_TRX_STAT
    mil->tx_stat[slot.addr][slot.subaddr]++;
#endif
    
    if (slot.command.req_pattern == 0 || slot.command.req_pattern == 0x1f) {
//        debug_printf("control command %x %x %x\n", slot.command.command, slot.command.control, slot.command.addr);
        mil->reg->CommandWord1 =
                // Количество слов выдаваемых данных/код команды
                MIL_STD_COMWORD_WORDSCNT_CODE(slot.command.command) |
                // Подадрес приёмника/режим управления
                MIL_STD_COMWORD_SUBADDR_MODE(slot.command.req_pattern) |
                (slot.command.mode == MIL_REQUEST_WRITE ? MIL_STD_COMWORD_BC_RT :MIL_STD_COMWORD_RT_BC) |
                // Адрес приёмника
                MIL_STD_COMWORD_ADDR(slot.command.addr);
        mil->reg->CommandWord2 = 0;
    } else if (slot.transmit_mode == MIL_SLOT_BC_RT) {
        // Режим передачи КШ-ОУ
        mil->reg->CommandWord1 =
                // Количество слов выдаваемых данных
                MIL_STD_COMWORD_WORDSCNT_CODE(slot.words_count) |
                // Подадрес приёмника
                MIL_STD_COMWORD_SUBADDR_MODE(slot.subaddr) |
                // Адрес приёмника
                MIL_STD_COMWORD_ADDR(slot.addr);

        arm_reg_t *preg = &mil->reg->DATA[slot.subaddr * MIL_SUBADDR_WORDS_COUNT];
        if (pdata) {
            unsigned wrdc = (slot.words_count == 0 ? 32 : slot.words_count);
#if BC_DEBUG
        uint16_t *que_elem_debug = pdata;
        int wordscount = wrdc;
#endif
            while (wrdc) {
                *preg++ = *pdata++;
                wrdc--;
            }
#if BC_DEBUG
        int i;
        debug_printf("\n");
        debug_printf("startslot wc=%d\n", wordscount);
        for (i=0;i<wordscount;i++) {
        	debug_printf("(%02d) %04x\n", i, *que_elem_debug++);
        }
        debug_printf("\n");
#endif
        }
    } else if (slot.transmit_mode == MIL_SLOT_RT_BC) {
        // Режим передачи ОУ-КШ
        mil->reg->CommandWord1 =
                // Количество слов принимаемых данных
                MIL_STD_COMWORD_WORDSCNT_CODE(slot.words_count) |
                // Подадрес источника
                MIL_STD_COMWORD_SUBADDR_MODE(slot.subaddr) |
                // Направление передачи: ОУ-КШ
                MIL_STD_COMWORD_RT_BC |
                // Адрес источника
                MIL_STD_COMWORD_ADDR(slot.addr);
    } else {
        // Режим передачи ОУ-ОУ
        mil->reg->CommandWord1 =
                // Количество слов выдаваемых данных
                MIL_STD_COMWORD_WORDSCNT_CODE(slot.words_count) |
                // Подадрес источника
                MIL_STD_COMWORD_SUBADDR_MODE(slot.subaddr_src) |
                // Адрес источника
                MIL_STD_COMWORD_ADDR(slot.addr_src);
        mil->reg->CommandWord2 =
                // Количество слов принимаемых данных
                MIL_STD_COMWORD_WORDSCNT_CODE(slot.words_count) |
                // Подадрес приёмника
                MIL_STD_COMWORD_SUBADDR_MODE(slot.subaddr) |
                // Направление передачи: ОУ-ОУ
                MIL_STD_COMWORD_RT_BC |
                // Адрес приёмника
                MIL_STD_COMWORD_ADDR(slot.addr);
    }

    // Ожидаем освобождение передатчика
    while(mil->reg->CONTROL & MIL_STD_CONTROL_BCSTART) {
        ;
    }
    // Инициировать передачу команды в канал в режиме КШ
    mil->reg->CONTROL |= MIL_STD_CONTROL_BCSTART;
}

void mil_std_1553_bc_handler(milandr_mil1553_t *mil, const unsigned short status, const unsigned short comWrd1, const unsigned short msg)
{
	mil1553_lock(&mil->milif);

    if (status & MIL_STD_STATUS_VALMESS) {
        int wc = 0; 
        if (mil->single_indx < mil->single_cnt) {
            // Была передача вне очереди
            if (mil->single_slots[mil->single_indx].command.req_pattern == 0 || mil->single_slots[mil->single_indx].command.req_pattern == 0x1f) {
                mil->nb_commands++;
            } else {
                if (mil->pool && mil->single_slots[mil->single_indx].transmit_mode == MIL_SLOT_RT_BC) {
                    copy_to_rxq(mil, mil->single_slots[mil->single_indx], 1);
                }
                wc = mil->single_slots[mil->single_indx].words_count;
                mil->nb_words += (wc>0?wc:32);
            }
#ifdef MIL_DETAILED_TRX_STAT
            mil->rx_stat[mil->single_slots[mil->single_indx].command.addr][mil->single_slots[mil->single_indx].command.req_pattern]++;
#endif
        } else {
            if (mil->cur_slot != 0) {
                mil_slot_desc_t slot = mil->cur_slot->desc;
                if (slot.command.req_pattern == 0 || slot.command.req_pattern == 0x1f) {
                    mil->nb_commands++;
                } else {
                    wc = slot.words_count;
                    if (mil->pool && slot.transmit_mode == MIL_SLOT_RT_BC) {
                        copy_to_rxq(mil, slot, 0);
                    }
                    mil->nb_words += (wc>0?wc:32);
                }
#ifdef MIL_DETAILED_TRX_STAT
            mil->rx_stat[slot.addr][slot.subaddr]++;
#endif
            }
        }

    } else if (status & MIL_STD_STATUS_ERR) {
	    mil->nb_errors++;
	    if (mil->single_indx < mil->single_cnt) {
	    	mil->nb_emergency_errors++;
	    }
	}

	if (mil->cur_slot != 0)
		mil->cur_slot = mil->cur_slot->next;
	
    if (mil->cur_slot == 0)
		mil->cur_slot = mil->cyclogram;

	if (mil->single_indx < mil->single_cnt) { // Есть однократные сообщения для передачи
		start_slot(mil, mil->single_slots[mil->single_indx], mil->single_data + mil->single_indx * MIL_SUBADDR_WORDS_COUNT);
		++mil->single_indx;
	} else if ((mil->cur_slot != mil->cyclogram) || (mil->tim_reg == 0) || (mil->period_ms == 0)) {
		// если таймер не задан, или его период равен нулю циклограмма начинается с начала
		if (mil->cur_slot != 0) {
			start_slot(mil, mil->cur_slot->desc, mil->cur_slot->data);
		}
	}

    mil1553_unlock(&mil->milif);
}


