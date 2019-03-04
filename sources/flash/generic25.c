//
// Общий драйвер для микросхем флеш-памяти 25-й серии.
//
#include <runtime/lib.h>
#include <kernel/uos.h>
#include "generic25.h"

#define CMD_WSR                 0x01    // Write Status Registers
#define CMD_PP                  0x02    // Page Program
#define CMD_READ                0x03    // Read
#define CMD_WRDI                0x04    // Write Disable 
#define CMD_RDSR1               0x05    // Read Status Register-1 
#define CMD_WREN                0x06    // Write Enable 
#define CMD_FAST_READ           0x0B    // Fast Read
#define CMD_E4K                 0x20    // 4 kB-sector Erase
#define CMD_RDSR3               0x33    // Read Status Register-3
#define CMD_RDSR2               0x35    // Read Status Register-2
#define CMD_SBPP                0x39    // Set Block / Pointer Protection (S25FL132K / S25FL164K)
#define CMD_DOR                 0x3B    // Fast Read Dual Output
#define CMD_PSR                 0x42    // Program Security Registers
#define CMD_ESR                 0x44    // Erase Security Registers
#define CMD_RSR                 0x48    // Read Security Registers
#define CMD_WEVSR               0x50    // Write Enable for Volatile Status Register
#define CMD_RSFDPR_RUID         0x5A    // Read SFDP Register / Read Unique ID Number
#define CMD_CE_1                0x60    // Chip Erase
#define CMD_SRE                 0x66    // Software Reset Enable
#define CMD_QOR                 0x6B    // Fast Read Quad Output
#define CMD_ERSP                0x75    // Erase / Program Suspend
#define CMD_SBWW                0x77    // Set Burst with Wrap
#define CMD_ERRS                0x7A    // Erase / Program Resume
#define CMD_READ_ID             0x90    // Read Electronic Manufacturer Signature 
#define CMD_SR                  0x99    // Software Reset
#define CMD_RDID                0x9F    // Read ID (JEDEC Manufacturer ID and JEDEC CFI) 
#define CMD_RES                 0xAB    // Release from Deep-Power-Down / Device ID
#define CMD_E4ADDR              0xB7    // Enable 4-bytes address mode
#define CMD_DPD                 0xB9    // Deep-Power-Down
#define CMD_DIOR                0xBB    // Dual I/O Read
#define CMD_CE_2                0xC7    // Chip Erase (alternate command)
#define CMD_E64K                0xD8    // Block Erase 64 kB
#define CMD_D4ADDR              0xE9    // Disable 4-bytes address mode
#define CMD_QIOR                0xEB    // Quad I/O Read
#define CMD_CRMR                0xFF    // Continuous Read Mode Reset

// Status Register 1 (SR1)
#define SR1_BUSY                (1 << 0) // BUSY
#define SR1_WEL                 (1 << 1) // Write Enable Latch 
#define SR1_BP0                 (1 << 2) // Block Protection 
#define SR1_BP1                 (1 << 3) // Block Protection 
#define SR1_BP2                 (1 << 4) // Block Protection 
#define SR1_TB                  (1 << 5) // Top / Bottom Protect
#define SR1_SEC                 (1 << 6) // Sector / Block Protect
#define SR1_SRP0                (1 << 7) // Status Register Protect

// Status Register 2 (SR2)
#define SR2_SRP1                (1 << 0) // Status Register Protect
#define SR2_QE                  (1 << 1) // Quad Enable
#define SR2_LB0                 (1 << 2) // Security Register Lock Bit 0
#define SR2_LB1                 (1 << 3) // Security Register Lock Bit 1
#define SR2_LB2                 (1 << 4) // Security Register Lock Bit 2
#define SR2_LB3                 (1 << 5) // Security Register Lock Bit 3
#define SR2_CMP                 (1 << 6) // Complement Protect
#define SR2_SUS                 (1 << 7) // Suspend Status

