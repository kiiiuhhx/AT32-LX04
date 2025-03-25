// File Name: key_user.c
// Author: kukukukuku~
// Date: 2025-03-11

#include "stddef.h"

#include "./Key/inc/key_user.h"
#include "./LCD/inc/lcd_user.h"
#include "./Servo/inc/servo_user.h"

void key_up_release_handle(void);
void key_up_long_press_handle(void);
void key_up_long_release_handle(void);

void key_dn_release_handle(void);
void key_dn_long_press_handle(void);
void key_dn_long_release_handle(void);

void key_lt_release_handle(void);
void key_lt_long_press_handle(void);
void key_lt_long_release_handle(void);

void key_rt_release_handle(void);
void key_rt_long_press_handle(void);
void key_rt_long_release_handle(void);

void key_md_release_handle(void);
void key_md_long_press_handle(void);
void key_md_long_release_handle(void);


//Key GPIO对象列表
KEY_GPIO_OBJ_T keyGpioObjList[KEY_NUM];

//Key状态机对象列表
KEY_FSM_OBJ_T keyFsmObjList[KEY_NUM];

//Key对象列表
KEY_OBJ_T keyObjList[KEY_NUM];

//Key UP状态表
static KEY_FSM_TABLE_T keyUpFsmTable[KEY_FSM_TABLE_SIZE] = 
{
    {KEY_FSM_EVENT_IDLE, KEY_FSM_STATE_RELEASE, KEY_FSM_STATE_IDLE, NULL},
    {KEY_FSM_EVENT_PRESS, KEY_FSM_STATE_IDLE, KEY_FSM_STATE_PRESS, NULL},
    {KEY_FSM_EVENT_RELEASE, KEY_FSM_STATE_PRESS, KEY_FSM_STATE_RELEASE, key_up_release_handle},
    {KEY_FSM_EVENT_LONG_PRESS, KEY_FSM_STATE_PRESS, KEY_FSM_STATE_LONG_PRESS, key_up_long_press_handle},
    {KEY_FSM_EVENT_LONG_PRESS_KEEP, KEY_FSM_STATE_LONG_PRESS, KEY_FSM_STATE_LONG_PRESS, key_up_long_press_handle},
    {KEY_FSM_EVENT_RELEASE, KEY_FSM_STATE_LONG_PRESS, KEY_FSM_STATE_RELEASE, key_up_long_release_handle},
};

//Key DN状态表
static KEY_FSM_TABLE_T keyDnFsmTable[KEY_FSM_TABLE_SIZE] = 
{
    {KEY_FSM_EVENT_IDLE, KEY_FSM_STATE_RELEASE, KEY_FSM_STATE_IDLE, NULL},
    {KEY_FSM_EVENT_PRESS, KEY_FSM_STATE_IDLE, KEY_FSM_STATE_PRESS, NULL},
    {KEY_FSM_EVENT_RELEASE, KEY_FSM_STATE_PRESS, KEY_FSM_STATE_RELEASE, key_dn_release_handle},
    {KEY_FSM_EVENT_LONG_PRESS, KEY_FSM_STATE_PRESS, KEY_FSM_STATE_LONG_PRESS, key_dn_long_press_handle},
    {KEY_FSM_EVENT_LONG_PRESS_KEEP, KEY_FSM_STATE_LONG_PRESS, KEY_FSM_STATE_LONG_PRESS, key_dn_long_press_handle},
    {KEY_FSM_EVENT_RELEASE, KEY_FSM_STATE_LONG_PRESS, KEY_FSM_STATE_RELEASE, key_dn_long_release_handle},
};

