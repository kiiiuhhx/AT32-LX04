// File Name: usart_task.c
// Author: kukukukuku~
// Date: 2025-03-25

#include "./USART/inc/usart_task.h"

//USART任务句柄
TaskHandle_t usartTaskHandle;


/**
 * @brief   USART任务
 * @param   arg:    任务参数
 * @retval  None
 * @note    None
*/
void usart_task(void *arg)
{
    volatile static UBaseType_t usartTaskStackHighWaterMark;
    
    //USART对象初始化
    usart_obj_init(&usartObj, USART_USART, USART_TX_DMA_CHANNEL, USART_RX_DMA_CHANNEL);
    
    while(1)
    {
        //接收数据处理
        if(usartObj.rxState == USART_RX_STATE_FINISHED)
        {
            //回显
            usart_copy_data_to_tx_buffer(&usartObj, usartObj.rxBuffer, usartObj.rxDataLen);
            usart_send_data(&usartObj);
            
            //重启接收DMA，准备接收下一轮数据
            usart_rx_dma_restart(&usartObj);
        }
        
        usartTaskStackHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        
        vTaskDelay(10);
    }
}


/**
 * @brief   USART TX DMA中断服务函数
 * @param   None
 * @retval  None
 * @note    不可调用
*/
void USART_TX_DMA_IRQ_HANDLER(void)
{
    if(dma_flag_get(USART_TX_DMA_FDT_FLAG))
    {
        usart_tx_irq_handler(&usartObj);
        
        dma_flag_clear(USART_TX_DMA_FDT_FLAG);
    }
}


/**
 * @brief   USART中断服务函数
 * @param   None
 * @retval  None
 * @note    不可调用
*/
void USART_USART_IRQ_HANDLER(void)
{
    if(usart_interrupt_flag_get(usartObj.usart, USART_IDLEF_FLAG))
    {
        usart_rx_irq_handler(&usartObj);
        
        usart_data_receive(usartObj.usart);
    }
}
