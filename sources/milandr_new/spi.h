#ifndef __SPI_MASTER_MILANDR_H__
#define __SPI_MASTER_MILANDR_H__

#include <spi/spi-master-interface.h>

#ifndef SPI_DMA_BUFSZ
#define SPI_DMA_BUFSZ   512
#endif

typedef void (* spi_cs_control_func)(unsigned port, unsigned cs_num, int level);

struct _milandr_spim_t {
    spimif_t            spimif;
    unsigned            port;
    unsigned            last_freq;
    unsigned            last_bits;
    unsigned            last_mode;
    
    SSP_t               *reg;
    spi_cs_control_func cs_control;    
#ifndef SPI_NO_DMA
    DMA_Data_t          *dma_prim;
    uint8_t             tx_dma_nb;
    uint8_t             rx_dma_nb;
    mutex_t             irq_lock;
    uint8_t             zeroes[SPI_DMA_BUFSZ];
#endif
};
typedef struct _milandr_spim_t milandr_spim_t;

int milandr_spim_init(milandr_spim_t *spi, unsigned port, spi_cs_control_func csc, DMA_Data_t *dma_prim);

#endif
