#include <runtime/lib.h>
#include <kernel/uos.h>
#include <milandr/spi.h>
#include <kernel/internal.h>

#define SSP1_TX_DMA     4
#define SSP1_RX_DMA     5
#define SSP2_TX_DMA     6
#define SSP2_RX_DMA     7

#ifdef ARM_1986BE1
#   define SSP3_TX_DMA     8
#   define SSP3_RX_DMA     9
#endif

#define FIFO_SIZE   (8 - 1)
#define MAX_DMA_TRANSFERS   1024

static inline int
init_hw(milandr_spim_t *spi, unsigned freq, unsigned bits_per_word, unsigned mode)
{
    SSP_t *reg = spi->reg;
    
    if (freq != spi->last_freq) {
        reg->CR1 &= ~ARM_SSP_CR1_SSE;

        unsigned div = (KHZ * (1000000000 / freq) + 1999999) / 2000000 - 1;
        if (div > 0xFF) {
            debug_printf ("SPI Master %d: too low frequency!\n", spi->port);
            spi->last_freq = 0;
            return SPI_ERR_BAD_FREQ;
        }
        
        //*real_freq = KHZ * 1000 / 2 / (1 + div);

        reg->CR0 = (reg->CR0 & ~ARM_SSP_CR0_SCR(0xFF)) | ARM_SSP_CR0_SCR(div);
        spi->last_freq = freq;
    }
    if (mode != spi->last_mode) {
        reg->CR1 &= ~ARM_SSP_CR1_SSE;
        
        if (mode & SPI_MODE_LSB_FIRST)
            return SPI_ERR_MODE_NOT_SUPP;
        
        if (mode & SPI_MODE_CPOL) 
            reg->CR0 |= ARM_SSP_CR0_SPO;
        else reg->CR0 &= ~ARM_SSP_CR0_SPO;

        if (mode & SPI_MODE_CPHA) 
            reg->CR0 |= ARM_SSP_CR0_SPH;
        else reg->CR0 &= ~ARM_SSP_CR0_SPH;

        spi->last_mode = mode;
    }
    if (bits_per_word != spi->last_bits) {
        reg->CR1 &= ~ARM_SSP_CR1_SSE;

        if (bits_per_word > 16 || bits_per_word < 4) {
            debug_printf ("SPI Master %d: unsupported number of bits per word: %d\n",
                spi->port, bits_per_word);
            return SPI_ERR_BAD_BITS;
        }
        reg->CR0 = (reg->CR0 & ~ARM_SSP_CR0_DSS(16)) | ARM_SSP_CR0_DSS(bits_per_word);

        spi->last_bits = bits_per_word;
    }

    reg->CR1 |= ARM_SSP_CR1_SSE;

    return SPI_ERR_OK;
}

