#include <runtime/lib.h>
#include <kernel/uos.h>
#include <flash/at45dbxx.h>

#define AT45_CMD_MAIN_MEMORY_PAGE_READ          0xD2
#define AT45_CMD_CONT_ARRAY_READ_LEGACY         0xE8
#define AT45_CMD_CONT_ARRAY_READ_LOW_FREQ       0x03
#define AT45_CMD_CONT_ARRAY_READ_HIGH_FREQ      0x0B
#define AT45_CMD_BUFFER_1_READ_LOW_FREQ         0xD1
#define AT45_CMD_BUFFER_2_READ_LOW_FREQ         0xD3
#define AT45_CMD_BUFFER_1_READ_HIGH_FREQ        0xD4
#define AT45_CMD_BUFFER_2_READ_HIGH_FREQ        0xD6
#define AT45_CMD_BUFFER_1_WRITE                 0x84
#define AT45_CMD_BUFFER_2_WRITE                 0x87
#define AT45_CMD_BUFFER_1_PROGRAM_WITH_ERASE    0x83
#define AT45_CMD_BUFFER_2_PROGRAM_WITH_ERASE    0x86
#define AT45_CMD_BUFFER_1_PROGRAM_WITHOUT_ERASE 0x88
#define AT45_CMD_BUFFER_2_PROGRAM_WITHOUT_ERASE 0x89
#define AT45_CMD_PAGE_ERASE                     0x81
#define AT45_CMD_BLOCK_ERASE                    0x50
#define AT45_CMD_SECTOR_ERASE                   0x7C
#define AT45_CMD_PAGE_PROGRAM_THROUGH_BUFFER_1  0x82
#define AT45_CMD_PAGE_PROGRAM_THROUGH_BUFFER_2  0x85
#define AT45_CMD_READ_SECTOR_PROTECTION_REG     0x32
#define AT45_CMD_READ_SECTOR_LOCKDOWN_REG       0x35
#define AT45_CMD_READ_SECURITY_REG              0x77
#define AT45_CMD_PAGE_TO_BUFFER_1_TRANSFER      0x53
#define AT45_CMD_PAGE_TO_BUFFER_2_TRANSFER      0x55
#define AT45_CMD_PAGE_TO_BUFFER_1_COMPARE       0x60
#define AT45_CMD_PAGE_TO_BUFFER_2_COMPARE       0x61
#define AT45_CMD_AUTO_PAGE_REWRITE_BUFFER_1     0x58
#define AT45_CMD_AUTO_PAGE_REWRITE_BUFFER_2     0x59
#define AT45_CMD_DEEP_POWER_DOWN                0xB9
#define AT45_CMD_RESUME_FROM_DEEP_POWER_DOWN    0xAB
#define AT45_CMD_STATUS_REG_READ                0xD7
#define AT45_CMD_MANUF_AND_DEVICE_ID_READ       0x9F

#define AT45_STATUS_PAGE_SIZE   (1 << 0)
#define AT45_STATUS_PROTECT     (1 << 1)
#define AT45_STATUS_COMP        (1 << 6)
#define AT45_STATUS_RDY         (1 << 7)

static int read_status(at45dbxx_t *m, uint8_t *status)
{
    m->databuf[0] = AT45_CMD_STATUS_REG_READ;
    m->msg.tx_data = m->databuf;
    m->msg.rx_data = m->databuf;
    m->msg.word_count = 2;
    m->msg.mode &= ~SPI_MODE_CS_HOLD;
    if (spim_trx(m->spi, &m->msg) != SPI_ERR_OK)
        return FLASH_ERR_IO;

    *status = m->databuf[1];

    if ((*status & 0x3C) != 0x1C)
        return FLASH_ERR_IO;

    return FLASH_ERR_OK;
}

