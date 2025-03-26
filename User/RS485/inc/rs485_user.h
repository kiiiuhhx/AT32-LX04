// File Name: rs485_user.h
// Author: kukukukuku~
// Date: 2025-03-26

#ifndef __RS485_USER_H
#define __RS485_USER_H

#include "./RS485/inc/rs485_interface.h"

//RS485对象
extern RS485_OBJ_T rs485Obj;

//RS485 MODBUS寄存器地址
extern uint16_t *rs485MobusRegAddrList[RS485_MODBUS_REG_NUM];

#endif //__RS485_USER_H
