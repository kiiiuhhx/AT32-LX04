// File Name: key_user.h
// Author: kukukukuku~
// Date: 2025-03-11

#ifndef __KEY_USER_H
#define __KEY_USER_H

#include "./Key/inc/key_interface.h"

//Key GPIO对象列表
extern KEY_GPIO_OBJ_T keyGpioObjList[KEY_NUM];

//Key状态机对象列表
extern KEY_FSM_OBJ_T keyFsmObjList[KEY_NUM];

//Key对象列表
extern KEY_OBJ_T keyObjList[KEY_NUM];

//Key状态表列表
extern pKEY_FSM_TABLE_T keyFsmTableList[KEY_NUM];

#endif //__KEY_USER_H