// Status Register 3 (SR3)
#define SR3_LC0                 (1 << 0) // Variable Read Latency Control
#define SR3_LC1                 (1 << 1) // Variable Read Latency Control
#define SR3_LC2                 (1 << 2) // Variable Read Latency Control
#define SR3_LC3                 (1 << 3) // Variable Read Latency Control
#define SR3_W4                  (1 << 4) // Burst Wrap Enable
#define SR3_W5                  (1 << 5) // Burst Wrap Length 0
#define SR3_W6                  (1 << 6) // Burst Wrap Length 1
#define SR3_RFU                 (1 << 7) // Reserved


static int enable_write(generic25_flash_t *m)
{
    m->databuf[0] = CMD_WREN;
    m->msg.tx_data = m->databuf;
    m->msg.rx_data = 0;
    m->msg.word_count = 1;
    m->msg.mode &= ~SPI_MODE_CS_HOLD;
    if (spim_trx(m->spi, &m->msg) == SPI_ERR_OK)
        return FLASH_ERR_OK;
    else return FLASH_ERR_IO;
}

static int generic25_flash_connect(flashif_t *flash)
{
    generic25_flash_t *m = (generic25_flash_t *) flash;

    mutex_lock(&flash->lock);

    m->databuf[0] = CMD_RDID;
    m->msg.tx_data = m->databuf;
    m->msg.rx_data = 0;
    m->msg.word_count = 1;
    m->msg.mode |= SPI_MODE_CS_HOLD;
    if (spim_trx(m->spi, &m->msg) != SPI_ERR_OK)
        return FLASH_ERR_IO;

    m->msg.tx_data = 0;
    m->msg.rx_data = m->databuf;
    m->msg.word_count = 3;
    m->msg.mode &= ~SPI_MODE_CS_HOLD;
    if (spim_trx(m->spi, &m->msg) != SPI_ERR_OK)
        return FLASH_ERR_IO;

    // проверка ID производителя и ID микросхемы
    /*
    if ((m->databuf[0] != 0xEF) || (m->databuf[1] != 0x40)) {
    mutex_unlock(&flash->lock);
    return FLASH_ERR_NOT_CONN;
    }
    */

    uint32_t size = 1 << m->databuf[2];
    flash->nb_sectors = size / ( 4*1024 );
    flash->page_size = 256;
    flash->nb_pages_in_sector = 16;

    flash->min_address = 0;
    
    if (flash_size(flash) > 16 * 1024 * 1024) {
        int res;
        
        res = enable_write(m);
        if (res != FLASH_ERR_OK) {
            mutex_unlock(&flash->lock);
            return res;
        }
        
        m->databuf[0] = CMD_E4ADDR;
        m->msg.tx_data = m->databuf;
        m->msg.rx_data = 0;
        m->msg.word_count = 1;
        if (spim_trx(m->spi, &m->msg) != SPI_ERR_OK)
            return FLASH_ERR_IO;
        
        m->addr4bytes = 1;
    }

    mutex_unlock(&flash->lock);
    return FLASH_ERR_OK;
}

static int read_sr1(generic25_flash_t *m, uint8_t *sr1)
{
    m->databuf[0] = CMD_RDSR1;
    m->msg.tx_data = m->databuf;
    m->msg.rx_data = 0;
    m->msg.word_count = 1;
    m->msg.mode |= SPI_MODE_CS_HOLD;
    if (spim_trx(m->spi, &m->msg) != SPI_ERR_OK)
        return FLASH_ERR_IO;

    m->msg.tx_data = 0;
    m->msg.rx_data = m->databuf;
    m->msg.word_count = 1;
    m->msg.mode &= ~SPI_MODE_CS_HOLD;
    if (spim_trx(m->spi, &m->msg) != SPI_ERR_OK)
        return FLASH_ERR_IO;

    *sr1 = m->databuf[0];

    return FLASH_ERR_OK;
}

