#include <loadable/uos-interface.h>
#include "prog_flash.h"

extern uos_loadable_t *uos;


#define LOADER_FUNCS_SECTION_SIZE   0x1000u

extern char __sram_code_start, __sram_code_end;

static uint8_t loader_funcs_cram [LOADER_FUNCS_SECTION_SIZE];


static void burn (flashif_t *flash, unsigned addr, void *data, int size) __attribute__ ((section (".sram_code")));
static void (*cram_burn) (flashif_t *flash, unsigned addr, void *data, int size);

static inline void prog_flash_unlock()
{
	// Unlock PECR
	FLASH->PEKEYR = FLASH_PEKEY1;
	FLASH->PEKEYR = FLASH_PEKEY2;
	// Unlock program memory
	FLASH->PRGKEYR = FLASH_PRGKEY1;
	FLASH->PRGKEYR = FLASH_PRGKEY2;
}

static inline void prog_flash_lock()
{
	FLASH->PECR |= FLASH_PRGLOCK | FLASH_PELOCK;
}

void burn (flashif_t *flash, unsigned addr, void *data, int size)
{
	uint32_t *pdst = (uint32_t *) addr;
	uint32_t *psrc = (uint32_t *) data;
	int w_cnt;
    
    FLASH->PECR |= FLASH_FPRG;
    FLASH->PECR |= FLASH_PROG;
    while (FLASH->SR & FLASH_BSY);
    
    while (size > 0)
		for (w_cnt = 0; w_cnt < flash_page_size(flash) / 4; ++w_cnt)
			if (size > 0) {
				*pdst++ = *psrc++;
				size -= 4;
			} else {
				*pdst++ = 0;
			}
}

static int stm32l1_connect(flashif_t *flash)
{
	stm32l1_prog_flash_t *sf = (stm32l1_prog_flash_t *) flash;
	
#ifndef STM32L_PROG_FLASH_DISABLE_THREAD_SAFE
	_UC(uos) mutex_lock(&flash->lock);
#endif
	
	flash->page_size = 256;
	flash->nb_pages_in_sector = 1;
	flash->nb_sectors = sf->size / 256;
	
	flash->direct_read = 1;
	flash->data_align = 4;
	
#ifndef STM32L_PROG_FLASH_DISABLE_THREAD_SAFE
	_UC(uos) mutex_unlock(&flash->lock);
#endif
	
    return FLASH_ERR_OK;
}

static int stm32l1_erase_sectors(flashif_t *flash, unsigned sector_num,
    unsigned nb_sectors)
{
    uint32_t *psector = (uint32_t *) (flash_min_address(flash) +
		sector_num * flash_sector_size(flash));
    
#ifndef STM32L_PROG_FLASH_DISABLE_THREAD_SAFE
    _UC(uos) mutex_lock(&flash->lock);
#endif
    
    prog_flash_unlock();
    
    FLASH->PECR |= FLASH_ERASE;
    FLASH->PECR |= FLASH_PROG;
    
    while (nb_sectors--) {
		*psector = 0;
		while (FLASH->SR & FLASH_BSY);
		psector += flash_sector_size(flash) / 4;
	}
    
	prog_flash_lock();

#ifndef STM32L_PROG_FLASH_DISABLE_THREAD_SAFE
    _UC(uos) mutex_unlock(&flash->lock);
#endif

    return FLASH_ERR_OK;
}

static int stm32l1_erase_all(flashif_t *flash)
{
	return stm32l1_erase_sectors(flash, 0, flash_nb_sectors(flash));
}

static int stm32l1_write(flashif_t *flash, unsigned page_num, unsigned offset, 
                        void *data, unsigned size)
{
#ifndef STM32L_PROG_FLASH_DISABLE_THREAD_SAFE
    _UC(uos) mutex_lock(&flash->lock);
#endif

    prog_flash_unlock();
    
#ifdef STM32L_PROG_FLASH_NO_SRAM
	uint32_t *pdst = (uint32_t *) (flash_min_address(flash) +
		flash_page_size(flash) * page_num + offset);
	uint32_t *psrc = data;
	int isize = (int) size;

    while (isize > 0) {
		*pdst++ = *psrc++;
		isize -= 4;
	}
#else
	arch_state_t x;

	arch_intr_disable(&x);
		
	cram_burn(flash, flash_min_address(flash) + 
		flash_page_size(flash) * page_num + offset, data, size);
		
	arch_intr_restore(x);
#endif

    prog_flash_lock();
    
#ifndef STM32L_PROG_FLASH_DISABLE_THREAD_SAFE
    _UC(uos) mutex_unlock(&flash->lock);
#endif
    return FLASH_ERR_OK;
}

static int stm32l1_read(flashif_t *flash, unsigned page_num, unsigned offset,
                       void *data, unsigned size)
{
#ifndef STM32L_PROG_FLASH_DISABLE_THREAD_SAFE
    _UC(uos) mutex_lock(&flash->lock);
#endif
    
    _UC(uos) memcpy(data, (void *) (flash_min_address(flash) +
		flash_page_size(flash) * page_num + offset), size);

#ifndef STM32L_PROG_FLASH_DISABLE_THREAD_SAFE
    _UC(uos) mutex_unlock(&flash->lock);
#endif
    return FLASH_ERR_OK;
}

static unsigned long stm32l1_min_address(flashif_t *flash)
{
	stm32l1_prog_flash_t *sf = (stm32l1_prog_flash_t *) flash;
	return sf->start_addr;
}

static int stm32l1_flush(flashif_t *flash)
{
    return FLASH_ERR_OK;
}

static int stm32l1_needs_explicit_erase(flashif_t *flash)
{
#ifdef STM32L_PROG_FLASH_NO_SRAM
    return 0;
#else
    return 1;
#endif
}

void stm32l1_prog_flash_init(stm32l1_prog_flash_t *m, uint32_t start_addr, uint32_t size)
{
    flashif_t *f = &m->flashif;
    
    m->start_addr = start_addr;
    m->size = size;

    f->connect = stm32l1_connect;
    f->erase_all = stm32l1_erase_all;
    f->erase_sectors = stm32l1_erase_sectors;
    f->write = stm32l1_write;
    f->read = stm32l1_read;
    f->min_address = stm32l1_min_address;
    f->flush = stm32l1_flush;
    f->needs_explicit_erase = stm32l1_needs_explicit_erase;
    
    // Copy function code to SRAM
	unsigned func_offset;
    func_offset = (unsigned) burn - (unsigned) &__sram_code_start;
    cram_burn = (void (*) (flashif_t *, unsigned, void *, int)) (loader_funcs_cram + func_offset);
    
    const int loader_funcs_size = &__sram_code_end - &__sram_code_start;
    _UC(uos) memcpy (loader_funcs_cram, (void *)(((unsigned)&__sram_code_start) & ~3), loader_funcs_size);
}
