// File Name: usart_interface.h
// Author: kukukukuku~
// Date: 2025-03-25

#ifndef __USART_INTERFACE_H
#define __USART_INTERFACE_H

#include "./USART/inc/usart_macros.h"

//USART发送状态
typedef enum USART_TX_STATE_T
{
    USART_TX_STATE_BUSY,
    USART_TX_STATE_IDLE,
}USART_TX_STATE_T, *pUSART_TX_STATE_T;

//USART接收状态
typedef enum USART_RX_STATE_T
{
    USART_RX_STATE_WATING,
    USART_RX_STATE_FINISHED,
}USART_RX_STATE_T, *pUSART_RX_STATE_T;

//USART对象
typedef struct USART_OBJ_T
{
    usart_type *usart;
    dma_channel_type *txDmaChannel;
    dma_channel_type *rxDmaChannel;
    uint16_t txDataLen;
    uint16_t rxDataLen;
    uint8_t txBuffer[USART_TX_BUFFER_LEN];
    uint8_t rxBuffer[USART_RX_BUFFER_LEN];
    USART_TX_STATE_T txState;
    USART_RX_STATE_T rxState;
}USART_OBJ_T, *pUSART_OBJ_T;


/**
 * @brief   USART对象初始化
 * @param   usartObj:       USART对象
 * @param   usart:          USART外设
 * @param   txDmaChannel:   发送DMA通道
 * @param   rxDmaChannel:   接收DMA通道
 * @retval  None
 * @note    None
*/
void usart_obj_init(USART_OBJ_T * const usartObj, usart_type * const usart, dma_channel_type * const txDmaChannel, dma_channel_type * const rxDmaChannel);


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
uint8_t usart_copy_data_to_tx_buffer(USART_OBJ_T * const usartObj, uint8_t * const data, const uint16_t dataLen);


/**
 * @brief   USART发送数据
 * @param   usartObj:   USART对象
 * @retval  发送结果
 *          @arg 0: 成功
 *          @arg 1: 失败
 * @note    None
*/
uint8_t usart_send_data(USART_OBJ_T * const usartObj);


/**
 * @brief   USART发送中断处理
 * @param   usartObj:   USART对象
 * @retval  None
 * @note    只能在发送DMA中断服务函数中调用
*/
void usart_tx_dma_irq_handler(USART_OBJ_T * const usartObj);


/**
 * @brief   USART接收中断处理
 * @param   usartObj:   USART对象
 * @retval  None
 * @note    只能在USART中断服务函数中调用
*/
void usart_rx_irq_handler(USART_OBJ_T * const usartObj);


/**
 * @brief   USART接收DMA重启
 * @param   usartObj:   USART对象
 * @retval  None
 * @note    在USART接收数据处理完成后调用，用于重启DMA
*/
void usart_rx_dma_restart(USART_OBJ_T * const usartObj);

#endif //__USART_INTERFACE_H
