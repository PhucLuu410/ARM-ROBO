#include "RingBuffer.h"
#include <stdint.h>

#define BUFFER_SIZE 9
#define MODBUS_FRAME_SIZE 8

volatile int front = 0;
volatile int rear = 0;
__attribute__((section(".custom_buffer"))) volatile uint8_t buffer[BUFFER_SIZE] = {0};

void RingBuffer_Push(uint8_t letter)
{
    int next = (rear + 1) % BUFFER_SIZE;

    if (next != front)
    {
        buffer[rear] = letter;
        rear = next;
    }
}

int RingBuffer_Available(void)
{
    if (rear >= front)
        return (rear - front);
    else
        return (BUFFER_SIZE - front + rear);
}

uint8_t PopFromBuffer(void)
{
    if (front == rear)
    {
        return 0;
    }

    uint8_t data = buffer[front];
    front = (front + 1) % BUFFER_SIZE;

    return data;
}

void Get_MODBUS_Data(uint8_t *data)
{
    int count = RingBuffer_Available();

    if (count >= MODBUS_FRAME_SIZE)
    {
        for (int i = 0; i < MODBUS_FRAME_SIZE; i++)
        {
            data[i] = PopFromBuffer();
        }
    }
}

void MODBUS_Error_Clear_Frame(void)
{
    int count = RingBuffer_Available();
    if (count > 0)
    {
        front = 0;
        rear = 0;
        for (int i = 0; i < BUFFER_SIZE; i++)
            buffer[i] = 0;
    }
}