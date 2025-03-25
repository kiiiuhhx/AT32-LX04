// File Name: usart_init.c
// Author: kukukukuku~
// Date: 2025-03-24

#include "./USART/inc/usart_init.h"

/**
 * @brief   USART GPIO配置
 * @param   None
 * @retval  None
 * @note    静态
*/
static void usart_gpio_config(void)
{
    gpio_init_type gpioInitStruct = 
    {
        .gpio_pins           = USART_TX_GPIO_PIN,
        .gpio_out_type       = GPIO_OUTPUT_PUSH_PULL,
        .gpio_pull           = GPIO_PULL_NONE,
        .gpio_mode           = GPIO_MODE_MUX,
        .gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER,
    };
    
    //TX
    gpio_init(USART_TX_GPIO_PORT, &gpioInitStruct);
    gpio_pin_mux_config(USART_TX_GPIO_PORT, USART_TX_GPIO_MUX_PIN_SOURCE, USART_TX_GPIO_MUX);
    
    //RX
    gpioInitStruct.gpio_pins = USART_RX_GPIO_PIN;
    gpio_init(USART_RX_GPIO_PORT, &gpioInitStruct);
    gpio_pin_mux_config(USART_RX_GPIO_PORT, USART_RX_GPIO_MUX_PIN_SOURCE, USART_RX_GPIO_MUX);
}


/**
 * @brief   USART DMA配置
 * @param   txBufferAddr:   发送缓冲区地址
 * @param   rxBufferAddr:   接收缓冲区地址
 * @retval  None
 * @note    静态
*/
static void usart_dma_config(const uint32_t txBufferAddr, const uint32_t rxBufferAddr)
{
    dma_init_type dmaInitStruct = 
    {
        .peripheral_base_addr  = (uint32_t)&USART_USART->dt,
        .memory_base_addr      = txBufferAddr,
        .direction             = DMA_DIR_MEMORY_TO_PERIPHERAL,
        .buffer_size           = USART_TX_BUFFER_LEN,
        .peripheral_inc_enable = FALSE,
        .memory_inc_enable     = TRUE,
        .peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_BYTE,
        .memory_data_width     = DMA_MEMORY_DATA_WIDTH_BYTE,
        .loop_mode_enable      = FALSE,
        .priority              = DMA_PRIORITY_HIGH,
    };
    
    //TX
    dma_reset(USART_TX_DMA_CHANNEL);
    dma_init(USART_TX_DMA_CHANNEL, &dmaInitStruct);
    dma_interrupt_enable(USART_TX_DMA_CHANNEL, DMA_FDT_INT, TRUE);
    nvic_irq_enable(USART_TX_DMA_IRQ, 1, 0);
    dma_channel_enable(USART_TX_DMA_CHANNEL, FALSE);
    
    //RX
    dmaInitStruct.memory_base_addr = rxBufferAddr;
    dmaInitStruct.direction        = DMA_DIR_PERIPHERAL_TO_MEMORY;
    dmaInitStruct.buffer_size      = USART_RX_BUFFER_LEN;
    dma_reset(USART_RX_DMA_CHANNEL);
    dma_init(USART_RX_DMA_CHANNEL, &dmaInitStruct);
    dma_channel_enable(USART_RX_DMA_CHANNEL, TRUE);
}


/**
 * @brief   USART USART配置
 * @param   None
 * @retval  None
 * @note    静态
*/
static void usart_usart_config(void)
{
    usart_init(USART_USART, USART_USART_BAUDRATE, USART_DATA_8BITS, USART_STOP_1_BIT);
    usart_parity_selection_config(USART_USART, USART_PARITY_NONE);
    usart_transmitter_enable(USART_USART, TRUE);
    usart_receiver_enable(USART_USART, TRUE);
    usart_dma_transmitter_enable(USART_USART, TRUE);
    usart_dma_receiver_enable(USART_USART, TRUE);
    usart_interrupt_enable(USART_USART, USART_IDLE_INT, TRUE);
    nvic_irq_enable(USART_USART_IRQ, 1, 0);
    usart_enable(USART_USART, TRUE);
}


/**
 * @brief   USART初始化
 * @param   txBufferAddr:   发送缓冲区地址
 * @param   rxBufferAddr:   接收缓冲区地址
 * @retval  None
 * @note    None
*/
void usart_config_init(const uint32_t txBufferAddr, const uint32_t rxBufferAddr)
{
    usart_gpio_config();
    usart_dma_config(txBufferAddr, rxBufferAddr);
    usart_usart_config();
}