static int generic25_flash_erase_all(flashif_t *flash)
{
    int res;
    uint8_t status;
    generic25_flash_t *m = (generic25_flash_t *) flash;
    mutex_lock(&flash->lock);
    
    res = enable_write(m);
    if (res != FLASH_ERR_OK) {
        mutex_unlock(&flash->lock);
        return res;
    }
    
    m->databuf[0] = CMD_CE_2;
    m->msg.tx_data = m->databuf;
    m->msg.rx_data = 0;
    m->msg.word_count = 1;
    m->msg.mode &= ~SPI_MODE_CS_HOLD;
    if (spim_trx(m->spi, &m->msg) != SPI_ERR_OK) {
        mutex_unlock(&flash->lock);
        return FLASH_ERR_IO;
    }

    while (1) {
        res = read_sr1(m, &status);
        if (res != FLASH_ERR_OK) {
            mutex_unlock(&flash->lock);
            return res;
        }
        if (! (status & SR1_BUSY)) break;
    }

    mutex_unlock(&flash->lock);
    return FLASH_ERR_OK;
}

static int erase_4k(flashif_t *flash, unsigned sector_num)
{
    int res;
    uint8_t status;
    generic25_flash_t *m = (generic25_flash_t *) flash;

    res = enable_write(m);
    if (res != FLASH_ERR_OK) {
        mutex_unlock(&flash->lock);
        return res;
    }

    uint32_t address = sector_num * 4 *1024;
    uint8_t* p = (uint8_t *) &address;
    m->databuf[0] = CMD_E4K;
    if (m->addr4bytes) {
        m->databuf[1] = p[3];
        m->databuf[2] = p[2];
        m->databuf[3] = p[1];
        m->databuf[4] = p[0];
        m->msg.word_count = 5;
    } else {
        m->databuf[1] = p[2];
        m->databuf[2] = p[1];
        m->databuf[3] = p[0];
        m->msg.word_count = 4;
    }
    m->msg.tx_data = m->databuf;
    m->msg.rx_data = 0;
    m->msg.mode &= ~SPI_MODE_CS_HOLD;
    if (spim_trx(m->spi, &m->msg) != SPI_ERR_OK) {
        mutex_unlock(&flash->lock);
        return FLASH_ERR_IO;
    }

    while (1) {
        res = read_sr1(m, &status);
        if (res != FLASH_ERR_OK) {
            mutex_unlock(&flash->lock);
            return res;
        }

        if (! (status & SR1_BUSY)) break;
    }

    return FLASH_ERR_OK;
}

/*
static int erase_64k(flashif_t *flash, unsigned sector_num)
{
    int res;
    uint8_t status;
    generic25_flash_t *m = (generic25_flash_t *) flash;

    res = enable_write(m);
    if (res != FLASH_ERR_OK) {
        mutex_unlock(&flash->lock);
        return res;
    }

    uint32_t address = sector_num * 64 * 1024;
    uint8_t *p = (uint8_t *) &address;
    m->databuf[0] = CMD_E64K;
    if (m->addr4bytes) {
        m->databuf[1] = p[3];
        m->databuf[2] = p[2];
        m->databuf[3] = p[1];
        m->databuf[3] = p[0];
        m->msg.word_count = 5;
    } else {
        m->databuf[1] = p[2];
        m->databuf[2] = p[1];
        m->databuf[3] = p[0];
        m->msg.word_count = 4;
    }
    m->msg.tx_data = m->databuf;
    m->msg.rx_data = 0;
    m->msg.mode &= ~SPI_MODE_CS_HOLD;
    if (spim_trx(m->spi, &m->msg) != SPI_ERR_OK) {
        mutex_unlock(&flash->lock);
        return FLASH_ERR_IO;
    }

    while (1) {
        res = read_sr1(m, &status);
        if (res != FLASH_ERR_OK) {
            mutex_unlock(&flash->lock);
            return res;
        }

        if (! (status & SR1_BUSY)) break;
    }

    return FLASH_ERR_OK;
}
*/