static inline int trx_no_dma(spimif_t *spimif, spi_message_t *msg, unsigned bits_per_word, unsigned cs_num, unsigned mode)
{
    milandr_spim_t      *spi = (milandr_spim_t *) spimif;
    SSP_t               *reg = spi->reg;
    uint8_t             *rxp_8bit;
    uint8_t             *txp_8bit;
    uint16_t            *rxp_16bit;
    uint16_t            *txp_16bit;
    unsigned            i, j;

    // Активируем CS
    // Если функция cs_control не установлена, то считаем, что для выборки 
    // устройства используется вывод FSS, и он работает автоматически
    if (spi->cs_control)
        spi->cs_control(spi->port, cs_num, mode & SPI_MODE_CS_HIGH);

    if (bits_per_word <= 8) {
        rxp_8bit = (uint8_t *) msg->rx_data;
        txp_8bit = (uint8_t *) msg->tx_data;

        i = 0;
        while (i < msg->word_count) {
            while (!(reg->SR & ARM_SSP_SR_TFE));
            for (j = 0; j < FIFO_SIZE; ++j) {
                if (txp_8bit)
                    reg->DR = *txp_8bit++;
                else
                    reg->DR = 0;

                ++i;
                if (i >= msg->word_count)
                    break;

                if (rxp_8bit) {
                    while (reg->SR & ARM_SSP_SR_RNE) {
                        *rxp_8bit = reg->DR;
                        rxp_8bit++;
                    }
                }
            }
        }
        while (reg->SR & ARM_SSP_SR_BSY);
        if (rxp_8bit) {
            while (rxp_8bit - (uint8_t *) msg->rx_data < msg->word_count) {
                while (reg->SR & ARM_SSP_SR_RNE) {
                    *rxp_8bit = reg->DR;
                    rxp_8bit++;
                }
            }
        } else {
            // Прочищаем входную FIFO
            for (j = 0; j < FIFO_SIZE + 1; ++j) reg->DR;
        }
    } else if (bits_per_word > 8 && bits_per_word <= 16) {
        rxp_16bit = (uint16_t *) msg->rx_data;
        txp_16bit = (uint16_t *) msg->tx_data;

        i = 0;
        while (i < msg->word_count) {
            while (!(reg->SR & ARM_SSP_SR_TFE));
            for (j = 0; j < FIFO_SIZE; ++j) {
                if (txp_16bit) {
                    reg->DR = *txp_16bit++;
                } else {
                    reg->DR = 0;
                }
                ++i;
                if (i >= msg->word_count)
                    break;

                if (rxp_16bit) {
                    while (reg->SR & ARM_SSP_SR_RNE) {
                        *rxp_16bit = reg->DR;
                        rxp_16bit++;
                    }
                }
            }
        }

        while (reg->SR & ARM_SSP_SR_BSY);

        if (rxp_16bit) {
            while (rxp_16bit - (uint16_t *) msg->rx_data < msg->word_count) {
                while (reg->SR & ARM_SSP_SR_RNE) {
                    *rxp_16bit = reg->DR;
                    rxp_16bit++;
                }
            }
        } else {
            // Прочищаем входную FIFO
            for (j = 0; j < FIFO_SIZE + 1; ++j) reg->DR;
        }
    } else {
        if (!(mode & SPI_MODE_CS_HOLD) && spi->cs_control)
            spi->cs_control(spi->port, cs_num, !(mode & SPI_MODE_CS_HIGH));
        return SPI_ERR_BAD_BITS;
    }

    // Деактивируем CS
    // Если функция cs_control не установлена, то считаем, что для выборки 
    // устройства используется вывод FSS, и он работает автоматически
    if (!(mode & SPI_MODE_CS_HOLD) && spi->cs_control)
        spi->cs_control(spi->port, cs_num, !(mode & SPI_MODE_CS_HIGH));
        
    return SPI_ERR_OK;
}

