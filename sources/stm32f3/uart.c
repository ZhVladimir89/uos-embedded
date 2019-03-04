/*
 * UART interface implementation for STM32F3 microcontrollers.
 *
 * Copyright (C) 2018 Dmitry Podkhvatilin <vatilin@gmail.com>
 *
 *
 * This file is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You can redistribute this file and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software Foundation;
 * either version 2 of the License, or (at your discretion) any later version.
 * See the accompanying file "COPYING.txt" for more details.
 *
 * As a special exception to the GPL, permission is granted for additional
 * uses of the text contained in this file.  See the accompanying file
 * "COPY-UOS.txt" for details.
 */
#include <runtime/lib.h>
#include <kernel/uos.h>
#include <kernel/internal.h>
#include <stm32f3/uart.h>

const unsigned USART_CR1_COMMON = USART_UE | USART_TE | USART_RE | USART_RXNEIE;

static int stm32f3_uart_set_param(uartif_t *uart, unsigned params, unsigned bytes_per_sec)
{
    stm32f3_uart_t* stmu = (stm32f3_uart_t*) uart;
    USART_t *uregs = stmu->reg;
    uint32_t cr1 = 0;

    mutex_lock(&uart->lock);
    uregs->CR1 = 0;
    
    if ((params & UART_BITS_MASK) != 8) {
        // Only 8-bit mode supported yet!
        mutex_unlock(&uart->lock);
        return UART_ERR_MODE_NOT_SUPP;
    }
    
    if (params & UART_PE_E)
        cr1 |= USART_PCE;
    else if (params & UART_PE_O)
        cr1 |= USART_PCE | USART_PS;
    
    uregs->CR2 &= ~USART_STOP_MASK;
    switch (params & UART_STOP_MASK) {
    case UART_STOP_1:
        uregs->CR2 |= USART_STOP_1;
        break;
    case UART_STOP_05:
        uregs->CR2 |= USART_STOP_05;
        break;
    case UART_STOP_2:
        uregs->CR2 |= USART_STOP_2;
        break;
    case UART_STOP_15:
        uregs->CR2 |= USART_STOP_15;
        break;
    }
    
    uregs->BRR = KHZ * 1000 / bytes_per_sec;
    uregs->CR1 = cr1 | USART_CR1_COMMON;
    
    mutex_unlock(&uart->lock);

    return UART_ERR_OK;
}

/*
static bool_t clear_tx_dma_intr_flags(void *arg)
{
    stm32f3_uart_t* stmu = arg;
    int tx_irq = 0;
    
    switch (stmu->port) {
    case 1:
        DMA1->IFCR = DMA_CTEIF(4) | DMA_CTCIF(4);
        tx_irq = IRQ_DMA1_CHANNEL4;
        break;
    case 2:
        DMA1->IFCR = DMA_CTEIF(7) | DMA_CTCIF(7);
        tx_irq = IRQ_DMA1_CHANNEL7;
        break;
    case 3:
        DMA1->IFCR = DMA_CTEIF(2) | DMA_CTCIF(2);
        tx_irq = IRQ_DMA1_CHANNEL2;
        break;
    case 4:
        DMA2->IFCR = DMA_CTEIF(5) | DMA_CTCIF(5);
        tx_irq = IRQ_DMA2_CHANNEL5;
        break;
    default:
        break;
    }
    
    arch_intr_allow(tx_irq);
    return 0;
}
*/

static int stm32f3_uart_tx(uartif_t *uart, const void *data, int size)
{
    if (size <= 0 || size > 65535)
        return UART_ERR_TOO_LONG;
        
    stm32f3_uart_t* stmu = (stm32f3_uart_t*) uart;
    
    mutex_lock(&uart->lock);

    arm_reg_t ccr = stmu->tx_dma->CCR;
    stmu->tx_dma->CCR = 0;
    stmu->tx_dma->CMAR = (unsigned)data;
    stmu->tx_dma->CNDTR = size;
    stmu->tx_dma->CCR = ccr | DMA_EN;

    mutex_wait(&uart->lock);

    mutex_unlock(&uart->lock);

    return size;
}

static int calc_rx_data_avail(stm32f3_uart_t* stmu)
{
    return (stmu->rx_dma->CMAR + UART_RXBUFSZ - stmu->rx_dma->CNDTR - 
        (unsigned)stmu->rx_curp + UART_RXBUFSZ) % UART_RXBUFSZ;
}

