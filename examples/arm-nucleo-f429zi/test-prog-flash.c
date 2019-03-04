#include <runtime/lib.h>
#include <kernel/uos.h>
#include <timer/timer.h>
#include <stm32f4/flash_bank2.h>

const char *flash_name = "STM32F4 program flash bank2";
stm32f4_flash_bank2_t flash;

#define ERASE_SECTOR    		5
#define SHORT_WRITE_PAGE		0

ARRAY (task, 1000);

timer_t timer;


uint32_t buf[33 * 1024] __attribute__((aligned(8)));

void hello (void *arg)
{
    flashif_t *f = (flashif_t *)arg;
    unsigned i, j;
    int cnt = 0;
    int cnt2;
    unsigned long t0, t1;

    if (flash_connect(f) == FLASH_ERR_OK)
        debug_printf("Found %s, size: %u Kb, nb pages: %u, page size: %d b\n\
            nb sectors: %u, sector size: %d b\n",
            flash_name, (unsigned)(flash_size(f) >> 10),
            flash_nb_pages(f), flash_page_size(f),
            flash_nb_sectors(f), flash_sector_size(f));
    else {
        debug_printf("%s not found\n", flash_name);
        for (;;);
    }

    debug_printf("Erasing all... ");
    t0 = timer_milliseconds(&timer);
    if (flash_erase_all(f) == FLASH_ERR_OK) {
        t1 = timer_milliseconds(&timer);
        debug_printf("OK! took %d ms, rate: %d bytes/sec\n", 
            t1 - t0, 1000 * (int)(flash_size(f) / (t1 - t0)));
    }
    else debug_printf("FAIL!\n");
    
    debug_printf("Checking erasure... 00%%");
    t0 = timer_milliseconds(&timer);
    for (i = 0; i < flash_nb_pages(f); ++i) {
        memset(buf, 0, flash_page_size(f));
        if (flash_read(f, i, 0, buf, flash_page_size(f)) != FLASH_ERR_OK) {
            debug_printf("READ FAILED!\n");
            for (;;);
        }
        for (j = 0; j < flash_page_size(f) / 4; ++j)
            if (buf[j] != 0xFFFFFFFF && buf[j] != 0x00000000) {
                debug_printf("FAIL, page #%d, word#%d, data = %08X\n", 
                    i, j, buf[j]);
                for (;;);
            }
        if (i % 1000 == 0)
            debug_printf("\b\b\b%02d%%", i * 100 / flash_nb_pages(f));
    }
    t1 = timer_milliseconds(&timer);
    debug_printf("\b\b\bOK! took %d ms, rate: %d bytes/sec\n", 
            t1 - t0, 1000 * (int)(flash_size(f) / (t1 - t0)));
            
    debug_printf("Filling memory with counter... 00%%");
    t0 = timer_milliseconds(&timer);
    i = 0;
    for (i = 0; i < flash_nb_pages(f); ++i) {
        for (j = 0; j < flash_page_size(f) / 4; ++j)
            buf[j] = cnt++;

        if (flash_write(f, i, 0, buf, flash_page_size(f)) != FLASH_ERR_OK) {
            debug_printf("FAIL!\n");
            for (;;);
        }
        debug_printf("\b\b\b%02d%%", i * 100 / flash_nb_pages(f));
    }
    t1 = timer_milliseconds(&timer);
    debug_printf("\b\b\bOK! took %d ms, rate: %d bytes/sec\n", 
            t1 - t0, 1000 * (int)(flash_size(f) / (t1 - t0)));

    debug_printf("Checking filling... 00%%");
    t0 = timer_milliseconds(&timer);
    cnt = 0;
    for (i = 0; i < flash_nb_pages(f); ++i) {
        memset(buf, 0, flash_page_size(f));
        if (flash_read(f, i, 0, buf, flash_page_size(f)) != FLASH_ERR_OK) {
            debug_printf("READ FAILED!\n");
            for (;;);
        }
        for (j = 0; j < flash_page_size(f) / 4; ++j)
            if (buf[j] != cnt++) {
                debug_printf("FAIL, page #%d, word#%d, data = %08X\n", 
                    i, j, buf[j]);
                for (;;);
            }
        debug_printf("\b\b\b%02d%%", i * 100 / flash_nb_pages(f));
    }
    t1 = timer_milliseconds(&timer);
    debug_printf("\b\b\bOK! took %d ms, rate: %d bytes/sec\n", 
            t1 - t0, 1000 * (int)(flash_size(f) / (t1 - t0)));

    debug_printf("Erasing sector #%d... ", ERASE_SECTOR);
    t0 = timer_milliseconds(&timer);
    if (flash_erase_sectors(f, ERASE_SECTOR, 1) == FLASH_ERR_OK) {
        t1 = timer_milliseconds(&timer);
        debug_printf("OK! took %d ms, rate: %d bytes/sec\n", 
            t1 - t0, 1000 * (int)(flash_sector_size(f) / (t1 - t0)));
    }
    else debug_printf("FAIL!\n");

    debug_printf("Checking erasure... 00%%");
    t0 = timer_milliseconds(&timer);
    cnt = 0;
    for (i = 0; i < flash_nb_pages(f); ++i) {
        memset(buf, 0, flash_page_size(f));
        if (flash_read(f, i, 0, buf, flash_page_size(f)) != FLASH_ERR_OK) {
            debug_printf("READ FAILED!\n");
            for (;;);
        }
        if (i >= ERASE_SECTOR * flash_nb_pages_in_sector(f) &&
            i < (ERASE_SECTOR + 1) * flash_nb_pages_in_sector(f)) {
            for (j = 0; j < flash_page_size(f) / 4; ++j) {
                if (buf[j] != 0xFFFFFFFF && buf[j] != 0x00000000) {
                    debug_printf("FAIL, page #%d, word#%d, data = %08X\n",
                        i, j, buf[j]);
                    for (;;);
                }
                cnt++;
            }
        } else {
            for (j = 0; j < flash_page_size(f) / 4; ++j)
                if (buf[j] != cnt++) {
                    debug_printf("FAIL, page #%d, word#%d, data = %08X\n",
                        i, j, buf[j]);
                    for (;;);
                }
        }
        debug_printf("\b\b\b%02d%%", i * 100 / flash_nb_pages(f));
    }
    t1 = timer_milliseconds(&timer);
    debug_printf("\b\b\bOK! took %d ms, rate: %d bytes/sec\n", 
            t1 - t0, 1000 * (int)(flash_size(f) / (t1 - t0)));
            
            
    debug_printf("Erasing all... ");
    t0 = timer_milliseconds(&timer);
    if (flash_erase_all(f) == FLASH_ERR_OK) {
        t1 = timer_milliseconds(&timer);
        debug_printf("OK! took %d ms, rate: %d bytes/sec\n", 
            t1 - t0, 1000 * (int)(flash_size(f) / (t1 - t0)));
    }
    else debug_printf("FAIL!\n");
    
    debug_printf("Checking erasure... 00%%");
    t0 = timer_milliseconds(&timer);
    for (i = 0; i < flash_nb_pages(f); ++i) {
        memset(buf, 0, flash_page_size(f));
        if (flash_read(f, i, 0, buf, flash_page_size(f)) != FLASH_ERR_OK) {
            debug_printf("READ FAILED!\n");
            for (;;);
        }
        for (j = 0; j < flash_page_size(f) / 4; ++j) {
            if (buf[j] != 0xFFFFFFFF && buf[j] != 0x00000000) {
                debug_printf("FAIL, page #%d, word#%d, data = %08X\n", 
                    i, j, buf[j]);
                for (;;);
            }
        }
        debug_printf("\b\b\b%02d%%", i * 100 / flash_nb_pages(f));
    }
    t1 = timer_milliseconds(&timer);
    debug_printf("\b\b\bOK! took %d ms, rate: %d bytes/sec\n", 
            t1 - t0, 1000 * (int)(flash_size(f) / (t1 - t0)));
            
    debug_printf("Writing less than 1 page starting at page %d... ", SHORT_WRITE_PAGE);
    memset(buf, 0, sizeof(buf));
    cnt = 0;
    t0 = timer_milliseconds(&timer);
    for (i = 0; i < flash_page_size(f) / 4 / 3; ++i)
		buf[i] = cnt++;

	if (flash_write(f, SHORT_WRITE_PAGE, 0, buf, cnt * 4) != FLASH_ERR_OK) {
		debug_printf("FAIL!\n");
		for (;;);
	}
    t1 = timer_milliseconds(&timer);
    debug_printf("OK! took %d ms, rate: %d bytes/sec\n", 
            t1 - t0, 1000 * (int)(cnt * 4 / (t1 - t0)));

    debug_printf("Checking result... ");
    memset(buf, 0, sizeof(buf));
    t0 = timer_milliseconds(&timer);
    cnt2 = 0;
	if (flash_read(f, SHORT_WRITE_PAGE, 0, buf, cnt * 4) != FLASH_ERR_OK) {
		debug_printf("READ FAILED!\n");
		for (;;);
	}
	for (j = 0; j < cnt; ++j)
		if (buf[j] != cnt2++) {
			debug_printf("FAIL, word#%d, data = %08X\n", j, buf[j]);
			for (;;);
		}
    t1 = timer_milliseconds(&timer);
    debug_printf("OK! took %d ms, rate: %d bytes/sec\n", 
            t1 - t0, 1000 * (int)(cnt * 4 / (t1 - t0)));


    debug_printf("Writing more than 1 page starting at page %d... ", SHORT_WRITE_PAGE + 1);
    cnt = 0;
    t0 = timer_milliseconds(&timer);
    for (i = 0; i < sizeof(buf) / 4; ++i)
		buf[i] = cnt++;

	if (flash_write(f, SHORT_WRITE_PAGE + 1, 0, buf, sizeof(buf)) != FLASH_ERR_OK) {
		debug_printf("FAIL!\n");
		for (;;);
	}
    t1 = timer_milliseconds(&timer);
    debug_printf("OK! took %d ms, rate: %d bytes/sec\n", 
            t1 - t0, 1000 * (int)(sizeof(buf) / (t1 - t0)));

    debug_printf("Checking result... ");
    memset(buf, 0, sizeof(buf));
    t0 = timer_milliseconds(&timer);
    cnt = 0;
	if (flash_read(f, SHORT_WRITE_PAGE + 1, 0, buf, sizeof(buf)) != FLASH_ERR_OK) {
		debug_printf("READ FAILED!\n");
		for (;;);
	}
	for (j = 0; j < sizeof(buf) / 4; ++j)
		if (buf[j] != cnt++) {
			debug_printf("FAIL, word#%d, data = %08X\n", j, buf[j]);
			for (;;);
		}
    t1 = timer_milliseconds(&timer);
    debug_printf("OK! took %d ms, rate: %d bytes/sec\n", 
            t1 - t0, 1000 * (int)(sizeof(buf) / (t1 - t0)));


    debug_printf("TEST FINISHED!\n\n");

    for (;;);
}

void uos_init (void)
{
	debug_printf("\nTesting %s\n", flash_name);

	timer_init (&timer, KHZ, 1);

	stm32f4_flash_bank2_init(&flash, FLASH_BANK2_SIZE_1MBYTE);

	task_create (hello, &flash, "hello", 1, task, sizeof (task));
}
