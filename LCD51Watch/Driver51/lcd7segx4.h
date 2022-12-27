/*====================================================
File    : lcd7segx4.h
Version : V1.1
Date    : 2022.10.19
Author  : Gavin Hu
Company : ZJOU
Comments: 
IDE     : 
MCU type: 8051 series
====================================================*/

#ifndef _LCD7SEGX4_H_
#define _LCD7SEGX4_H_ 1

#include "typedef.h"
#include "cpu.h"

/*----------------------------------------------
The following parameters are based on
the hardware configuration
----------------------------------------------*/
#if defined STC8G
#define LCD_UPSIDE_DOWN true
sbit LCD_SEG0_PIN=P5^5;
sbit LCD_SEG1_PIN=P5^4;
sbit LCD_SEG2_PIN=P1^7;
sbit LCD_SEG3_PIN=P1^6;
sbit LCD_SEG4_PIN=P3^2;
sbit LCD_SEG5_PIN=P3^3;
sbit LCD_SEG6_PIN=P3^4;
sbit LCD_SEG7_PIN=P3^5;
#define SetLcdSeg0PushPull() P5n_push_pull(Pin5)
#define SetLcdSeg1PushPull() P5n_push_pull(Pin4)
#define SetLcdSeg2PushPull() P1n_push_pull(Pin7)
#define SetLcdSeg3PushPull() P1n_push_pull(Pin6)
#define SetLcdSeg4PushPull() P3n_push_pull(Pin2)
#define SetLcdSeg5PushPull() P3n_push_pull(Pin3)
#define SetLcdSeg6PushPull() P3n_push_pull(Pin4)
#define SetLcdSeg7PushPull() P3n_push_pull(Pin5)

sbit LCD_COM0_PIN=P1^2;
sbit LCD_COM1_PIN=P1^3;
sbit LCD_COM2_PIN=P1^4;
sbit LCD_COM3_PIN=P1^5;

#define SetLcdCom0Tristate()    P1n_pure_input(Pin2)
#define SetLcdCom0Low()         LCD_COM0_PIN=0,P1n_push_pull(Pin2)
#define SetLcdCom0High()        LCD_COM0_PIN=1,P1n_push_pull(Pin2)
#define SetLcdCom1Tristate()    P1n_pure_input(Pin3)
#define SetLcdCom1Low()         LCD_COM1_PIN=0,P1n_push_pull(Pin3)
#define SetLcdCom1High()        LCD_COM1_PIN=1,P1n_push_pull(Pin3)
#define SetLcdCom2Tristate()    P1n_pure_input(Pin4)
#define SetLcdCom2Low()         LCD_COM2_PIN=0,P1n_push_pull(Pin4)
#define SetLcdCom2High()        LCD_COM2_PIN=1,P1n_push_pull(Pin4)
#define SetLcdCom3Tristate()    P1n_pure_input(Pin5)
#define SetLcdCom3Low()         LCD_COM3_PIN=0,P1n_push_pull(Pin5)
#define SetLcdCom3High()        LCD_COM3_PIN=1,P1n_push_pull(Pin5)
#else
#define LCD_UPSIDE_DOWN false
sbit LCD_SEG0_PIN=P1^6;
sbit LCD_SEG1_PIN=P1^7;
sbit LCD_SEG2_PIN=P5^4;
sbit LCD_SEG3_PIN=P5^5;
sbit LCD_SEG4_PIN=P3^2;
sbit LCD_SEG5_PIN=P3^3;
sbit LCD_SEG6_PIN=P3^4;
sbit LCD_SEG7_PIN=P3^5;
#define SetLcdSeg0PushPull() P1n_push_pull(Pin6)
#define SetLcdSeg1PushPull() P1n_push_pull(Pin7)
#define SetLcdSeg2PushPull() P5n_push_pull(Pin4)
#define SetLcdSeg3PushPull() P5n_push_pull(Pin5)
#define SetLcdSeg4PushPull() P3n_push_pull(Pin2)
#define SetLcdSeg5PushPull() P3n_push_pull(Pin3)
#define SetLcdSeg6PushPull() P3n_push_pull(Pin4)
#define SetLcdSeg7PushPull() P3n_push_pull(Pin5)

sbit LCD_COM0_PIN=P1^2;
sbit LCD_COM1_PIN=P1^3;
sbit LCD_COM2_PIN=P1^4;
sbit LCD_COM3_PIN=P1^5;

#define SetLcdCom0Tristate()    P1n_pure_input(Pin2)
#define SetLcdCom0Low()         LCD_COM0_PIN=0,P1n_push_pull(Pin2)
#define SetLcdCom0High()        LCD_COM0_PIN=1,P1n_push_pull(Pin2)
#define SetLcdCom1Tristate()    P1n_pure_input(Pin3)
#define SetLcdCom1Low()         LCD_COM1_PIN=0,P1n_push_pull(Pin3)
#define SetLcdCom1High()        LCD_COM1_PIN=1,P1n_push_pull(Pin3)
#define SetLcdCom2Tristate()    P1n_pure_input(Pin4)
#define SetLcdCom2Low()         LCD_COM2_PIN=0,P1n_push_pull(Pin4)
#define SetLcdCom2High()        LCD_COM2_PIN=1,P1n_push_pull(Pin4)
#define SetLcdCom3Tristate()    P1n_pure_input(Pin5)
#define SetLcdCom3Low()         LCD_COM3_PIN=0,P1n_push_pull(Pin5)
#define SetLcdCom3High()        LCD_COM3_PIN=1,P1n_push_pull(Pin5)
#endif

#define LCD7SEGMENTS_SCAN_PERIOD    1   /*Scan 1 comm pin*/

#define DISPLAY_A_CODE          10
#define DISPLAY_B_CODE          11
#define DISPLAY_C_CODE          12
#define DISPLAY_D_CODE          13
#define DISPLAY_E_CODE          14
#define DISPLAY_F_CODE          15
#define DISPLAY_G_CODE          16
#define DISPLAY_H_CODE          17
#define DISPLAY_I_CODE          1
#define DISPLAY_J_CODE          18
#define DISPLAY_L_CODE          19
#define DISPLAY_N_CODE          20
#define DISPLAY_O_CODE          21
#define DISPLAY_P_CODE          22
#define DISPLAY_R_CODE          23
#define DISPLAY_S_CODE          5
#define DISPLAY_T_CODE          24
#define DISPLAY_U_CODE          25
#define DISPLAY_MINUS_CODE      26
#define DISPLAY_OFF_CODE        27
#define DISPLAY_DP_CODE         0x40
#define DISPLAY_TWINKLE_CODE    0x80

extern void Lcd7SegX4Initialization(void);
extern void LCD7SegmentsDisplay(const uint8_t c_a_u8DisplayRam[]);
extern void Lcd7SegX4Drive(void);

#define DATA2STRING_REMOVE_PREFIX_0     0

#if 0 /* DATA2STRING_USE_32BITS */
    #define DATA2STRING_USE_32BITS  1
    extern void Data2String(uint32_t u32Data, uint8_t u8Digits, uint8_t u8StrLen, uint8_t a_u8String[]);
#else
    extern void Data2String(uint16_t u16Data, uint8_t u8Digits, uint8_t u8StrLen, uint8_t a_u8String[]);
#endif

#endif  /* _LCD7SEGX4_H_ */
