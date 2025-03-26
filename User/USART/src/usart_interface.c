// File Name: usart_interface.c
// Author: kukukukuku~
// Date: 2025-03-25

#include "stddef.h"
#include "string.h"

#include "./USART/inc/usart_interface.h"

/**
 * @brief   USART对象初始化
 * @param   usartObj:       USART对象
 * @param   usart:          USART外设
 * @param   txDmaChannel:   发送DMA通道
 * @param   rxDmaChannel:   接收DMA通道
 * @retval  None
 * @note    None
*/
void usart_obj_init(USART_OBJ_T * const usartObj, usart_type * const usart, dma_channel_type * const txDmaChannel, dma_channel_type * const rxDmaChannel)
{
    if(usartObj == NULL || usart == NULL || rxDmaChannel == NULL)
        return;
    
    memset(usartObj, 0, sizeof(USART_OBJ_T));
    usartObj->usart        = usart;
    usartObj->txDmaChannel = txDmaChannel;
    usartObj->rxDmaChannel = rxDmaChannel;
    usartObj->txState      = USART_TX_STATE_IDLE;
    usartObj->rxState      = USART_RX_STATE_WATING;
}


/**
 * @brief   USART拷贝数据到发送缓冲区
 * @param   usartObj:   USART对象
 * @param   data:       要拷贝的数据
 * @param   dataLen:    要拷贝的数据长度
 * @retval  拷贝结果
 *          @arg 0: 成功
 *          @arg 1: 成功，但是数据长度溢出，只截取到最大长度
 *          @arg 2: 失败
 * @note    None
*/
uint8_t usart_copy_data_to_tx_buffer(USART_OBJ_T * const usartObj, uint8_t * const data, const uint16_t dataLen)
{
    uint16_t txBufferSize = 0;
    
    if(usartObj == NULL || data == NULL || dataLen == 0)
        return 2;
    
    txBufferSize = sizeof(usartObj->txBuffer);
    
    memset(usartObj->txBuffer, 0, txBufferSize);
    
    if(dataLen > txBufferSize)
    {
        memcpy(usartObj->txBuffer, data, txBufferSize);
        usartObj->txDataLen = txBufferSize;
        return 1;
    }
    else
    {
        memcpy(usartObj->txBuffer, data, dataLen);
        usartObj->txDataLen = dataLen;
    }
    
    return 0;
}


/**
 * @brief   USART发送数据
 * @param   usartObj:   USART对象
 * @retval  发送结果
 *          @arg 0: 成功
 *          @arg 1: 失败
 * @note    None
*/
uint8_t usart_send_data(USART_OBJ_T * const usartObj)
{
    if(usartObj == NULL || usartObj->txState == USART_TX_STATE_BUSY)
        return 1;
    
    dma_data_number_set(usartObj->txDmaChannel, usartObj->txDataLen);
    dma_channel_enable(usartObj->txDmaChannel, TRUE);
    usartObj->txState = USART_TX_STATE_BUSY;
    
    return 0;
}


/**
 * @brief   USART发送中断处理
 * @param   usartObj:   USART对象
 * @retval  None
 * @note    只能在发送DMA中断服务函数中调用
*/
void usart_tx_dma_irq_handler(USART_OBJ_T * const usartObj)
{
    if(usartObj == NULL || usartObj->txDmaChannel == NULL)
        return;
    
    dma_channel_enable(usartObj->txDmaChannel, FALSE);
    usartObj->txState = USART_TX_STATE_IDLE;
}


/**
 * @brief   USART接收中断处理
 * @param   usartObj:   USART对象
 * @retval  None
 * @note    只能在USART中断服务函数中调用
*/
void usart_rx_irq_handler(USART_OBJ_T * const usartObj)
{
    if(usartObj == NULL || usartObj->rxDmaChannel == NULL)
        return;
    
    dma_channel_enable(usartObj->rxDmaChannel, FALSE);
    usartObj->rxDataLen = USART_RX_BUFFER_LEN - dma_data_number_get(usartObj->rxDmaChannel);
    dma_data_number_set(usartObj->rxDmaChannel, USART_RX_BUFFER_LEN);
    
    usartObj->rxState = USART_RX_STATE_FINISHED;
}


/**
 * @brief   USART接收DMA重启
 * @param   usartObj:   USART对象
 * @retval  None
 * @note    在USART接收数据处理完成后调用，用于重启DMA
*/
void usart_rx_dma_restart(USART_OBJ_T * const usartObj)
{
    if(usartObj == NULL || usartObj->rxDmaChannel == NULL)
        return;
    
    memset(usartObj->rxBuffer, 0, sizeof(usartObj->rxBuffer));
    dma_channel_enable(usartObj->rxDmaChannel, TRUE);
    usartObj->rxState = USART_RX_STATE_WATING;
}