static int at45dbxx_connect(flashif_t *flash)
{
    at45dbxx_t *m = (at45dbxx_t *) flash;
    mutex_lock(&flash->lock);

    m->databuf[0] = AT45_CMD_MANUF_AND_DEVICE_ID_READ;
    m->msg.tx_data = m->databuf;
    m->msg.rx_data = m->databuf;
    m->msg.word_count = 3;
    m->msg.mode &= ~SPI_MODE_CS_HOLD;
    if (spim_trx(m->spi, &m->msg) != SPI_ERR_OK) {
        mutex_unlock(&flash->lock);
        return FLASH_ERR_IO;
    }

    if ((m->databuf[1] != 0x1F) || ((m->databuf[2] & 0xE0) != 0x20)) {
        mutex_unlock(&flash->lock);
        return FLASH_ERR_NOT_CONN;
    }

    uint8_t status;
    int res = read_status(m, &status);
    if (res != FLASH_ERR_OK) {
        mutex_unlock(&flash->lock);
        return res;
    }

	unsigned size;
    if (status & AT45_STATUS_PAGE_SIZE) {
		size = 32768 << (m->databuf[2] & 0x1F);
        flash->nb_sectors = size / 2048;
    } else {
        size = 33792 << (m->databuf[2] & 0x1F);
        flash->nb_sectors = size / 2112;
    }
    flash->nb_pages_in_sector = 8;
    flash->page_size = size / flash->nb_sectors /
		flash->nb_pages_in_sector;
    flash->min_address = 0;

    mutex_unlock(&flash->lock);
    return FLASH_ERR_OK;
}

static unsigned at45dbxx_page_address(flashif_t *flash, unsigned page_num)
{
    if (flash_page_size(flash) == 264)
        return page_num << 9;
    else return page_num << 8;
}

static unsigned at45dbxx_sector_address(flashif_t *flash, unsigned sector_num)
{
    if (flash_page_size(flash) == 264)
        return sector_num << 12;
    else return sector_num << 11;
}

static int at45dbxx_erase_all(flashif_t *flash)
{
    int res;
    uint8_t status;
    at45dbxx_t *m = (at45dbxx_t *) flash;
    mutex_lock(&flash->lock);

    m->databuf[0] = 0xC7;
    m->databuf[1] = 0x94;
    m->databuf[2] = 0x80;
    m->databuf[3] = 0x9A;
    m->msg.tx_data = m->databuf;
    m->msg.rx_data = 0;
    m->msg.word_count = 4;
    m->msg.mode &= ~SPI_MODE_CS_HOLD;
    if (spim_trx(m->spi, &m->msg) != SPI_ERR_OK) {
        mutex_unlock(&flash->lock);
        return FLASH_ERR_IO;
    }

    while (1) {
        res = read_status(m, &status);
        if (res != FLASH_ERR_OK) {
            mutex_unlock(&flash->lock);
            return res;
        }
        if (status & AT45_STATUS_RDY) break;
    }

    mutex_unlock(&flash->lock);
    return FLASH_ERR_OK;
}

static int erase_sector(flashif_t *flash, unsigned sector_num)
{
    int res;
    uint8_t status;
    at45dbxx_t *m = (at45dbxx_t *) flash;

    unsigned address = at45dbxx_sector_address(flash, sector_num);
    uint8_t *p = (uint8_t *) &address;
    m->databuf[0] = AT45_CMD_BLOCK_ERASE;
    m->databuf[1] = p[2];
    m->databuf[2] = p[1];
    m->databuf[3] = p[0];
    m->msg.tx_data = m->databuf;
    m->msg.rx_data = 0;
    m->msg.word_count = 4;
    m->msg.mode &= ~SPI_MODE_CS_HOLD;
    if (spim_trx(m->spi, &m->msg) != SPI_ERR_OK)
        return FLASH_ERR_IO;

    while (1) {
        res = read_status(m, &status);
        if (res != FLASH_ERR_OK)
            return res;

        if (status & AT45_STATUS_RDY) break;
    }

    return FLASH_ERR_OK;
}

static int at45dbxx_erase_sectors(flashif_t *flash, unsigned sector_num,
    unsigned nb_sectors)
{
    int res;
    int i;
    mutex_lock(&flash->lock);
    for (i = 0; i < nb_sectors; ++i) {
        res = erase_sector(flash, sector_num + i);
        if (res != FLASH_ERR_OK) {
            mutex_unlock(&flash->lock);
            return res;
        }
    }
    mutex_unlock(&flash->lock);
    return FLASH_ERR_OK;
}

