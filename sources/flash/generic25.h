#ifndef __GENERIC25_H__
#define __GENERIC25_H__

#include <flash/flash-interface.h>
#include <spi/spi-master-interface.h>

struct _generic25_flash_t
{
    flashif_t       flashif;
    spimif_t       *spi;
    spi_message_t   msg;
    int             addr4bytes;
    uint8_t         databuf[20];
};
typedef struct _generic25_flash_t generic25_flash_t;

void generic25_flash_init(generic25_flash_t *m, spimif_t *s, unsigned freq, unsigned mode);

#endif // __GENERIC25_H__
