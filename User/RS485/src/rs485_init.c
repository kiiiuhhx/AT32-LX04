// File Name: rs485_init.c
// Author: kukukukuku~
// Date: 2025-03-26

#include "./RS485/inc/rs485_init.h"

/**
 * @brief   RS485 GPIO配置
 * @param   None
 * @retval  None
 * @note    静态
*/
static void rs485_gpio_config(void)
{
    gpio_init_type gpioInitStruct = 
    {
        .gpio_pins           = RS485_M_GPIO_PIN,
        .gpio_out_type       = GPIO_OUTPUT_PUSH_PULL,
        .gpio_pull           = GPIO_PULL_NONE,
        .gpio_mode           = GPIO_MODE_OUTPUT,
        .gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER,
    };
    
    //M
    gpio_init(RS485_M_GPIO_PORT, &gpioInitStruct);
    gpio_bits_reset(RS485_M_GPIO_PORT, RS485_M_GPIO_PIN);
    
    //TX
    gpioInitStruct.gpio_mode = GPIO_MODE_MUX;
    gpioInitStruct.gpio_pins = RS485_TX_GPIO_PIN;
    gpio_init(RS485_TX_GPIO_PORT, &gpioInitStruct);
    gpio_pin_mux_config(RS485_TX_GPIO_PORT, RS485_TX_GPIO_MUX_PIN_SOURCE, RS485_TX_GPIO_MUX);
    
    //RX
    gpioInitStruct.gpio_pins = RS485_RX_GPIO_PIN;
    gpio_init(RS485_RX_GPIO_PORT, &gpioInitStruct);
    gpio_pin_mux_config(RS485_RX_GPIO_PORT, RS485_RX_GPIO_MUX_PIN_SOURCE, RS485_RX_GPIO_MUX);
}


/**
 * @brief   RS485 DMA配置
 * @param   txBufferAddr:   发送缓冲区地址
 * @param   rxBufferAddr:   接收缓冲区地址
 * @retval  None
 * @note    静态
*/
static void rs485_dma_config(const uint32_t txBufferAddr, const uint32_t rxBufferAddr)
{
    dma_init_type dmaInitStruct = 
    {
        .peripheral_base_addr  = (uint32_t)&RS485_USART->dt,
        .memory_base_addr      = txBufferAddr,
        .direction             = DMA_DIR_MEMORY_TO_PERIPHERAL,
        .buffer_size           = RS485_TX_BUFFER_LEN,
        .peripheral_inc_enable = FALSE,
        .memory_inc_enable     = TRUE,
        .peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_BYTE,
        .memory_data_width     = DMA_MEMORY_DATA_WIDTH_BYTE,
        .loop_mode_enable      = FALSE,
        .priority              = DMA_PRIORITY_HIGH,
    };
    
    //TX
    dma_reset(RS485_TX_DMA_CHANNEL);
    dma_init(RS485_TX_DMA_CHANNEL, &dmaInitStruct);
    dma_interrupt_enable(RS485_TX_DMA_CHANNEL, DMA_FDT_INT, TRUE);
    nvic_irq_enable(RS485_TX_DMA_IRQ, 1, 0);
    dma_channel_enable(RS485_TX_DMA_CHANNEL, FALSE);
    
    //RX
    dmaInitStruct.memory_base_addr = rxBufferAddr;
    dmaInitStruct.direction        = DMA_DIR_PERIPHERAL_TO_MEMORY;
    dmaInitStruct.buffer_size      = RS485_RX_BUFFER_LEN;
    dma_reset(RS485_RX_DMA_CHANNEL);
    dma_init(RS485_RX_DMA_CHANNEL, &dmaInitStruct);
    dma_channel_enable(RS485_RX_DMA_CHANNEL, TRUE);
}


/**
 * @brief   RS485 USART配置
 * @param   None
 * @retval  None
 * @note    静态
*/
static void rs485_usart_config(void)
{
    usart_init(RS485_USART, RS485_USART_BAUDRATE, USART_DATA_8BITS, USART_STOP_1_BIT);
    usart_parity_selection_config(RS485_USART, USART_PARITY_NONE);
    usart_transmitter_enable(RS485_USART, TRUE);
    usart_receiver_enable(RS485_USART, TRUE);
    usart_dma_transmitter_enable(RS485_USART, TRUE);
    usart_dma_receiver_enable(RS485_USART, TRUE);
    usart_interrupt_enable(RS485_USART, USART_IDLE_INT, TRUE);
    nvic_irq_enable(RS485_USART_IRQ, 1, 0);
    usart_enable(RS485_USART, TRUE);
}


/**
 * @brief   RS485初始化
 * @param   txBufferAddr:   发送缓冲区地址
 * @param   rxBufferAddr:   接收缓冲区地址
 * @retval  None
 * @note    None
*/
void rs485_init(const uint32_t txBufferAddr, const uint32_t rxBufferAddr)
{
    rs485_gpio_config();
    rs485_dma_config(txBufferAddr, rxBufferAddr);
    rs485_usart_config();
}