#ifdef SPI_NO_DMA
static int trx(spimif_t *spimif, spi_message_t *msg)
{
    unsigned            bits_per_word = SPI_MODE_GET_NB_BITS(msg->mode);
    unsigned            cs_num = SPI_MODE_GET_CS_NUM(msg->mode);
    unsigned            mode = SPI_MODE_GET_MODE(msg->mode);
    int res;
    
    mutex_lock(&spimif->lock);
    
    res = init_hw((milandr_spim_t *)spimif, msg->freq, bits_per_word, mode);
    if (res != SPI_ERR_OK) {
        mutex_unlock(&spimif->lock);
        return res;
    }

    res = trx_no_dma(spimif, msg, bits_per_word, cs_num, mode);
    
    mutex_unlock(&spimif->lock);
    
    return res;
}
#else
static int trx(spimif_t *spimif, spi_message_t *msg)
{
    milandr_spim_t      *spi = (milandr_spim_t *) spimif;
    SSP_t               *reg = spi->reg;
    unsigned            bits_per_word = SPI_MODE_GET_NB_BITS(msg->mode);
    unsigned            cs_num = SPI_MODE_GET_CS_NUM(msg->mode);
    unsigned            mode = SPI_MODE_GET_MODE(msg->mode);
    int                 res;
    unsigned            dmacr = 0;
    unsigned            byte_width = (bits_per_word > 8) + 1;
    unsigned            rest = msg->word_count;
    unsigned            curp = 0;
    unsigned            portion;

    mutex_lock(&spimif->lock);
    
    res = init_hw(spi, msg->freq, bits_per_word, mode);
    if (res != SPI_ERR_OK) {
        mutex_unlock(&spimif->lock);
        return res;
    }
    
    // Вычисляем по эмпирической формуле, что лучше: использовать DMA или нет.
    if (msg->word_count < ((msg->freq / bits_per_word / KHZ) >> 1)) {
        res = trx_no_dma(spimif, msg, bits_per_word, cs_num, mode);
        mutex_unlock(&spimif->lock);
        return res;
    }
    
    // Активируем CS
    // Если функция cs_control не установлена, то считаем, что для выборки 
    // устройства используется вывод FSS, и он работает автоматически
    if (spi->cs_control)
        spi->cs_control(spi->port, cs_num, mode & SPI_MODE_CS_HIGH);
        
    mutex_lock_irq (&spi->irq_lock, DMA_IRQn, 0, 0);
    
    while (rest > 0) {
        if (msg->tx_data) {
            portion = (rest < 1024) ? rest : 1024;
            spi->dma_prim[spi->tx_dma_nb].SOURCE_END_POINTER = (unsigned) msg->tx_data + (curp + portion - 1) * byte_width;
        } else {
            portion = (rest * byte_width < (SPI_DMA_BUFSZ & 0xFFFFFFFE)) ? rest : (SPI_DMA_BUFSZ / byte_width);
            spi->dma_prim[spi->tx_dma_nb].SOURCE_END_POINTER = (unsigned) spi->zeroes + (portion - 1) * byte_width;
        }
        
        if (msg->rx_data) {
            spi->dma_prim[spi->rx_dma_nb].DEST_END_POINTER = (unsigned) msg->rx_data + (curp + portion - 1) * byte_width;
            spi->dma_prim[spi->rx_dma_nb].CONTROL =
            		ARM_DMA_DST_INC(byte_width - 1) |
            		ARM_DMA_DST_SIZE(byte_width - 1) |
            		ARM_DMA_SRC_INC(ARM_DMA_ADDR_NOINC) |
            		ARM_DMA_SRC_SIZE(byte_width - 1) |
            		ARM_DMA_SRC_PROT(1) | ARM_DMA_DST_PROT(1) |
                    ARM_DMA_RPOWER(1) | ARM_DMA_TRANSFERS(portion) | ARM_DMA_BASIC;

            ARM_DMA->CHNL_REQ_MASK_CLR = ARM_DMA_SELECT(spi->rx_dma_nb);
            ARM_DMA->CHNL_USEBURST_CLR = ARM_DMA_SELECT(spi->rx_dma_nb);
            dmacr = ARM_SSP_DMACR_RX;
        }
        
        spi->dma_prim[spi->tx_dma_nb].CONTROL =
        		ARM_DMA_DST_INC(ARM_DMA_ADDR_NOINC) |
        		ARM_DMA_DST_SIZE(byte_width - 1) |
        		ARM_DMA_SRC_INC(byte_width - 1) |
        		ARM_DMA_SRC_SIZE(byte_width - 1) |
        		ARM_DMA_SRC_PROT(1) | ARM_DMA_DST_PROT(1) |
                ARM_DMA_RPOWER(1) | ARM_DMA_TRANSFERS(portion) | ARM_DMA_BASIC;

        ARM_DMA->CHNL_REQ_MASK_CLR = ARM_DMA_SELECT(spi->tx_dma_nb);
        ARM_DMA->CHNL_USEBURST_CLR = ARM_DMA_SELECT(spi->tx_dma_nb);
        
        dmacr |= ARM_SSP_DMACR_TX;
        reg->DMACR = dmacr;
        
        while (1) {
            mutex_wait(&spi->irq_lock);
            if ((spi->dma_prim[spi->tx_dma_nb].CONTROL & 7) != ARM_DMA_BASIC)
                reg->DMACR &= ~ARM_SSP_DMACR_TX;
            if ((spi->dma_prim[spi->rx_dma_nb].CONTROL & 7) != ARM_DMA_BASIC)
                reg->DMACR &= ~ARM_SSP_DMACR_RX;
            if (reg->DMACR == 0) {
                ARM_DMA->CHNL_ENABLE_SET = ARM_DMA_SELECT(spi->rx_dma_nb) | ARM_DMA_SELECT(spi->tx_dma_nb);
                break;
            }
        }
        curp += portion;
        rest -= portion;
    }
        
    mutex_unlock(&spi->irq_lock);    
    
    // Деактивируем CS
    // Если функция cs_control не установлена, то считаем, что для выборки 
    // устройства используется вывод FSS, и он работает автоматически
    if (!(mode & SPI_MODE_CS_HOLD) && spi->cs_control)
        spi->cs_control(spi->port, cs_num, !(mode & SPI_MODE_CS_HIGH));

    mutex_unlock(&spimif->lock);

    return SPI_ERR_OK;
}
#endif


