// File Name: rs485_init.h
// Author: kukukukuku~
// Date: 2025-03-26

#ifndef __RS485_INIT_H
#define __RS485_INIT_H

#include "./RS485/inc/rs485_macros.h"

/**
 * @brief   RS485初始化
 * @param   txBufferAddr:   发送缓冲区地址
 * @param   rxBufferAddr:   接收缓冲区地址
 * @retval  None
 * @note    None
*/
void rs485_init(const uint32_t txBufferAddr, const uint32_t rxBufferAddr);

#endif //__RS485_INIT_H
