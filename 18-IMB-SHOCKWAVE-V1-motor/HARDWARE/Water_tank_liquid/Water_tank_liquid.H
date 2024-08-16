#ifndef _Water_tank_liquid_H
#define _Water_tank_liquid_H
#include "sys.h"
#include "main.h"
#include "gpio.h"

#define Water_tank_Low_flag    HAL_GPIO_ReadPin(Water_tank_Low_GPIO_Port,Water_tank_Low_Pin) //低液位的读取口
#define Water_tank_High_flag   HAL_GPIO_ReadPin(Water_tank_High_GPIO_Port,Water_tank_High_Pin) //高液位的读取口

u8 Water_tank_liquid_low(u8 mode);
u8 Water_tank_liquid_high(u8 mode);
extern uint16_t water_flag_fill_flag;








#endif


