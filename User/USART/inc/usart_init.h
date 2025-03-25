// File Name: usart_init.h
// Author: kukukukuku~
// Date: 2025-03-24

#ifndef __USART_INIT_H
#define __USART_INIT_H

#include "./USART/inc/usart_macros.h"

/**
 * @brief   USART初始化
 * @param   txBufferAddr:   发送缓冲区地址
 * @param   rxBufferAddr:   接收缓冲区地址
 * @retval  None
 * @note    None
*/
void usart_config_init(const uint32_t txBufferAddr, const uint32_t rxBufferAddr);

#endif //__USART_INIT_H
