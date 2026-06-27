#ifndef FIRMWARE_H
#define FIRMWARE_H

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "Flash.h"
#include "Dio.h"

typedef struct
{
    uint8_t Firmware_Version[16];
    uint32_t Firmware_Size;
} Firmware_Info;

#endif