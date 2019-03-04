#ifndef __MILANDR_PROG_FLASH_H__
#define __MILANDR_PROG_FLASH_H__

#include <flash/flash-interface.h>

struct _milandr_prog_flash_t
{
    flashif_t       flashif;
    
    uint32_t		start_addr;
    uint32_t		size;
};
typedef struct _milandr_prog_flash_t milandr_prog_flash_t;

void milandr_prog_flash_init(milandr_prog_flash_t *m, uint32_t start_addr, uint32_t size);

#endif // __MILANDR_PROG_FLASH_H__
