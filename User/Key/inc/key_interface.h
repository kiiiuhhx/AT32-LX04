// File Name: key_interface.h
// Author: kukukukuku~
// Date: 2025-03-11

#ifndef __KEY_INTERFACE_H
#define __KEY_INTERFACE_H

#include "./Key/inc/key_macros.h"

//Key状态机状态
typedef enum KEY_FSM_STATE_T
{
    KEY_FSM_STATE_IDLE = 0,
    KEY_FSM_STATE_RELEASE,
    KEY_FSM_STATE_PRESS,
    KEY_FSM_STATE_LONG_PRESS,
    KEY_FSM_STATE_LONG_PRESS_KEEP,
}KEY_FSM_STATE_T, *pKEY_FSM_STATE_T;

//Key状态机事件
typedef enum KEY_FSM_EVENT_T
{
    KEY_FSM_EVENT_IDLE = 0,
    KEY_FSM_EVENT_RELEASE,
    KEY_FSM_EVENT_PRESS,
    KEY_FSM_EVENT_LONG_PRESS,
    KEY_FSM_EVENT_LONG_PRESS_KEEP,
}KEY_FSM_EVENT_T, *pKEY_FSM_EVENT_T;

//Key状态机状态表
typedef struct KEY_FSM_TABLE_T
{
    KEY_FSM_EVENT_T event;
    KEY_FSM_STATE_T state;
    KEY_FSM_STATE_T nextState;
    void (*action)(void);
}KEY_FSM_TABLE_T, *pKEY_FSM_TABLE_T;

//Key状态机对象
typedef struct KEY_FSM_OBJ_T
{
    pKEY_FSM_TABLE_T table;
    uint16_t tableSize;
    KEY_FSM_STATE_T currentState;
}KEY_FSM_OBJ_T, *pKEY_FSM_OBJ_T;

//Key GPIO对象
typedef struct KEY_GPIO_OBJ_T
{
    gpio_type *port;
    uint32_t pin;
    uint8_t pressPolarity;
}KEY_GPIO_OBJ_T, *pKEY_GPIO_OBJ_T;

//Key对象
typedef struct KEY_OBJ_T
{
    pKEY_GPIO_OBJ_T gpio;
    tmr_type *tmr;
    pKEY_FSM_OBJ_T fsm;
    KEY_FSM_EVENT_T event;
    uint16_t longPressJudgeCnt;
    uint16_t longPressJudgeNum;
    uint8_t longPressScanFlag;
    uint8_t pressFlag;
    uint8_t pressFlagLast;
}KEY_OBJ_T, *pKEY_OBJ_T;


/**
 * @brief   Key GPIO对象初始化
 * @param   keyGpioObj:     Key GPIO对象
 * @param   port:           GPIO端口
 * @param   pin:            GPIO引脚
 * @param   pressPolarity:  Key按下时的电平
 * @retval  None
 * @note    None
*/
void key_gpio_obj_init(KEY_GPIO_OBJ_T * const keyGpioObj, gpio_type * const port, const uint32_t pin, const uint8_t pressPolarity);


/**
 * @brief   Key 状态机对象初始化
 * @param   keyFsmObj:          Key状态机对象
 * @param   keyFsmTable:        Key状态机表
 * @param   keyFsmTableSize:    Key状态机表项数
 * @retval  None
 * @note    None
*/
void key_fsm_obj_init(KEY_FSM_OBJ_T * const keyFsmObj, KEY_FSM_TABLE_T * const keyFsmTable, const uint16_t keyFsmTableSize);


/**
 * @brief   Key对象初始化
 * @param   keyObj:             Key对象
 * @param   keyGpioObj:         Key GPIO对象
 * @param   tmr:                定时器
 * @param   keyFsmObj:          Key状态机对象
 * @param   longPressJudgeNum:  长按判定时间
 * @retval  None
 * @note    None
*/
void key_obj_init(KEY_OBJ_T * const keyObj, KEY_GPIO_OBJ_T * const keyGpioObj, tmr_type * const tmr, KEY_FSM_OBJ_T * const keyFsmObj, const uint16_t longPressJudgeNum);


/**
 * @brief   关闭Key定时器
 * @param   keyObj: Key对象
 * @retval  None
 * @note    None
*/
void key_tmr_stop(KEY_OBJ_T * const keyObj);


/**
 * @brief   Key扫描
 * @param   keyObj: Key对象
 * @retval  None
 * @note    None
*/
void key_scan(KEY_OBJ_T * const keyObj);


/**
 * @brief   Key长按扫描
 * @param   keyObj: Key对象
 * @retval  None
 * @note    只能在Key定时器中断服务函数中调用
*/
void key_long_press_scan(KEY_OBJ_T * const keyObj);


/**
 * @brief   Key状态机事件处理
 * @param   keyObj: Key对象
 * @retval  None
 * @note    None
*/
void key_fsm_event_handle(KEY_OBJ_T * const keyObj);

#endif //__KEY_INTERFACE_H
