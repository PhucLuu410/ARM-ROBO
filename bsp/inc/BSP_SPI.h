#ifndef __BSP_SPI_H
#define __BSP_SPI_H

#include <stdint.h>
#include "stm32f103xb.h"
#include "BSP_GPIO.h"

void SPI_Init(void);
void SPI_SendByte(uint8_t data);
void SPI_SendData(uint8_t *data, uint16_t size);

#endif