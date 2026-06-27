#define APP_ADDR 0x08001000

#include <stdint.h>
#include "stm32f103xb.h"
#include <stddef.h>
#include "System.h"
#include "Uart.h"

uint8_t data[16] = {0};

static void delay(volatile uint32_t d)
{
    while (d--)
        ;
}

void JumpToApp(void)
{
}

int main(void)
{
    System_Init();
    for (int i = 0; i < 16; i++)
    {
        data[i] = 0;
    }
    Uart_SendData(0x10);
    delay(1000);
    Uart_SendData(0x01);
    while (1)
    {
        if (Uart_GetData(data))
        {
            Uart_SendData(0x10);
            delay(1000);
            Uart_SendData(0x03);
        }
    }
}

void HardFault_Handler(void)
{
    while (1)
    {
    }
}