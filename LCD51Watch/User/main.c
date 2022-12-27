/*====================================================
File    : main.c
Project : 
Version : V1.0
Date    : 2022.8.7
Author  : Gavin Hu
Company : ZJOU
Comments: 
    PCB: 
IDE     : 
Chip type           : STC8G1K08
Program type        : Application
Clock frequency     : 18.432000 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 256
====================================================*/
/*#pragma  src*/
#define DEBUG 0
#include "cpu.h"
#include "typedef.h"
#include "system_timer.h"
#include "uart.h"
#include "stdio.h"
#include "gpio.h"
#include "pca.h"
#include "touch_sensing.h"
#include "adc.h"
#include "lcd7segx4.h"

/*----------------------------------------------
  Constant define
----------------------------------------------*/


/*----------------------------------------------
  Port define
----------------------------------------------*/

/*----------------------------------------------
  Function declaration
----------------------------------------------*/
void SystemInitialization(void);
void ClockRun(void);
void ClockSet(uint8_t u8Key);

/*----------------------------------------------
  Public variable declaration
----------------------------------------------*/
static uint8_t idata l_a_u8DisplayRam[4]={0,0,0,0};
static uint8_t l_u8Second=0,l_u8Minute=0,l_u8Hour=0;
static uint8_t l_u8ClockSet=0;

/*----------------------------------------------
Function: Main routine
Parameter: void
Return: void
----------------------------------------------*/
void main(void)
{
    uint8_t u8Key;
    /*System timer initialization*/
    SystemTimerInit();
    SystemInitialization();

    EA = 1;

    while (1)
    {
        SystemTimerUpdate();
        u8Key = u8TouchKeyScan();
        if (0 == l_u8ClockSet)
        {
            if (TOUCH_KEY_LONG_PRESS == u8Key)
            {
                ClockSet(u8Key);
            }
            else
            {
                ClockRun();
            }
        }
        else
        {
            ClockSet(u8Key);
        }
        LCD7SegmentsDisplay(l_a_u8DisplayRam);
        Lcd7SegX4Drive();
        PCON = 0x01;    /* save power */
      #if DEBUG
        if (0 != UART1_Available())
        {
            getchar();
            printf("%u\r\n",(uint16_t)u16Get_ADC_Result(TOUCH_KEY0_ADC));
        }
      #endif
    }
}

/*----------------------------------------------
Function: System initialization
Parameter: void
Return: void
----------------------------------------------*/
void SystemInitialization(void)
{
  #if DEBUG
    /*UART initialization*/
    UART_ConfInf_t UART_Inf;
    UART_Inf.UART_Mode = UART_8BIT_BRTX;
    UART_Inf.UART_BRT_Use = BRT_TIMER2;
    UART_Inf.UART_BaudRate = 115200;
    UART_Inf.UART_RxEnable = ENABLE;
    UART_Inf.UART_Interrupt = ENABLE;
    u8UART_Configuration(UART1, &UART_Inf);
  #endif
    TouchSensingInitialization();
    Lcd7SegX4Initialization();
}

/*----------------------------------------------
Function: Clock running routine
Parameter: void
Return: void
----------------------------------------------*/
void ClockRun(void)
{
    static bit bColon;
    static uint16_t s_u16Record;
    uint16_t u16Value;
    u16Value = u32GetSysTicks();
    if ((uint16_t)(u16Value - s_u16Record) >= 500)
    {
        s_u16Record = u16Value;
        bColon = !bColon;
        if (bColon)
        {
            l_u8Second ++;
            if (l_u8Second >= 60)
            {
                l_u8Second = 0;
                l_u8Minute ++;
                if (l_u8Minute >= 60)
                {
                    l_u8Minute = 0;
                    l_u8Hour ++;
                    if (l_u8Hour >= 24)
                    {
                        l_u8Hour = 0;
                    }
                    Data2String(l_u8Hour, 0, 2, l_a_u8DisplayRam);
                }
                Data2String(l_u8Minute, 0, 2, l_a_u8DisplayRam+2);
            }
            l_a_u8DisplayRam[3] |= DISPLAY_DP_CODE;
        }
        else
        {
            l_a_u8DisplayRam[3] &= ~DISPLAY_DP_CODE;
        }
    }
}

