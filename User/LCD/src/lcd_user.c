// File Name: lcd_user.c
// Author: kukukukuku~
// Date: 2025-03-06

#include "./LCD/inc/lcd_user.h"

//LCD GPIO对象
LCD_GPIO_OBJ_T lcdGpioObj;

//LCD对象
LCD_OBJ_T lcdObj;

//LCD界面
LCD_PAGE_T lcdPage = {LCD_PAGE_MODE_MAIN, 0, 2};
