// File Name: lcd_user.h
// Author: kukukukuku~
// Date: 2025-03-06

#ifndef __LCD_USER_H
#define __LCD_USER_H

#include "./LCD/inc/lcd_interface.h"

//LCD界面模式
typedef enum LCD_PAGE_MODE_T
{
    LCD_PAGE_MODE_MAIN = 0,
    LCD_PAGE_MODE_SETTING,
}LCD_PAGE_MODE_T, *pLCD_PAGE_MODE_T;

//LCD界面
typedef struct LCD_PAGE_T
{
    LCD_PAGE_MODE_T mode;
    uint8_t mainId;
    uint8_t mainPageNum;
}LCD_PAGE_T, *pLCD_PAGE_T;

//LCD GPIO对象
extern LCD_GPIO_OBJ_T lcdGpioObj;

//LCD对象
extern LCD_OBJ_T lcdObj;

//LCD界面
extern LCD_PAGE_T lcdPage;

#endif //__LCD_USER_H
