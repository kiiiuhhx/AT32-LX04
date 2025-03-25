// File Name: usart_macros.h
// Author: kukukukuku~
// Date: 2025-03-24

#ifndef __USART_MACROS_H
#define __USART_MACROS_H

#include "at32f421.h"

#define USART_TX_GPIO_PORT                  GPIOA
#define USART_TX_GPIO_PIN                   GPIO_PINS_9
#define USART_TX_GPIO_MUX_PIN_SOURCE        GPIO_PINS_SOURCE9
#define USART_TX_GPIO_MUX                   GPIO_MUX_1

#define USART_RX_GPIO_PORT                  GPIOA
#define USART_RX_GPIO_PIN                   GPIO_PINS_10
#define USART_RX_GPIO_MUX_PIN_SOURCE        GPIO_PINS_SOURCE10
#define USART_RX_GPIO_MUX                   GPIO_MUX_1


#define USART_TX_DMA                        DMA1
#define USART_TX_DMA_CHANNEL                DMA1_CHANNEL2
#define USART_TX_DMA_IRQ                    DMA1_Channel3_2_IRQn
#define USART_TX_DMA_IRQ_HANDLER            DMA1_Channel3_2_IRQHandler
#define USART_TX_DMA_FDT_FLAG               DMA1_FDT2_FLAG

#define USART_RX_DMA                        DMA1
#define USART_RX_DMA_CHANNEL                DMA1_CHANNEL3


#define USART_USART                         USART1
#define USART_USART_BAUDRATE                (115200)
#define USART_USART_IRQ                     USART1_IRQn
#define USART_USART_IRQ_HANDLER             USART1_IRQHandler
#define USART_TX_BUFFER_LEN                 (128)
#define USART_RX_BUFFER_LEN                 (128)

#endif //__USART_MACROS_H
