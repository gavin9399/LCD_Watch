/*====================================================
File    : lcd7segx4.c
Version : V1.1
Date    : 2022.10.19
Author  : Gavin Hu
Company : ZJOU
Comments: 
IDE     : 
MCU type: 51 MCU
====================================================*/

#include "lcd7segx4.h"
#include "system_timer.h"

/*----------------------------------------------
  Public variable declaration
----------------------------------------------*/
static idata uint8_t l_a_u8LcdDispBuff[4];

static void LcdWriteSegments(uint8_t u8Value);

/*----------------------------------------------
Function: lcd7segx4 initialization
Parameter: void
Return: void
----------------------------------------------*/
void Lcd7SegX4Initialization(void)
{
    SetLcdCom0Tristate();
    SetLcdCom1Tristate();
    SetLcdCom2Tristate();
    SetLcdCom3Tristate();
    SetLcdSeg0PushPull();
    SetLcdSeg1PushPull();
    SetLcdSeg2PushPull();
    SetLcdSeg3PushPull();
    SetLcdSeg4PushPull();
    SetLcdSeg5PushPull();
    SetLcdSeg6PushPull();
    SetLcdSeg7PushPull();
}

/*----------------------------------------------
Function: 7-segments LCD display function
Parameter: display string
    Bit6 DP, Bit7 flash
Return: void
Call the function within 125ms
----------------------------------------------*/
void LCD7SegmentsDisplay(const uint8_t c_a_u8DisplayRam[])
{
  #if LCD_UPSIDE_DOWN
    code const uint8_t c_a_u8DisplayTable[]={0xB7,0x14,0x9B,0x9D,0x3C,0xAD,0xAF,0x94,0xBF,0xBD,0xBE,0x2F,0xA3,0x1F,0xAB,0xAA,0xA7,0x3E,0x15,0x23,0x0E,0x0F,0xBA,0x0A,0x2B,0x37,0x08,0x00};
  #else
    code const uint8_t c_a_u8DisplayTable[]={0xB7,0x22,0x9B,0xAB,0x2E,0xAD,0xBD,0x23,0xBF,0xAF,0x3F,0xBC,0x95,0xBA,0x9D,0x1D,0xB5,0x3E,0xA2,0x94,0x38,0xB8,0x1F,0x18,0x9C,0xB6,0x08,0x00};
  #endif
                                            /* 0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   A,   b,   C,   d,   E,   F,   G,   H,   J,   L,   n,   P,  r,   t,   U,   -,  OFF*/
                                            /* 0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26*/
    static uint8_t s_u8ScanedTime=-125;
    static uint8_t s_u8TwinkleCount=0;
    int8_t i;
    uint8_t j, u8Value;

  #if SYSTEM_TIME_USE_INTERRUPT
    i = g_u8SystemTimeTicks;
  #else
    i = u32GetSysTicks();
  #endif
    if ((uint8_t)(i - s_u8ScanedTime) >= 125)
    {
        s_u8ScanedTime = i;
        s_u8TwinkleCount++;
        s_u8TwinkleCount &= 0x07;
      #if LCD_UPSIDE_DOWN
        for (i=0;i<4;i++)
      #else
        for (i=3;i>=0;i--)
      #endif
        {
            if (((c_a_u8DisplayRam[i] & DISPLAY_TWINKLE_CODE) != 0) && (s_u8TwinkleCount > 4))
            {
                u8Value = 0;
            }
            else
            {
                u8Value = c_a_u8DisplayTable[c_a_u8DisplayRam[i] & ~(DISPLAY_DP_CODE|DISPLAY_TWINKLE_CODE)];
                u8Value |= (c_a_u8DisplayRam[i] & DISPLAY_DP_CODE);
            }
            for (j=0;j<4;j++)
            {
                l_a_u8LcdDispBuff[j] = (l_a_u8LcdDispBuff[j] << 2) | (u8Value & 0x03);
                u8Value >>= 2;
            }
        }
    }
}

/*----------------------------------------------
Function: Drive Lcd7SegX4
Parameter: void
Return: void
Call the function within 1ms
----------------------------------------------*/
void Lcd7SegX4Drive(void)
{
    static uint8_t s_u8ScanedTime;
    static uint8_t s_u8Index=0;
    uint8_t u8Value;
  #if SYSTEM_TIME_USE_INTERRUPT
    u8Value = g_u8SystemTimeTicks;
  #else
    u8Value = u32GetSysTicks();
  #endif
    if ((uint8_t)(u8Value - s_u8ScanedTime) >= LCD7SEGMENTS_SCAN_PERIOD)
    {
        s_u8ScanedTime = u8Value;
        switch (s_u8Index)
        {
            case 0: SetLcdCom3Tristate();
            LcdWriteSegments(l_a_u8LcdDispBuff[0]);
            SetLcdCom0Low();
            break;
            case 1: SetLcdCom0Tristate();
            LcdWriteSegments(~l_a_u8LcdDispBuff[0]);
            SetLcdCom0High();
            break;
            case 2: SetLcdCom0Tristate();
            LcdWriteSegments(l_a_u8LcdDispBuff[1]);
            SetLcdCom1Low();
            break;
            case 3: SetLcdCom1Tristate();
            LcdWriteSegments(~l_a_u8LcdDispBuff[1]);
            SetLcdCom1High();
            break;
            case 4: SetLcdCom1Tristate();
            LcdWriteSegments(l_a_u8LcdDispBuff[2]);
            SetLcdCom2Low();
            break;
            case 5: SetLcdCom2Tristate();
            LcdWriteSegments(~l_a_u8LcdDispBuff[2]);
            SetLcdCom2High();
            break;
            case 6: SetLcdCom2Tristate();
            LcdWriteSegments(l_a_u8LcdDispBuff[3]);
            SetLcdCom3Low();
            break;
            case 7: SetLcdCom3Tristate();
            LcdWriteSegments(~l_a_u8LcdDispBuff[3]);
            SetLcdCom3High();
            break;
        }
        s_u8Index ++;
        s_u8Index &= 0x07;
    }
}

