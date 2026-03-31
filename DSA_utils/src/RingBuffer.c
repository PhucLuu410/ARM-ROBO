#include "RingBuffer.h"

int front = 0;
int rear = 0;
int idx = 0;
static volatile char buffer[10];
__attribute__((section(".custom_buffer"))) volatile char receive[10];

void RingBuffer(char letter)
{
    if (letter != '\0')
    {
        int next = (rear + 1) % sizeof(buffer);
        if (next != front)
        {
            buffer[rear] = letter;
            rear = next;
        }
    }
}
char PopFromBuffer(void)
{
    char data = '\0';
    if (front != rear)
    {
        data = buffer[front];
        front = (front + 1) % sizeof(buffer);
    }
    return data;
}

int ReceiveData(void)
{
    char c = PopFromBuffer();

    if (c == '\0')
        return 0;

    if (c == '\n')
    {
        receive[idx] = '\0';
        idx = 0;
        return 1;
    }

    if (idx < sizeof(receive) - 1)
    {
        receive[idx] = c;
        idx++;
    }

    return 0;
}