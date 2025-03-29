// File Name: lcd_task.c
// Author: kukukukuku~
// Date: 2025-03-06

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
    static uint16_t adcTempVal;
    static uint8_t delayCnt = 100;
    static uint8_t lcdAdcFlag;
    static uint8_t lcdPageMainIdLast;
    static LCD_PAGE_MODE_T lcdPageModeLast = LCD_PAGE_MODE_MAIN;
    
    //LCD GPIO对象初始化
    lcd_gpio_obj_init(&lcdGpioObj, LCD_MOSI_GPIO_PORT, LCD_SCK_GPIO_PORT, LCD_CS_GPIO_PORT, LCD_DC_GPIO_PORT, LCD_RES_GPIO_PORT, LCD_BLK_GPIO_PORT,
                      LCD_MOSI_GPIO_PIN, LCD_SCK_GPIO_PIN, LCD_CS_GPIO_PIN, LCD_DC_GPIO_PIN, LCD_RES_GPIO_PIN, LCD_BLK_GPIO_PIN);
    
    //LCD对象初始化
    lcd_obj_init(&lcdObj, &lcdGpioObj, LCD_SPI, LCD_WIDTH, LCD_HEIGHT, LCD_DIR_NORMAL);
    
    lcd_reset(&lcdObj);                     //LCD复位
    lcd_power_on_code_init(&lcdObj);        //LCD上电指令初始化
    lcd_clear(&lcdObj, LCD_COLOR_BLACK);    //LCD清屏
    
    lcd_show_picture(&lcdObj, 45, 0, 71, 40, lcdPicJujingyi71x40, sizeof(lcdPicJujingyi71x40));
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
                delayCnt = 100;
            }
            
            //界面模式切换，刷新LCD
            if(lcdPageModeLast != lcdPage.mode)
            {
                lcd_clear(&lcdObj, LCD_COLOR_BLACK);
                lcdPageModeLast = lcdPage.mode;
                delayCnt = 100;
                lcdAdcFlag = 0;
            }
            
            //主界面模式
            if(lcdPage.mode == LCD_PAGE_MODE_MAIN)
            {
                //显示Key状态
                {
                    lcd_show_char(&lcdObj, 120, 24, keyObjList[KEY_UP_INDEX].pressFlag ? 'U' : ' ', LCD_EN_FONT_SIZE_8X16, lcdEnFontSourceCodePro8x16, LCD_COLOR_ORANGE, LCD_COLOR_BLACK);
                    lcd_show_char(&lcdObj, 128, 24, keyObjList[KEY_DN_INDEX].pressFlag ? 'D' : ' ', LCD_EN_FONT_SIZE_8X16, lcdEnFontSourceCodePro8x16, LCD_COLOR_ORANGE, LCD_COLOR_BLACK);
                    lcd_show_char(&lcdObj, 136, 24, keyObjList[KEY_LT_INDEX].pressFlag ? 'L' : ' ', LCD_EN_FONT_SIZE_8X16, lcdEnFontSourceCodePro8x16, LCD_COLOR_ORANGE, LCD_COLOR_BLACK);
                    lcd_show_char(&lcdObj, 144, 24, keyObjList[KEY_MD_INDEX].pressFlag ? 'M' : ' ', LCD_EN_FONT_SIZE_8X16, lcdEnFontSourceCodePro8x16, LCD_COLOR_ORANGE, LCD_COLOR_BLACK);
                    lcd_show_char(&lcdObj, 152, 24, keyObjList[KEY_RT_INDEX].pressFlag ? 'R' : ' ', LCD_EN_FONT_SIZE_8X16, lcdEnFontSourceCodePro8x16, LCD_COLOR_ORANGE, LCD_COLOR_BLACK);
                }
                
                switch(lcdPage.mainId)
                {
                    case 0:
                    {
                        if(++delayCnt >= 100)
                        {
                            delayCnt = 0;
                            
                            switch(lcdAdcFlag)
                            {
                                case 0:
                                {
                                    adcTempVal = adcObj.averageFilterData[ADC_CHANNEL_ADC_INDEX];
                                    lcd_show_str(&lcdObj, 128, 0, (uint8_t *)" ADC", LCD_EN_FONT_SIZE_8X16, lcdEnFontSourceCodePro8x16, LCD_COLOR_CYAN, LCD_COLOR_BLACK);
                                    lcd_show_integer(&lcdObj, 0, 0, adcTempVal, "%-6u", LCD_EN_FONT_SIZE_16X16, lcdNumFontFZYTK16x16, LCD_COLOR_YELLOW, LCD_COLOR_BLACK);
                                    lcd_fill(&lcdObj, 0, 20, 95, 21, LCD_COLOR_WHITE);
                                    lcd_show_float(&lcdObj, 0, 24, adcTempVal * 3.3 / 4095, "%-6.4lf", LCD_EN_FONT_SIZE_16X16, lcdNumFontFZYTK16x16, LCD_COLOR_YELLOW, LCD_COLOR_BLACK);
                                    break;
                                }
                                case 1:
                                {
                                    lcd_show_str(&lcdObj, 128, 0, (uint8_t *)"Temp", LCD_EN_FONT_SIZE_8X16, lcdEnFontSourceCodePro8x16, LCD_COLOR_CYAN, LCD_COLOR_BLACK);
                                    lcd_show_integer(&lcdObj, 0, 0, adcObj.averageFilterData[ADC_CHANNEL_TEMPERATURE_INDEX], "%-6u", LCD_EN_FONT_SIZE_16X16, lcdNumFontFZYTK16x16, LCD_COLOR_YELLOW, LCD_COLOR_BLACK);
                                    lcd_fill(&lcdObj, 0, 20, 95, 21, LCD_COLOR_WHITE);
                                    lcd_show_float(&lcdObj, 0, 24, temperature, "%-6.1lf", LCD_EN_FONT_SIZE_16X16, lcdNumFontFZYTK16x16, LCD_COLOR_YELLOW, LCD_COLOR_BLACK);
                                    break;
                                }
                                case 2:
                                {
                                    lcd_show_str(&lcdObj, 128, 0, (uint8_t *)"Vref", LCD_EN_FONT_SIZE_8X16, lcdEnFontSourceCodePro8x16, LCD_COLOR_CYAN, LCD_COLOR_BLACK);
                                    lcd_show_integer(&lcdObj, 0, 0, adcObj.averageFilterData[ADC_CHANNEL_VREF_INDEX], "%-6u", LCD_EN_FONT_SIZE_16X16, lcdNumFontFZYTK16x16, LCD_COLOR_YELLOW, LCD_COLOR_BLACK);
                                    lcd_fill(&lcdObj, 0, 20, 95, 21, LCD_COLOR_WHITE);
                                    lcd_show_float(&lcdObj, 0, 24, externalVref, "%-6.4lf", LCD_EN_FONT_SIZE_16X16, lcdNumFontFZYTK16x16, LCD_COLOR_YELLOW, LCD_COLOR_BLACK);
                                    break;
                                }
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
                        lcd_show_str(&lcdObj, 120, 0, (uint8_t *)"Servo", LCD_EN_FONT_SIZE_8X16, lcdEnFontSourceCodePro8x16, LCD_COLOR_PURPLE, LCD_COLOR_BLACK);
                        lcd_show_integer(&lcdObj, 0, 4, servoObj.degree, "%-3u", LCD_EN_FONT_SIZE_24X32, lcdNumFontFZYTK24x32, LCD_COLOR_CYAN, LCD_COLOR_BLACK);
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
        }
        
        lcdTaskStackHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        
        vTaskDelay(10);
    }
}