//Key LT状态表
static KEY_FSM_TABLE_T keyLtFsmTable[KEY_FSM_TABLE_SIZE] = 
{
    {KEY_FSM_EVENT_IDLE, KEY_FSM_STATE_RELEASE, KEY_FSM_STATE_IDLE, NULL},
    {KEY_FSM_EVENT_PRESS, KEY_FSM_STATE_IDLE, KEY_FSM_STATE_PRESS, NULL},
    {KEY_FSM_EVENT_RELEASE, KEY_FSM_STATE_PRESS, KEY_FSM_STATE_RELEASE, key_lt_release_handle},
    {KEY_FSM_EVENT_LONG_PRESS, KEY_FSM_STATE_PRESS, KEY_FSM_STATE_LONG_PRESS, key_lt_long_press_handle},
    {KEY_FSM_EVENT_LONG_PRESS_KEEP, KEY_FSM_STATE_LONG_PRESS, KEY_FSM_STATE_LONG_PRESS, key_lt_long_press_handle},
    {KEY_FSM_EVENT_RELEASE, KEY_FSM_STATE_LONG_PRESS, KEY_FSM_STATE_RELEASE, key_lt_long_release_handle},
};

//Key RT状态表
static KEY_FSM_TABLE_T keyRtFsmTable[KEY_FSM_TABLE_SIZE] = 
{
    {KEY_FSM_EVENT_IDLE, KEY_FSM_STATE_RELEASE, KEY_FSM_STATE_IDLE, NULL},
    {KEY_FSM_EVENT_PRESS, KEY_FSM_STATE_IDLE, KEY_FSM_STATE_PRESS, NULL},
    {KEY_FSM_EVENT_RELEASE, KEY_FSM_STATE_PRESS, KEY_FSM_STATE_RELEASE, key_rt_release_handle},
    {KEY_FSM_EVENT_LONG_PRESS, KEY_FSM_STATE_PRESS, KEY_FSM_STATE_LONG_PRESS, key_rt_long_press_handle},
    {KEY_FSM_EVENT_LONG_PRESS_KEEP, KEY_FSM_STATE_LONG_PRESS, KEY_FSM_STATE_LONG_PRESS, key_rt_long_press_handle},
    {KEY_FSM_EVENT_RELEASE, KEY_FSM_STATE_LONG_PRESS, KEY_FSM_STATE_RELEASE, key_rt_long_release_handle},
};

//Key MD状态表
static KEY_FSM_TABLE_T keyMdFsmTable[KEY_FSM_TABLE_SIZE] = 
{
    {KEY_FSM_EVENT_IDLE, KEY_FSM_STATE_RELEASE, KEY_FSM_STATE_IDLE, NULL},
    {KEY_FSM_EVENT_PRESS, KEY_FSM_STATE_IDLE, KEY_FSM_STATE_PRESS, NULL},
    {KEY_FSM_EVENT_RELEASE, KEY_FSM_STATE_PRESS, KEY_FSM_STATE_RELEASE, key_md_release_handle},
    {KEY_FSM_EVENT_LONG_PRESS, KEY_FSM_STATE_PRESS, KEY_FSM_STATE_LONG_PRESS, key_md_long_press_handle},
    {KEY_FSM_EVENT_LONG_PRESS_KEEP, KEY_FSM_STATE_LONG_PRESS, KEY_FSM_STATE_LONG_PRESS, key_md_long_press_handle},
    {KEY_FSM_EVENT_RELEASE, KEY_FSM_STATE_LONG_PRESS, KEY_FSM_STATE_RELEASE, key_md_long_release_handle},
};

//Key状态表列表
pKEY_FSM_TABLE_T keyFsmTableList[KEY_NUM] = 
{
    keyUpFsmTable,
    keyDnFsmTable,
    keyLtFsmTable,
    keyRtFsmTable,
    keyMdFsmTable,
};


/**
 * @brief   Key UP松开处理
 * @param   None
 * @retval  None
 * @note    None
*/
void key_up_release_handle(void)
{
    keyObjList[KEY_UP_INDEX].event = KEY_FSM_EVENT_IDLE;
}


/**
 * @brief   Key UP长按处理
 * @param   None
 * @retval  None
 * @note    None
*/
void key_up_long_press_handle(void)
{
    keyObjList[KEY_UP_INDEX].event = KEY_FSM_EVENT_LONG_PRESS_KEEP;
}


/**
 * @brief   Key UP长按松开处理
 * @param   None
 * @retval  None
 * @note    None
*/
void key_up_long_release_handle(void)
{
    keyObjList[KEY_UP_INDEX].event = KEY_FSM_EVENT_IDLE;
}


