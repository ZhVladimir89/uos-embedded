#ifndef __N25Q_H__
#define __N25Q_H__

#include <flash/flash-interface.h>
#include <spi/spi-master-interface.h>

struct _n25q_t
{
    flashif_t       flashif;
    spimif_t       *spi;
    spi_message_t   msg;
    int             addr4bytes;
    uint8_t         databuf[20];
};
typedef struct _n25q_t n25q_t;

void n25q_init(n25q_t *m, spimif_t *s, unsigned freq, unsigned mode);

#endif // __N25Q_H__
