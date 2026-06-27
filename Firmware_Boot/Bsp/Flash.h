#ifndef FLASH_H
#define FLASH_H

#include "stm32f103xb.h"

void Flash_Init(void);
void Flash_ErasePage(uint32_t Address);
void Flash_WriteHalfWord(uint32_t Address, uint16_t Data);

#endif