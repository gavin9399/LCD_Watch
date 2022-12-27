/*====================================================
File    : touch_sensing.h
Version : V1.0
Date    : 2017.5.27
Author  : Gavin Hu
Company : ZJOU
Comments: Touch sensing use ADC & clock out
IDE     : 
MCU type: 51 MCU
====================================================*/

#ifndef _TOUCH_SENSING_H_
#define _TOUCH_SENSING_H_ 1

#include "typedef.h"

/*----------------------------------------------
The following parameters are based on the hardware configuration
----------------------------------------------*/
#define TOUCH_KEY_QUANTIY          2   /*1~8*/

#define TOUCH_KEY0_ADC  0   /*ADC channel 0*/
#define TOUCH_KEY1_ADC  1   /*ADC channel 1*/

#define TOUCH_SENSING_THRESHOLD     200
/*----------------------------------------------
The above parameters are based on the hardware configuration
----------------------------------------------*/

#define TOUCH_KEY_SCAN_PERIOD      100 /*100msec*/
#define LONG_PRESS_TIME         (2000/TOUCH_KEY_SCAN_PERIOD)

#define TOUCH_KEY_NO_CHANGE           0x00
#define TOUCH_KEY_DOWN                0x20
#define TOUCH_KEY_UP                  0x40
#define TOUCH_KEY_LONG_PRESS          0x80

extern void TouchSensingInitialization(void);
extern uint8_t u8TouchKeyScan(void);

#endif  /* _TOUCH_SENSING_H_ */