/*----------------------------------------------
Function: lcd7segx4 write segments
Parameter: value
Return: void
----------------------------------------------*/
static void LcdWriteSegments(uint8_t u8Value)
{
    LCD_SEG0_PIN = ((u8Value & Bit0) != 0);
    LCD_SEG1_PIN = ((u8Value & Bit1) != 0);
    LCD_SEG2_PIN = ((u8Value & Bit2) != 0);
    LCD_SEG3_PIN = ((u8Value & Bit3) != 0);
    LCD_SEG4_PIN = ((u8Value & Bit4) != 0);
    LCD_SEG5_PIN = ((u8Value & Bit5) != 0);
    LCD_SEG6_PIN = ((u8Value & Bit6) != 0);
    LCD_SEG7_PIN = ((u8Value & Bit7) != 0);
}

/*----------------------------------------------
Function: Convert data to display string
Parameter: Data, Decimal digits, String len, String
Return: void
Comments: "Decimal digits" must less than "String len"
----------------------------------------------*/
#ifdef DATA2STRING_USE_32BITS
void Data2String(uint32_t u32Data, uint8_t u8Digits, uint8_t u8StrLen, uint8_t a_u8String[])
{
    uint8_t u8i;
    uint32_t u32Temp=1;
    for (u8i=0;u8i<u8StrLen;u8i++)
    {   /*Calculate the capacity of the string*/
        u32Temp *= 10;
    }
    u8i = 0;
    while ((u32Data >= u32Temp) && (u8Digits > 0))
    {   /*Truncate insignificant trailing decimals*/
        u8i = u32Data % 10;
        u32Data /= 10;
        u8Digits--;
    }
    if (u8i >= 5)   /*Rounding*/
    {
        u32Data++;
    }
    u8i = u8StrLen;
    while (u8i > 0)
    {   /*Split data */
        --u8i;
        a_u8String[u8i] = u32Data % 10;
        u32Data /= 10;
    }
    u8StrLen = u8StrLen - u8Digits - 1;
  #ifdef DISPLAY_DP_CODE
    if (0 != u8Digits)  /*Set the decimal point*/
    {
        a_u8String[u8StrLen] |= DISPLAY_DP_CODE;
    }
  #endif
  #if DATA2STRING_REMOVE_PREFIX_0
    for (u8i=0;(u8i<u8StrLen)&&(0==a_u8String[u8i]);u8i++)
    {   /*Remove insignificant prefix zero*/
        a_u8String[u8i] = DISPLAY_OFF_CODE;
    }
  #endif
}
#else
void Data2String(uint16_t u16Data, uint8_t u8Digits, uint8_t u8StrLen, uint8_t a_u8String[])
{
    uint8_t u8i;
    uint32_t u32Temp=1;
    for (u8i=0;u8i<u8StrLen;u8i++)
    {   /*Calculate the capacity of the string*/
        u32Temp *= 10;
    }
    u8i = 0;
    while ((u16Data >= u32Temp) && (u8Digits > 0))
    {   /*Truncate insignificant trailing decimals*/
        u8i = u16Data % 10;
        u16Data /= 10;
        u8Digits--;
    }
    if (u8i >= 5)   /*Rounding*/
    {
        u16Data++;
    }
    u8i = u8StrLen;
    while (u8i > 0)
    {   /*Split data */
        --u8i;
        a_u8String[u8i] = u16Data % 10;
        u16Data /= 10;
    }
    u8StrLen = u8StrLen - u8Digits - 1;
  #ifdef DISPLAY_DP_CODE
    if (0 != u8Digits)  /*Set the decimal point*/
    {
        a_u8String[u8StrLen] |= DISPLAY_DP_CODE;
    }
  #endif
  #if DATA2STRING_REMOVE_PREFIX_0
    for (u8i=0;(u8i<u8StrLen)&&(0==a_u8String[u8i]);u8i++)
    {   /*Remove insignificant prefix zero*/
        a_u8String[u8i] = DISPLAY_OFF_CODE;
    }
  #endif
}
#endif
