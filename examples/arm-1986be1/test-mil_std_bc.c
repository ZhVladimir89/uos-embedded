#include <runtime/lib.h>
#include <kernel/uos.h>
#include <stream/stream.h>
#include <kernel/internal.h>
#include <milandr/mil1553-interface.h>
#include <milandr/mil-std-1553_setup.h>
#include <milandr/mil-std-1553_bc.h>


// Номер контроллера MIL-STD (всегда 0)
#define MY_MIL_STD_PORT 0
// Канал основной или резервный (0 или 1)
#define MY_MIL_STD_CHANNEL 0
// Минимальный период в циклограмме или продолжительность одного слота (в миллисекундах)
#define CYCLOGRAM_SLOT_TIME 200
// Количество слотов в циклограмме
#define CYCLOGRAM_SLOTS_COUNT 2
// Длина приемной очереди
#define MIL_RXQ_LEN	100

ARRAY (test_milstd_bc_stack, 1000);
mem_pool_t pool;

static void test_milstd_bc_main();

static milandr_mil1553_t mil;
static mil_slot_t *cyclogram;
static unsigned cyclogram_nb_slots = 0;
static uint16_t slot_data[32];

// debug
// Адрес ОУ
static const unsigned char rt_addr = 2;
// Подадрес для получения данных от ОУ (ОУ-КШ)
static const unsigned char rt_subaddr_from = 7;
// Подадрес для передачи данных в ОУ (КШ-ОУ)
static const unsigned char rt_subaddr_to = 8;

void uos_init (void)
{
    debug_printf("bc_init\n");

    milandr_mil1553_init(&mil, 0, &pool, MIL_RXQ_LEN, ARM_TIMER1);
    milandr_mil1553_init_pins(0);

    task_create(test_milstd_bc_main, 0, "test_milstd_bc_main", 1, test_milstd_bc_stack, sizeof(test_milstd_bc_stack));
}

static int add_slot(mil_slot_desc_t cyclogram_data)
{
	mil_slot_t *slot = mem_alloc_dirty(mil.pool, sizeof(mil_slot_t));
	slot->desc.raw = cyclogram_data.raw;
	int wc = (slot->desc.words_count==0?32:slot->desc.words_count);
	slot->data = mem_alloc(mil.pool, wc * 2);
	if (!slot->data) {
		mem_free(slot);
	    debug_printf("No memory\n");
		return -1;
	}

	if (cyclogram == 0) {
		cyclogram = slot;
	} else {
		mil_slot_t *sl = cyclogram;
		while (sl->next)
			sl = sl->next;
		sl->next = slot;
	}
	slot->next = 0;

	if (mil1553_bc_set_cyclogram(&mil.milif, cyclogram, ++cyclogram_nb_slots) != MIL_ERR_OK) {
		debug_printf("Error setting cyclogram\n");
		return -1;
	}
	return 0;
}

static int fill_buffer(mem_queue_t *queue)
{
	int i;
    for(;;) {
        if (mem_queue_is_empty(queue)) {
            break;
        }
        uint32_t *que_elem = mem_queue_current(queue);
        mil_slot_desc_t desc;
        desc.raw = *(que_elem + 1);
        int wc = (desc.words_count == 0 ? 32 : desc.words_count);

        uint16_t *ptr = (uint16_t *)(que_elem + 2);
        debug_printf("wc=%02d\n", wc);
        for (i=0;i<wc;i++) {
        	debug_printf(" %04x", *ptr++);
        }
        debug_printf("\n");

        mem_queue_get(queue, (void*)&que_elem);
        mem_free(que_elem);

    }
    return 1;
}


static void test_milstd_bc_main()
{
#if MY_MIL_STD_CHANNEL==0
	mil1553_set_mode(&mil.milif, MIL_MODE_BC_MAIN);
#else
	mil1553_set_mode(&mil.milif, MIL_MODE_BC_RSRV);
#endif

	mil_slot_desc_t cyclogram_data;

	memset(&cyclogram_data, 0, sizeof(cyclogram_data));

	cyclogram_data.transmit_mode = MIL_SLOT_BC_RT;
    cyclogram_data.addr = rt_addr;
    cyclogram_data.subaddr = rt_subaddr_to;
    cyclogram_data.words_count = 3;

    if (add_slot(cyclogram_data)<0) {
    	for(;;);
    }

    cyclogram_data.transmit_mode = MIL_SLOT_RT_BC;
    cyclogram_data.addr = rt_addr;
    cyclogram_data.subaddr = rt_subaddr_from;
    cyclogram_data.words_count = 3;

    if (add_slot(cyclogram_data)<0) {
    	for(;;);
    }

    mil1553_bc_set_period(&mil.milif, CYCLOGRAM_SLOT_TIME);
    mil1553_start(&mil.milif);

    int counter = 0;
    for (;;) {
		if (status_array[read_idx].done) {
			uint32_t status = status_array[read_idx].status;
			switch(mil.mode) {
				case MIL_MODE_BC_MAIN:
				case MIL_MODE_BC_RSRV:
					mil_std_1553_bc_handler(&mil, status, status_array[read_idx].command_word_1, status_array[read_idx].msg);
					//debug_printf("status_word_1 %08x\n", status_array[read_idx].status_word_1);
					break;
				case MIL_MODE_RT:
					break;
				default:
					break;
			}
			status_array[read_idx].done = 0;
			read_idx = (read_idx+1>=STATUS_ITEMS_SIZE?0:read_idx+1);
		}

		if (!mem_queue_is_empty(&mil.bc_rxq)) {
			if (!fill_buffer(&mil.bc_rxq)) {
				debug_printf("Error gettinng rx data\n");
				for(;;);
			}
        }

		counter++;
		slot_data[0] = counter;
		if (mil1553_bc_ordinary_send(&mil.milif, 0, (void*)slot_data) != MIL_ERR_OK) {
			debug_printf("Error sending data\n");
			for(;;);
		}
    }
}
