// File Name: rs485_interface.c
// Author: kukukukuku~
// Date: 2025-03-26

#include "stddef.h"
#include "string.h"

#include "FreeRTOS.h"
#include "task.h"

#include "./RS485/inc/rs485_interface.h"

/**
 * @brief   RS485对象初始化
 * @param   rs485Obj:       RS485对象
 * @param   modePort:       RS485模式控制端口
 * @param   modePin:        RS485模式控制引脚
 * @param   usart:          USART外设
 * @param   txDmaChannel:   发送DMA通道
 * @param   rxDmaChannel:   接收DMA通道
 * @retval  None
 * @note    None
*/
void rs485_obj_init(RS485_OBJ_T * const rs485Obj, gpio_type * const modePort, const uint32_t modePin, usart_type * const usart,
    dma_channel_type * const txDmaChannel, dma_channel_type * const rxDmaChannel)
{
    if(rs485Obj == NULL || usart == NULL || rxDmaChannel == NULL)
        return;
    
    memset(rs485Obj, 0, sizeof(RS485_OBJ_T));
    rs485Obj->modePort     = modePort;
    rs485Obj->modePin      = modePin;
    rs485Obj->usart        = usart;
    rs485Obj->txDmaChannel = txDmaChannel;
    rs485Obj->rxDmaChannel = rxDmaChannel;
    rs485Obj->txState      = RS485_TX_STATE_IDLE;
    rs485Obj->rxState      = RS485_RX_STATE_WATING;
    rs485Obj->addr         = RS485_MODBUS_ADDR;
}


/**
 * @brief   RS485切换到发送模式
 * @param   rs485Obj:   RS485对象
 * @retval  None
 * @note    静态
*/
static void rs485_set_mode_send(RS485_OBJ_T * const rs485Obj)
{
    if(rs485Obj == NULL || rs485Obj->modePort == NULL)
        return;
    
    gpio_bits_set(rs485Obj->modePort, rs485Obj->modePin);
}


/**
 * @brief   RS485切换到接收模式
 * @param   rs485Obj:   RS485对象
 * @retval  None
 * @note    静态
*/
static void rs485_set_mode_receive(RS485_OBJ_T * const rs485Obj)
{
    if(rs485Obj == NULL || rs485Obj->modePort == NULL)
        return;
    
    gpio_bits_reset(rs485Obj->modePort, rs485Obj->modePin);
}


/**
 * @brief   RS485拷贝数据到发送缓冲区
 * @param   rs485Obj:   RS485对象
 * @param   data:       要拷贝的数据
 * @param   dataLen:    要拷贝的数据长度
 * @retval  拷贝结果
 *          @arg 0: 成功
 *          @arg 1: 成功，但是数据长度溢出，只截取到最大长度
 *          @arg 2: 失败
 * @note    None
*/
uint8_t rs485_copy_data_to_tx_buffer(RS485_OBJ_T * const rs485Obj, uint8_t * const data, const uint16_t dataLen)
{
    uint16_t txBufferSize = 0;
    
    if(rs485Obj == NULL || data == NULL || dataLen == 0)
        return 2;
    
    txBufferSize = sizeof(rs485Obj->txBuffer);
    
    memset(rs485Obj->txBuffer, 0, txBufferSize);
    
    if(dataLen > txBufferSize)
    {
        memcpy(rs485Obj->txBuffer, data, txBufferSize);
        rs485Obj->txDataLen = txBufferSize;
        return 1;
    }
    else
    {
        memcpy(rs485Obj->txBuffer, data, dataLen);
        rs485Obj->txDataLen = dataLen;
    }
    
    return 0;
}


