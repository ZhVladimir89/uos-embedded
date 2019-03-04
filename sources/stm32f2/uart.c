/*
 * UART interface implementation for STM32L1 microcontrollers.
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
#include <stm32f4/uart.h>

const unsigned USART_CR1_COMMON = USART_UE | USART_TE | USART_RE | USART_RXNEIE;

static inline unsigned get_apb_div(stm32f4_uart_t* stmu)
{
    if (stmu->port == 1 || stmu->port == 6)
        return APB2_DIV;
    else
        return APB1_DIV;
}

static int stm32f4_uart_set_param(uartif_t *uart, unsigned params, unsigned bytes_per_sec)
{
    stm32f4_uart_t* stmu = (stm32f4_uart_t*) uart;
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
    
    uregs->BRR = KHZ * 1000 / get_apb_div(stmu) / bytes_per_sec;
    uregs->CR1 = cr1 | USART_CR1_COMMON;
    
    mutex_unlock(&uart->lock);

    return UART_ERR_OK;
}

static bool_t clear_tx_dma_intr_flags(void *arg)
{
    stm32f4_uart_t* stmu = arg;
    int tx_irq = 0;
    
    switch (stmu->port) {
    case 1:
        DMA2->HIFCR = DMA_CTEIF(7) | DMA_CTCIF(7);
        tx_irq = IRQ_DMA2_STREAM7;
        break;
    case 2:
        DMA1->HIFCR = DMA_CTEIF(6) | DMA_CTCIF(6);
        tx_irq = IRQ_DMA1_STREAM6;
        break;
    case 3:
        DMA1->LIFCR = DMA_CTEIF(3) | DMA_CTCIF(3);
        tx_irq = IRQ_DMA1_STREAM3;
        break;  
    case 4:
        DMA1->HIFCR = DMA_CTEIF(4) | DMA_CTCIF(4);
        tx_irq = IRQ_DMA1_STREAM4;
        break;
    case 5:
        DMA1->HIFCR = DMA_CTEIF(7) | DMA_CTCIF(7);
        tx_irq = IRQ_DMA1_STREAM7;
        break;
    case 6:
        DMA2->HIFCR = DMA_CTEIF(6) | DMA_CTCIF(6);
        tx_irq = IRQ_DMA2_STREAM6;
        break;
    case 7:
        DMA1->LIFCR = DMA_CTEIF(1) | DMA_CTCIF(1);
        tx_irq = IRQ_DMA1_STREAM1;
        break;
    case 8:
        DMA2->LIFCR = DMA_CTEIF(0) | DMA_CTCIF(0);
        tx_irq = IRQ_DMA2_STREAM0;
        break;
    default:
        break;
    }
    
    arch_intr_allow(tx_irq);
    return 0;
}

static int stm32f4_uart_tx(uartif_t *uart, const void *data, int size)
{
    if (size <= 0 || size > 65535)
        return UART_ERR_TOO_LONG;
        
    stm32f4_uart_t* stmu = (stm32f4_uart_t*) uart;
    
    mutex_lock(&uart->lock);

    arm_reg_t cr = stmu->tx_dma->CR;
    stmu->tx_dma->CR = 0;
    stmu->tx_dma->M0AR = (unsigned)data;
    stmu->tx_dma->NDTR = size;
    stmu->tx_dma->CR = cr | DMA_EN;

    mutex_wait(&uart->lock);

    mutex_unlock(&uart->lock);

    return size;
}

static int calc_rx_data_avail(stm32f4_uart_t* stmu)
{
    return (stmu->rx_dma->M0AR + UART_RXBUFSZ - stmu->rx_dma->NDTR - 
        (unsigned)stmu->rx_curp + UART_RXBUFSZ) % UART_RXBUFSZ;
}

static int stm32f4_uart_rx(uartif_t *uart, void *data, int size, int non_block)
{
    if (size <= 0)
        return UART_ERR_TOO_LONG;
    
    stm32f4_uart_t* stmu = (stm32f4_uart_t*) uart;
   
    mutex_lock(&uart->lock);

    int copied = 0;
    
    mutex_lock(&stmu->rxne_mutex);
    int data_avail = calc_rx_data_avail(stmu);
    if (! non_block) {
        stmu->reg->CR1 |= USART_RXNEIE;
        while (! data_avail) {
            mutex_wait(&stmu->rxne_mutex);
            data_avail = calc_rx_data_avail(stmu);
        }
        stmu->reg->CR1 &= ~USART_RXNEIE;
    }
    mutex_unlock(&stmu->rxne_mutex);
    
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

static int stm32f4_uart_flush_rx(uartif_t *uart)
{
    stm32f4_uart_t* stmu = (stm32f4_uart_t*) uart;
   
    mutex_lock(&uart->lock);
    stmu->rx_curp = stmu->rx_buffer + UART_RXBUFSZ - stmu->rx_dma->NDTR;
    mutex_unlock(&uart->lock);
    
    return UART_ERR_OK;
}

int stm32f4_uart_init(stm32f4_uart_t* stm_uart, unsigned port) 
{
    stm_uart->port = port;
    stm_uart->uartif.set_param       = stm32f4_uart_set_param;
    stm_uart->uartif.tx              = stm32f4_uart_tx;
    stm_uart->uartif.rx              = stm32f4_uart_rx;
    stm_uart->uartif.flush_rx        = stm32f4_uart_flush_rx;
    stm_uart->rx_curp = stm_uart->rx_buffer;
    
    int tx_irq;
    int rx_irq;
    switch (port) {
    case 1:
        RCC->APB2ENR |= RCC_USART1EN;
        RCC->APB2LPENR |= RCC_USART1LPEN;
        RCC->AHB1ENR |= RCC_DMA2EN;
        stm_uart->reg = USART1;
        stm_uart->tx_dma = DMA2_STR(7);
        stm_uart->tx_dma->CR = DMA_CHSEL(4);
        stm_uart->rx_dma = DMA2_STR(2); // DMA22_STR(5)
        stm_uart->rx_dma->CR = DMA_CHSEL(4);
        tx_irq = IRQ_DMA2_STREAM7;
        rx_irq = IRQ_USART1;
        break;
    case 2:
        RCC->APB1ENR |= RCC_USART2EN;
        RCC->APB1LPENR |= RCC_USART2LPEN;
        RCC->AHB1ENR |= RCC_DMA1EN;
        stm_uart->reg = USART2;
        stm_uart->tx_dma = DMA1_STR(6);
        stm_uart->tx_dma->CR = DMA_CHSEL(4);
        stm_uart->rx_dma = DMA1_STR(5);
        stm_uart->rx_dma->CR = DMA_CHSEL(4);
        tx_irq = IRQ_DMA1_STREAM6;
        rx_irq = IRQ_USART2;
        break;
    case 3:
        RCC->APB1ENR |= RCC_USART3EN;
        RCC->APB1LPENR |= RCC_USART3LPEN;
        RCC->AHB1ENR |= RCC_DMA1EN;
        stm_uart->reg = USART3;
        stm_uart->tx_dma = DMA1_STR(3); // DMA1_STR(4)
        stm_uart->tx_dma->CR = DMA_CHSEL(4);
        stm_uart->rx_dma = DMA1_STR(1);
        stm_uart->rx_dma->CR = DMA_CHSEL(4);
        tx_irq = IRQ_DMA1_STREAM3;
        rx_irq = IRQ_USART3;
        break;
    case 4:
        RCC->APB1ENR |= RCC_UART4EN;
        RCC->APB1LPENR |= RCC_UART4LPEN;
        RCC->AHB1ENR |= RCC_DMA1EN;
        stm_uart->reg = UART4;
        stm_uart->tx_dma = DMA1_STR(4);
        stm_uart->tx_dma->CR = DMA_CHSEL(4);
        stm_uart->rx_dma = DMA1_STR(2);
        stm_uart->rx_dma->CR = DMA_CHSEL(4);
        tx_irq = IRQ_DMA1_STREAM4;
        rx_irq = IRQ_UART4;
        break;
    case 5:
        RCC->APB1ENR |= RCC_UART5EN;
        RCC->APB1LPENR |= RCC_UART5LPEN;
        RCC->AHB1ENR |= RCC_DMA1EN;
        stm_uart->reg = UART5;
        stm_uart->tx_dma = DMA1_STR(7);
        stm_uart->tx_dma->CR = DMA_CHSEL(4);
        stm_uart->rx_dma = DMA1_STR(0);
        stm_uart->rx_dma->CR = DMA_CHSEL(4);
        tx_irq = IRQ_DMA1_STREAM7;
        rx_irq = IRQ_UART5;
        break;
    case 6:
        RCC->APB2ENR |= RCC_USART6EN;
        RCC->APB2LPENR |= RCC_USART6LPEN;
        RCC->AHB1ENR |= RCC_DMA2EN;
        stm_uart->reg = USART6;
        stm_uart->tx_dma = DMA2_STR(6); // DMA2_STR(7)
        stm_uart->tx_dma->CR = DMA_CHSEL(5);
        stm_uart->rx_dma = DMA2_STR(1); // DMA2_STR(2)
        stm_uart->rx_dma->CR = DMA_CHSEL(5);
        tx_irq = IRQ_DMA2_STREAM6;
        rx_irq = IRQ_USART6;
        break;
#if defined(ARM_STM32F42xxx) || defined(ARM_STM32F43xxx)
    case 7:
        RCC->APB1ENR |= RCC_UART7EN;
        RCC->APB1LPENR |= RCC_UART7LPEN;
        RCC->AHB1ENR |= RCC_DMA1EN;
        stm_uart->reg = UART7;
        stm_uart->tx_dma = DMA1_STR(1);
        stm_uart->tx_dma->CR = DMA_CHSEL(5);
        stm_uart->rx_dma = DMA1_STR(3);
        stm_uart->rx_dma->CR = DMA_CHSEL(5);
        tx_irq = IRQ_DMA1_STREAM1;
        rx_irq = IRQ_UART7;
        break;
    case 8:
        RCC->APB1ENR |= RCC_UART8EN;
        RCC->APB1LPENR |= RCC_UART8LPEN;
        RCC->AHB1ENR |= RCC_DMA1EN;
        stm_uart->reg = UART8;
        stm_uart->tx_dma = DMA1_STR(0);
        stm_uart->tx_dma->CR = DMA_CHSEL(5);
        stm_uart->rx_dma = DMA1_STR(6);
        stm_uart->rx_dma->CR = DMA_CHSEL(5);
        tx_irq = IRQ_DMA2_STREAM0;
        rx_irq = IRQ_UART8;
        break;
#endif
    default:
        return UART_ERR_BAD_PORT;
    }
    
    stm_uart->reg->CR3 = USART_DMAT | USART_DMAR;
    
    stm_uart->tx_dma->PAR = (unsigned)&stm_uart->reg->DR;
    stm_uart->tx_dma->CR |= DMA_MSIZE_8 | DMA_PSIZE_8 | DMA_MINC |
        DMA_DIR_M2P | DMA_TEIE | DMA_TCIE;
        
    stm_uart->rx_dma->PAR = (unsigned)&stm_uart->reg->DR;
    stm_uart->rx_dma->M0AR = (unsigned)stm_uart->rx_buffer;
    stm_uart->rx_dma->NDTR = UART_RXBUFSZ;
    stm_uart->rx_dma->CR |= DMA_MSIZE_8 | DMA_PSIZE_8 | DMA_MINC | 
        DMA_DIR_P2M | DMA_CIRC | DMA_EN;
    
    mutex_attach_irq(&stm_uart->uartif.lock, tx_irq, clear_tx_dma_intr_flags, stm_uart);
    mutex_attach_irq(&stm_uart->rxne_mutex, rx_irq, 0, 0);
    
    return UART_ERR_OK;
}

