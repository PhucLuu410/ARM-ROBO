#ifndef __RINGBUFFER_H
#define __RINGBUFFER_H

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

void RingBuffer_Push(uint8_t letter, volatile int *front, volatile int *rear, volatile uint8_t *buffer, int buffer_size);
uint8_t RingBuffer_Pop(volatile int *front, volatile int *rear, volatile uint8_t *buffer, int buffer_size);
int RingBuffer_Available(volatile int *front, volatile int *rear, int buffer_size);
// bool Get_MODBUS_Data(uint8_t *data);
// void MODBUS_Error_Clear_Frame(void);

#endif