int milandr_spim_init(milandr_spim_t *spi, unsigned port, spi_cs_control_func csc, DMA_Data_t *dma_prim)
{
    memset(spi, 0, sizeof(milandr_spim_t));
    
    spi->port = port;
    spi->spimif.trx = trx;
    spi->cs_control = csc;
	spi->spimif.data_align = 1;
	
    unsigned per_clock = ARM_RSTCLK->PER_CLOCK;
    
#ifndef SPI_NO_DMA

#ifdef ARM_1986BE1
    ARM_RSTCLK->PER_CLOCK |= ARM_PER_CLOCK_SSP1 | ARM_PER_CLOCK_SSP2 | ARM_PER_CLOCK_SSP3 | ARM_PER_CLOCK_DMA;
#else
    ARM_RSTCLK->PER_CLOCK |= ARM_PER_CLOCK_SSP1 | ARM_PER_CLOCK_SSP2 | ARM_PER_CLOCK_DMA;
#endif
    spi->dma_prim = dma_prim;
    ARM_DMA->CONFIG = ARM_DMA_ENABLE | ARM_DMA_CFG_CH_PROT1;
    ARM_DMA->CHNL_ENABLE_SET = 0xFFFFFFFF;
    ARM_DMA->CHNL_REQ_MASK_SET = 0xFFFFFFFF;
    ARM_DMA->CHNL_PRI_ALT_CLR = 0xFFFFFFFF;
    ARM_DMA->CTRL_BASE_PTR = (unsigned) dma_prim;
#endif
    
    if (port == 1) {
        ARM_RSTCLK->PER_CLOCK = per_clock | ARM_PER_CLOCK_SSP1 | ARM_PER_CLOCK_DMA;
        ARM_RSTCLK->SSP_CLOCK = (ARM_RSTCLK->SSP_CLOCK & ~ARM_SSP_CLOCK_BRG1(7)) |
            ARM_SSP_CLOCK_EN1 | ARM_SSP_CLOCK_BRG1(0);
        spi->reg = ARM_SSP1;
#ifndef SPI_NO_DMA
        spi->tx_dma_nb = SSP1_TX_DMA;
        spi->rx_dma_nb = SSP1_RX_DMA;
#endif // !SPI_NO_DMA
    } else if (port == 2) {
        ARM_RSTCLK->PER_CLOCK = per_clock | ARM_PER_CLOCK_SSP2 | ARM_PER_CLOCK_DMA;
        ARM_RSTCLK->SSP_CLOCK = (ARM_RSTCLK->SSP_CLOCK & ~ARM_SSP_CLOCK_BRG2(7)) |
            ARM_SSP_CLOCK_EN2 | ARM_SSP_CLOCK_BRG2(0);
        spi->reg = ARM_SSP2;
#ifndef SPI_NO_DMA
        spi->tx_dma_nb = SSP2_TX_DMA;
        spi->rx_dma_nb = SSP2_RX_DMA;
#endif // !SPI_NO_DMA

#ifdef ARM_1986BE1
    } else if (port == 3) {
        ARM_RSTCLK->PER_CLOCK = per_clock | ARM_PER_CLOCK_SSP3 | ARM_PER_CLOCK_DMA;
        ARM_RSTCLK->SSP_CLOCK = (ARM_RSTCLK->SSP_CLOCK & ~ARM_SSP_CLOCK_BRG3(7)) |
            ARM_SSP_CLOCK_EN3 | ARM_SSP_CLOCK_BRG3(0);
        spi->reg = ARM_SSP3;
#ifndef SPI_NO_DMA
        spi->tx_dma_nb = SSP3_TX_DMA;
        spi->rx_dma_nb = SSP3_RX_DMA;
#endif // !SPI_NO_DMA
#endif // ARM_1986BE1
    } else {
        return SPI_ERR_BAD_PORT;
    }

#ifndef SPI_NO_DMA    
    dma_prim[spi->rx_dma_nb].SOURCE_END_POINTER = (unsigned) &spi->reg->DR;
    dma_prim[spi->tx_dma_nb].DEST_END_POINTER = (unsigned) &spi->reg->DR;
#endif
    
    spi->reg->CR0 = ARM_SSP_CR0_FRF_SPI;
    spi->reg->CPSR = 2;
    
    return 0;
}