/**
 * @brief   RS485发送数据
 * @param   rs485Obj:   RS485对象
 * @retval  发送结果
 *          @arg 0: 成功
 *          @arg 1: 失败
 * @note    None
*/
uint8_t rs485_send_data(RS485_OBJ_T * const rs485Obj)
{
    if(rs485Obj == NULL || rs485Obj->txState == RS485_TX_STATE_BUSY)
        return 1;
    
    //发送数据之前禁止接收
    dma_channel_enable(rs485Obj->rxDmaChannel, FALSE);
    
    //切换到发送模式
    rs485_set_mode_send(rs485Obj);
    usart_transmitter_enable(rs485Obj->usart, TRUE);
    usart_receiver_enable(rs485Obj->usart, FALSE);
    
    dma_data_number_set(rs485Obj->txDmaChannel, rs485Obj->txDataLen);
    dma_channel_enable(rs485Obj->txDmaChannel, TRUE);
    
    rs485Obj->txState = RS485_TX_STATE_BUSY;
    
    return 0;
}


/**
 * @brief   RS485发送中断处理
 * @param   rs485Obj:   RS485对象
 * @retval  None
 * @note    只能在发送DMA中断服务函数中调用
*/
void rs485_tx_dma_irq_handler(RS485_OBJ_T * const rs485Obj)
{
    if(rs485Obj == NULL || rs485Obj->txDmaChannel == NULL)
        return;
    
    //等待发送完成
    while(!usart_flag_get(RS485_USART, USART_TDC_FLAG));
   
    //切换到接收模式
    rs485_set_mode_receive(rs485Obj);
    usart_transmitter_enable(rs485Obj->usart, FALSE);
    usart_receiver_enable(rs485Obj->usart, TRUE);
    
    //恢复接收
    dma_channel_enable(rs485Obj->txDmaChannel, FALSE);
    dma_channel_enable(rs485Obj->rxDmaChannel, TRUE);
    
    rs485Obj->txState = RS485_TX_STATE_IDLE;
}


/**
 * @brief   RS485接收中断处理
 * @param   rs485Obj:   RS485对象
 * @retval  None
 * @note    只能在RS485 USART中断服务函数中调用
*/
void rs485_rx_irq_handler(RS485_OBJ_T * const rs485Obj)
{
    if(rs485Obj == NULL || rs485Obj->rxDmaChannel == NULL)
        return;
    
    dma_channel_enable(rs485Obj->rxDmaChannel, FALSE);
    rs485Obj->rxDataLen = RS485_RX_BUFFER_LEN - dma_data_number_get(rs485Obj->rxDmaChannel);
    dma_data_number_set(rs485Obj->rxDmaChannel, RS485_RX_BUFFER_LEN);
    
    rs485Obj->rxState = RS485_RX_STATE_FINISHED;
}


/**
 * @brief   RS485接收DMA重启
 * @param   rs485Obj:   RS485对象
 * @retval  None
 * @note    在RS485接收数据处理完成后调用，用于重启DMA
*/
void rs485_rx_dma_restart(RS485_OBJ_T * const rs485Obj)
{
    if(rs485Obj == NULL || rs485Obj->rxDmaChannel == NULL)
        return;
    
    memset(rs485Obj->rxBuffer, 0, sizeof(rs485Obj->rxBuffer));
    dma_channel_enable(rs485Obj->rxDmaChannel, TRUE);
    rs485Obj->rxState = RS485_RX_STATE_WATING;
}


/**
 * @brief   RS485 MODBUS CRC16检验码计算
 * @param   rs485Obj:   RS485对象
 * @param   data:       数据源
 * @param   dataLen:    数据个数
 * @retval  计算结果
 * @note    None
*/
uint16_t rs485_modbus_crc16_calc(RS485_OBJ_T * const rs485Obj, uint8_t * const data, const uint16_t dataLen)
{
    uint16_t crcInit = 0xFFFF;
    uint16_t crcPoly = 0xA001;
    uint16_t crcVal = crcInit;
    
    if(rs485Obj == NULL)
        return 0;
    
    for(uint16_t i = 0; i < dataLen; i++)
    {
        crcVal ^= data[i];
        for(uint8_t j = 0; j < 8; j++)
        {
            crcVal & 0x0001 ? (crcVal = (crcVal>>1) ^ crcPoly) : (crcVal >>= 1);
        }
    }
    
    return crcVal;
}