static int generic25_flash_erase_sectors(flashif_t *flash, unsigned sector_num,
    unsigned nb_sectors)
{
    int res;
    int i;
    mutex_lock(&flash->lock);
    for (i = 0; i < nb_sectors; ++i) {
        res = erase_4k(flash, sector_num + i);
        if (res != FLASH_ERR_OK) return res;
    }
    mutex_unlock(&flash->lock);
    return FLASH_ERR_OK;
}

static int write_one_page(flashif_t *flash, unsigned address,
                            void *data, unsigned size)
{
    int res;
    uint8_t status;
    generic25_flash_t *m = (generic25_flash_t *) flash;

    res = enable_write(m);
    if (res != FLASH_ERR_OK) return res;

    uint8_t *p = (uint8_t *) &address;
    m->databuf[0] = CMD_PP;
    if (m->addr4bytes) {
        m->databuf[1] = p[3];
        m->databuf[2] = p[2];
        m->databuf[3] = p[1];
        m->databuf[4] = p[0];
        m->msg.word_count = 5;
    } else {
        m->databuf[1] = p[2];
        m->databuf[2] = p[1];
        m->databuf[3] = p[0];
        m->msg.word_count = 4;
    }
    m->msg.tx_data = m->databuf;
    m->msg.rx_data = 0;
    m->msg.mode |= SPI_MODE_CS_HOLD;
    if (spim_trx(m->spi, &m->msg) != SPI_ERR_OK)
        return FLASH_ERR_IO;

    m->msg.tx_data = data;
    m->msg.rx_data = 0;
    m->msg.word_count = size;
    m->msg.mode &= ~SPI_MODE_CS_HOLD;
    if (spim_trx(m->spi, &m->msg) != SPI_ERR_OK)
        return FLASH_ERR_IO;

    while (1) {
        res = read_sr1(m, &status);
        if (res != FLASH_ERR_OK) return res;
        if (! (status & SR1_BUSY)) break;
    }
    
    return FLASH_ERR_OK;
}

static int read_one_page(flashif_t *flash, unsigned address,
                            void *data, unsigned size)
{
    generic25_flash_t *m = (generic25_flash_t *) flash;

    uint8_t *p = (uint8_t *) &address;
    m->databuf[0] = CMD_READ;
    if (m->addr4bytes) {
        m->databuf[1] = p[3];
        m->databuf[2] = p[2];
        m->databuf[3] = p[1];
        m->databuf[4] = p[0];
        m->msg.word_count = 5;
    } else {
        m->databuf[1] = p[2];
        m->databuf[2] = p[1];
        m->databuf[3] = p[0];
        m->msg.word_count = 4;
    }
    m->msg.tx_data = m->databuf;
    m->msg.rx_data = 0;
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

static int generic25_flash_write(flashif_t *flash, unsigned page_num, unsigned offset,
                        void *data, unsigned size)
{
    return cyclic_func(flash, page_num * flash_page_size(flash) + offset,
        data, size, write_one_page);
}

static int generic25_flash_read(flashif_t *flash, unsigned page_num, unsigned offset,
                        void *data, unsigned size)
{
    return cyclic_func(flash, page_num * flash_page_size(flash) + offset,
        data, size, read_one_page);
}

static int generic25_flash_needs_explicit_erase(flashif_t *flash)
{
	return 1;
}

void generic25_flash_init(generic25_flash_t *m, spimif_t *s, unsigned freq, unsigned mode)
{
    m->spi = s;
    flashif_t *f = &m->flashif;

    f->connect = generic25_flash_connect;
    f->erase_all = generic25_flash_erase_all;
    f->erase_sectors = generic25_flash_erase_sectors;
    f->write = generic25_flash_write;
    f->read = generic25_flash_read;
    f->needs_explicit_erase = generic25_flash_needs_explicit_erase;

    m->msg.freq = freq;
    m->msg.mode = (mode & 0xFF0F) | SPI_MODE_NB_BITS(8);
}
