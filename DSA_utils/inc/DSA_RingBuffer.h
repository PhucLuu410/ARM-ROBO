#ifndef __RINGBUFFER_H
#define __RINGBUFFER_H

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

void RingBuffer_Push(uint8_t letter);
uint8_t PopFromBuffer(void);
bool Get_MODBUS_Data(uint8_t *data);
void MODBUS_Error_Clear_Frame(void);

#endif