/**
 * @brief   RS485 MODBUS读单个寄存器响应
 * @param   rs485Obj:       RS485对象
 * @param   regAddrList:    寄存器地址列表
 * @param   regAddr:        寄存器地址
 * @retval  None
 * @note    None
*/
void rs485_modbus_read_single_register_response(RS485_OBJ_T * const rs485Obj, uint16_t ** const regAddrList, const uint16_t regAddr)
{
    uint16_t i = 0;
    uint16_t crcVal = 0;
    uint8_t data[RS485_TX_BUFFER_LEN] = {0};
    
    if(rs485Obj == NULL || rs485Obj->usart == NULL || rs485Obj->modePort == NULL || rs485Obj->txDmaChannel == NULL ||
        regAddrList == NULL || regAddr >= RS485_MODBUS_REG_NUM || regAddrList[regAddr] == NULL)
        return;
    
    data[i++] = rs485Obj->addr;
    data[i++] = RS485_MODBUS_READ_CODE;
    data[i++] = regAddr >> 8 & 0xFF;
    data[i++] = regAddr & 0xFF;
    data[i++] = *(regAddrList[regAddr]) >> 8 & 0xFF;
    data[i++] = *(regAddrList[regAddr]) & 0xFF;
    crcVal = rs485_modbus_crc16_calc(rs485Obj, data, i);
    data[i++] = crcVal >> 8 & 0xFF;
    data[i++] = crcVal & 0xFF;
    
    rs485_copy_data_to_tx_buffer(rs485Obj, data, i);
    rs485_send_data(rs485Obj);
}


/**
 * @brief   RS485 MODBUS写单个寄存器响应
 * @param   rs485Obj:       RS485对象
 * @param   regAddrList:    寄存器地址列表
 * @param   regAddr:        寄存器地址
 * @param   dataRec:        接收到的数据
 * @param   regDataMax:     寄存器数据最大值
 * @param   regDataMin:     寄存器数据最小值
 * @retval  寄存器数据
 * @note    None
*/
uint16_t rs485_modbus_write_single_register_response(RS485_OBJ_T * const rs485Obj, uint16_t ** const regAddrList,
    const uint16_t regAddr, uint16_t dataRec, const uint16_t regDataMax, const uint16_t regDataMin)
{
    uint16_t i = 0;
    uint16_t crcVal = 0;
    uint8_t data[RS485_TX_BUFFER_LEN] = {0};
    
    if(rs485Obj == NULL || rs485Obj->usart == NULL || rs485Obj->modePort == NULL || rs485Obj->txDmaChannel == NULL ||
        regAddrList == NULL || regAddr >= RS485_MODBUS_REG_NUM || regAddrList[regAddr] == NULL || regDataMax < regDataMin)
        return 0xFFFF;
    
    if(dataRec > regDataMax)
        dataRec = regDataMax;
    else if(dataRec < regDataMin)
        dataRec = regDataMin;
    
    data[i++] = rs485Obj->addr;
    data[i++] = RS485_MODBUS_WRITE_CODE;
    data[i++] = regAddr >> 8 & 0xFF;
    data[i++] = regAddr & 0xFF;
    data[i++] = dataRec >> 8 & 0xFF;
    data[i++] = dataRec & 0xFF;
    crcVal = rs485_modbus_crc16_calc(rs485Obj, data, i);
    data[i++] = crcVal >> 8 & 0xFF;
    data[i++] = crcVal & 0xFF;
    
    //写入数据
    *(regAddrList[regAddr]) = dataRec;
    
    rs485_copy_data_to_tx_buffer(rs485Obj, data, i);
    rs485_send_data(rs485Obj);
    
    return dataRec;
}
