#include <runtime/lib.h>
#include <kernel/uos.h>
#include <kernel/internal.h>
#include <milandr_new/prog_flash.h>


#ifndef LOADER_FUNCS_SECTION_SIZE
#define LOADER_FUNCS_SECTION_SIZE   0x1000u
#endif

extern unsigned long _etext, _estack, __loader_funcs_start, __loader_funcs_end;

static uint8_t loader_funcs_cram [LOADER_FUNCS_SECTION_SIZE];

void my_udelay (unsigned usec) __attribute__ ((section (".loader_funcs")));
void turn_burning_on () __attribute__ ((section (".loader_funcs")));
void turn_burning_off () __attribute__ ((section (".loader_funcs")));
void do_erase (unsigned addr) __attribute__ ((section (".loader_funcs")));
void erase (unsigned addr, unsigned nb_sectors, unsigned sector_size) __attribute__ ((section (".loader_funcs")));
void burn (unsigned addr, void *data, int size) __attribute__ ((section (".loader_funcs")));

static void (*cram_udelay) (unsigned);
static void (*cram_turn_burning_on) ();
static void (*cram_turn_burning_off) ();
static void (*cram_do_erase) (unsigned);
static void (*cram_erase) (unsigned, unsigned, unsigned);
static void (*cram_burn) (unsigned addr, void *data, int size);


void my_udelay (unsigned usec)
{
    if (! usec)
        return;

    unsigned ctrl = ARM_SYSTICK->CTRL;
    const unsigned load = ARM_SYSTICK->LOAD & 0xFFFFFF;
    unsigned now = ARM_SYSTICK->VAL & 0xFFFFFF;
    unsigned final = now - usec * (KHZ / 1000);

    for (;;) {
        ctrl = ARM_SYSTICK->CTRL;
        if (ctrl & ARM_SYSTICK_CTRL_COUNTFLAG) {
            final += load;
        }

        now = ARM_SYSTICK->VAL & 0xFFFFFF;

        if ((int) ((now - final) << 8) < 0)
            break;
    }
}

void turn_burning_on ()
{
    ARM_RSTCLK->PER_CLOCK |= ARM_PER_CLOCK_EEPROM;

    // Переводим флеш в режим записи
    ARM_EEPROM->CMD = ARM_EEPROM_CMD_CON;
    ARM_EEPROM->KEY = 0x8AAA5551;
    ARM_EEPROM->DI  = 0;
}

void turn_burning_off ()
{
    ARM_EEPROM->CMD = ARM_EEPROM_CMD_CON;
    cram_udelay (1);

    ARM_EEPROM->CMD = ARM_EEPROM_CMD_DELAY_4;
    
    cram_udelay (1);
}

void do_erase (unsigned addr)
{
    int i;
    for (i = 0; i < 16; i += 4) {
        ARM_EEPROM->ADR = addr + i;

        ARM_EEPROM->CMD = ARM_EEPROM_CMD_CON;
        ARM_EEPROM->CMD = ARM_EEPROM_CMD_CON | ARM_EEPROM_CMD_WR;
        ARM_EEPROM->CMD = ARM_EEPROM_CMD_CON;                    

        ARM_EEPROM->CMD = ARM_EEPROM_CMD_CON | ARM_EEPROM_CMD_XE | ARM_EEPROM_CMD_ERASE;
        cram_udelay (5);
        ARM_EEPROM->CMD = ARM_EEPROM_CMD_CON | ARM_EEPROM_CMD_XE | ARM_EEPROM_CMD_ERASE | ARM_EEPROM_CMD_NVSTR;
        cram_udelay (40000);
        ARM_EEPROM->CMD = ARM_EEPROM_CMD_CON | ARM_EEPROM_CMD_XE | ARM_EEPROM_CMD_NVSTR;
        cram_udelay (5);
        ARM_EEPROM->CMD = ARM_EEPROM_CMD_CON;
        cram_udelay (1);
    }
}

void erase (unsigned addr, unsigned nb_sectors, unsigned sector_size)
{
    cram_turn_burning_on ();
    unsigned i;
    for (i = 0; i < nb_sectors; ++i)
        cram_do_erase (addr + i * sector_size);
    cram_turn_burning_off ();   
}