static int stm32f3_uart_rx(uartif_t *uart, void *data, int size, int non_block)
{
    if (size <= 0)
        return UART_ERR_TOO_LONG;
    
    stm32f3_uart_t* stmu = (stm32f3_uart_t*) uart;
   
    mutex_lock(&uart->lock);

    int copied = 0;
    
    int data_avail = calc_rx_data_avail(stmu);
    if (! non_block) {
        while (! data_avail) {
            mutex_wait(&stmu->rxne_mutex);
            data_avail = calc_rx_data_avail(stmu);
        }
    }
    
    if (data_avail) {
        uint8_t *pu8 = data;
        int size_to_end = stmu->rx_buffer + UART_RXBUFSZ - stmu->rx_curp;
        int min_size = (size_to_end < data_avail) ? size_to_end : data_avail;
        min_size = (min_size < size) ? min_size : size;
        memcpy(pu8, stmu->rx_curp, min_size);
        copied = min_size;
        if (min_size == size_to_end) {
            data_avail -= min_size;
            size -= min_size;
            if (data_avail && size) {
                pu8 += min_size;
                min_size = (data_avail < size) ? data_avail : size;
                memcpy(pu8, stmu->rx_buffer, min_size);
                stmu->rx_curp = stmu->rx_buffer + min_size;
                copied += min_size;
            } else {
                stmu->rx_curp = stmu->rx_buffer;
            }
        } else {
            stmu->rx_curp += min_size;
        }
    }
    
    mutex_unlock(&uart->lock);
    
    return copied;
}

static int stm32f3_uart_flush_rx(uartif_t *uart)
{
    stm32f3_uart_t* stmu = (stm32f3_uart_t*) uart;
   
    mutex_lock(&uart->lock);
    stmu->rx_curp = stmu->rx_buffer + UART_RXBUFSZ - stmu->rx_dma->CNDTR;
    mutex_unlock(&uart->lock);
    
    return UART_ERR_OK;
}

int stm32f3_uart_init(stm32f3_uart_t* stm_uart, unsigned port) 
{
    stm_uart->port = port;
    stm_uart->uartif.set_param       = stm32f3_uart_set_param;
    stm_uart->uartif.tx              = stm32f3_uart_tx;
    stm_uart->uartif.rx              = stm32f3_uart_rx;
    stm_uart->uartif.flush_rx        = stm32f3_uart_flush_rx;
    stm_uart->rx_curp = stm_uart->rx_buffer;
    
    int tx_irq;
    int rx_irq;
    switch (port) {
    case 1:
        RCC->APB2ENR |= RCC_USART1EN;
        RCC->AHBENR |= RCC_DMA1EN;
        stm_uart->reg = USART1;
        stm_uart->tx_dma = DMA1_CH(4);
        stm_uart->rx_dma = DMA1_CH(5);
        tx_irq = IRQ_DMA1_CHANNEL4;
        rx_irq = IRQ_USART1;
        break;
    case 2:
        RCC->APB1ENR |= RCC_USART2EN;
        RCC->AHBENR |= RCC_DMA1EN;
        stm_uart->reg = USART2;
        stm_uart->tx_dma = DMA1_CH(7);
        stm_uart->rx_dma = DMA1_CH(6);
        tx_irq = IRQ_DMA1_CHANNEL7;
        rx_irq = IRQ_USART2;
        break;
    case 3:
        RCC->APB1ENR |= RCC_USART3EN;
        RCC->AHBENR |= RCC_DMA1EN;
        stm_uart->reg = USART3;
        stm_uart->tx_dma = DMA1_CH(2);
        stm_uart->rx_dma = DMA1_CH(3);
        tx_irq = IRQ_DMA1_CHANNEL2;
        rx_irq = IRQ_USART3;
        break;
    case 4:
        RCC->APB1ENR |= RCC_UART4EN;
        RCC->AHBENR |= RCC_DMA2EN;
        stm_uart->reg = UART4;
        stm_uart->tx_dma = DMA2_CH(5);
        stm_uart->rx_dma = DMA2_CH(3);
        tx_irq = IRQ_DMA2_CHANNEL5;
        rx_irq = IRQ_UART4;
        break;
    default:
        return UART_ERR_BAD_PORT;
    }
    
    stm_uart->reg->CR3 = USART_DMAT | USART_DMAR;
    
    stm_uart->tx_dma->CPAR = (unsigned)&stm_uart->reg->TDR;
    stm_uart->tx_dma->CCR = DMA_MSIZE_8 | DMA_PSIZE_8 | DMA_MINC |
        DMA_DIR_FROM_MEM | DMA_TEIE | DMA_TCIE;

    stm_uart->rx_dma->CPAR = (unsigned)&stm_uart->reg->RDR;
    stm_uart->rx_dma->CMAR = (unsigned)stm_uart->rx_buffer;
    stm_uart->rx_dma->CNDTR = UART_RXBUFSZ;
    stm_uart->rx_dma->CCR = DMA_MSIZE_8 | DMA_PSIZE_8 | DMA_MINC | DMA_CIRC | DMA_EN;
    
    mutex_attach_irq(&stm_uart->uartif.lock, tx_irq, 0, 0);
    mutex_attach_irq(&stm_uart->rxne_mutex, rx_irq, 0, 0);
    
    return UART_ERR_OK;
}

