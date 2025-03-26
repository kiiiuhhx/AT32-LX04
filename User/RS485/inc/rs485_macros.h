// File Name: rs485_macros.h
// Author: kukukukuku~
// Date: 2025-03-26

#ifndef __RS485_MACROS_H
#define __RS485_MACROS_H

#include "at32f421.h"

#define RS485_M_GPIO_PORT                   GPIOA
#define RS485_M_GPIO_PIN                    GPIO_PINS_1

#define RS485_TX_GPIO_PORT                  GPIOA
#define RS485_TX_GPIO_PIN                   GPIO_PINS_2
#define RS485_TX_GPIO_MUX_PIN_SOURCE        GPIO_PINS_SOURCE2
#define RS485_TX_GPIO_MUX                   GPIO_MUX_1

#define RS485_RX_GPIO_PORT                  GPIOA
#define RS485_RX_GPIO_PIN                   GPIO_PINS_3
#define RS485_RX_GPIO_MUX_PIN_SOURCE        GPIO_PINS_SOURCE3
#define RS485_RX_GPIO_MUX                   GPIO_MUX_1


#define RS485_TX_DMA                        DMA1
#define RS485_TX_DMA_CHANNEL                DMA1_CHANNEL4
#define RS485_TX_DMA_IRQ                    DMA1_Channel5_4_IRQn
#define RS485_TX_DMA_IRQ_HANDLER            DMA1_Channel5_4_IRQHandler
#define RS485_TX_DMA_FDT_FLAG               DMA1_FDT4_FLAG

#define RS485_RX_DMA                        DMA1
#define RS485_RX_DMA_CHANNEL                DMA1_CHANNEL5


#define RS485_USART                         USART2
#define RS485_USART_BAUDRATE                (115200)
#define RS485_USART_IRQ                     USART2_IRQn
#define RS485_USART_IRQ_HANDLER             USART2_IRQHandler
#define RS485_TX_BUFFER_LEN                 (128)
#define RS485_RX_BUFFER_LEN                 (128)


#define RS485_MODBUS_ADDR                   (0x01)
#define RS485_MODBUS_READ_CODE              (0x11)
#define RS485_MODBUS_WRITE_CODE             (0x12)


#define RS485_MODBUS_REG_NUM                (10)

#endif //__RS485_MACROS_H
