// File Name: lcd_task.c
// Author: kukukukuku~
// Date: 2025-03-06

#include "stdlib.h"

#include "./ADC/inc/adc_user.h"

#include "./Key/inc/key_user.h"

#include "./LCD/inc/lcd_font.h"
#include "./LCD/inc/lcd_picture.h"
#include "./LCD/inc/lcd_task.h"

#include "./Servo/inc/servo_user.h"

//LCD任务句柄
TaskHandle_t lcdTaskHandle;


/**
 * @brief   LCD任务
 * @param   arg:    任务参数
 * @retval  None
 * @note    None
*/
void lcd_task(void *arg)
{
    volatile static UBaseType_t lcdTaskStackHighWaterMark;
    static uint32_t randSeek;
    static uint8_t delayCnt = 50;
    static uint8_t lcdAdcFlag;
    static uint8_t lcdPageMainIdLast;
    static LCD_PAGE_MODE_T lcdPageModeLast = LCD_PAGE_MODE_MAIN;
    
    //LCD GPIO对象初始化
    lcd_gpio_obj_init(&lcdGpioObj, LCD_MOSI_GPIO_PORT, LCD_SCK_GPIO_PORT, LCD_CS_GPIO_PORT, LCD_DC_GPIO_PORT, LCD_RES_GPIO_PORT, LCD_BLK_GPIO_PORT,
                      LCD_MOSI_GPIO_PIN, LCD_SCK_GPIO_PIN, LCD_CS_GPIO_PIN, LCD_DC_GPIO_PIN, LCD_RES_GPIO_PIN, LCD_BLK_GPIO_PIN);
    
    //LCD对象初始化
    lcd_obj_init(&lcdObj, &lcdGpioObj, LCD_WIDTH, LCD_HEIGHT, LCD_DIR_NORMAL);
    
    lcd_reset(&lcdObj);                     //LCD复位
    lcd_power_on_code_init(&lcdObj);        //LCD上电指令初始化
    lcd_clear(&lcdObj, LCD_COLOR_WHITE);    //LCD清屏
    
    lcd_show_picture(&lcdObj, 33, 0, 95, 40, lcdPicSuenwLogo95x40, sizeof(lcdPicSuenwLogo95x40));
    vTaskDelay(2000);
    
    lcd_clear(&lcdObj, LCD_COLOR_BLACK);
    
    while(1)
    {
        if(lcdObj.updateSemaphoreHandle && xSemaphoreTake(lcdObj.updateSemaphoreHandle, portMAX_DELAY) == pdTRUE)
        {
            //主界面切换，刷新LCD
            if(lcdPageMainIdLast != lcdPage.mainId)
            {
                lcd_clear(&lcdObj, LCD_COLOR_BLACK);
                lcdPageMainIdLast = lcdPage.mainId;
                delayCnt = 50;
            }
            
            //界面模式切换，刷新LCD
            if(lcdPageModeLast != lcdPage.mode)
            {
                lcd_clear(&lcdObj, LCD_COLOR_BLACK);
                lcdPageModeLast = lcdPage.mode;
                delayCnt = 50;
                lcdAdcFlag = 0;
            }
            
            //主界面模式
            if(lcdPage.mode == LCD_PAGE_MODE_MAIN)
            {
                switch(lcdPage.mainId)
                {
                    case 0:
                    {
                        if(++delayCnt >= 50)
                        {
                            delayCnt = 0;
                            srand(randSeek++);
                            
                            switch(lcdAdcFlag)
                            {
                                case 0:
                                    lcd_show_str(&lcdObj, 0, 0, (uint8_t *)"adc ", LCD_EN_FONT_SIZE_8X16, lcdEnFontSourceCodePro8x16, LCD_COLOR(rand() % 0x20, rand() % 0x40, rand() % 0x20), LCD_COLOR_BLACK);
                                    lcd_show_num(&lcdObj, 64, 4, adcObj.averageFilterData[ADC_CHANNEL_ADC_INDEX], "%4u", LCD_EN_FONT_SIZE_24X32, lcdNumFontFZYTK24x32, LCD_COLOR(rand() % 0x20, rand() % 0x40, rand() % 0x20), LCD_COLOR_BLACK);
                                    break;
                                case 1:
                                    lcd_show_str(&lcdObj, 0, 0, (uint8_t *)"temp", LCD_EN_FONT_SIZE_8X16, lcdEnFontSourceCodePro8x16, LCD_COLOR(rand() % 0x20, rand() % 0x40, rand() % 0x20), LCD_COLOR_BLACK);
                                    lcd_show_num(&lcdObj, 64, 4, adcObj.averageFilterData[ADC_CHANNEL_TEMPERATURE_INDEX], "%4u", LCD_EN_FONT_SIZE_24X32, lcdNumFontFZYTK24x32, LCD_COLOR(rand() % 0x20, rand() % 0x40, rand() % 0x20), LCD_COLOR_BLACK);
                                    break;
                                case 2:
                                    lcd_show_str(&lcdObj, 0, 0, (uint8_t *)"vref", LCD_EN_FONT_SIZE_8X16, lcdEnFontSourceCodePro8x16, LCD_COLOR(rand() % 0x20, rand() % 0x40, rand() % 0x20), LCD_COLOR_BLACK);
                                    lcd_show_num(&lcdObj, 64, 4, adcObj.averageFilterData[ADC_CHANNEL_VREF_INDEX], "%4u", LCD_EN_FONT_SIZE_24X32, lcdNumFontFZYTK24x32, LCD_COLOR(rand() % 0x20, rand() % 0x40, rand() % 0x20), LCD_COLOR_BLACK);
                                    break;
                                default:
                                    break;
                            }
                            
                            if(++lcdAdcFlag >= 3)
                                lcdAdcFlag = 0;
                        }
                        
                        break;
                    }
                    case 1:
                    {
                        lcd_show_str(&lcdObj, 0, 0, (uint8_t *)"servo", LCD_EN_FONT_SIZE_8X16, lcdEnFontSourceCodePro8x16, LCD_COLOR_PURPLE, LCD_COLOR_BLACK);
                        lcd_show_num(&lcdObj, 88, 4, servoObj.degree, "%3u", LCD_EN_FONT_SIZE_24X32, lcdNumFontFZYTK24x32, LCD_COLOR_CYAN, LCD_COLOR_BLACK);
                        break;
                    }
                    default:
                        break;
                }
                
            }
            //设置界面
            else if(lcdPage.mode == LCD_PAGE_MODE_SETTING)
            {
                
            }
            
            //显示Key状态
            {
                lcd_show_char(&lcdObj, 0, 24, keyObjList[KEY_UP_INDEX].pressFlag ? 'U' : ' ', LCD_EN_FONT_SIZE_8X16, lcdEnFontSourceCodePro8x16, LCD_COLOR_ORANGE, LCD_COLOR_BLACK);
                lcd_show_char(&lcdObj, 8, 24, keyObjList[KEY_DN_INDEX].pressFlag ? 'D' : ' ', LCD_EN_FONT_SIZE_8X16, lcdEnFontSourceCodePro8x16, LCD_COLOR_ORANGE, LCD_COLOR_BLACK);
                lcd_show_char(&lcdObj, 16, 24, keyObjList[KEY_LT_INDEX].pressFlag ? 'L' : ' ', LCD_EN_FONT_SIZE_8X16, lcdEnFontSourceCodePro8x16, LCD_COLOR_ORANGE, LCD_COLOR_BLACK);
                lcd_show_char(&lcdObj, 24, 24, keyObjList[KEY_MD_INDEX].pressFlag ? 'M' : ' ', LCD_EN_FONT_SIZE_8X16, lcdEnFontSourceCodePro8x16, LCD_COLOR_ORANGE, LCD_COLOR_BLACK);
                lcd_show_char(&lcdObj, 32, 24, keyObjList[KEY_RT_INDEX].pressFlag ? 'R' : ' ', LCD_EN_FONT_SIZE_8X16, lcdEnFontSourceCodePro8x16, LCD_COLOR_ORANGE, LCD_COLOR_BLACK);
            }
        }
        
        lcdTaskStackHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        
        vTaskDelay(10);
    }
}
