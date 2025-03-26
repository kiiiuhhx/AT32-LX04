// File Name: rs485_interface.h
// Author: kukukukuku~
// Date: 2025-03-26

#ifndef __RS485_INTERFACE_H
#define __RS485_INTERFACE_H

#include "./RS485/inc/rs485_macros.h"

//RS485发送状态
typedef enum RS485_TX_STATE_T
{
    RS485_TX_STATE_BUSY,
    RS485_TX_STATE_IDLE,
}RS485_TX_STATE_T, *pRS485_TX_STATE_T;

//RS485接收状态
typedef enum RS485_RX_STATE_T
{
    RS485_RX_STATE_WATING,
    RS485_RX_STATE_FINISHED,
}RS485_RX_STATE_T, *pRS485_RX_STATE_T;

//RS485对象
typedef struct RS485_OBJ_T
{
    gpio_type *modePort;
    uint32_t modePin;
    usart_type *usart;
    dma_channel_type *txDmaChannel;
    dma_channel_type *rxDmaChannel;
    uint16_t txDataLen;
    uint16_t rxDataLen;
    uint8_t txBuffer[RS485_TX_BUFFER_LEN];
    uint8_t rxBuffer[RS485_RX_BUFFER_LEN];
    RS485_TX_STATE_T txState;
    RS485_RX_STATE_T rxState;
    uint8_t addr;
}RS485_OBJ_T, *pRS485_OBJ_T;


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
    dma_channel_type * const txDmaChannel, dma_channel_type * const rxDmaChannel);


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
uint8_t rs485_copy_data_to_tx_buffer(RS485_OBJ_T * const rs485Obj, uint8_t * const data, const uint16_t dataLen);


/**
 * @brief   RS485发送数据
 * @param   rs485Obj:   RS485对象
 * @retval  发送结果
 *          @arg 0: 成功
 *          @arg 1: 失败
 * @note    None
*/
uint8_t rs485_send_data(RS485_OBJ_T * const rs485Obj);


/**
 * @brief   RS485发送中断处理
 * @param   rs485Obj:   RS485对象
 * @retval  None
 * @note    只能在发送DMA中断服务函数中调用
*/
void rs485_tx_dma_irq_handler(RS485_OBJ_T * const rs485Obj);


/**
 * @brief   RS485接收中断处理
 * @param   rs485Obj:   RS485对象
 * @retval  None
 * @note    只能在RS485 USART中断服务函数中调用
*/
void rs485_rx_irq_handler(RS485_OBJ_T * const rs485Obj);


/**
 * @brief   RS485接收DMA重启
 * @param   rs485Obj:   RS485对象
 * @retval  None
 * @note    在RS485接收数据处理完成后调用，用于重启DMA
*/
void rs485_rx_dma_restart(RS485_OBJ_T * const rs485Obj);


/**
 * @brief   RS485 MODBUS CRC16检验码计算
 * @param   rs485Obj:   RS485对象
 * @param   data:       数据源
 * @param   dataLen:    数据个数
 * @retval  计算结果
 * @note    None
*/
uint16_t rs485_modbus_crc16_calc(RS485_OBJ_T * const rs485Obj, uint8_t * const data, const uint16_t dataLen);


/**
 * @brief   RS485 MODBUS读单个寄存器响应
 * @param   rs485Obj:       RS485对象
 * @param   regAddrList:    寄存器地址列表
 * @param   regAddr:        寄存器地址
 * @retval  None
 * @note    None
*/
void rs485_modbus_read_single_register_response(RS485_OBJ_T * const rs485Obj, uint16_t ** const regAddrList, const uint16_t regAddr);


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
    const uint16_t regAddr, uint16_t dataRec, const uint16_t regDataMax, const uint16_t regDataMin);

#endif //__RS485_INTERFACE_H
