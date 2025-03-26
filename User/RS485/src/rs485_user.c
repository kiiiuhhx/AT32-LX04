// File Name: rs485_user.c
// Author: kukukukuku~
// Date: 2025-03-26

#include "./RS485/inc/rs485_user.h"

//RS485对象
RS485_OBJ_T rs485Obj;

//RS485 MODBUS寄存器地址列表
uint16_t *rs485MobusRegAddrList[RS485_MODBUS_REG_NUM];
