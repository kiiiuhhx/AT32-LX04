// File Name: rs485_task.c
// Author: kukukukuku~
// Date: 2025-03-26

#include "./RS485/inc/rs485_task.h"

//RS485任务句柄
TaskHandle_t rs485TaskHandle;


/**
 * @brief   RS485任务
 * @param   arg:    任务参数
 * @retval  None
 * @note    None
*/
void rs485_task(void *arg)
{
    volatile static UBaseType_t rs485TaskStackHighWaterMark;
    
    //RS485对象初始化
    rs485_obj_init(&rs485Obj, RS485_M_GPIO_PORT, RS485_M_GPIO_PIN, RS485_USART, RS485_TX_DMA_CHANNEL, RS485_RX_DMA_CHANNEL);
    
    while(1)
    {
        //接收数据处理
        if(rs485Obj.rxState == RS485_RX_STATE_FINISHED)
        {
            //判断接收到的数据是否完整
            if(rs485Obj.rxDataLen >= 6)
            {
                //计算CRC校验码
                uint16_t crcVal = rs485_modbus_crc16_calc(&rs485Obj, rs485Obj.rxBuffer, rs485Obj.rxDataLen - 2);
                
                //匹配从机地址和校验码
                if(rs485Obj.rxBuffer[0] == rs485Obj.addr && rs485Obj.rxBuffer[rs485Obj.rxDataLen - 2] << 8 | rs485Obj.rxBuffer[rs485Obj.rxDataLen - 1] == crcVal)
                {
                    //匹配寄存器地址
                    switch(rs485Obj.rxBuffer[2] << 8 | rs485Obj.rxBuffer[3])
                    {
                        default:
                            break;
                    }
                }
            }
            
            //重启接收DMA，准备接收下一轮数据
            rs485_rx_dma_restart(&rs485Obj);
        }
        
        rs485TaskStackHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        
        vTaskDelay(50);
    }
}


/**
 * @brief   RS485 TX DMA中断服务函数
 * @param   None
 * @retval  None
 * @note    不可调用
*/
void RS485_TX_DMA_IRQ_HANDLER(void)
{
    if(dma_flag_get(RS485_TX_DMA_FDT_FLAG))
    {
        rs485_tx_dma_irq_handler(&rs485Obj);
        
        dma_flag_clear(RS485_TX_DMA_FDT_FLAG);
    }
}


/**
 * @brief   RS485 USART中断服务函数
 * @param   None
 * @retval  None
 * @note    不可调用
*/
void RS485_USART_IRQ_HANDLER(void)
{
    if(usart_interrupt_flag_get(rs485Obj.usart, USART_IDLEF_FLAG))
    {
        rs485_rx_irq_handler(&rs485Obj);
        
        usart_data_receive(rs485Obj.usart);
    }
}
