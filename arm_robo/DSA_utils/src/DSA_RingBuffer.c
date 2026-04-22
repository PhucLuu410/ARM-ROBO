#include "DSA_RingBuffer.h"
#include <stdint.h>

void RingBuffer_Push(uint8_t letter, volatile int *front, volatile int *rear, volatile uint8_t *buffer, int buffer_size)
{
    int next = (*rear + 1) % buffer_size;

    if (next != *front)
    {
        buffer[*rear] = letter;
        *rear = next;
    }
}

int RingBuffer_Available(volatile int *front, volatile int *rear, int buffer_size)
{
    if (*rear >= *front)
        return (*rear - *front);
    else
        return (buffer_size - *front + *rear);
}

uint8_t RingBuffer_Pop(volatile int *front, volatile int *rear, volatile uint8_t *buffer, int buffer_size)
{
    if (*front == *rear)
    {
        return 0;
    }

    uint8_t data = buffer[*front];
    *front = (*front + 1) % buffer_size;

    return data;
}