#ifndef __RINGBUFFER_H
#define __RINGBUFFER_H

#include "stdio.h"
#include "string.h"

void RingBuffer(char letter);
char PopFromBuffer(void);
int ReceiveData(void);

#endif