/*----------------------------------------------
Function: Set clock time
Parameter: void
Return: void
----------------------------------------------*/
void ClockSet(uint8_t u8Key)
{
    switch (l_u8ClockSet)
    {
        case 0 :
            if (TOUCH_KEY_LONG_PRESS == u8Key) /*Button 0 long press start set mode*/
            {
                l_u8Second = 0;
                l_a_u8DisplayRam[0] |= DISPLAY_TWINKLE_CODE;
                l_a_u8DisplayRam[1] |= DISPLAY_TWINKLE_CODE;
                l_u8ClockSet ++;
            }
            break;
        case 1 :
            if (TOUCH_KEY_UP == u8Key) /*Button 0 plus*/
            {
                if (l_u8Hour < 23)
                {
                    l_u8Hour ++;
                    Data2String(l_u8Hour, 0, 2, l_a_u8DisplayRam);
                    l_a_u8DisplayRam[0] |= DISPLAY_TWINKLE_CODE;
                    l_a_u8DisplayRam[1] |= DISPLAY_TWINKLE_CODE;
                }
            }
            else if (TOUCH_KEY_UP+1 == u8Key) /*Button 1 minus*/
            {
                if (l_u8Hour > 0)
                {
                    l_u8Hour --;
                    Data2String(l_u8Hour, 0, 2, l_a_u8DisplayRam);
                    l_a_u8DisplayRam[0] |= DISPLAY_TWINKLE_CODE;
                    l_a_u8DisplayRam[1] |= DISPLAY_TWINKLE_CODE;
                }
            }
            else if (TOUCH_KEY_LONG_PRESS == u8Key) /*Button 0 longpress next step*/
            {
                l_a_u8DisplayRam[0] &= ~DISPLAY_TWINKLE_CODE;
                l_a_u8DisplayRam[1] &= ~DISPLAY_TWINKLE_CODE;
                l_a_u8DisplayRam[2] |= DISPLAY_TWINKLE_CODE;
                l_a_u8DisplayRam[3] |= DISPLAY_TWINKLE_CODE;
                l_u8ClockSet ++;
            }
            else
            {
                ;
            }
            break;
        case 2 :
            if (TOUCH_KEY_UP == u8Key) /*Button 0 plus*/
            {
                if (l_u8Minute < 59)
                {
                    l_u8Minute ++;
                    Data2String(l_u8Minute, 0, 2, l_a_u8DisplayRam+2);
                    l_a_u8DisplayRam[2] |= DISPLAY_TWINKLE_CODE;
                    l_a_u8DisplayRam[3] |= DISPLAY_TWINKLE_CODE;
                }
            }
            else if (TOUCH_KEY_UP+1 == u8Key) /*Button 1 minus*/
            {
                if (l_u8Minute != 0)
                {
                    l_u8Minute --;
                    Data2String(l_u8Minute, 0, 2, l_a_u8DisplayRam+2);
                    l_a_u8DisplayRam[2] |= DISPLAY_TWINKLE_CODE;
                    l_a_u8DisplayRam[3] |= DISPLAY_TWINKLE_CODE;
                }
            }
            else if (TOUCH_KEY_LONG_PRESS == u8Key) /*Button 0 longpress next step*/
            {
                l_a_u8DisplayRam[2] &= ~DISPLAY_TWINKLE_CODE;
                l_a_u8DisplayRam[3] &= ~DISPLAY_TWINKLE_CODE;
                l_u8ClockSet = 0;
            }
            else
            {
                ;
            }
            break;
        default:
            break;
    }
}
