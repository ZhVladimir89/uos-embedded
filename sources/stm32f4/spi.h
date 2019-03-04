#ifndef __STM32F4_SPI__
#define __STM32F4_SPI__

#include <spi/spi-master-interface.h>

//#define SPI_NO_DMA
//
//
//#ifndef SPI_DMA_BUFSZ
//#define SPI_DMA_BUFSZ   512
//#endif

typedef void (* spi_cs_control_func)(unsigned port, unsigned cs_num, int level);

struct _stm32f4_spim_t {
    spimif_t            spimif;
    unsigned            port;
    unsigned            last_freq;
    unsigned            last_bits;
    unsigned            last_mode;
    
    SPI_t               *reg;
    spi_cs_control_func cs_control;
//#ifndef SPI_NO_DMA
//    DMA_Data_t          *dma_prim;
//    uint8_t             tx_dma_nb;
//    uint8_t             rx_dma_nb;
//    mutex_t             irq_lock;
//    uint8_t             zeroes[SPI_DMA_BUFSZ];
//#endif
};
typedef struct _stm32f4_spim_t stm32f4_spim_t;


/////////////////////////////////////////////////////////////////////////////
// spimif - SPI Master interface see spi-master-interface.h
// port - phisical port number : 0 means SPI1, 1 means SPI1, 2 means SPI3
// csc - crystal select control callback function
// return: see spi-master-interface.h

int stm32f4_spi_init(stm32f4_spim_t *spi, unsigned port, spi_cs_control_func csc);

#endif // __STM32F4_SPI__
