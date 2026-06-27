#include "System.h"

void System_Init(void)
{
    Rcc_Init();
    Flash_Init();
    Uart_Init();
}