void burn (unsigned addr, void *data, int size)
{
    unsigned cur_addr;
    unsigned *pdata = data;
    
    cram_turn_burning_on ();
    
    ARM_EEPROM->CMD = ARM_EEPROM_CMD_CON | ARM_EEPROM_CMD_PROG;
    unsigned aligned_end = (addr + size + 3) & ~3;
    for (cur_addr = addr; cur_addr < aligned_end; cur_addr += 4) {
        ARM_EEPROM->ADR = cur_addr;
        ARM_EEPROM->DI = *pdata;
        pdata++;
        ARM_EEPROM->CMD = ARM_EEPROM_CMD_CON | ARM_EEPROM_CMD_XE | ARM_EEPROM_CMD_PROG;
        cram_udelay (5);
        ARM_EEPROM->CMD = ARM_EEPROM_CMD_CON | ARM_EEPROM_CMD_XE | ARM_EEPROM_CMD_PROG | ARM_EEPROM_CMD_NVSTR;
        cram_udelay (10);
        ARM_EEPROM->CMD = ARM_EEPROM_CMD_CON | ARM_EEPROM_CMD_XE | ARM_EEPROM_CMD_PROG | ARM_EEPROM_CMD_NVSTR | ARM_EEPROM_CMD_YE;
        cram_udelay (40);
        ARM_EEPROM->CMD = ARM_EEPROM_CMD_CON | ARM_EEPROM_CMD_XE | ARM_EEPROM_CMD_PROG | ARM_EEPROM_CMD_NVSTR;
        cram_udelay(1);
    }

    cram_turn_burning_off ();
}

static int milandr_connect(flashif_t *flash)
{
	milandr_prog_flash_t *sf = (milandr_prog_flash_t *) flash;
	
	flash->page_size = 4096;
	flash->nb_pages_in_sector = 1;
	flash->nb_sectors = sf->size >> 12;
	
	flash->direct_read = 1;
	flash->data_align = 4;
	
    return FLASH_ERR_OK;
}

static int milandr_erase_sectors(flashif_t *flash, unsigned sector_num,
    unsigned nb_sectors)
{
    arch_state_t x;
    
    arch_intr_disable (&x);
    cram_erase (flash_min_address(flash) + sector_num * flash_sector_size(flash),
        nb_sectors, flash_sector_size(flash));
    arch_intr_restore (x); 

    return FLASH_ERR_OK;
}

static int milandr_erase_all(flashif_t *flash)
{
	return milandr_erase_sectors(flash, 0, flash_nb_sectors(flash));
}

static int milandr_write(flashif_t *flash, unsigned page_num, unsigned offset, 
                        void *data, unsigned size)
{
    arch_state_t x;
    
    arch_intr_disable (&x);
    cram_burn (flash_min_address(flash) + page_num * flash_page_size(flash) + offset,
         data, size);
    arch_intr_restore (x);
    
    return FLASH_ERR_OK;
}

static int milandr_read(flashif_t *flash, unsigned page_num, unsigned offset,
                       void *data, unsigned size)
{
    memcpy(data, (void *) (flash_min_address(flash) +
		flash_page_size(flash) * page_num + offset), size);

    return FLASH_ERR_OK;
}

static unsigned long milandr_min_address(flashif_t *flash)
{
	milandr_prog_flash_t *sf = (milandr_prog_flash_t *) flash;
	return sf->start_addr;
}

static int milandr_flush(flashif_t *flash)
{
    return FLASH_ERR_OK;
}

static int milandr_needs_explicit_erase(flashif_t *flash)
{
    return 1;
}

void milandr_prog_flash_init(milandr_prog_flash_t *m, uint32_t start_addr, uint32_t size)
{
    flashif_t *f = &m->flashif;
    
    m->start_addr = start_addr;
    m->size = size;

    f->connect = milandr_connect;
    f->erase_all = milandr_erase_all;
    f->erase_sectors = milandr_erase_sectors;
    f->write = milandr_write;
    f->read = milandr_read;
    f->min_address = milandr_min_address;
    f->flush = milandr_flush;
    f->needs_explicit_erase = milandr_needs_explicit_erase;
    
    // Copy function code to SRAM
	unsigned func_offset;
    func_offset = (unsigned) my_udelay - (unsigned) &__loader_funcs_start;
    cram_udelay = (void (*) (unsigned)) (loader_funcs_cram + func_offset);
    func_offset = (unsigned) turn_burning_on - (unsigned) &__loader_funcs_start;
    cram_turn_burning_on = (void (*) ()) (loader_funcs_cram + func_offset);
    func_offset = (unsigned) turn_burning_off - (unsigned) &__loader_funcs_start;
    cram_turn_burning_off = (void (*) ()) (loader_funcs_cram + func_offset);
    func_offset = (unsigned) do_erase - (unsigned) &__loader_funcs_start;
    cram_do_erase = (void (*) (unsigned)) (loader_funcs_cram + func_offset);
    func_offset = (unsigned) erase - (unsigned) &__loader_funcs_start;
    cram_erase = (void (*) (unsigned, unsigned, unsigned)) (loader_funcs_cram + func_offset);
    func_offset = (unsigned) burn - (unsigned) &__loader_funcs_start;
    cram_burn = (void (*) (unsigned, void *, int)) (loader_funcs_cram + func_offset);
    
    const int loader_funcs_size = (uint8_t *)&__loader_funcs_end - (uint8_t *)&__loader_funcs_start;
    memcpy (loader_funcs_cram, (void *)(((unsigned)&__loader_funcs_start) & ~3), (loader_funcs_size + 3) & ~3);
}