static int write_one_page(flashif_t *flash, unsigned address, 
                            void *data, unsigned size)
{
    int res;
    uint8_t status;
    at45dbxx_t *m = (at45dbxx_t *) flash;

    uint8_t *p = (uint8_t *) &address;
    m->databuf[0] = AT45_CMD_BUFFER_1_WRITE;
    m->databuf[1] = p[2];
    m->databuf[2] = p[1];
    m->databuf[3] = p[0];
    m->msg.tx_data = m->databuf;
    m->msg.rx_data = 0;
    m->msg.word_count = 4;
    m->msg.mode |= SPI_MODE_CS_HOLD;
    if (spim_trx(m->spi, &m->msg) != SPI_ERR_OK)
        return FLASH_ERR_IO;

    m->msg.tx_data = data;
    m->msg.rx_data = 0;
    m->msg.word_count = size;
    m->msg.mode &= ~SPI_MODE_CS_HOLD;
    if (spim_trx(m->spi, &m->msg) != SPI_ERR_OK)
        return FLASH_ERR_IO;

    m->databuf[0] = AT45_CMD_BUFFER_1_PROGRAM_WITHOUT_ERASE;
    m->msg.tx_data = m->databuf;
    m->msg.rx_data = 0;
    m->msg.word_count = 4;
    //m->msg.mode &= ~SPI_MODE_CS_HOLD;
    if (spim_trx(m->spi, &m->msg) != SPI_ERR_OK)
        return FLASH_ERR_IO;

    while (1) {
        res = read_status(m, &status);
        if (res != FLASH_ERR_OK)
            return res;
        if (status & AT45_STATUS_RDY) break;
    }

    return FLASH_ERR_OK;
}

static int read_one_page(flashif_t *flash, unsigned address, 
                            void *data, unsigned size)
{
    at45dbxx_t *m = (at45dbxx_t *) flash;

    uint8_t *p = (uint8_t *) &address;
    m->databuf[0] = AT45_CMD_CONT_ARRAY_READ_HIGH_FREQ;
    m->databuf[1] = p[2];
    m->databuf[2] = p[1];
    m->databuf[3] = p[0];
    m->databuf[4] = 0;
    m->msg.tx_data = m->databuf;
    m->msg.rx_data = 0;
    m->msg.word_count = 5;
    m->msg.mode |= SPI_MODE_CS_HOLD;
    if (spim_trx(m->spi, &m->msg) != SPI_ERR_OK)
        return FLASH_ERR_IO;

    m->msg.tx_data = 0;
    m->msg.rx_data = data;
    m->msg.word_count = size;
    m->msg.mode &= ~SPI_MODE_CS_HOLD;
    if (spim_trx(m->spi, &m->msg) != SPI_ERR_OK)
        return FLASH_ERR_IO;

    return FLASH_ERR_OK;
}

typedef int (* io_func)(flashif_t *flash, unsigned address, 
                        void *data, unsigned size);
                
static int cyclic_func(flashif_t *flash, unsigned address,
                        void *data, unsigned size, io_func func)
{
    int res;
    unsigned cur_size = size;
    uint8_t *cur_data = data;
    
    mutex_lock(&flash->lock);
    
    while (size > 0) {
        cur_size = (size < flash_page_size(flash)) ?
            size : flash_page_size(flash);
        res = func(flash, address, cur_data, cur_size);
        if (res != FLASH_ERR_OK) {
            mutex_unlock(&flash->lock);
            return res;
        }
        size -= cur_size;
        address += cur_size;
        cur_data += cur_size;
    }
    
    mutex_unlock(&flash->lock);
    return FLASH_ERR_OK;
}

static int at45dbxx_write(flashif_t *flash, unsigned page_num, unsigned offset,
                        void *data, unsigned size)
{
    return cyclic_func(flash, at45dbxx_page_address(flash, page_num) + offset,
        data, size, write_one_page);
}

static int at45dbxx_read(flashif_t *flash, unsigned page_num, unsigned offset,
                        void *data, unsigned size)
{
    return cyclic_func(flash, at45dbxx_page_address(flash, page_num) + offset,
        data, size, read_one_page);
}

void at45dbxx_init(at45dbxx_t *m, spimif_t *s, unsigned freq, unsigned mode)
{
    m->spi = s;
    flashif_t *f = &m->flashif;

    f->connect = at45dbxx_connect;
    f->erase_all = at45dbxx_erase_all;
    f->erase_sectors = at45dbxx_erase_sectors;
    f->write = at45dbxx_write;
    f->read = at45dbxx_read;

    m->msg.freq = freq;
    m->msg.mode = (mode & 0xFF07) | SPI_MODE_NB_BITS(8);
}
