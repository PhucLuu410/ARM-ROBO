#include "MAX7219.h"

void MAX7219_Init(void)
{
    SPI_SendData((uint8_t[]){MAX7219_REG_SCANLIMIT, MAX7219_SCAN_LIMIT_FORMAT}, 2);
    SPI_SendData((uint8_t[]){MAX7219_REG_DECODEMODE, MAX7219_DATA_DECODE_NONE}, 2);
    SPI_SendData((uint8_t[]){MAX7219_REG_SHUTDOWN, MAX7219_DATA_NORMAL_MODE}, 2);
    SPI_SendData((uint8_t[]){MAX7219_REG_INTENSITY, MAX7219_BRIGHTNESS_MEDIUM}, 2);
}

void MAX7219_SetBrightness(uint8_t brightness)
{
    if (brightness > MAX7219_BRIGHTNESS_MAX)
        brightness = MAX7219_BRIGHTNESS_MAX;
    SPI_SendData((uint8_t[]){MAX7219_REG_INTENSITY, brightness}, 2);
}

void MAX7219_DisplayDigit(uint8_t digit, uint8_t value)
{
    if (digit > 7)
        return;
    SPI_SendData((uint8_t[]){MAX7219_REG_DIGIT0 + digit, value}, 2);
}

void MAX7219_DisplayNumber(int number)
{
    for (int i = 0; i < 8; i++)
    {
        MAX7219_DisplayDigit(i, number % 10);
        number /= 10;
    }
}

void MAX7219_Clear(void)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        MAX7219_DisplayDigit(i, 0x00);
    }
}

void MAX7219_DisplayTest(void)
{
    SPI_SendData((uint8_t[]){0x0F, 0x01}, 2);
}

