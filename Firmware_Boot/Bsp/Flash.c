#include "Flash.h"

void Flash_Init(void)
{
    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xCDEF89AB;
    while (FLASH->CR & FLASH_CR_LOCK)
        ;
}

void Flash_ErasePage(uint32_t Address)
{
    while (FLASH->SR & (1 << 0))
        ;
    FLASH->CR |= (1 << 1);
    FLASH->AR = Address;
    FLASH->CR |= (1 << 6);

    while (FLASH->SR & (1 << 0))
        ;
    FLASH->CR &= ~(1 << 1);
}

void Flash_WriteHalfWord(uint32_t Address, uint16_t Data)
{
    if (!(FLASH->SR & (1 << 0)))
    {
        FLASH->CR |= (1 << 0);
        *(volatile uint16_t *)Address = Data;
        while (FLASH->SR & (1 << 0))
            ;
        FLASH->CR &= ~(1 << 0);
    }
}