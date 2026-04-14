#include "APP_Display.h"

uint8_t Error_String[] = {0x4F, 0x05, 0x05, 0x1D, 0x05, 0x00, 0x00, 0x00};
uint8_t Stop_String[] = {0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x00, 0x00, 0x00};
uint8_t Mode1_String[] = {0x7E, 0x7E, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t Mode2_String[] = {0x7E, 0x7E, 0x6D, 0x00, 0x00, 0x00, 0x00, 0x00};

void Display_ShowError(void)
{
    MAX7219_DisplayDigit(7, Error_String[0]);
    MAX7219_DisplayDigit(6, Error_String[1]);
    MAX7219_DisplayDigit(5, Error_String[2]);
    MAX7219_DisplayDigit(4, Error_String[3]);
    MAX7219_DisplayDigit(3, Error_String[4]);
    MAX7219_DisplayDigit(2, Error_String[5]);
    MAX7219_DisplayDigit(1, Error_String[6]);
    MAX7219_DisplayDigit(0, Error_String[7]);
}
void Display_ShowMode1(void)
{
    MAX7219_DisplayDigit(7, Mode1_String[0]);
    MAX7219_DisplayDigit(6, Mode1_String[1]);
    MAX7219_DisplayDigit(5, Mode1_String[2]);
    MAX7219_DisplayDigit(4, Mode1_String[3]);
    MAX7219_DisplayDigit(3, Mode1_String[4]);
    MAX7219_DisplayDigit(2, Mode1_String[5]);
    MAX7219_DisplayDigit(1, Mode1_String[6]);
    MAX7219_DisplayDigit(0, 0x00);
}
void Display_ShowMode2(void)
{
    MAX7219_DisplayDigit(7, Mode2_String[0]);
    MAX7219_DisplayDigit(6, Mode2_String[1]);
    MAX7219_DisplayDigit(5, Mode2_String[2]);
    MAX7219_DisplayDigit(4, Mode2_String[3]);
    MAX7219_DisplayDigit(3, Mode2_String[4]);
    MAX7219_DisplayDigit(2, Mode2_String[5]);
    MAX7219_DisplayDigit(1, Mode2_String[6]);
    MAX7219_DisplayDigit(0, Mode2_String[7]);
}
void Display_ShowStop(void)
{
    MAX7219_DisplayDigit(7, Stop_String[0]);
    MAX7219_DisplayDigit(6, Stop_String[1]);
    MAX7219_DisplayDigit(5, Stop_String[2]);
    MAX7219_DisplayDigit(4, Stop_String[3]);
    MAX7219_DisplayDigit(3, Stop_String[4]);
    MAX7219_DisplayDigit(2, Stop_String[5]);
    MAX7219_DisplayDigit(1, Stop_String[6]);
    MAX7219_DisplayDigit(0, Stop_String[7]);
}

void Display_Init(void)
{
    MAX7219_Init();
    MAX7219_SetBrightness(MAX7219_BRIGHTNESS_MAX);
    Display_ShowStop();
}