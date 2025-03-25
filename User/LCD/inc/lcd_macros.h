// File Name: lcd_macros.h
// Author: kukukukuku~
// Date: 2025-03-05

#ifndef __LCD_MACROS_H
#define __LCD_MACROS_H

#include "at32f421.h"

#define LCD_MOSI_GPIO_PORT                  GPIOB
#define LCD_MOSI_GPIO_PIN                   GPIO_PINS_5

#define LCD_SCK_GPIO_PORT                   GPIOB
#define LCD_SCK_GPIO_PIN                    GPIO_PINS_3

#define LCD_CS_GPIO_PORT                    GPIOA
#define LCD_CS_GPIO_PIN                     GPIO_PINS_15

#define LCD_DC_GPIO_PORT                    GPIOB
#define LCD_DC_GPIO_PIN                     GPIO_PINS_4

#define LCD_RES_GPIO_PORT                   GPIOB
#define LCD_RES_GPIO_PIN                    GPIO_PINS_6

#define LCD_BLK_GPIO_PORT                   GPIOB
#define LCD_BLK_GPIO_PIN                    GPIO_PINS_7


#define LCD_WIDTH                           (160)
#define LCD_HEIGHT                          (40)


#define LCD_COLOR(r, g, b)                  ((r & 0x1F) << 11 | (g & 0x3F) << 5 | (b & 0x1F))
#define LCD_COLOR_BLACK                     (0x0000)
#define LCD_COLOR_WHITE                     (0xFFFF)
#define LCD_COLOR_RED                       (0xF800)
#define LCD_COLOR_GREEN                     (0x07E0)
#define LCD_COLOR_BLUE                      (0x001F)
#define LCD_COLOR_YELLOW                    (0xFFE0)
#define LCD_COLOR_CYAN                      (0x07FF)
#define LCD_COLOR_PURPLE                    (0xF81F)
#define LCD_COLOR_ORANGE                    (0xFD20)

#endif //__LCD_MACROS_H