/**
 * @brief   Key DN松开处理
 * @param   None
 * @retval  None
 * @note    None
*/
void key_dn_release_handle(void)
{
    keyObjList[KEY_DN_INDEX].event = KEY_FSM_EVENT_IDLE;
}


/**
 * @brief   Key DN长按处理
 * @param   None
 * @retval  None
 * @note    None
*/
void key_dn_long_press_handle(void)
{
    keyObjList[KEY_DN_INDEX].event = KEY_FSM_EVENT_LONG_PRESS_KEEP;
}


/**
 * @brief   Key DN长按松开处理
 * @param   None
 * @retval  None
 * @note    None
*/
void key_dn_long_release_handle(void)
{
    keyObjList[KEY_DN_INDEX].event = KEY_FSM_EVENT_IDLE;
}


/**
 * @brief   Key LT松开处理
 * @param   None
 * @retval  None
 * @note    None
*/
void key_lt_release_handle(void)
{
    if(lcdPage.mainId == 1)
        servoDegree > 0 ? --servoDegree : 0;
    
    keyObjList[KEY_LT_INDEX].event = KEY_FSM_EVENT_IDLE;
}


/**
 * @brief   Key LT长按处理
 * @param   None
 * @retval  None
 * @note    None
*/
void key_lt_long_press_handle(void)
{
    if(lcdPage.mainId == 1)
        servoDegree > 0 ? --servoDegree : 0;
    
    keyObjList[KEY_LT_INDEX].event = KEY_FSM_EVENT_LONG_PRESS_KEEP;
}


/**
 * @brief   Key LT长按松开处理
 * @param   None
 * @retval  None
 * @note    None
*/
void key_lt_long_release_handle(void)
{
    keyObjList[KEY_LT_INDEX].event = KEY_FSM_EVENT_IDLE;
}


/**
 * @brief   Key RT松开处理
 * @param   None
 * @retval  None
 * @note    None
*/
void key_rt_release_handle(void)
{
    if(lcdPage.mainId == 1)
        servoDegree < 180 ? ++servoDegree : 180;
    
    keyObjList[KEY_RT_INDEX].event = KEY_FSM_EVENT_IDLE;
}


/**
 * @brief   Key RT长按处理
 * @param   None
 * @retval  None
 * @note    None
*/
void key_rt_long_press_handle(void)
{
    if(lcdPage.mainId == 1)
    servoDegree < 180 ? ++servoDegree : 180;
    
    keyObjList[KEY_RT_INDEX].event = KEY_FSM_EVENT_LONG_PRESS_KEEP;
}


/**
 * @brief   Key RT长按松开处理
 * @param   None
 * @retval  None
 * @note    None
*/
void key_rt_long_release_handle(void)
{
    keyObjList[KEY_RT_INDEX].event = KEY_FSM_EVENT_IDLE;
}


/**
 * @brief   Key MD松开处理
 * @param   None
 * @retval  None
 * @note    None
*/
void key_md_release_handle(void)
{
    if(lcdPage.mode == LCD_PAGE_MODE_MAIN)
    {
        if(++lcdPage.mainId >= lcdPage.mainPageNum)
            lcdPage.mainId = 0;
    }
    
    keyObjList[KEY_MD_INDEX].event = KEY_FSM_EVENT_IDLE;
}


/**
 * @brief   Key MD长按处理
 * @param   None
 * @retval  None
 * @note    None
*/
void key_md_long_press_handle(void)
{
    //主界面切换到设置界面
    if(lcdPage.mode == LCD_PAGE_MODE_MAIN)
    {
        lcdPage.mode = LCD_PAGE_MODE_SETTING;
    }
    //设置界面切换到主界面
    else
    {
        lcdPage.mode = LCD_PAGE_MODE_MAIN;
    }
    
    //keyObjList[KEY_MD_INDEX].event = KEY_FSM_EVENT_LONG_PRESS_KEEP;
}


/**
 * @brief   Key MD长按松开处理
 * @param   None
 * @retval  None
 * @note    None
*/
void key_md_long_release_handle(void)
{
    keyObjList[KEY_MD_INDEX].event = KEY_FSM_EVENT_IDLE;
}
