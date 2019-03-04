#include <runtime/lib.h>
#include <kernel/uos.h>
#include "flash_bank2.h"

#define SECTOR_PHYS_OFFSET      16
#define LOGICAL_SECTOR1_OFFSET  5

static inline void unlock_two_banks_mode()
{
    unsigned optcr = FLASH->OPTCR;
    
    if (optcr & FLASH_DB1M)
        return;
        
    FLASH->OPTKEYR = FLASH_OPTKEY1;
    FLASH->OPTKEYR = FLASH_OPTKEY2;
    
    while (FLASH->SR & FLASH_BSY);
    
    FLASH->OPTCR1 = FLASH_ALL_WRITABLE;
    FLASH->OPTCR = optcr | FLASH_DB1M | FLASH_OPTSTRT;
    
    while (FLASH->SR & FLASH_BSY);
}

static inline void prog_unlock()
{
    FLASH->SR = FLASH_RDERR | FLASH_PGSERR | FLASH_PGPERR | FLASH_PGAERR | FLASH_WRPERR | FLASH_OPERR;
    
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
    
    while (FLASH->SR & FLASH_BSY);
    
    FLASH->CR = FLASH_PSIZE_32 | FLASH_ERRIE;
}

static inline void prog_lock()
{
    FLASH->CR = FLASH_LOCK;
}

static int stm32f4_connect(flashif_t *flash)
{
	stm32f4_flash_bank2_t *sf = (stm32f4_flash_bank2_t *) flash;
	
#ifndef FLASH_BANK2_DISABLE_THREAD_SAFE
	mutex_lock(&flash->lock);
#endif
	
	flash->page_size = 128 * 1024;
	flash->nb_pages_in_sector = 1;
	if (sf->size == FLASH_BANK2_SIZE_512KBYTE)
    	flash->nb_sectors = 4;
    else
        flash->nb_sectors = 8;
	
	flash->direct_read = 1;
	flash->data_align = 4;
	
#ifndef FLASH_BANK2_DISABLE_THREAD_SAFE
	mutex_unlock(&flash->lock);
#endif
	
    return FLASH_ERR_OK;
}

static void erase_sector(unsigned sector_num)
{
    if (sector_num == 0) {
        for (sector_num = SECTOR_PHYS_OFFSET; sector_num < SECTOR_PHYS_OFFSET + LOGICAL_SECTOR1_OFFSET; ++sector_num) {
            prog_unlock();
            FLASH->CR |= FLASH_SER | FLASH_SNB(sector_num) | FLASH_STRT;
            while (FLASH->SR & FLASH_BSY);
            prog_lock();
        }
    } else {
        sector_num += SECTOR_PHYS_OFFSET + LOGICAL_SECTOR1_OFFSET - 1;
        prog_unlock();
        FLASH->CR = FLASH_SER | FLASH_SNB(sector_num) | FLASH_STRT;
        while (FLASH->SR & FLASH_BSY);
        prog_lock();
    }
}

static int stm32f4_erase_sectors(flashif_t *flash, unsigned sector_num,
    unsigned nb_sectors)
{
    if (sector_num + nb_sectors > flash->nb_sectors)
        return FLASH_ERR_INVAL_SIZE;

#ifndef FLASH_BANK2_DISABLE_THREAD_SAFE
    mutex_lock(&flash->lock);
#endif

    while (nb_sectors--)
        erase_sector(sector_num++);
	
#ifndef FLASH_BANK2_DISABLE_THREAD_SAFE
    mutex_unlock(&flash->lock);
#endif

    if (FLASH->SR & FLASH_OPERR)
        return FLASH_ERR_PROGRAM;
    else
        return FLASH_ERR_OK;
}

static int stm32f4_erase_all(flashif_t *flash)
{
#ifndef FLASH_BANK2_DISABLE_THREAD_SAFE
    mutex_lock(&flash->lock);
#endif

    prog_unlock();
    
    FLASH->CR |= FLASH_MER1 | FLASH_STRT;
    while (FLASH->SR & FLASH_BSY);
    
    prog_lock();

#ifndef FLASH_BANK2_DISABLE_THREAD_SAFE
    mutex_unlock(&flash->lock);
#endif

    if (FLASH->SR & FLASH_OPERR)
        return FLASH_ERR_PROGRAM;
    else
        return FLASH_ERR_OK;
}

static int stm32f4_write(flashif_t *flash, unsigned page_num, unsigned offset, 
                        void *data, unsigned size)
{
    if (page_num >= flash->nb_sectors)
        return FLASH_ERR_INVAL_SIZE;
        
    if ((((unsigned) data) & (flash->data_align - 1)) ||
        (size & (flash->data_align - 1)))
            return FLASH_ERR_NOT_ALIGNED;
        
#ifndef FLASH_BANK2_DISABLE_THREAD_SAFE
    mutex_lock(&flash->lock);
#endif

    prog_unlock();
    
    FLASH->ACR &= ~FLASH_DCEN;
    
    FLASH->CR |= FLASH_PG;
    
	uint32_t *pdst = (uint32_t *) (flash_min_address(flash) +
		flash_page_size(flash) * page_num + offset);
	uint32_t *psrc = data;
	
    while (size) {
		*pdst++ = *psrc++;
		size -= 4;
	}
	
	while (FLASH->SR & FLASH_BSY);

	FLASH->ACR |= FLASH_DCRST;
	FLASH->ACR |= FLASH_DCEN;
	
    prog_lock();

    
#ifndef FLASH_BANK2_DISABLE_THREAD_SAFE
    mutex_unlock(&flash->lock);
#endif

    if (FLASH->SR & FLASH_OPERR)
        return FLASH_ERR_PROGRAM;
    else
        return FLASH_ERR_OK;
}

static int stm32f4_read(flashif_t *flash, unsigned page_num, unsigned offset,
                       void *data, unsigned size)
{
#ifndef FLASH_BANK2_DISABLE_THREAD_SAFE
    mutex_lock(&flash->lock);
#endif
    
    memcpy(data, (void *) (flash_min_address(flash) +
		flash_page_size(flash) * page_num + offset), size);

#ifndef FLASH_BANK2_DISABLE_THREAD_SAFE
    mutex_unlock(&flash->lock);
#endif

    return FLASH_ERR_OK;
}

static unsigned long stm32f4_min_address(flashif_t *flash)
{
	return 0x08100000;
}

static int stm32f4_flush(flashif_t *flash)
{
    return FLASH_ERR_OK;
}

static int stm32f4_needs_explicit_erase(flashif_t *flash)
{
    return 1;
}

void stm32f4_flash_bank2_init(stm32f4_flash_bank2_t *m, int size_flag)
{
    flashif_t *f = &m->flashif;
    
    m->size = size_flag;

    f->connect = stm32f4_connect;
    f->erase_all = stm32f4_erase_all;
    f->erase_sectors = stm32f4_erase_sectors;
    f->write = stm32f4_write;
    f->read = stm32f4_read;
    f->min_address = stm32f4_min_address;
    f->flush = stm32f4_flush;
    f->needs_explicit_erase = stm32f4_needs_explicit_erase;

    unlock_two_banks_mode();
}
