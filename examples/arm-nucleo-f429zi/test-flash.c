#include <runtime/lib.h>
#include <kernel/uos.h>
#include <timer/timer.h>
#include <stm32f4/spi.h>
#include <stm32f4/gpio.h>


//#define M25PXX
#define AT45DBXX
//#define SDHC_SPI

#if defined(M25PXX)
    #include <flash/m25pxx.h>
    const char *flash_name = "M25Pxx";
    m25pxx_t flash;
#elif defined(AT45DBXX)
    #include <flash/at45dbxx.h>
    const char *flash_name = "AT45DBxx";
    at45dbxx_t flash;
#elif defined(SDHC_SPI)
    #include <flash/sdhc-spi.h>
    const char *flash_name = "SD over SPI";
    sdhc_spi_t flash;
#endif

#define SPI_FREQUENCY   20000000

#define ERASE_SECTOR    5

ARRAY (task, 1000);
stm32f4_spim_t spi;
stm32f4_gpio_t pin_miso;
stm32f4_gpio_t pin_mosi;
stm32f4_gpio_t pin_clk;
stm32f4_gpio_t pin_cs;
timer_t timer;

uint32_t buf[1024] __attribute__((aligned(8)));

void hello (void *arg)
{
    flashif_t *f = (flashif_t *)arg;
    unsigned i, j;
    int cnt = 0;
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
    
    // DEBUG
    //f->nb_sectors = 2;

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
        if (i % 1000 == 0)
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
        if (i % 1000 == 0)
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
        if (i % 1000 == 0)
            debug_printf("\b\b\b%02d%%", i * 100 / flash_nb_pages(f));
    }
    t1 = timer_milliseconds(&timer);
    debug_printf("\b\b\bOK! took %d ms, rate: %d bytes/sec\n", 
            t1 - t0, 1000 * (int)(flash_size(f) / (t1 - t0)));

    debug_printf("TEST FINISHED!\n\n");

    for (;;);
}

void spi_cs_control(unsigned port, unsigned cs_num, int level)
{
    gpio_set_val(to_gpioif(&pin_cs), level);
}

void uos_init (void)
{
	debug_printf("\nTesting %s\n", flash_name);

	stm32f4_gpio_init(&pin_cs,   GPIO_PORT_D, 14, GPIO_FLAGS_OUTPUT | GPIO_FLAGS_100MHZ);
	gpio_set_val(to_gpioif(&pin_cs), 1);
	stm32f4_gpio_init(&pin_clk,  GPIO_PORT_A,  5, GPIO_FLAGS_ALT_SPI1 | GPIO_FLAGS_100MHZ);
	stm32f4_gpio_init(&pin_miso, GPIO_PORT_A,  6, GPIO_FLAGS_ALT_SPI1 | GPIO_FLAGS_100MHZ);
	stm32f4_gpio_init(&pin_mosi, GPIO_PORT_A,  7, GPIO_FLAGS_ALT_SPI1 | GPIO_FLAGS_100MHZ);
	
	timer_init(&timer, KHZ, 1);
	
    stm32f4_spi_init(&spi, 1, spi_cs_control);
    
#if defined(M25PXX)
    m25pxx_init(&flash, (spimif_t *)&spi, SPI_FREQUENCY, 0);
#elif defined(AT45DBXX)
    at45dbxx_init(&flash, (spimif_t *)&spi, SPI_FREQUENCY, 0);
#elif defined(SDHC_SPI)
    sd_spi_init(&flash, (spimif_t *)&spi, 0);
#endif

	task_create (hello, &flash, "hello", 1, task, sizeof (task));
}
